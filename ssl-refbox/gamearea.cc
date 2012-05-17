#include <GL/gl.h>
#include <GL/glut.h>
#include <QResizeEvent>
#include <iostream>
#include <libbsmart/field.h>
#include <libbsmart/math.h>

#include "glextra.h"
#include "gamearea.h"
#include "particle_filter.h"

Gamearea::Gamearea ( QWidget* p ) :
        QGLWidget ( p ), m_timer ( -1 )
{
    setMouseTracking ( true );
    rules_wait_condition = new QWaitCondition();
    new_data_wait_condition = new QWaitCondition();
    pf_data = new Pre_Filter_Data();
    filter_data = new Filter_Data();
    gamestate = new BSmart::Game_States();
    vision = new SSLVision ( pf_data, gamestate, new_data_wait_condition );
    refbox_listener = new RefboxListener ( gamestate );
    particle_filter = new Particle_Filter_Mother ( pf_data, filter_data,
            rules_wait_condition, new_data_wait_condition );
    pf_tester = new PF_Tester ( pf_data, gamestate );
    rules = new SSL_Refbox_Rules ( rules_wait_condition, filter_data, gamestate );
    glextra = GLExtra ( filter_data );

    connect ( pf_tester, SIGNAL ( new_frame() ), this, SLOT ( show_world() ) );
    connect ( rules, SIGNAL ( new_filter_data() ), this, SLOT ( show_world() ) );
    connect ( pf_tester, SIGNAL ( new_frame() ), particle_filter, SLOT ( new_frame() ) );
    connect ( vision, SIGNAL ( new_refbox_cmd ( char ) ), refbox_listener,
              SLOT ( new_refbox_cmd ( char ) ) );

    vision->start(); //34-36%
    //    pf_tester->start(); //64%
    refbox_listener->start(); //2%
    particle_filter->start(); //67%
    rules->start();

    start_time = BSmart::Systemcall::get_current_system_time();
    show_rule_data = true;
}

Gamearea::~Gamearea()
{
    if ( glIsList ( m_field ) )
        glDeleteLists ( m_field, 1 );
    if ( glIsList ( world_model ) )
        glDeleteLists ( world_model, 1 );
}

void Gamearea::paintGL()
{

    glClear ( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode ( GL_MODELVIEW );

    glPushMatrix();

    //... OSD-like stuff (Screen Size = Field Size)
    //... "camera" transforms
    //... Field lines, goals etc.
    //... Field Objects
    glCallList ( m_field );

    //transform world_model into showable
    glextra.bglDrawFilterData();

    if ( show_rule_data ) {
        glextra.bglDrawRulesystemData();
    }

    glPushMatrix();

    //draw current game state
    glColor3f ( 1.0, 1.0, 1.0 );
    bitmap_output ( GLUT_BITMAP_TIMES_ROMAN_10 );

    glPopMatrix();

    glPopMatrix();
}

void Gamearea::initializeGL()
{

    qglClearColor ( QColor ( 0, 119, 0 ) );

    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity();

    GLdouble zFar = 2001.;
    GLdouble zNear = 1.;

    // angle (winkel) with some padding, so that you can see the edge
    GLdouble winkel = atan2 ( ( double ) BSmart::Field::half_height, ( zFar - 1. ) );
    GLdouble fovy = 2. * winkel * 180. / BSmart::pi;
    GLdouble aspect = ( double ) BSmart::Field::width
                      / ( double ) BSmart::Field::height;

    gluPerspective ( fovy, aspect, zNear, zFar );

    glMatrixMode ( GL_MODELVIEW );
    glLoadIdentity();
    gluLookAt ( 0., 0., zFar - 1, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0 );

    // creation of game field into a list. single, because static
    m_field = glGenLists ( 1 );
    glNewList ( m_field, GL_COMPILE );
    glPushMatrix();
    glColor3d ( 1.0, 1.0, 1.0 );
    GLExtra::bglDrawField();
    glPopMatrix();
    glEndList();

    // OpenGL should refresh
    setAutoBufferSwap ( true );
    m_timer = startTimer ( 10000 );

    emit showLogControl ( false );
}

void Gamearea::resizeGL ( int width, int height )
{

    glViewport ( 0, 0, width, height );
    glMatrixMode ( GL_PROJECTION );
    glLoadIdentity();

    const double zFar = 2001.0;
    glOrtho ( -BSmart::Field::half_width, BSmart::Field::half_width,
              -BSmart::Field::half_height, BSmart::Field::half_height, -5.0, zFar );
    glMatrixMode ( GL_MODELVIEW );

    emit resizeSlider ( width );
}

///Draws next frame (calls paintGL() properly)
void Gamearea::timerEvent ( QTimerEvent* )
{
    glDraw();
}

//output of current refbox_cmd
void Gamearea::bitmap_output ( void *font )
{
    char refbox_cmd[2];
    refbox_cmd[0] = gamestate->get_refbox_cmd();
    refbox_cmd[1] = 0;

    std::string string = "";
    string += "Current Refbox Command: ";
    string += refbox_cmd;

    glPushMatrix();
    GLfloat x = -BSmart::Field::half_field_width;
    GLfloat y = 2162.;
    glRasterPos2f ( x, y );

    int len, i;
    len = string.size();
    for ( i = 0; i < len; i++ ) {
        glutBitmapCharacter ( font, string[i] );
    }
    glPopMatrix();

    string = "external Play_State: ";
    std::string play_state ( gamestate->play_state_string() );
    string += play_state;

    glPushMatrix();
    x = -1500.;
    y = 2162.;
    glRasterPos2f ( x, y );

    len = string.size();
    for ( i = 0; i < len; i++ ) {
        glutBitmapCharacter ( font, string[i] );
    }
    glPopMatrix();
}

//Slot called from other threads to update gui
void Gamearea::show_world()
{
    glDraw();
}

//Slot called from guiactions to switch on/off Rule System Data
void Gamearea::show_rules_changed()
{
    if ( show_rule_data ) {
        show_rule_data = false;
        QString tmp = "Show Rules";
        emit change_show_rules ( tmp );
    } else {
        show_rule_data = true;
        QString tmp = "No Rules";
        emit change_show_rules ( tmp );
    }
}
