﻿#include "mainwindow.h"
#include "ui_mainwindow.h"

// 0 - hello screen
// 11 - add city screen
// 12 - add store screen
// 13 - add product screen
// 20 - remove element screen
// 31 - edit city
// 32 - edit store
// 33 - edit product
// 40 - search screen
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

    // create top item pointer
    QTreeWidgetItem *topItem = new QTreeWidgetItem();
    topItem->setText(0,"Ukraine");

    // configure headers
    QStringList headers;
    headers.append("країна");
    headers.append("місто");
    headers.append("магазин");
    headers.append("товар");

    // set headers
    ui->treeWidget->setHeaderLabels(headers);

    for (int i = 0; i <cnt; i++) // loop
    {   // output third level
        QTreeWidgetItem * itm = new QTreeWidgetItem();
        // setting text to item
        itm->setText(1, ((TCity*)Start[i])->name + "\n" + ((TCity*)Start[i])->region + "\n" + QString::number(((TCity*)Start[i])->postcode));

        void ** s = (void**)(((TCity*)Start[i])->sublev); // go to the next level
        for (int j = 0; j < ((int*)(s))[POS_CNT];j++) // loop
        {   // store item
            QTreeWidgetItem * store = new QTreeWidgetItem();
            // set text
            store->setText(2, ((TStore*)s[j])->name + "\n" + ((TStore*)s[j])->adress + "\n" + (((TStore*)s[j])->phnumber));
            // make it expanded
            store->setExpanded(true);
            itm->addChild(store); // add child

            void ** p = (void**)(((TStore*)s[j])->sublev); // go to the next level
            for (int k = 0; k < ((int*)(p))[POS_CNT];k++) // loop
            {   // create product item
                QTreeWidgetItem * product = new QTreeWidgetItem();
                // set text to it
                product->setText(3, ((TProduct*)p[k])->name + "\n" + QString::number(((TProduct*)p[k])->id) + "\n" + (((TProduct*)p[k])->category) + "\n" + ((TProduct*)p[k])->description + "\n" + QString::number(((TProduct*)p[k])->count) + "\n" + QString::number(((TProduct*)p[k])->price) + "$");
                // make it expanded
                product->setExpanded(true);
                store->addChild(product); // add child
            }
        }
        // add child to the top item
        topItem->addChild(itm);
        topItem->setExpanded(true);
        ui->treeWidget->addTopLevelItem(topItem);
    }
}


// function for hiding all the items on the scene
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
    ui->comboBox_3->setVisible(false);
    ui->tableWidget->setVisible(false);

    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->tableWidget->clear();
}


// create new file tab
void MainWindow::on_action_6_triggered()
{
    makeInvisible();
    ui->subheader->setVisible(true);
    ui->lineEdit->setVisible(true);
    ui->pushButton->setVisible(true);
}


// the main button on the scene
void MainWindow::on_pushButton_clicked()
{
    // if it's about adding new items
    if (((tab == 12 && ui->comboBox->currentText() == "")) || (tab == 13 && ((ui->comboBox->currentText() == "") || (ui->comboBox_2->currentText() == ""))))
    {   // warning message
        QMessageBox *message = new QMessageBox(QMessageBox::Warning, "Warning",  "Створіть, будь ласка, елементи на попередніх етапах, аби мати можливість працювати з ними на цьому!");
        message->setStandardButtons(QMessageBox::Ok);
        message->exec();
        return;
    }
    // common variables
    int pos; bool fnd;
    // if we want to add a new city
    if (tab == 11)
    {   // request memory and fill the data
        void *city = new TCity;
        ((TCity*)city)->name = ui->lineEdit_2->text();
        ((TCity*)city)->region = ui->lineEdit_3->text();
        ((TCity*)city)->postcode = ui->lineEdit_4->text().toUInt();
        ((TCity*)city)->sublev = InitArray();
        // add item to sort list
        FindElList(Start,((TCity*)city)->name,pos,fnd,cmpCity);
        addToSort(Start,city, pos);
    }
    // if we want to add new store
    if (tab == 12)
    {   // find city
        FindElList(Start, ui->comboBox->currentText(),pos,fnd,cmpCity);
        // request memory
        void * store = new TStore;
        ((TStore *)store)->name = ui->lineEdit_2->text();
        ((TStore *)store)->adress = ui->lineEdit_3->text();
        ((TStore *)store)->phnumber = ui->lineEdit_4->text();
        ((TStore *)store)->sublev = InitArray();
        // finding the place and adding
        int ps;bool fnd;
        FindElList(((TCity*)Start[pos])->sublev,((TStore*)store)->name,ps,fnd,cmpStore);
        addToSort(((TCity*)Start[pos])->sublev,store, ps);
    }
    // if we'd like to add new product
    if (tab == 13)
    {   //find city
        FindElList(Start, ui->comboBox->currentText(),pos,fnd,cmpCity);
        // find store
        int poss;bool found;
        void ** s = (void**)(((TCity*)Start[pos])->sublev);
        FindElList(s,ui->comboBox_2->currentText(),poss,found,cmpStore);
        // request memory
        void * product = new TProduct;
        ((TProduct *)product)->name = ui->lineEdit_2->text();
        ((TProduct *)product)->id = ui->lineEdit_3->text().toUShort();
        ((TProduct *)product)->category = ui->lineEdit_4->text();
        ((TProduct *)product)->description = ui->lineEdit->text();
        ((TProduct *)product)->count = ui->lineEdit_5->text().toUShort();
        ((TProduct *)product)->price = ui->lineEdit_6->text().toFloat();
        // add to sorted
        int posProduct; bool fndProduct;
        FindElList(((TStore*)(s[poss]))->sublev, ((TProduct *)product)->name, posProduct, fndProduct, cmpProduct);
        addToSort(((TStore*)s[poss])->sublev, product, posProduct);
    }
    // if we'd like to delete some stuff
    if (tab == 20)
    {   // if we'd like to delete city
        if (ui->comboBox_2->currentText() == "null")
        {   // find if and delete, then clear the combobox
            FindElList(Start, ui->comboBox->currentText(),pos, fnd, cmpCity);
            delEl(Start, pos, 3);
            ui->comboBox->removeItem(ui->comboBox->currentIndex());
        }// if we'd like to delete store
        else if (ui->comboBox_3->currentText() == "null")
        {   // find city and store, then delete store
            FindElList(Start, ui->comboBox->currentText(),pos, fnd, cmpCity);
            int spos;bool sfound;
            FindElList(((TCity*)Start[pos])->sublev, ui->comboBox_2->currentText(), spos, sfound, cmpStore);
            delEl(((TCity*)Start[pos])->sublev, spos, 2);
            ui->comboBox_2->removeItem(ui->comboBox_2->currentIndex());
        }
        else
        {   // search for city & store & product, then delete product
            FindElList(Start, ui->comboBox->currentText(),pos, fnd, cmpCity);
            int spos;bool sfound;
            FindElList(((TCity*)Start[pos])->sublev, ui->comboBox_2->currentText(), spos, sfound, cmpStore);
            int ppos;bool pfound;
            FindElList(((TStore*)(((TCity*)Start[pos])->sublev[spos]))->sublev, ui->comboBox_3->currentText(), ppos, pfound, cmpProduct);
            delEl(((TStore*)(((TCity*)Start[pos])->sublev[spos]))->sublev, ppos, 1);
            ui->comboBox_3->removeItem(ui->comboBox_3->currentIndex());
        }
    } // if we'd like to edit city
    if (tab == 31)
    {   // search for a city and editing corresponding pointer
        QStringList item = ui->treeWidget->currentItem()->text(ui->treeWidget->currentColumn()).split("\n");
        FindElList(Start, item[0], pos, fnd, cmpCity);
        ((TCity*)Start[pos])->name = ui->lineEdit_2->text();
        ((TCity*)Start[pos])->region = ui->lineEdit_3->text();
        ((TCity*)Start[pos])->postcode = ui->lineEdit_4->text().toUInt();
    }// if we'd like to edit store
    if (tab == 32)
    {   // search for city and store
        QStringList item = ui->treeWidget->currentItem()->text(ui->treeWidget->currentColumn()).split("\n");
        FindElList(Start, item[0],pos, fnd, cmpCity);
        void ** s = (void**)(((TCity*)Start[pos])->sublev); // go to the next level
        int spos;bool sfound;
        FindElList(((TCity*)Start[pos])->sublev, ui->lineEdit_2->text(), spos, sfound, cmpStore);
        // edit this element
        ((TStore*)s[spos])->name = ui->lineEdit_2->text();
        ((TStore*)s[spos])->adress = ui->lineEdit_3->text();
        ((TStore*)s[spos])->phnumber = ui->lineEdit_4->text();

    }// if we'd like to edit product
    if (tab == 33)
    {   // search for city, store & product
        QStringList item = ui->treeWidget->currentItem()->text(ui->treeWidget->currentColumn()).split("\n");

        FindElList(Start, ui->comboBox->currentText(),pos, fnd, cmpCity);
        int spos;bool sfound;

        FindElList(((TCity*)Start[pos])->sublev, ui->comboBox_2->currentText(), spos, sfound, cmpStore);
        int ppos;bool pfound;
        FindElList(((TStore*)(((TCity*)Start[pos])->sublev[spos]))->sublev, ui->lineEdit_2->text(), ppos, pfound, cmpProduct);

        void ** p = ((TStore*)(((TCity*)Start[pos])->sublev[spos]))->sublev;
        // edit product
        ((TProduct *)p[ppos])->name = ui->lineEdit_2->text();
        ((TProduct *)p[ppos])->id = ui->lineEdit_3->text().toUShort();
        ((TProduct *)p[ppos])->category = ui->lineEdit_4->text();
        ((TProduct *)p[ppos])->description = ui->lineEdit->text();
        ((TProduct *)p[ppos])->count = ui->lineEdit_5->text().toUShort();
        ((TProduct *)p[ppos])->price = ui->lineEdit_6->text().toFloat();
    }// if we're on the editing screen
    if (tab == 31 || tab == 32 || tab == 33)
    {
        makeInvisible();
        ui->treeWidget->setVisible(true);
    }

    refreshTreeView();
    // if we'd like to find some things
    if (tab == 40)
    {   // make treewidget visible
        ui->treeWidget->setVisible(true);
        QList<QTreeWidgetItem*> found;
        // request memory for top item
        QTreeWidgetItem *topItem = new QTreeWidgetItem();
        topItem->setText(0,"Ukraine");
        // make searchquery
        QString searchquery = ui->lineEdit_2->text() + "\n" + ui->lineEdit_3->text() + "\n" + ui->lineEdit_4->text();

        // if we're searching for a city
        if (ui->comboBox->currentText() == "Місто")
        {   // find items
            found = ui->treeWidget->findItems(searchquery, Qt::MatchContains | Qt::MatchRecursive, 1);
//            for (int i = 0; i  < found.length(); i++)
//                found[i]->parent()->removeChild(found[i]);
//            topItem->addChildren(found);
            // removing childs
            for (int i = 0; i < found.length();i++)
            {
                found[i]->parent()->removeChild(found[i]);
                topItem->addChild(found[i]);
            }
        } // if we'd like to find a store
        if (ui->comboBox->currentText() == "Магазин")
        {   // finding result
            found = ui->treeWidget->findItems(searchquery, Qt::MatchContains | Qt::MatchRecursive ,2);
            // removing childs
            for (int i = 0; i < found.length();i++)
            {
                found[i]->parent()->parent()->removeChild(found[i]->parent());
                topItem->addChild(found[i]->parent());
            }
//            for (int i = 0; i  < found.length(); i++)
//            {
//                found[i]->parent()->parent()->removeChild(found[i]->parent());
//                for (int j = 0; j < found[i]->parent()->childCount();j++)
//                {
//                    if (found[i]->parent()->child(j) != found[i])
//                    {
//                        found[i]->parent()->takeChild(j);
//                        j--;
//                    }
//                }
//            }
//            for (int i = 0; i < found.length();i++)
//                topItem->addChild(found[i]->parent());
        } // if we'd like to find a product
        if (ui->comboBox->currentText() == "Товар")
        {   // generate searchquery and search
            searchquery += "\n" + ui->lineEdit->text() + "\n" + ui->lineEdit_5->text() + "\n" + ui->lineEdit_6->text();
            found =  ui->treeWidget->findItems(searchquery, Qt::MatchContains | Qt::MatchRecursive ,3);

//            for (int i = 0; i  < found.length(); i++)
//            {
//                found[i]->parent()->parent()->parent()->removeChild(found[i]->parent()->parent());
//                for (int j = 0; j < found[i]->parent()->childCount();j++)
//                {
//                    if (found.indexOf(found[i]->parent()->child(j)) == -1)
//                    {
//                        found[i]->parent()->takeChild(j);
//                        j--;
//                    }
//                }
//                for (int j = 0; j < found[i]->parent()->parent()->childCount();j++)
//                {
//                    if (found[i]->parent()->indexOfChild(found[i]->parent()->parent()->child(j)) == -1)
//                    {
//                        found[i]->parent()->parent()->takeChild(j);
//                        j--;
//                    }
//                }
//            }

            // V 2
//            QTreeWidgetItem * itm = new QTreeWidgetItem();
//            for (int i = 0; i < found.length();i++)
//            {
//                QTreeWidgetItem * city = found[i]->parent()->parent(), *store = found[i]->parent();
//                itm->addChild(city);
//                itm->child(itm->childCount())->addChild(store);
//                itm->child(itm->childCount())->child(itm->child(itm->childCount())->childCount())->addChild(found[i]);
//                topItem->addChild(itm);
//            }
            //v 3

            // removing children
            for (int i = 0; i < found.length();i++)
            {
                found[i]->parent()->parent()->parent()->removeChild(found[i]->parent()->parent());
                topItem->addChild(found[i]->parent()->parent());
            }
        }
        // taking top level items
        for (int i = 0; i < ui->treeWidget->topLevelItemCount();i++)
            ui->treeWidget->takeTopLevelItem(i);
        // adding top level items
        ui->treeWidget->addTopLevelItem(topItem);
    }
}

// if we'd like to add a new city
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

// if we'd like to add new store
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

    // fill the combobox item
    for (int i = 0; i < ((int*)Start)[POS_CNT]; i++)
    {
        ui->comboBox->insertItem(i,QString(((TCity*)Start[i])->name));
    }

    ui->pushButton->setVisible(true);
    ui->pushButton->setText("Додати");
}

// if we'd like to add new product
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

    // fill the combobox item
    for (int i = 0; i < ((int*)Start)[POS_CNT]; i++)
    {
        ui->comboBox->insertItem(i,QString(((TCity*)Start[i])->name));
    }

    ui->pushButton->setVisible(true);
    ui->pushButton->setText("Додати");
}


// if we'd like to remove some items
void MainWindow::on_action_5_triggered()
{
    makeInvisible();

    tab = 20;

    ui->comboBox->setVisible(true);
    ui->comboBox_2->setVisible(true);
    ui->comboBox_3->setVisible(true);

    ui->treeWidget->setVisible(true);
    ui->comboBox->clear();
    // filling the combobox
    for (int i = 0; i < ((int*)Start)[POS_CNT]; i++)
    {
        ui->comboBox->insertItem(i,QString(((TCity*)Start[i])->name));
    }

    ui->pushButton->setVisible(true);
    ui->pushButton->setText("Вилучити");
}


// combobox event
void MainWindow::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    int pos; bool fnd;
    ui->comboBox_3->clear();
    // searching for a city
    FindElList(Start,ui->comboBox->currentText(),pos,fnd,cmpCity);
    if (tab == 20)
    {   // if the city was found
        if (fnd)
        {   // go to it's stores
            void ** s = (void**)(((TCity*)Start[pos])->sublev); // search for a corresponding store
            FindElList(s,ui->comboBox_2->currentText(), pos,fnd, cmpStore);
            if (fnd) // if it's found
            {   // go to it's products
                void ** p = (void**)(((TStore*)s[pos])->sublev);
                for (int i = 0; i < ((int*)(p))[POS_CNT]; i++)
                {   // inserting elements
                    ui->comboBox_3->insertItem(i, ((TProduct*)(p[i]))->name);
                }
                // add special field
                ui->comboBox_3->addItem("null");
            }
        }
    }
}


//mode 1 - product
//mode 2 - store
//mode 3 - city
void delEl(void** &start,  int posFnd, int mode)
{   // if we'd like to delete city
    if (mode == 3)
    {   // go to stores
        void ** s = (((TCity*)start[posFnd])->sublev);
        for (int j = 0; j < ((int*)(s))[POS_CNT];j++)
        {   // go to products
            void * * p = (((TStore*)s[j])->sublev);
            for (int k = 0; k < ((int*)(p))[POS_CNT];k++)
            {   // delete each product
                delete ((TProduct*)p[k]);
            } // delete each store
            delete (TStore*)s[j];
        }
    } // if we'd like to delete a store
    else if (mode == 2)
    {   // go to products
        void * * p = (((TStore*)start[posFnd])->sublev);
        for (int k = 0; k < ((int*)(p))[POS_CNT];k++)
        {   // delete product
            delete ((TProduct*)p[k]);
        }
    }
    // if there's only one element
    if (1 == ((int*)start)[POS_CNT])
    {   // delete it & init again
        delete *start;
        start = InitArray();
        return;
    }
    // moving items
    for (int i = posFnd; i < ((int*)start)[POS_CNT];i++)
    {
        ((void**)start)[i] = ((void**)start)[i+1];
    } // decrease size
    ((int*)start)[POS_CNT]--;
}


 // treewidget event
void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{   // if it's empty element
    if (item->text(column) == "")
    {   // show message
        QMessageBox *message = new QMessageBox(QMessageBox::Warning, "Warning",  "Спробуйте влучити по елементу");
        message->setStandardButtons(QMessageBox::Ok);
        message->exec();
        return;
    } // if it's root
    if (column == 0)
    {   // show message
        QMessageBox *message = new QMessageBox(QMessageBox::Warning, "Warning",  "Кореневий елемент не можна змінювати!");
        message->setStandardButtons(QMessageBox::Ok);
        message->exec();
        return;
    }
    makeInvisible();
    ui->pushButton->setVisible(true);
    ui->treeWidget->setVisible(true);

    ui->subheader->setVisible(true);
    ui->subheader->setText("Укажіть необхідні дані");

    ui->lineEdit_2->setVisible(true);
    ui->lineEdit_3->setVisible(true);
    ui->lineEdit_4->setVisible(true);
    // get the data
    QStringList data = item->text(column).split("\n");
    // if it's the first column
    if (column == 1)
    {   // change tab index
        tab = 31;
        // fill the data
        ui->lineEdit_2->setPlaceholderText("назва міста");
        ui->lineEdit_2->setText(data[0]);
        ui->lineEdit_3->setPlaceholderText("регіон");
        ui->lineEdit_3->setText(data[1]);
        ui->lineEdit_4->setPlaceholderText("поштовий індекс");
        ui->lineEdit_4->setText(data[2]);
    } // if it's the second column
    else if (column == 2)
    {   // change tab index
        tab = 32;
        // fill the data
        ui->lineEdit_2->setPlaceholderText("назва магазину");
        ui->lineEdit_2->setText(data[0]);
        ui->lineEdit_3->setPlaceholderText("адреса");
        ui->lineEdit_3->setText(data[1]);
        ui->lineEdit_4->setPlaceholderText("мобільний телефон");
        ui->lineEdit_4->setText(data[2]);
    } // if it's the third column
    else if (column == 3)
    {   // change tab index
        tab = 33;

        ui->lineEdit->setVisible(true);
        ui->lineEdit_5->setVisible(true);
        ui->lineEdit_6->setVisible(true);
        // fill the data
        ui->lineEdit_2->setPlaceholderText("назва товару");
        ui->lineEdit_2->setText(data[0]);
        ui->lineEdit_3->setPlaceholderText("id");
        ui->lineEdit_3->setText(data[1]);
        ui->lineEdit_4->setPlaceholderText("категорія");
        ui->lineEdit_4->setText(data[2]);
        ui->lineEdit->setPlaceholderText("короткий опис");
        ui->lineEdit->setText(data[3]);
        ui->lineEdit_5->setPlaceholderText("кількість товару на складі");
        ui->lineEdit_5->setText(data[4]);
        ui->lineEdit_6->setPlaceholderText("ціна за 1 шт у долярах");
        ui->lineEdit_6->setText(data[5]);
    }
    // insert data to combobox
    for (int i = 0; i < ((int*)Start)[POS_CNT]; i++)
    {
        ui->comboBox->insertItem(i,QString(((TCity*)Start[i])->name));
    }


    ui->pushButton->setVisible(true);
    ui->pushButton->setText("Зберегти");
}


// combobox event
void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    int pos; bool fnd;
    ui->comboBox_2->clear();// search for a city
    FindElList(Start,arg1,pos,fnd,cmpCity);
    if (tab == 13 || tab == 20 || tab == 33)
    {   // if the city was found
        if (fnd)
        {   // find the stores
            void ** s = (void**)(((TCity*)Start[pos])->sublev);
            for (int i = 0; i < ((int*)(s))[POS_CNT]; i++) // insert to combobox
                ui->comboBox_2->insertItem(i, ((TStore*)(s[i]))->name);
            if (tab == 20) // add special item
                ui->comboBox_2->addItem("null");
        }
    }// if we'd like to find items
    if (tab == 40)
    {   // if we're searching for a city
        if (ui->comboBox->currentText() == "Місто")
        {
            ui->lineEdit_2->setVisible(true);
            ui->lineEdit_2->setPlaceholderText("назва міста");
            ui->lineEdit_3->setVisible(true);
            ui->lineEdit_3->setPlaceholderText("регіон");
            ui->lineEdit_4->setVisible(true);
            ui->lineEdit_4->setPlaceholderText("поштовий індекс");
        } // if we're searching for a store
        else if (ui->comboBox->currentText() == "Магазин")
        {
            ui->lineEdit_2->setVisible(true);
            ui->lineEdit_2->setPlaceholderText("назва магазину");
            ui->lineEdit_3->setVisible(true);
            ui->lineEdit_3->setPlaceholderText("адреса");
            ui->lineEdit_4->setVisible(true);
            ui->lineEdit_4->setPlaceholderText("мобільний телефон");
        } // if we'are searching for a product
        else if (ui->comboBox->currentText() == "Товар")
        {
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
        }
    }
}


// if we'd like to find smth
void MainWindow::on_action_3_triggered()
{
    tab = 40;
    makeInvisible();

    ui->subheader->setVisible(true);
    ui->subheader->setText("Укажіть дані для пошуку");

    ui->comboBox->setVisible(true);
    // insert data to combobox
    QStringList items;
    items.append("Місто");
    items.append("Магазин");
    items.append("Товар");
    ui->comboBox->insertItems(0,items);

    ui->pushButton->setVisible(true);
    ui->pushButton->setText("Пошук");
}


template <typename T>
struct Rating
{
    T item;
    double  cost;
};

struct toLower {
    bool operator()(Rating<QTreeWidgetItem*> const& p1, Rating<QTreeWidgetItem*> const& p2)
    {
        return p1.cost < p2.cost;
    }
};


struct toHigher {
    bool operator()(Rating<QTreeWidgetItem*> const& p1, Rating<QTreeWidgetItem*> const& p2)
    {
        return p1.cost > p2.cost;
    }
};

// sort things function
template <typename T>
void MainWindow::sortThings()
{
    int cnt=((int*)Start)[POS_CNT];
    std::priority_queue<Rating<QTreeWidgetItem*>, std::vector<Rating<QTreeWidgetItem *>>, T> sorted;

    for (int i = 0; i <cnt; i++) // loop
    {   // output third level

        void ** s = (void**)(((TCity*)Start[i])->sublev); // go to the next level
        for (int j = 0; j < ((int*)(s))[POS_CNT];j++) // loop
        {   // store item

            void ** p = (void**)(((TStore*)s[j])->sublev); // go to the next level
            for (int k = 0; k < ((int*)(p))[POS_CNT];k++) // loop
            {   // create product item

                QTreeWidgetItem * itm = new QTreeWidgetItem();
                // setting text to item
                itm->setText(1, ((TCity*)Start[i])->name + "\n" + ((TCity*)Start[i])->region + "\n" + QString::number(((TCity*)Start[i])->postcode));

                QTreeWidgetItem * store = new QTreeWidgetItem();
                // set text
                store->setText(2, ((TStore*)s[j])->name + "\n" + ((TStore*)s[j])->adress + "\n" + (((TStore*)s[j])->phnumber));
                // make it expanded
                store->setExpanded(true);
                itm->addChild(store); // add child

                QTreeWidgetItem * product = new QTreeWidgetItem();
                float cost = QString::number(((TProduct*)p[k])->price).toFloat();
                // set text to it
                product->setText(3, ((TProduct*)p[k])->name + "\n" + QString::number(((TProduct*)p[k])->id) + "\n" + (((TProduct*)p[k])->category) + "\n" + ((TProduct*)p[k])->description + "\n" + QString::number(((TProduct*)p[k])->count) + "\n" + QString::number(((TProduct*)p[k])->price) + "$");
                // make it expanded
                product->setExpanded(true);
                store->addChild(product); // add child

                Rating<QTreeWidgetItem*> item;
                item.item = itm;
                item.cost = cost;

                sorted.push(item);
            }
        }
    }

    ui->treeWidget->clear();

    // create top item pointer
    QTreeWidgetItem *topItem = new QTreeWidgetItem();
    topItem->setText(0,"Ukraine");

    // configure headers
    QStringList headers;
    headers.append("країна");
    headers.append("місто");
    headers.append("магазин");
    headers.append("товар");

    // set headers
    ui->treeWidget->setHeaderLabels(headers);


    for (;sorted.size() != 0;)
    {
        topItem->addChild(sorted.top().item);
        sorted.pop();
    }

    ui->treeWidget->addTopLevelItem(topItem);

}


void MainWindow::on_action_4_triggered()
{
   makeInvisible();

   refreshTreeView();
}

//   ui->treeWidget->setVisible(true);
//   ui->tableWidget->setVisible(true);
//   ui->tableWidget->setColumnCount(2);

//   QStringList heads;
//   heads.append("магазин");
//   heads.append("капітал");

//   ui->subheader->setVisible(true);

//   ui->subheader->setText("Запаси магазинів: ");

//   int cnt=((int*)Start)[POS_CNT];

//   std::priority_queue<Rating, std::vector<Rating>, toLower> rating;

//    // create top item pointer
//    QTreeWidgetItem *topItem = new QTreeWidgetItem();
//    topItem->setText(0,"Ukraine");

//   for (int i = 0; i <cnt; i++) // loop
//   {   // output third level

//       void ** s = (void**)(((TCity*)Start[i])->sublev); // go to the next level
//       for (int j = 0; j < ((int*)(s))[POS_CNT];j++) // loop
//       {   // store item
//           double summary = 0;
//           Rating item;

//           QList<QTreeWidgetItem*> store = ui->treeWidget->findItems(((TStore*)s[j])->name + "\n" + ((TStore*)s[j])->adress + "\n" + (((TStore*)s[j])->phnumber), Qt::MatchContains | Qt::MatchRecursive ,2);

//            if (store.length() > 0)
//                store[0]->parent()->parent()->removeChild(store[0]->parent());

//           item.item = store[0];

//           void ** p = (void**)(((TStore*)s[j])->sublev); // go to the next level
//           for (int k = 0; k < ((int*)(p))[POS_CNT];k++)
//           {
//               summary += ((TProduct*)p[k])->count * ((TProduct*)p[k])->price;
//           }
//           item.cost = summary;
//           rating.push(item);
//        }
//    }

//   ui->tableWidget->setRowCount(rating.size());

//    std::stack<QTreeWidgetItem*> items;
//   for (int i = 0;rating.size() != 0;i++)
//   {
//       Rating rate = rating.top();
//       items.push(rate.item);
//       rating.pop();

//       topItem->addChild(items.top()->parent());

//       QTableWidgetItem * column1 = new QTableWidgetItem();
//       column1->setText(rate.item->text(2).split("\n")[0]);

//       QTableWidgetItem * column2 = new QTableWidgetItem();
//       column2->setText(QString::number(rate.cost) + "$");

//       ui->tableWidget->setItem(i,0, column1);
//       ui->tableWidget->setItem(i,1,column2);
//   }

//   ui->treeWidget->clear();


//   // configure headers
//   QStringList headers;
//   headers.append("країна");
//   headers.append("місто");
//   headers.append("магазин");
//   headers.append("товар");

//   ui->treeWidget->setHeaderLabels(headers);

//   ui->treeWidget->addTopLevelItem(topItem);
//}


void MainWindow::on_action_10_triggered()
{
    sortThings<toHigher>();
}

void MainWindow::on_action_11_triggered()
{
    sortThings<toLower>();
}
