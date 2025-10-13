/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_2;
    QSpacerItem *verticalSpacer_3;
    QHBoxLayout *horizontalLayout_4;
    QLabel *counter;
    QSpacerItem *verticalSpacer;
    QGridLayout *gridLayout;
    QPushButton *Ficha1;
    QPushButton *Ficha6;
    QPushButton *Ficha5;
    QPushButton *emptyButton;
    QPushButton *Ficha7;
    QPushButton *Ficha8;
    QPushButton *Ficha2;
    QPushButton *Ficha3;
    QPushButton *Ficha4;
    QSpacerItem *verticalSpacer_2;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *resetButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(600, 500);
        MainWindow->setMinimumSize(QSize(0, 0));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalSpacer_3 = new QSpacerItem(5, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_3);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setObjectName("horizontalLayout_4");
        counter = new QLabel(centralwidget);
        counter->setObjectName("counter");
        QSizePolicy sizePolicy(QSizePolicy::Policy::Fixed, QSizePolicy::Policy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(counter->sizePolicy().hasHeightForWidth());
        counter->setSizePolicy(sizePolicy);
        QFont font;
        font.setFamilies({QString::fromUtf8("Bell MT")});
        font.setPointSize(14);
        counter->setFont(font);
        counter->setTextFormat(Qt::TextFormat::AutoText);
        counter->setAlignment(Qt::AlignmentFlag::AlignLeading|Qt::AlignmentFlag::AlignLeft|Qt::AlignmentFlag::AlignVCenter);

        horizontalLayout_4->addWidget(counter);


        verticalLayout_2->addLayout(horizontalLayout_4);

        verticalSpacer = new QSpacerItem(5, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout_2->addItem(verticalSpacer);

        gridLayout = new QGridLayout();
        gridLayout->setObjectName("gridLayout");
        Ficha1 = new QPushButton(centralwidget);
        Ficha1->setObjectName("Ficha1");
        Ficha1->setEnabled(false);
        QSizePolicy sizePolicy1(QSizePolicy::Policy::MinimumExpanding, QSizePolicy::Policy::MinimumExpanding);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(Ficha1->sizePolicy().hasHeightForWidth());
        Ficha1->setSizePolicy(sizePolicy1);
        QFont font1;
        font1.setFamilies({QString::fromUtf8("Bell MT")});
        font1.setPointSize(24);
        Ficha1->setFont(font1);
        Ficha1->setStyleSheet(QString::fromUtf8("QPushButton\n"
" {\n"
"	/* Color base del bot\303\263n (lila) */\n"
"	background-color: #a8a8f0; \n"
"	/* Color de texto en estado normal (negro) */    \n"
"	color: #000000; \n"
"  	 /* Ajusta el radio del borde */\n"
"	border-radius: 5px; \n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover:!pressed \n"
"{\n"
"   	/* Color cuando pasas el cursor (morado) */\n"
"	background-color: #6a6ad8; \n"
"	/* Color de texto en hover (blanco) */    \n"
"	color: #ffffff; \n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"	/* Color cuando presionas el bot\303\263n (morado) */\n"
"	background-color: #6a6ad8; \n"
"}"));

        gridLayout->addWidget(Ficha1, 0, 0, 1, 1);

        Ficha6 = new QPushButton(centralwidget);
        Ficha6->setObjectName("Ficha6");
        Ficha6->setEnabled(false);
        sizePolicy1.setHeightForWidth(Ficha6->sizePolicy().hasHeightForWidth());
        Ficha6->setSizePolicy(sizePolicy1);
        Ficha6->setFont(font1);
        Ficha6->setStyleSheet(QString::fromUtf8("QPushButton\n"
" {\n"
"	/* Color base del bot\303\263n (lila) */\n"
"	background-color: #a8a8f0; \n"
"	/* Color de texto en estado normal (negro) */    \n"
"	color: #000000; \n"
"  	 /* Ajusta el radio del borde */\n"
"	border-radius: 5px; \n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover:!pressed \n"
"{\n"
"   	/* Color cuando pasas el cursor (morado) */\n"
"	background-color: #6a6ad8; \n"
"	/* Color de texto en hover (blanco) */    \n"
"	color: #ffffff; \n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"	/* Color cuando presionas el bot\303\263n (morado) */\n"
"	background-color: #6a6ad8; \n"
"}"));

        gridLayout->addWidget(Ficha6, 1, 2, 1, 1);

        Ficha5 = new QPushButton(centralwidget);
        Ficha5->setObjectName("Ficha5");
        Ficha5->setEnabled(false);
        sizePolicy1.setHeightForWidth(Ficha5->sizePolicy().hasHeightForWidth());
        Ficha5->setSizePolicy(sizePolicy1);
        Ficha5->setFont(font1);
        Ficha5->setStyleSheet(QString::fromUtf8("QPushButton\n"
" {\n"
"	/* Color base del bot\303\263n (lila) */\n"
"	background-color: #a8a8f0; \n"
"	/* Color de texto en estado normal (negro) */    \n"
"	color: #000000; \n"
"  	 /* Ajusta el radio del borde */\n"
"	border-radius: 5px; \n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover:!pressed \n"
"{\n"
"   	/* Color cuando pasas el cursor (morado) */\n"
"	background-color: #6a6ad8; \n"
"	/* Color de texto en hover (blanco) */    \n"
"	color: #ffffff; \n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"	/* Color cuando presionas el bot\303\263n (morado) */\n"
"	background-color: #6a6ad8; \n"
"}"));

        gridLayout->addWidget(Ficha5, 1, 1, 1, 1);

        emptyButton = new QPushButton(centralwidget);
        emptyButton->setObjectName("emptyButton");
        emptyButton->setEnabled(false);
        sizePolicy1.setHeightForWidth(emptyButton->sizePolicy().hasHeightForWidth());
        emptyButton->setSizePolicy(sizePolicy1);
        emptyButton->setStyleSheet(QString::fromUtf8("QPushButton {\n"
"    background-color: transparent; /* Fondo transparente */\n"
"    border: none; /* Sin borde */\n"
"}\n"
""));

        gridLayout->addWidget(emptyButton, 2, 2, 1, 1);

        Ficha7 = new QPushButton(centralwidget);
        Ficha7->setObjectName("Ficha7");
        Ficha7->setEnabled(false);
        sizePolicy1.setHeightForWidth(Ficha7->sizePolicy().hasHeightForWidth());
        Ficha7->setSizePolicy(sizePolicy1);
        Ficha7->setFont(font1);
        Ficha7->setStyleSheet(QString::fromUtf8("QPushButton\n"
" {\n"
"	/* Color base del bot\303\263n (lila) */\n"
"	background-color: #a8a8f0; \n"
"	/* Color de texto en estado normal (negro) */    \n"
"	color: #000000; \n"
"  	 /* Ajusta el radio del borde */\n"
"	border-radius: 5px; \n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover:!pressed \n"
"{\n"
"   	/* Color cuando pasas el cursor (morado) */\n"
"	background-color: #6a6ad8; \n"
"	/* Color de texto en hover (blanco) */    \n"
"	color: #ffffff; \n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"	/* Color cuando presionas el bot\303\263n (morado) */\n"
"	background-color: #6a6ad8; \n"
"}"));

        gridLayout->addWidget(Ficha7, 2, 0, 1, 1);

        Ficha8 = new QPushButton(centralwidget);
        Ficha8->setObjectName("Ficha8");
        Ficha8->setEnabled(false);
        sizePolicy1.setHeightForWidth(Ficha8->sizePolicy().hasHeightForWidth());
        Ficha8->setSizePolicy(sizePolicy1);
        Ficha8->setFont(font1);
        Ficha8->setStyleSheet(QString::fromUtf8("QPushButton\n"
" {\n"
"	/* Color base del bot\303\263n (lila) */\n"
"	background-color: #a8a8f0; \n"
"	/* Color de texto en estado normal (negro) */    \n"
"	color: #000000; \n"
"  	 /* Ajusta el radio del borde */\n"
"	border-radius: 5px; \n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover:!pressed \n"
"{\n"
"   	/* Color cuando pasas el cursor (morado) */\n"
"	background-color: #6a6ad8; \n"
"	/* Color de texto en hover (blanco) */    \n"
"	color: #ffffff; \n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"	/* Color cuando presionas el bot\303\263n (morado) */\n"
"	background-color: #6a6ad8; \n"
"}"));

        gridLayout->addWidget(Ficha8, 2, 1, 1, 1);

        Ficha2 = new QPushButton(centralwidget);
        Ficha2->setObjectName("Ficha2");
        Ficha2->setEnabled(false);
        sizePolicy1.setHeightForWidth(Ficha2->sizePolicy().hasHeightForWidth());
        Ficha2->setSizePolicy(sizePolicy1);
        Ficha2->setFont(font1);
        Ficha2->setStyleSheet(QString::fromUtf8("QPushButton\n"
" {\n"
"	/* Color base del bot\303\263n (lila) */\n"
"	background-color: #a8a8f0; \n"
"	/* Color de texto en estado normal (negro) */    \n"
"	color: #000000; \n"
"  	 /* Ajusta el radio del borde */\n"
"	border-radius: 5px; \n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover:!pressed \n"
"{\n"
"   	/* Color cuando pasas el cursor (morado) */\n"
"	background-color: #6a6ad8; \n"
"	/* Color de texto en hover (blanco) */    \n"
"	color: #ffffff; \n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"	/* Color cuando presionas el bot\303\263n (morado) */\n"
"	background-color: #6a6ad8; \n"
"}"));

        gridLayout->addWidget(Ficha2, 0, 1, 1, 1);

        Ficha3 = new QPushButton(centralwidget);
        Ficha3->setObjectName("Ficha3");
        Ficha3->setEnabled(false);
        sizePolicy1.setHeightForWidth(Ficha3->sizePolicy().hasHeightForWidth());
        Ficha3->setSizePolicy(sizePolicy1);
        Ficha3->setFont(font1);
        Ficha3->setStyleSheet(QString::fromUtf8("QPushButton\n"
" {\n"
"	/* Color base del bot\303\263n (lila) */\n"
"	background-color: #a8a8f0; \n"
"	/* Color de texto en estado normal (negro) */    \n"
"	color: #000000; \n"
"  	 /* Ajusta el radio del borde */\n"
"	border-radius: 5px; \n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover:!pressed \n"
"{\n"
"   	/* Color cuando pasas el cursor (morado) */\n"
"	background-color: #6a6ad8; \n"
"	/* Color de texto en hover (blanco) */    \n"
"	color: #ffffff; \n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"	/* Color cuando presionas el bot\303\263n (morado) */\n"
"	background-color: #6a6ad8; \n"
"}"));

        gridLayout->addWidget(Ficha3, 0, 2, 1, 1);

        Ficha4 = new QPushButton(centralwidget);
        Ficha4->setObjectName("Ficha4");
        Ficha4->setEnabled(false);
        sizePolicy1.setHeightForWidth(Ficha4->sizePolicy().hasHeightForWidth());
        Ficha4->setSizePolicy(sizePolicy1);
        QPalette palette;
        QBrush brush(QColor(0, 0, 0, 255));
        brush.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::WindowText, brush);
        QBrush brush1(QColor(168, 168, 240, 255));
        brush1.setStyle(Qt::SolidPattern);
        palette.setBrush(QPalette::Active, QPalette::Button, brush1);
        palette.setBrush(QPalette::Active, QPalette::Text, brush);
        palette.setBrush(QPalette::Active, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Active, QPalette::Base, brush1);
        palette.setBrush(QPalette::Active, QPalette::Window, brush1);
        QBrush brush2(QColor(0, 0, 0, 128));
        brush2.setStyle(Qt::SolidPattern);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Active, QPalette::PlaceholderText, brush2);
#endif
        palette.setBrush(QPalette::Inactive, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Button, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Text, brush);
        palette.setBrush(QPalette::Inactive, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Inactive, QPalette::Base, brush1);
        palette.setBrush(QPalette::Inactive, QPalette::Window, brush1);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Inactive, QPalette::PlaceholderText, brush2);
#endif
        palette.setBrush(QPalette::Disabled, QPalette::WindowText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Button, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Text, brush);
        palette.setBrush(QPalette::Disabled, QPalette::ButtonText, brush);
        palette.setBrush(QPalette::Disabled, QPalette::Base, brush1);
        palette.setBrush(QPalette::Disabled, QPalette::Window, brush1);
#if QT_VERSION >= QT_VERSION_CHECK(5, 12, 0)
        palette.setBrush(QPalette::Disabled, QPalette::PlaceholderText, brush2);
#endif
        Ficha4->setPalette(palette);
        Ficha4->setFont(font1);
        Ficha4->setStyleSheet(QString::fromUtf8("QPushButton\n"
" {\n"
"	/* Color base del bot\303\263n (lila) */\n"
"	background-color: #a8a8f0; \n"
"	/* Color de texto en estado normal (negro) */    \n"
"	color: #000000; \n"
"  	 /* Ajusta el radio del borde */\n"
"	border-radius: 5px; \n"
"    padding: 5px;\n"
"}\n"
"\n"
"QPushButton:hover:!pressed \n"
"{\n"
"   	/* Color cuando pasas el cursor (morado) */\n"
"	background-color: #6a6ad8; \n"
"	/* Color de texto en hover (blanco) */    \n"
"	color: #ffffff; \n"
"}\n"
"\n"
"QPushButton:pressed\n"
"{\n"
"	/* Color cuando presionas el bot\303\263n (morado) */\n"
"	background-color: #6a6ad8; \n"
"}"));

        gridLayout->addWidget(Ficha4, 1, 0, 1, 1);


        verticalLayout_2->addLayout(gridLayout);

        verticalSpacer_2 = new QSpacerItem(5, 5, QSizePolicy::Policy::Minimum, QSizePolicy::Policy::Fixed);

        verticalLayout_2->addItem(verticalSpacer_2);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setObjectName("horizontalLayout_5");
        resetButton = new QPushButton(centralwidget);
        resetButton->setObjectName("resetButton");
        sizePolicy.setHeightForWidth(resetButton->sizePolicy().hasHeightForWidth());
        resetButton->setSizePolicy(sizePolicy);
        resetButton->setMinimumSize(QSize(80, 30));
        resetButton->setBaseSize(QSize(0, 0));
        resetButton->setFont(font);
        resetButton->setLayoutDirection(Qt::LayoutDirection::LeftToRight);
        resetButton->setStyleSheet(QString::fromUtf8("QPushButton:hover {\n"
"    border: 2px solid #5a5a5a; /* Borde al pasar el cursor */\n"
"    border-radius: 4px; /* Ajuste para bordes redondeados */\n"
"}"));

        horizontalLayout_5->addWidget(resetButton);


        verticalLayout_2->addLayout(horizontalLayout_5);


        verticalLayout->addLayout(verticalLayout_2);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 600, 21));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "8 Puzzle", nullptr));
        counter->setText(QCoreApplication::translate("MainWindow", "ESTADO SOLUCI\303\223N", nullptr));
        Ficha1->setText(QCoreApplication::translate("MainWindow", "1", nullptr));
        Ficha6->setText(QCoreApplication::translate("MainWindow", "6", nullptr));
        Ficha5->setText(QCoreApplication::translate("MainWindow", "5", nullptr));
        emptyButton->setText(QString());
        Ficha7->setText(QCoreApplication::translate("MainWindow", "7", nullptr));
        Ficha8->setText(QCoreApplication::translate("MainWindow", "8", nullptr));
        Ficha2->setText(QCoreApplication::translate("MainWindow", "2", nullptr));
        Ficha3->setText(QCoreApplication::translate("MainWindow", "3", nullptr));
        Ficha4->setText(QCoreApplication::translate("MainWindow", "4", nullptr));
        resetButton->setText(QCoreApplication::translate("MainWindow", "Start", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
