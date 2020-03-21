/********************************************************************************
** Form generated from reading UI file 'slndetailswindow.ui'
**
** Created by: Qt User Interface Compiler version 5.11.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SLNDETAILSWINDOW_H
#define UI_SLNDETAILSWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_slndetailswindow
{
public:
    QVBoxLayout *verticalLayout;
    QTextEdit *textEdit;
    QPushButton *pushClose;

    void setupUi(QWidget *slndetailswindow)
    {
        if (slndetailswindow->objectName().isEmpty())
            slndetailswindow->setObjectName(QStringLiteral("slndetailswindow"));
        slndetailswindow->resize(400, 380);
        verticalLayout = new QVBoxLayout(slndetailswindow);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        textEdit = new QTextEdit(slndetailswindow);
        textEdit->setObjectName(QStringLiteral("textEdit"));

        verticalLayout->addWidget(textEdit);

        pushClose = new QPushButton(slndetailswindow);
        pushClose->setObjectName(QStringLiteral("pushClose"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushClose->sizePolicy().hasHeightForWidth());
        pushClose->setSizePolicy(sizePolicy);

        verticalLayout->addWidget(pushClose);


        retranslateUi(slndetailswindow);

        QMetaObject::connectSlotsByName(slndetailswindow);
    } // setupUi

    void retranslateUi(QWidget *slndetailswindow)
    {
        slndetailswindow->setWindowTitle(QApplication::translate("slndetailswindow", "Solution Details", nullptr));
        pushClose->setText(QApplication::translate("slndetailswindow", "Close", nullptr));
    } // retranslateUi

};

namespace Ui {
    class slndetailswindow: public Ui_slndetailswindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SLNDETAILSWINDOW_H
