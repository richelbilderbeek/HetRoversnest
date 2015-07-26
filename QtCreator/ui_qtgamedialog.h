/********************************************************************************
** Form generated from reading UI file 'qtgamedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.4.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTGAMEDIALOG_H
#define UI_QTGAMEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPlainTextEdit>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtGameDialog
{
public:
    QGridLayout *gridLayout;
    QPlainTextEdit *plainTextEdit;
    QWidget *widget;
    QGridLayout *gridLayout_2;
    QLabel *label_condition;
    QLabel *label_provisions;
    QSpacerItem *verticalSpacer;
    QLabel *label_skill;
    QLabel *label_gold;
    QLabel *label_luck;
    QLabel *image;
    QLabel *label_items;

    void setupUi(QDialog *QtGameDialog)
    {
        if (QtGameDialog->objectName().isEmpty())
            QtGameDialog->setObjectName(QStringLiteral("QtGameDialog"));
        QtGameDialog->resize(712, 579);
        QtGameDialog->setStyleSheet(QLatin1String("QWidget {\n"
"  background-color: black;\n"
"  border: 1px solid lime\n"
"}\n"
"\n"
"QPlainTextEdit {\n"
"  font: 32px  \"URW Chancery L\";\n"
"  color: lime;\n"
"  background-color: black;\n"
"  border: 1px solid green\n"
"}\n"
"\n"
"QLabel {\n"
"  font: 18px  monospace;\n"
"  border: 1px solid black;\n"
"  color: lime;\n"
"  background-color: black\n"
"}\n"
"\n"
"QScrollBar {\n"
"  color: lime;\n"
"  background-color: black;\n"
"  border: 1px solid lime\n"
"}\n"
"\n"
"QScrollBar::handle {\n"
"  background: green\n"
"}\n"
""));
        gridLayout = new QGridLayout(QtGameDialog);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        plainTextEdit = new QPlainTextEdit(QtGameDialog);
        plainTextEdit->setObjectName(QStringLiteral("plainTextEdit"));
        plainTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        plainTextEdit->setReadOnly(true);
        plainTextEdit->setTextInteractionFlags(Qt::NoTextInteraction);
        plainTextEdit->setBackgroundVisible(false);
        plainTextEdit->setCenterOnScroll(false);

        gridLayout->addWidget(plainTextEdit, 0, 0, 1, 1);

        widget = new QWidget(QtGameDialog);
        widget->setObjectName(QStringLiteral("widget"));
        gridLayout_2 = new QGridLayout(widget);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        label_condition = new QLabel(widget);
        label_condition->setObjectName(QStringLiteral("label_condition"));
        label_condition->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignVCenter);

        gridLayout_2->addWidget(label_condition, 0, 0, 1, 2);

        label_provisions = new QLabel(widget);
        label_provisions->setObjectName(QStringLiteral("label_provisions"));

        gridLayout_2->addWidget(label_provisions, 4, 0, 1, 2);

        verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

        gridLayout_2->addItem(verticalSpacer, 6, 0, 1, 1);

        label_skill = new QLabel(widget);
        label_skill->setObjectName(QStringLiteral("label_skill"));

        gridLayout_2->addWidget(label_skill, 1, 0, 1, 2);

        label_gold = new QLabel(widget);
        label_gold->setObjectName(QStringLiteral("label_gold"));

        gridLayout_2->addWidget(label_gold, 3, 0, 1, 2);

        label_luck = new QLabel(widget);
        label_luck->setObjectName(QStringLiteral("label_luck"));

        gridLayout_2->addWidget(label_luck, 2, 0, 1, 2);

        image = new QLabel(widget);
        image->setObjectName(QStringLiteral("image"));
        image->setPixmap(QPixmap(QString::fromUtf8(":/images/ZanbarBone.jpg")));
        image->setAlignment(Qt::AlignCenter);

        gridLayout_2->addWidget(image, 7, 0, 1, 2);

        label_items = new QLabel(widget);
        label_items->setObjectName(QStringLiteral("label_items"));

        gridLayout_2->addWidget(label_items, 5, 0, 1, 1);


        gridLayout->addWidget(widget, 0, 1, 1, 1);


        retranslateUi(QtGameDialog);

        QMetaObject::connectSlotsByName(QtGameDialog);
    } // setupUi

    void retranslateUi(QDialog *QtGameDialog)
    {
        QtGameDialog->setWindowTitle(QApplication::translate("QtGameDialog", "City of Thieves", 0));
        plainTextEdit->setPlainText(QString());
        plainTextEdit->setPlaceholderText(QString());
        label_condition->setText(QApplication::translate("QtGameDialog", "Condition: ?", 0));
        label_provisions->setText(QApplication::translate("QtGameDialog", "Provisions: ?", 0));
        label_skill->setText(QApplication::translate("QtGameDialog", "Skill: ?", 0));
        label_gold->setText(QApplication::translate("QtGameDialog", "Gold:  ?", 0));
        label_luck->setText(QApplication::translate("QtGameDialog", "Luck: ?", 0));
        image->setText(QString());
        label_items->setText(QApplication::translate("QtGameDialog", "Items: ?", 0));
    } // retranslateUi

};

namespace Ui {
    class QtGameDialog: public Ui_QtGameDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTGAMEDIALOG_H
