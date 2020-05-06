/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.5
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *action;
    QAction *action_2;
    QAction *action_3;
    QAction *action_5;
    QAction *action_6;
    QAction *action_7;
    QAction *action_8;
    QAction *action_9;
    QWidget *centralwidget;
    QHBoxLayout *horizontalLayout;
    QGridLayout *gridLayout_2;
    QTreeWidget *treeWidget;
    QVBoxLayout *verticalLayout;
    QLabel *header;
    QLabel *subheader;
    QComboBox *comboBox;
    QComboBox *comboBox_2;
    QLineEdit *lineEdit_2;
    QLineEdit *lineEdit_3;
    QLineEdit *lineEdit_4;
    QLineEdit *lineEdit;
    QLineEdit *lineEdit_5;
    QLineEdit *lineEdit_6;
    QPushButton *pushButton;
    QMenuBar *menubar;
    QMenu *menunfdnfdnfd;
    QMenu *menu;
    QMenu *menu_2;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(932, 600);
        action = new QAction(MainWindow);
        action->setObjectName(QString::fromUtf8("action"));
        action_2 = new QAction(MainWindow);
        action_2->setObjectName(QString::fromUtf8("action_2"));
        action_3 = new QAction(MainWindow);
        action_3->setObjectName(QString::fromUtf8("action_3"));
        action_5 = new QAction(MainWindow);
        action_5->setObjectName(QString::fromUtf8("action_5"));
        action_6 = new QAction(MainWindow);
        action_6->setObjectName(QString::fromUtf8("action_6"));
        action_7 = new QAction(MainWindow);
        action_7->setObjectName(QString::fromUtf8("action_7"));
        action_8 = new QAction(MainWindow);
        action_8->setObjectName(QString::fromUtf8("action_8"));
        action_9 = new QAction(MainWindow);
        action_9->setObjectName(QString::fromUtf8("action_9"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        horizontalLayout = new QHBoxLayout(centralwidget);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        gridLayout_2 = new QGridLayout();
        gridLayout_2->setObjectName(QString::fromUtf8("gridLayout_2"));
        treeWidget = new QTreeWidget(centralwidget);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QString::fromUtf8("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QString::fromUtf8("treeWidget"));

        gridLayout_2->addWidget(treeWidget, 0, 0, 1, 1);


        horizontalLayout->addLayout(gridLayout_2);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        header = new QLabel(centralwidget);
        header->setObjectName(QString::fromUtf8("header"));
        QFont font;
        font.setPointSize(36);
        header->setFont(font);
        header->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(header);

        subheader = new QLabel(centralwidget);
        subheader->setObjectName(QString::fromUtf8("subheader"));
        subheader->setEnabled(true);
        QFont font1;
        font1.setPointSize(24);
        subheader->setFont(font1);
        subheader->setStyleSheet(QString::fromUtf8("hide:true"));
        subheader->setAlignment(Qt::AlignCenter);

        verticalLayout->addWidget(subheader);

        comboBox = new QComboBox(centralwidget);
        comboBox->setObjectName(QString::fromUtf8("comboBox"));
        QFont font2;
        font2.setPointSize(14);
        comboBox->setFont(font2);

        verticalLayout->addWidget(comboBox);

        comboBox_2 = new QComboBox(centralwidget);
        comboBox_2->setObjectName(QString::fromUtf8("comboBox_2"));
        comboBox_2->setFont(font2);

        verticalLayout->addWidget(comboBox_2);

        lineEdit_2 = new QLineEdit(centralwidget);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        lineEdit_2->setFont(font2);

        verticalLayout->addWidget(lineEdit_2);

        lineEdit_3 = new QLineEdit(centralwidget);
        lineEdit_3->setObjectName(QString::fromUtf8("lineEdit_3"));
        lineEdit_3->setFont(font2);

        verticalLayout->addWidget(lineEdit_3);

        lineEdit_4 = new QLineEdit(centralwidget);
        lineEdit_4->setObjectName(QString::fromUtf8("lineEdit_4"));
        lineEdit_4->setFont(font2);

        verticalLayout->addWidget(lineEdit_4);

        lineEdit = new QLineEdit(centralwidget);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        lineEdit->setFont(font2);

        verticalLayout->addWidget(lineEdit);

        lineEdit_5 = new QLineEdit(centralwidget);
        lineEdit_5->setObjectName(QString::fromUtf8("lineEdit_5"));
        lineEdit_5->setFont(font2);

        verticalLayout->addWidget(lineEdit_5);

        lineEdit_6 = new QLineEdit(centralwidget);
        lineEdit_6->setObjectName(QString::fromUtf8("lineEdit_6"));
        lineEdit_6->setFont(font2);

        verticalLayout->addWidget(lineEdit_6);

        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        QFont font3;
        font3.setPointSize(12);
        pushButton->setFont(font3);

        verticalLayout->addWidget(pushButton);


        horizontalLayout->addLayout(verticalLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 932, 22));
        menunfdnfdnfd = new QMenu(menubar);
        menunfdnfdnfd->setObjectName(QString::fromUtf8("menunfdnfdnfd"));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menu);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menunfdnfdnfd->menuAction());
        menubar->addAction(menu->menuAction());
        menunfdnfdnfd->addAction(action_6);
        menunfdnfdnfd->addAction(action);
        menunfdnfdnfd->addAction(action_2);
        menu->addAction(action_3);
        menu->addAction(menu_2->menuAction());
        menu->addAction(action_5);
        menu_2->addAction(action_7);
        menu_2->addAction(action_8);
        menu_2->addAction(action_9);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "QWESDA", nullptr));
        action->setText(QApplication::translate("MainWindow", "\320\222\321\226\320\264\320\272\321\200\320\270\321\202\320\270", nullptr));
        action_2->setText(QApplication::translate("MainWindow", "\320\227\320\261\320\265\321\200\320\265\320\263\321\202\320\270", nullptr));
        action_3->setText(QApplication::translate("MainWindow", "\320\237\320\276\321\210\321\203\320\272", nullptr));
        action_5->setText(QApplication::translate("MainWindow", "\320\222\320\270\320\264\320\260\320\273\320\270\321\202\320\270", nullptr));
        action_6->setText(QApplication::translate("MainWindow", "\320\241\321\202\320\262\320\276\321\200\320\270\321\202\320\270 \320\275\320\276\320\262\320\270\320\271", nullptr));
        action_7->setText(QApplication::translate("MainWindow", "\320\234\321\226\321\201\321\202\320\276", nullptr));
        action_8->setText(QApplication::translate("MainWindow", "\320\234\320\260\320\263\320\260\320\267\320\270\320\275", nullptr));
        action_9->setText(QApplication::translate("MainWindow", "\320\242\320\276\320\262\320\260\321\200", nullptr));
        header->setText(QApplication::translate("MainWindow", "\320\224\320\273\321\217 \320\277\320\276\321\207\320\260\321\202\320\272\321\203 \321\200\320\276\320\261\320\276\321\202\320\270 \321\201\321\202\320\262\320\276\321\200\321\226\321\202\321\214 \320\275\320\276\320\262\320\270\320\271\n"
"\320\260\320\261\320\276 \320\262\321\226\320\264\320\272\321\200\320\270\320\271\321\202\320\265 \321\226\321\201\320\275\321\203\321\216\321\207\320\270\320\271 \321\204\320\260\320\271\320\273", nullptr));
        subheader->setText(QApplication::translate("MainWindow", "\320\222\320\272\320\260\320\266\321\226\321\202\321\214 \321\226\320\274'\321\217 \321\204\320\260\320\271\320\273\321\203", nullptr));
        lineEdit_2->setPlaceholderText(QApplication::translate("MainWindow", "\320\275\320\260\320\267\320\262\320\260", nullptr));
        lineEdit_3->setPlaceholderText(QApplication::translate("MainWindow", "\321\200\320\265\320\263\321\226\320\276\320\275", nullptr));
        lineEdit_4->setPlaceholderText(QApplication::translate("MainWindow", "\320\277\320\276\321\210\321\202\320\276\320\262\320\270\320\271 \321\226\320\275\320\264\320\265\320\272\321\201", nullptr));
        pushButton->setText(QApplication::translate("MainWindow", "\320\241\321\202\320\262\320\276\321\200\320\270\321\202\320\270", nullptr));
        menunfdnfdnfd->setTitle(QApplication::translate("MainWindow", "\320\244\320\260\320\271\320\273", nullptr));
        menu->setTitle(QApplication::translate("MainWindow", "\320\240\320\276\320\261\320\276\321\202\320\260", nullptr));
        menu_2->setTitle(QApplication::translate("MainWindow", "\320\224\320\276\320\264\320\260\321\202\320\270", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
