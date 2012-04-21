#include "GuiPropertiesDlg.h"
#include "ui_GuiPropertiesDlg.h"

#include "global.h"

#include <string>

using std::string;

GuiPropertiesDlg::GuiPropertiesDlg(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	string value;
	Global::config.readInto( value, "ssl_vision_ip" );
	ui.sslv_ip_Edit->setText( value.c_str() );
	Global::config.readInto( value, "ssl_vision_port" );
	ui.sslv_port_Edit->setText( value.c_str() );

	Global::config.readInto( value, "refbox_ip" );
	ui.refbox_ip_Edit->setText( value.c_str() );
	Global::config.readInto( value, "refbox_port" );
	ui.refbox_port_Edit->setText( value.c_str() );

	connect ( ui.saveBtn, SIGNAL ( clicked() ), this, SLOT ( configUpdate() ) );
	connect ( ui.cancelBtn, SIGNAL ( clicked() ), this, SLOT ( close() ) );
}

GuiPropertiesDlg::~GuiPropertiesDlg()
{
}

void GuiPropertiesDlg::configUpdate() {
	Global::config.add( "ssl_vision_ip", ui.sslv_ip_Edit->text().toStdString() );
	Global::config.add( "ssl_vision_port", ui.sslv_port_Edit->text().toStdString() );

	Global::config.add( "refbox_ip", ui.refbox_ip_Edit->text().toStdString() );
	Global::config.add( "refbox_port", ui.refbox_port_Edit->text().toStdString() );

	Global::saveConfig();

	this->close();
}
