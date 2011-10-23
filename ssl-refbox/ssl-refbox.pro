
TEMPLATE = app
TARGET = ../bin/ssl-refbox
CONFIG += qt debug console thread warn_on link_pkgconfig
PKGCONFIG += swipl

DEPENDPATH += ../libbsmart ../proto
INCLUDEPATH += ../

LIBS += -lprotobuf -lglut

QMAKE_LINK=swipl-ld ssl_refbox_rules_prolog.pl

# Input
HEADERS += glextra.h \
           gamearea.h \
           guiactions.h \
           sslvision.h \
           pre_filter_data.h \
           filter_data.h \
           colors.h \
           refboxlistener.h \
           commands.h \
           log_control.h \
           particle_filter.h \
           sample.h \
           pf_tester.h \
           field_hardware.h \
           ssl_refbox_rules.h \
           ../proto/messages_robocup_ssl_detection.pb.h \
           ../proto/messages_robocup_ssl_geometry.pb.h \
           ../proto/messages_robocup_ssl_refbox_log.pb.h \
           ../proto/messages_robocup_ssl_wrapper.pb.h
FORMS += GuiControls.ui
SOURCES += glextra.cc \
           gamearea.cc \
           guiactions.cc \
           sslvision.cc \
           pre_filter_data.cc \
           filter_data.cc \
           refboxlistener.cc \
           main.cc \
           log_control.cc \
           particle_filter.cc \
           sample.cc \
           pf_tester.cc \
           field_hardware.cc \
           ssl_refbox_rules.cc \
           ../libbsmart/pose.cc \
           ../libbsmart/vector2.cc \
           ../libbsmart/multicast_socket.cc \
           ../libbsmart/game_states.cc \
           ../libbsmart/timer.cc \
           ../libbsmart/systemcall.cc \
           ../libbsmart/pose3d.cc \
           ../libbsmart/vector3.cc \
           ../libbsmart/circle.cc \
           ../proto/messages_robocup_ssl_detection.pb.cc \
           ../proto/messages_robocup_ssl_geometry.pb.cc \
           ../proto/messages_robocup_ssl_refbox_log.pb.cc \
           ../proto/messages_robocup_ssl_wrapper.pb.cc

QT += opengl
