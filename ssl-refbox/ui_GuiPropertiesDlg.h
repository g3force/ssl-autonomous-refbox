/********************************************************************************
** Form generated from reading UI file 'GuiPropertiesDlg.ui'
**
** Created: Mon Apr 2 16:40:50 2012
**      by: Qt User Interface Compiler version 4.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_GUIPROPERTIESDLG_H
#define UI_GUIPROPERTIESDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_GuiPropertiesDlgClass
{
public:
    QPushButton *okBtn;
    QPushButton *pushButton_2;
    QLabel *label_3;
    QLineEdit *refbox_ip_Edit;
    QLabel *label;
    QLineEdit *refbox_port_Edit;
    QLabel *label_2;
    QLineEdit *sslv_ip_Edit;
    QLineEdit *sslv_port_Edit;
    QLabel *label_4;

    void setupUi(QDialog *GuiPropertiesDlgClass)
    {
        if (GuiPropertiesDlgClass->objectName().isEmpty())
            GuiPropertiesDlgClass->setObjectName(QString::fromUtf8("GuiPropertiesDlgClass"));
        GuiPropertiesDlgClass->resize(383, 209);
        okBtn = new QPushButton(GuiPropertiesDlgClass);
        okBtn->setObjectName(QString::fromUtf8("okBtn"));
        okBtn->setGeometry(QRect(40, 170, 97, 27));
        pushButton_2 = new QPushButton(GuiPropertiesDlgClass);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(190, 170, 97, 27));
        label_3 = new QLabel(GuiPropertiesDlgClass);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(10, 87, 69, 21));
        refbox_ip_Edit = new QLineEdit(GuiPropertiesDlgClass);
        refbox_ip_Edit->setObjectName(QString::fromUtf8("refbox_ip_Edit"));
        refbox_ip_Edit->setGeometry(QRect(122, 87, 257, 27));
        label = new QLabel(GuiPropertiesDlgClass);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(10, 5, 89, 21));
        refbox_port_Edit = new QLineEdit(GuiPropertiesDlgClass);
        refbox_port_Edit->setObjectName(QString::fromUtf8("refbox_port_Edit"));
        refbox_port_Edit->setGeometry(QRect(122, 120, 257, 27));
        label_2 = new QLabel(GuiPropertiesDlgClass);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        label_2->setGeometry(QRect(10, 38, 106, 21));
        sslv_ip_Edit = new QLineEdit(GuiPropertiesDlgClass);
        sslv_ip_Edit->setObjectName(QString::fromUtf8("sslv_ip_Edit"));
        sslv_ip_Edit->setGeometry(QRect(122, 5, 257, 27));
        sslv_port_Edit = new QLineEdit(GuiPropertiesDlgClass);
        sslv_port_Edit->setObjectName(QString::fromUtf8("sslv_port_Edit"));
        sslv_port_Edit->setGeometry(QRect(122, 38, 257, 27));
        label_4 = new QLabel(GuiPropertiesDlgClass);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        label_4->setGeometry(QRect(10, 120, 86, 21));

        retranslateUi(GuiPropertiesDlgClass);

        QMetaObject::connectSlotsByName(GuiPropertiesDlgClass);
    } // setupUi

    void retranslateUi(QDialog *GuiPropertiesDlgClass)
    {
        GuiPropertiesDlgClass->setWindowTitle(QApplication::translate("GuiPropertiesDlgClass", "GuiPropertiesDlg", 0, QApplication::UnicodeUTF8));
        okBtn->setText(QApplication::translate("GuiPropertiesDlgClass", "OK", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("GuiPropertiesDlgClass", "PushButton", 0, QApplication::UnicodeUTF8));
        label_3->setText(QApplication::translate("GuiPropertiesDlgClass", "Refbox IP:", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("GuiPropertiesDlgClass", "SSL Vision IP:", 0, QApplication::UnicodeUTF8));
        label_2->setText(QApplication::translate("GuiPropertiesDlgClass", "SSL Vision Port:", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("GuiPropertiesDlgClass", "Refbox Port:", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class GuiPropertiesDlgClass: public Ui_GuiPropertiesDlgClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_GUIPROPERTIESDLG_H
