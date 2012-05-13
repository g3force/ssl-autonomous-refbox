#include "GuiPropertiesDlg.h"
#include "ui_GuiPropertiesDlg.h"

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
	string value;
	bool isValid = true;

	QString validStyle		= "border: 1px solid green";
	QString nonValidStyle	= "border: 1px solid red";

	value = ui.sslv_ip_Edit->text().toStdString();
	if( isIpAddress(value) ) {
		Global::config.add( "ssl_vision_ip", value );
		ui.sslv_ip_Edit->setStyleSheet( validStyle );
	} else {
		isValid = false;
		ui.sslv_ip_Edit->setStyleSheet( nonValidStyle );
	}

	value = ui.sslv_port_Edit->text().toStdString();
	if( isPort(value) ) {
		Global::config.add( "ssl_vision_port", value );
		ui.sslv_port_Edit->setStyleSheet( validStyle );
	} else {
		isValid = false;
		ui.sslv_port_Edit->setStyleSheet( nonValidStyle );
	}

	value = ui.refbox_ip_Edit->text().toStdString();
	if( isIpAddress(value) ) {
		Global::config.add( "refbox_ip", value );
		ui.refbox_ip_Edit->setStyleSheet( validStyle );
	} else {
		isValid = false;
		ui.refbox_ip_Edit->setStyleSheet( nonValidStyle );
	}

	value = ui.refbox_port_Edit->text().toStdString();
	if( isPort(value) ) {
		Global::config.add( "refbox_port", value );
		ui.refbox_port_Edit->setStyleSheet( validStyle );
	} else {
		isValid = false;
		ui.refbox_port_Edit->setStyleSheet( nonValidStyle );
	}

	Global::saveConfig();

	if(isValid)
		this->close();
}

bool GuiPropertiesDlg::isIpAddress(string ip) {
	string tmp;

	int n = getNextNumber(ip, '.');
	if( n<0 || 255<n ) return false;

	ip = ip.substr(ip.find('.',0)+1, ip.find('\0',0) );
	n = getNextNumber(ip, '.');
	if( n<0 || 255<n ) return false;

	ip = ip.substr(ip.find('.',0)+1, ip.find('\0',0) );
	n = getNextNumber(ip, '.');
	if( n<0 || 255<n ) return false;

	ip = ip.substr(ip.find('.',0)+1, ip.find('\0',0) );
	n = getNextNumber(ip, '\0');
	if( n<0 || 255<n ) return false;

	return true;
}

bool GuiPropertiesDlg::isPort(string port) {
	int n = getNextNumber(port, '\0');
	if( 0<=n && n<=65000 ) {
		return true;
	}

	return false;
}

int GuiPropertiesDlg::getNextNumber(string str, char d) {
	int i=0;
	int num=0;
	while( str[i] != d ) {
		if( isdigit( str[i] ) ) {
			num *= 10;
			num += (int)str[i] - 48;
		} else {
			return -1;
		}
		i++;
	}

	return num;
}
