﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

// 0 - hello screen
// 11 - add city screen
// 12 - add store screen
// 13 - add product screen
ushort tab = 0;
void ** Start = InitArray();

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    makeInvisible();
    ui->header->setVisible(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::refreshTreeView()
{
    int cnt=((int*)Start)[POS_CNT];
    ui->treeWidget->clear();

    QTreeWidgetItem *topItem = new QTreeWidgetItem();
    topItem->setText(0,"Ukraine");

    for (int i = 0; i <cnt; i++) // loop
    {   // output third level
        QTreeWidgetItem * itm = new QTreeWidgetItem();

        itm->setText(0, ((TCity*)Start[i])->name + "\n" + ((TCity*)Start[i])->region + "\n" + QString::number(((TCity*)Start[i])->postcode));

        void ** s = (void**)(((TCity*)Start[i])->sublev); // go to the next level
        for (int j = 0; j < ((int*)(s))[POS_CNT];j++) // loop
        {   // output
            QTreeWidgetItem * store = new QTreeWidgetItem();

            store->setText(0, ((TStore*)s[j])->name + "\n" + ((TStore*)s[j])->adress + "\n" + (((TStore*)s[j])->phnumber));

            store->setExpanded(true);
            itm->addChild(store);

            void ** p = (void**)(((TStore*)s[j])->sublev); // go to the next level
            for (int k = 0; k < ((int*)(p))[POS_CNT];k++) // loop
            {
                QTreeWidgetItem * product = new QTreeWidgetItem();

                product->setText(0, ((TProduct*)p[k])->name + "\n" + QString::number(((TProduct*)p[k])->id) + "\n" + (((TProduct*)p[k])->category) + "\n" + ((TProduct*)p[k])->description + "\n" + QString::number(((TProduct*)p[k])->count) + "\n" + QString::number(((TProduct*)p[k])->price) + "$");

                product->setExpanded(true);
                store->addChild(product);
            }
        }
        topItem->addChild(itm);
        topItem->setExpanded(true);
        ui->treeWidget->addTopLevelItem(topItem);
    }
}


void MainWindow::makeInvisible()
{
    ui->header->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->treeWidget->setVisible(false);
    ui->subheader->setVisible(false);
    ui->lineEdit_2->setVisible(false);
    ui->lineEdit_3->setVisible(false);
    ui->lineEdit_4->setVisible(false);
    ui->lineEdit_5->setVisible(false);
    ui->lineEdit_6->setVisible(false);
    ui->comboBox->setVisible(false);
    ui->comboBox_2->setVisible(false);

    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
}


void MainWindow::on_action_6_triggered()
{
    makeInvisible();
    ui->subheader->setVisible(true);
    ui->lineEdit->setVisible(true);
    ui->pushButton->setVisible(true);
}

void MainWindow::on_pushButton_clicked()
{
    if ((tab == 12 || tab == 13) && (ui->comboBox->currentText() == "" || ui->comboBox_2->currentText() == ""))
    {
        QMessageBox *message = new QMessageBox(QMessageBox::Warning, "Warning",  "Створіть, будь ласка, елементи на попередніх етапах, аби мати можливість працювати з ними на цьому!");
        message->setStandardButtons(QMessageBox::Ok);
        message->exec();
        return;
    }
    if (tab == 11)
    {
        void *city = new TCity;
        ((TCity*)city)->name = ui->lineEdit_2->text();
        ((TCity*)city)->region = ui->lineEdit_3->text();
        ((TCity*)city)->postcode = ui->lineEdit_4->text().toUInt();
        ((TCity*)city)->sublev = InitArray();

        int pos;bool found;
        FindElList(Start,((TCity*)city)->name,pos,found,cmpCity);
        addToSort(Start,city, pos);
    }

    if (tab == 12)
    {
        bool fndCity; int fndPos;
        FindElList(Start, ui->comboBox->currentText(),fndPos,fndCity,cmpCity);

        void * store = new TStore;
        ((TStore *)store)->name = ui->lineEdit_2->text();
        ((TStore *)store)->adress = ui->lineEdit_3->text();
        ((TStore *)store)->phnumber = ui->lineEdit_4->text();
        ((TStore *)store)->sublev = InitArray();

        int pos;bool found;
        FindElList(((TCity*)Start[fndPos])->sublev,((TStore*)store)->name,pos,found,cmpStore);
        addToSort(((TCity*)Start[fndPos])->sublev,store, pos);
    }

    if (tab == 13)
    {
        bool fndCity; int fndPos;
        FindElList(Start, ui->comboBox->currentText(),fndPos,fndCity,cmpCity);

        int pos;bool found;
        void ** s = (void**)(((TCity*)Start[fndPos])->sublev);
        FindElList(s,ui->comboBox_2->currentText(),pos,found,cmpStore);

        void * product = new TProduct;
        ((TProduct *)product)->name = ui->lineEdit_2->text();
        ((TProduct *)product)->id = ui->lineEdit_3->text().toUShort();
        ((TProduct *)product)->category = ui->lineEdit_4->text();
        ((TProduct *)product)->description = ui->lineEdit->text();
        ((TProduct *)product)->count = ui->lineEdit_5->text().toUShort();
        ((TProduct *)product)->price = ui->lineEdit_6->text().toFloat();

        int posProduct; bool fndProduct;
        FindElList(((TStore*)(s[pos]))->sublev, ((TProduct *)product)->name, posProduct, fndProduct, cmpProduct);

        addToSort(((TStore*)s[pos])->sublev, product, posProduct);
    }

    refreshTreeView();
}

void MainWindow::on_action_7_triggered()
{
    makeInvisible();
    ui->treeWidget->setVisible(true);
    ui->subheader->setVisible(true);
    ui->subheader->setText("Укажіть необхідні дані");

    tab = 11;

    ui->lineEdit_2->setVisible(true);
    ui->lineEdit_2->setPlaceholderText("назва міста");
    ui->lineEdit_3->setVisible(true);
    ui->lineEdit_3->setPlaceholderText("регіон");
    ui->lineEdit_4->setVisible(true);
    ui->lineEdit_4->setPlaceholderText("поштовий індекс");

    ui->pushButton->setVisible(true);
    ui->pushButton->setText("Додати");
}

void MainWindow::on_action_8_triggered()
{
    makeInvisible();
    ui->treeWidget->setVisible(true);
    ui->subheader->setVisible(true);
    ui->subheader->setText("Укажіть необхідні дані");

    tab = 12;

    ui->comboBox->setVisible(true);
    ui->lineEdit_2->setVisible(true);
    ui->lineEdit_2->setPlaceholderText("назва магазину");
    ui->lineEdit_3->setVisible(true);
    ui->lineEdit_3->setPlaceholderText("адреса");
    ui->lineEdit_4->setVisible(true);
    ui->lineEdit_4->setPlaceholderText("мобільний телефон");

    for (int i = 0; i < ((int*)Start)[POS_CNT]; i++)
    {
        ui->comboBox->insertItem(i,QString(((TCity*)Start[i])->name));
    }

    ui->pushButton->setVisible(true);
    ui->pushButton->setText("Додати");
}

void MainWindow::on_action_9_triggered()
{
    makeInvisible();
    ui->treeWidget->setVisible(true);
    ui->subheader->setVisible(true);
    ui->subheader->setText("Укажіть необхідні дані");

    tab = 13;

    ui->comboBox->setVisible(true);
    ui->comboBox_2->setVisible(true);
    ui->lineEdit_2->setVisible(true);
    ui->lineEdit_2->setPlaceholderText("назва товару");
    ui->lineEdit_3->setVisible(true);
    ui->lineEdit_3->setPlaceholderText("id");
    ui->lineEdit_4->setVisible(true);
    ui->lineEdit_4->setPlaceholderText("категорія");
    ui->lineEdit->setVisible(true);
    ui->lineEdit->setPlaceholderText("короткий опис");
    ui->lineEdit_5->setVisible(true);
    ui->lineEdit_5->setPlaceholderText("кількість товару на складі");
    ui->lineEdit_6->setVisible(true);
    ui->lineEdit_6->setPlaceholderText("ціна з 1 шт у долярах");

    for (int i = 0; i < ((int*)Start)[POS_CNT]; i++)
    {
        ui->comboBox->insertItem(i,QString(((TCity*)Start[i])->name));
    }

    ui->pushButton->setVisible(true);
    ui->pushButton->setText("Додати");
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    int pos; bool fnd;
    FindElList(Start,arg1,pos,fnd,cmpCity);
    if (tab == 13)
    {
        void ** s = (void**)(((TCity*)Start[pos])->sublev);
        for (int i = 0; i < ((int*)(s))[POS_CNT]; i++)
        {
            ui->comboBox_2->insertItem(i, ((TStore*)(s[i]))->name);
        }
    }
}
