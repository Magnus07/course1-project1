#include "mainwindow.h"
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
// 51 - search in range screen
// 52 - search among cities or stores screen
// 60 - about screen
ushort tab = 0;
void ** Start = InitArray();
QVBoxLayout * lay;
QList<QCheckBox *> checkBoxes;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    makeInvisible();
    ui->header->setVisible(true);
    lay = new QVBoxLayout(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::setTreeWidgetHeaders()
{
    // configure headers
    QStringList headers;
    headers.append("країна");
    headers.append("місто");
    headers.append("магазин");
    headers.append("товар");

    // set headers
    ui->treeWidget->setHeaderLabels(headers);
}


void sameNameError()
{   // message text
    QMessageBox *message = new QMessageBox(QMessageBox::Critical,"Warning",  "Неможливо додавати елементи з однаковими іменами! Змініть ім'я або видаліть вже існуючий об'єкт.");
    message->setStandardButtons(QMessageBox::Ok);
    message->exec();
}


void emptyFieldError()
{   // message text
    QMessageBox *message = new QMessageBox(QMessageBox::Warning, "Warning",  "Укажіть усю інформацію. Жодне поле не має бути порожнім!");
    message->setStandardButtons(QMessageBox::Ok);
    message->exec();
}


void MainWindow::refreshTreeView()
{   // array size
    int cnt=((int*)Start)[POS_CNT];
    ui->treeWidget->clear();

    // create top item pointer
    QTreeWidgetItem *topItem = new QTreeWidgetItem();
    topItem->setText(0,"Ukraine");
    // set headers to tree widget
    setTreeWidgetHeaders();

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
    ui->scrollArea->setVisible(false);
}


// clearing the widget's values
void MainWindow::cleanUp()
{
    ui->comboBox->clear();
    ui->lineEdit->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();
    ui->lineEdit_4->clear();
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();
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
    if ((tab == 11 || tab == 12 || tab == 13 || tab == 20 || tab == 31 || tab == 32 || tab == 33 || tab == 40 || tab == 52) && ( ui->lineEdit_2->text() == "" || ui->lineEdit_3->text() == "" || ui->lineEdit_4->text() == ""))
    {   // warning message
        emptyFieldError();
        return;
    }
    if (((tab == 33 || tab == 13)) && (ui->lineEdit->text() == "" || ui->lineEdit_5->text() == "" || ui->lineEdit_6->text() == ""))
    {   // warning message
        emptyFieldError();
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
        if (fnd)
        {
            sameNameError();
            return;
        }
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
        if (fnd)
        {
            sameNameError();
            return;
        }
        addToSort(((TCity*)Start[pos])->sublev,store, ps);
    }
    // if we'd like to add new product
    if (tab == 13)
    {
        //find city
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
        if (fndProduct)
        {
            sameNameError();
            return;
        }
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
        QStringList item = ui->treeWidget->currentItem()->parent()->text(ui->treeWidget->currentColumn() - 1).split("\n");
        FindElList(Start, item[0],pos, fnd, cmpCity);
        void ** s = (void**)(((TCity*)Start[pos])->sublev); // go to the next level
        int spos;bool sfound;
        FindElList(((TCity*)Start[pos])->sublev, ui->treeWidget->currentItem()->text(ui->treeWidget->currentColumn()).split("\n")[0], spos, sfound, cmpStore);
        // edit this element
        ((TStore*)s[spos])->name = ui->lineEdit_2->text();
        ((TStore*)s[spos])->adress = ui->lineEdit_3->text();
        ((TStore*)s[spos])->phnumber = ui->lineEdit_4->text();
    }// if we'd like to edit product
    if (tab == 33)
    {   // search for city, store & product
        QStringList item = ui->treeWidget->currentItem()->text(ui->treeWidget->currentColumn()).split("\n");

        FindElList(Start, ui->treeWidget->currentItem()->parent()->parent()->text(ui->treeWidget->currentColumn() - 2).split("\n")[0],pos, fnd, cmpCity);
        int spos;bool sfound;

        FindElList(((TCity*)Start[pos])->sublev, ui->treeWidget->currentItem()->parent()->text(ui->treeWidget->currentColumn() - 1).split("\n")[0], spos, sfound, cmpStore);
        int ppos;bool pfound;
        FindElList(((TStore*)(((TCity*)Start[pos])->sublev[spos]))->sublev, item[0], ppos, pfound, cmpProduct);

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
        } // if we'd like to find a product
        if (ui->comboBox->currentText() == "Товар")
        {   // generate searchquery and search
            found =  ui->treeWidget->findItems(searchquery, Qt::MatchContains | Qt::MatchRecursive ,3);
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
    if (tab == 51)
    {
        makeInvisible();
        ui->treeWidget->setVisible(true);

        QTreeWidgetItem * top = new QTreeWidgetItem();
        top->setText(0,"Ukraine");
        ui->treeWidget->takeTopLevelItem(0);

        int cnt=((int*)Start)[POS_CNT];

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

                void ** p = (void**)(((TStore*)s[j])->sublev); // go to the next level
                for (int k = 0; k < ((int*)(p))[POS_CNT];k++) // loop
                {   // create product item
                    if (((TProduct*)p[k])->price > ui->lineEdit_2->text().toFloat() && ((TProduct*)p[k])->price < ui->lineEdit->text().toFloat())
                    {
                        QTreeWidgetItem * product = new QTreeWidgetItem();
                        // set text to it
                        product->setText(3, ((TProduct*)p[k])->name + "\n" + QString::number(((TProduct*)p[k])->id) + "\n" + (((TProduct*)p[k])->category) + "\n" + ((TProduct*)p[k])->description + "\n" + QString::number(((TProduct*)p[k])->count) + "\n" + QString::number(((TProduct*)p[k])->price) + "$");
                        store->addChild(product);
                    }
                }
                if (store->childCount()!=0)
                {
                    itm->addChild(store);
                }
            }
            if (itm->childCount()!=0)
            {
                top->addChild(itm);
            }
        }
        ui->treeWidget->addTopLevelItem(top);
    }// search among cities/stores
    if (tab == 52)
    {   // names' container
        QList<QString> data;
        for (int i = 0;i < checkBoxes.size();i++)
        {   // add some data
            if (ui->comboBox->currentText() == "магазини" && checkBoxes[i]->isChecked())
                data.push_back(checkBoxes[i]->text().split(":")[1]);
            else if (checkBoxes[i]->isChecked())
                data.push_back(checkBoxes[i]->text());
        }
        // array size
        int cnt=((int*)Start)[POS_CNT];
        ui->treeWidget->clear();

        // create top item pointer
        QTreeWidgetItem *topItem = new QTreeWidgetItem();
        topItem->setText(0,"Ukraine");

        setTreeWidgetHeaders();

        for (int i = 0; i <cnt; i++) // loop
        {   // output third level

            if (ui->comboBox->currentText() == "міста")
            {
                if (data.indexOf(((TCity*)Start[i])->name) == -1)
                    continue;
                else
                    data.removeAt(data.indexOf(((TCity*)Start[i])->name));
            }

            QTreeWidgetItem * itm = new QTreeWidgetItem();
            // setting text to item
            itm->setText(1, ((TCity*)Start[i])->name + "\n" + ((TCity*)Start[i])->region + "\n" + QString::number(((TCity*)Start[i])->postcode));

            void ** s = (void**)(((TCity*)Start[i])->sublev); // go to the next level
            for (int j = 0; j < ((int*)(s))[POS_CNT];j++) // loop
            {   // store item

                if (ui->comboBox->currentText() == "магазини")
                {
                    if (data.indexOf(((TStore*)s[j])->name) == -1)
                        continue;
                    else
                        data.removeAt(data.indexOf(((TStore*)s[j])->name));
                }
                QTreeWidgetItem * store = new QTreeWidgetItem();
                // set text
                store->setText(2, ((TStore*)s[j])->name + "\n" + ((TStore*)s[j])->adress + "\n" + (((TStore*)s[j])->phnumber));
                // make it expanded
                store->setExpanded(true);

                void ** p = (void**)(((TStore*)s[j])->sublev); // go to the next level
                for (int k = 0; k < ((int*)(p))[POS_CNT];k++) // loop
                {   // create product item
                    if (((TProduct*)p[k])->name == ui->lineEdit_2->text() && ((TProduct*)p[k])->id == ui->lineEdit_3->text().toUShort() && (((TProduct*)p[k])->category) == ui->lineEdit_4->text())
                    {
                        QTreeWidgetItem * product = new QTreeWidgetItem();
                        // set text to it
                        product->setText(3, ((TProduct*)p[k])->name + "\n" + QString::number(((TProduct*)p[k])->id) + "\n" + (((TProduct*)p[k])->category) + "\n" + ((TProduct*)p[k])->description + "\n" + QString::number(((TProduct*)p[k])->count) + "\n" + QString::number(((TProduct*)p[k])->price) + "$");
                        // make it expanded
                        product->setExpanded(true);
                        store->addChild(product); // add child
                    }
                }
                if (store->childCount()!=0)
                {
                    itm->addChild(store);
                }
            }// add child to the top item
            if (itm->childCount()!=0)
            {
                topItem->addChild(itm);
            }

            topItem->setExpanded(true);
            ui->treeWidget->addTopLevelItem(topItem);
        }
    }
}


// city template
void MainWindow::showCityEdits()
{
    ui->lineEdit_2->setVisible(true);
    ui->lineEdit_2->setPlaceholderText("назва міста");
    ui->lineEdit_3->setVisible(true);
    ui->lineEdit_3->setPlaceholderText("область");
    ui->lineEdit_4->setVisible(true);
    ui->lineEdit_4->setPlaceholderText("поштовий індекс");
}


// store template
void MainWindow::showStoreEdits()
{
    ui->lineEdit_2->setVisible(true);
    ui->lineEdit_2->setPlaceholderText("назва магазину");
    ui->lineEdit_3->setVisible(true);
    ui->lineEdit_3->setPlaceholderText("адреса");
    ui->lineEdit_4->setVisible(true);
    ui->lineEdit_4->setPlaceholderText("мобільний телефон");
}


// product template
void MainWindow::showProductEdits()
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
    ui->lineEdit_6->setPlaceholderText("ціна за 1 шт у долярах");
}

// if we'd like to add a new city
void MainWindow::on_action_7_triggered()
{
    makeInvisible();
    cleanUp();
    ui->treeWidget->setVisible(true);
    ui->subheader->setVisible(true);
    ui->subheader->setText("Укажіть необхідні дані");

    tab = 11;

    showCityEdits();

    ui->pushButton->setVisible(true);
    ui->pushButton->setText("Додати");
}

// if we'd like to add new store
void MainWindow::on_action_8_triggered()
{
    makeInvisible();
    cleanUp();
    ui->comboBox->setVisible(true);
    ui->treeWidget->setVisible(true);
    ui->subheader->setVisible(true);
    ui->subheader->setText("Укажіть необхідні дані");

    tab = 12;

    showStoreEdits();

    // fill the combobox item
    fillCityCombobox();

    ui->pushButton->setVisible(true);
    ui->pushButton->setText("Додати");
}

// if we'd like to add new product
void MainWindow::on_action_9_triggered()
{
    makeInvisible();
    cleanUp();
    ui->treeWidget->setVisible(true);
    ui->subheader->setVisible(true);
    ui->subheader->setText("Укажіть необхідні дані");

    tab = 13;

    ui->comboBox->setVisible(true);
    ui->comboBox_2->setVisible(true);

    showProductEdits();

    // fill the combobox item
    cleanUp();
    fillCityCombobox();

    ui->pushButton->setVisible(true);
    ui->pushButton->setText("Додати");
}


void MainWindow::fillCityCombobox()
{
    // filling the combobox
    for (int i = 0; i < ((int*)Start)[POS_CNT]; i++)
    {
        ui->comboBox->insertItem(i,QString(((TCity*)Start[i])->name));
    }
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
    cleanUp();
    fillCityCombobox();

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

    // get the data
    QStringList data = item->text(column).split("\n");
    // if it's the first column
    if (column == 1)
    {   // change tab index
        tab = 31;
        // fill the data
        showCityEdits();
        ui->lineEdit_2->setText(data[0]);
        ui->lineEdit_3->setText(data[1]);
        ui->lineEdit_4->setText(data[2]);
    } // if it's the second column
    else if (column == 2)
    {   // change tab index
        tab = 32;
        // fill the data
        showStoreEdits();
        ui->lineEdit_2->setText(data[0]);
        ui->lineEdit_3->setText(data[1]);
        ui->lineEdit_4->setText(data[2]);
    } // if it's the third column
    else if (column == 3)
    {   // change tab index
        tab = 33;

        showProductEdits();
        // fill the data
        ui->lineEdit_2->setText(data[0]);
        ui->lineEdit_3->setText(data[1]);
        ui->lineEdit_4->setText(data[2]);
        ui->lineEdit->setText(data[3]);
        ui->lineEdit_5->setText(data[4]);
        ui->lineEdit_6->setText(data[5]);
    }
    // insert data to combobox
    fillCityCombobox();

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
            showCityEdits();

        } // if we're searching for a store
        else if (ui->comboBox->currentText() == "Магазин")
        {
            showStoreEdits();
        } // if we'are searching for a product
        else if (ui->comboBox->currentText() == "Товар")
        {
            showProductEdits();

            ui->lineEdit->setVisible(false);
            ui->lineEdit_5->setVisible(false);
            ui->lineEdit_6->setVisible(false);
        }
    }
    if (tab == 52)
    {
        int cnt=((int*)Start)[POS_CNT];

        for (;checkBoxes.size() != 0;)
        {
            delete checkBoxes[0];
            checkBoxes.removeAt(0);
        }

        if (ui->comboBox->currentText() == "міста")
        {
            for (int i = 0; i <cnt; i++) // loop
            {
                QCheckBox *ch = new QCheckBox(((TCity*)Start[i])->name);
                checkBoxes.append(ch);
            }
        }
        else if (ui->comboBox->currentText() == "магазини")
        {
            for (int i = 0; i <cnt; i++) // loop
            {
                void ** s = (void**)(((TCity*)Start[i])->sublev); // go to the next level
                for (int j = 0; j < ((int*)(s))[POS_CNT];j++) // loop
                {   // store item
                    QCheckBox *ch = new QCheckBox(((TCity*)Start[i])->name + ":" + ((TStore*)s[j])->name);
                    checkBoxes.append(ch);
                }
            }
        }
        for (int i = 0; i < checkBoxes.size();i++)
            lay->addWidget(checkBoxes[i]);
        ui->scrollAreaWidgetContents->setLayout(lay);
    }
}


// if we'd like to find smth
void MainWindow::on_action_3_triggered()
{
    tab = 40;
    makeInvisible();
    cleanUp();

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


// container structure
template <typename T>
struct Rating
{
    T item;
    double  cost;
};


// sort overloading operators
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

    setTreeWidgetHeaders();

    // add childs to top item
    for (;sorted.size() != 0;)
    {
        topItem->addChild(sorted.top().item);
        sorted.pop();
    }

    ui->treeWidget->addTopLevelItem(topItem);

}


// individual task solution
void MainWindow::on_action_4_triggered()
{
   makeInvisible();

   refreshTreeView();

   ui->treeWidget->setVisible(true);
   ui->tableWidget->setVisible(true);
   ui->tableWidget->setColumnCount(2);

   QStringList heads;
   heads.append("магазин");
   heads.append("капітал");

   ui->tableWidget->setHorizontalHeaderLabels(heads);

   ui->subheader->setVisible(true);

   ui->subheader->setText("Запаси магазинів: ");

   int cnt=((int*)Start)[POS_CNT];

   std::priority_queue<Rating<QTreeWidgetItem*>, std::vector<Rating<QTreeWidgetItem*>>, toLower> rating;

    // create top item pointer
    QTreeWidgetItem *topItem = new QTreeWidgetItem();
    topItem->setText(0,"Ukraine");

    for (int i = 0; i <cnt; i++) // loop
    {   // output third level

        void ** s = (void**)(((TCity*)Start[i])->sublev); // go to the next level
        for (int j = 0; j < ((int*)(s))[POS_CNT];j++) // loop
        {   // store item
            QTreeWidgetItem * itm = new QTreeWidgetItem();
            // setting text to item
            itm->setText(1, ((TCity*)Start[i])->name + "\n" + ((TCity*)Start[i])->region + "\n" + QString::number(((TCity*)Start[i])->postcode));

            QTreeWidgetItem * store = new QTreeWidgetItem();
            // set text
            store->setText(2, ((TStore*)s[j])->name + "\n" + ((TStore*)s[j])->adress + "\n" + (((TStore*)s[j])->phnumber));
            // make it expanded
            store->setExpanded(true);
            itm->addChild(store); // add child

            Rating<QTreeWidgetItem*> item;

            double cost = 0;

            void ** p = (void**)(((TStore*)s[j])->sublev); // go to the next level
            for (int k = 0; k < ((int*)(p))[POS_CNT];k++) // loop
            {   // create product item

                QTreeWidgetItem * product = new QTreeWidgetItem();
                // set text to it
                product->setText(3, ((TProduct*)p[k])->name + "\n" + QString::number(((TProduct*)p[k])->id) + "\n" + (((TProduct*)p[k])->category) + "\n" + ((TProduct*)p[k])->description + "\n" + QString::number(((TProduct*)p[k])->count) + "\n" + QString::number(((TProduct*)p[k])->price) + "$");
                // make it expanded
                product->setExpanded(true);
                store->addChild(product); // add child
                cost += QString::number(((TProduct*)p[k])->price).toFloat() * QString::number(((TProduct*)p[k])->count).toFloat();
            }
            item.item = store;
            item.cost = cost;

            rating.push(item);
        }
    }

   ui->tableWidget->setRowCount(rating.size());
    // go through the rating container
   for (int i = 0;rating.size() != 0;i++)
   {   // get top element
       Rating<QTreeWidgetItem*> rate = rating.top();
       topItem->addChild(rate.item->parent());
       rating.pop();
        // create columns
       QTableWidgetItem * column1 = new QTableWidgetItem();
       column1->setText(rate.item->text(2).split("\n")[0]);

       QTableWidgetItem * column2 = new QTableWidgetItem();
       column2->setText(QString::number(rate.cost) + "$");
        // set items
       ui->tableWidget->setItem(i,0, column1);
       ui->tableWidget->setItem(i,1,column2);
   }
    // clear widget and add headers
   ui->treeWidget->clear();
   setTreeWidgetHeaders();

   ui->treeWidget->addTopLevelItem(topItem);
}


void MainWindow::on_action_10_triggered()
{   // call function
    sortThings<toHigher>();
}

void MainWindow::on_action_11_triggered()
{   // call function
    sortThings<toLower>();
}


// find some stuff in between
void MainWindow::on_action_12_triggered()
{
    makeInvisible();

    tab = 51;
    // ui preparations
    ui->subheader->setVisible(true);
    ui->subheader->setText("Укажіть ціновий діапазон: ");

    ui->lineEdit->setVisible(true);
    ui->lineEdit->setPlaceholderText("ціна до");
    ui->lineEdit_2->setVisible(true);
    ui->lineEdit_2->setPlaceholderText("ціна від");

    ui->pushButton->setVisible(true);
    ui->pushButton->setText("Пошук");
}


// search among the cities/stores
void MainWindow::on_action_13_triggered()
{
    makeInvisible();
    cleanUp();

    tab = 52;
    // ui preparations
    ui->comboBox->setVisible(true);
    ui->subheader->setVisible(true);

    ui->treeWidget->setVisible(true);

    ui->scrollArea->setVisible(true);

    showProductEdits();

    ui->lineEdit->setVisible(false);
    ui->lineEdit_5->setVisible(false);
    ui->lineEdit_6->setVisible(false);

    ui->subheader->setText("Оберіть необхідні міста\n(магазини)");
    // combobox items
    QStringList items;
    items.append("міста");
    items.append("магазини");

    ui->pushButton->setVisible(true);
    ui->pushButton->setText("Пошук");

    ui->comboBox->insertItems(0,items);
}


// function for converting to json
QJsonDocument jsonize()
{
    QJsonDocument json;
    // size array
    int cnt=((int*)Start)[POS_CNT];
    QJsonArray global;

    for (int i = 0; i <cnt; i++) // loop
    {   // third level
        QJsonObject city;
        city["name"] = ((TCity*)Start[i])->name;
        city["region"] = ((TCity*)Start[i])->region;
        city["postcode"] = (int)((TCity*)Start[i])->postcode;
        QJsonArray stores;
        void ** s = (void**)(((TCity*)Start[i])->sublev); // go to the next level
        for (int j = 0; j < ((int*)(s))[POS_CNT];j++) // loop
        {   // store item
            QJsonObject store;
            store["name"] = ((TStore*)s[j])->name;
            store["adress"] = ((TStore*)s[j])->adress;
            store["phnumber"] = ((TStore*)s[j])->phnumber;

            QJsonArray products;
            void ** p = (void**)(((TStore*)s[j])->sublev); // go to the next level
            for (int k = 0; k < ((int*)(p))[POS_CNT];k++) // loop
            {   // create product item
                QJsonObject product;
                product["name"] = ((TProduct*)p[k])->name;
                product["id"] = ((TProduct*)p[k])->id;
                product["category"] = ((TProduct*)p[k])->category;
                product["description"] = ((TProduct*)p[k])->description;
                product["count"] = ((TProduct*)p[k])->count;
                product["price"] = ((TProduct*)p[k])->price;

                products.append(product);
            }
            store["products"] = products;
            stores.append(store);
        }
        city["stores"] = stores;
        global.append(city);
    }// set global item
    json.setArray(global);
    return json;
}

void MainWindow::on_action_2_triggered()
{
    // search for an empty place
    int f = 0;
    while (QFile::exists("data" + QString::number(f) +".json"))
        f++;
    // open file
    QFile jsonFile("data" + QString::number(f) +".json");
    jsonFile.open(QFile::WriteOnly);
    // write to a file
    jsonFile.write(jsonize().toJson());
    // message
    QMessageBox *message = new QMessageBox(QMessageBox::Information, "Успішно",  "Збережено у кореневій директорії як data" + QString::number(f) +".json");
    message->setStandardButtons(QMessageBox::Ok);
    message->exec();
}


void deleteWhole()
{
    // size
    int cnt=((int*)Start)[POS_CNT];
    for (int i = 0; i <cnt; i++) // loop
    {   // go to the third level
        void ** s = (void**)(((TCity*)Start[i])->sublev); // go to the next level
        for (int j = 0; j < ((int*)(s))[POS_CNT];j++) // loop
        {   // store item
            void ** p = (void**)(((TStore*)s[j])->sublev); // go to the next level
            for (int k = 0; k < ((int*)(p))[POS_CNT];k++) // loop
            {   // delete product item
                delete (TProduct*)p[k];
            }
            delete (TStore*)s[j];
        }
        delete (TCity*)Start[i];
    } // init array
    Start = InitArray();
}

void MainWindow::on_action_triggered()
{
    QMessageBox msgBox;
    // message icon
    msgBox.setIcon(QMessageBox::Information);
    // message text
    msgBox.setInformativeText("Якщо ви продовжите, усі дані будуть видалені. Продовжити?");
    // message buttons
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    // if it's canceled
    switch (msgBox.exec()) {
        case QMessageBox::Cancel:
             return;
    } // open file dialog
    QString fileName = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.json");
    QFile jsonFile(fileName);
    // open file
    jsonFile.open(QFile::ReadOnly);
    QJsonDocument source = QJsonDocument().fromJson(jsonFile.readAll());
    // get objects
    QJsonArray cities = source.array();
    // if there's no cities
    if (cities.size() == 0)
        return;
    // remove items
    deleteWhole();
    // go through all elements
    for (int i = 0; i < cities.size();i++)
    {
        TCity * city = new TCity();
        // new item
        city->name = cities[i].toObject().value("name").toString();
        city->region = cities[i].toObject().value("region").toString();
        city->postcode = cities[i].toObject().value("postcode").toInt();
        city->sublev = InitArray();
        // stores array
        QJsonArray stores = cities[i].toObject().value("stores").toArray();
        addToSort(Start,city,i);
        for (int j = 0; j < stores.size();j++)
        {
            void ** s = (void**)(((TCity*)Start[i])->sublev); // go to the next level
            TStore * store = new TStore();
            store->name = stores[j].toObject().value("name").toString();
            store->adress = stores[j].toObject().value("adress").toString();
            store->phnumber = stores[j].toObject().value("phnumber").toString();
            store->sublev = InitArray();
            // products array
            QJsonArray products = stores[j].toObject().value("products").toArray();
            addToSort(((TCity*)Start[i])->sublev, store, j);
            for (int k = 0; k < products.size();k++)
            {
                TProduct * product = new TProduct();
                product->name = products[k].toObject().value("name").toString();
                product->id = products[k].toObject().value("id").toInt();
                product->category = products[k].toObject().value("category").toString();
                product->description = products[k].toObject().value("description").toString();
                product->count = products[k].toObject().value("count").toInt();
                product->price = products[k].toObject().value("price").toInt();
                // add to sort
                addToSort(((TStore*)s[j])->sublev,product,k);
            }
        }
    }
    refreshTreeView();
    makeInvisible();
    ui->treeWidget->setVisible(true);
}

void MainWindow::on_action_15_triggered()
{
    makeInvisible();

    ui->header->setVisible(true);
    ui->subheader->setVisible(true);

    tab = 60;

    ui->header->setText("QWESDA 2020\nрозроблена Pinchuk S.S.");
    ui->subheader->setText("open source software\nhttps://github.com/Magnus07/course1-project1\nCircled Q icon by Icons8\n\nCPNU 2020");
}
