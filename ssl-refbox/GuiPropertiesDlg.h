#ifndef GUIPROPERTIESDLG_H
#define GUIPROPERTIESDLG_H

#include <QtGui/QDialog>
#include <string>
#include "ui_GuiPropertiesDlg.h"
#include "global.h"

using std::string;

class GuiPropertiesDlg : public QDialog
{
    Q_OBJECT
public:
    GuiPropertiesDlg(QWidget *parent = 0);
    ~GuiPropertiesDlg();

private:
    Ui::GuiPropertiesDlgClass ui;

    void init();
    bool isIpAddress(string ip);
    bool isPort(string port);
    int getNextNumber(string str, char d);

 private slots:
	 void configUpdate();
};

#endif // GUIPROPERTIESDLG_H
