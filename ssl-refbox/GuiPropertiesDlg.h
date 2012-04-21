#ifndef GUIPROPERTIESDLG_H
#define GUIPROPERTIESDLG_H

#include <QtGui/QDialog>
#include "ui_GuiPropertiesDlg.h"

class GuiPropertiesDlg : public QDialog
{
    Q_OBJECT
public:
    GuiPropertiesDlg(QWidget *parent = 0);
    ~GuiPropertiesDlg();

private:
    Ui::GuiPropertiesDlgClass ui;

    void init();

 private slots:
	 void configUpdate();
};

#endif // GUIPROPERTIESDLG_H
