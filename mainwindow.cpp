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

    QStringList headers;
    headers.append("країна");
    headers.append("місто");
    headers.append("магазин");
    headers.append("товар");

    ui->treeWidget->setHeaderLabels(headers);

    for (int i = 0; i <cnt; i++) // loop
    {   // output third level
        QTreeWidgetItem * itm = new QTreeWidgetItem();

        itm->setText(1, ((TCity*)Start[i])->name + "\n" + ((TCity*)Start[i])->region + "\n" + QString::number(((TCity*)Start[i])->postcode));

        void ** s = (void**)(((TCity*)Start[i])->sublev); // go to the next level
        for (int j = 0; j < ((int*)(s))[POS_CNT];j++) // loop
        {   // output
            QTreeWidgetItem * store = new QTreeWidgetItem();

            store->setText(2, ((TStore*)s[j])->name + "\n" + ((TStore*)s[j])->adress + "\n" + (((TStore*)s[j])->phnumber));

            store->setExpanded(true);
            itm->addChild(store);

            void ** p = (void**)(((TStore*)s[j])->sublev); // go to the next level
            for (int k = 0; k < ((int*)(p))[POS_CNT];k++) // loop
            {
                QTreeWidgetItem * product = new QTreeWidgetItem();

                product->setText(3, ((TProduct*)p[k])->name + "\n" + QString::number(((TProduct*)p[k])->id) + "\n" + (((TProduct*)p[k])->category) + "\n" + ((TProduct*)p[k])->description + "\n" + QString::number(((TProduct*)p[k])->count) + "\n" + QString::number(((TProduct*)p[k])->price) + "$");

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
    ui->comboBox_3->setVisible(false);

    ui->comboBox->clear();
    ui->comboBox_2->clear();
    ui->comboBox_3->clear();
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
    if (((tab == 12 && ui->comboBox->currentText() == "")) || (tab == 13 && ((ui->comboBox->currentText() == "") || (ui->comboBox_2->currentText() == ""))))
    {
        QMessageBox *message = new QMessageBox(QMessageBox::Warning, "Warning",  "Створіть, будь ласка, елементи на попередніх етапах, аби мати можливість працювати з ними на цьому!");
        message->setStandardButtons(QMessageBox::Ok);
        message->exec();
        return;
    }

    int pos; bool fnd;

    if (tab == 11)
    {
        void *city = new TCity;
        ((TCity*)city)->name = ui->lineEdit_2->text();
        ((TCity*)city)->region = ui->lineEdit_3->text();
        ((TCity*)city)->postcode = ui->lineEdit_4->text().toUInt();
        ((TCity*)city)->sublev = InitArray();

        FindElList(Start,((TCity*)city)->name,pos,fnd,cmpCity);
        addToSort(Start,city, pos);
    }

    if (tab == 12)
    {
        FindElList(Start, ui->comboBox->currentText(),pos,fnd,cmpCity);

        void * store = new TStore;
        ((TStore *)store)->name = ui->lineEdit_2->text();
        ((TStore *)store)->adress = ui->lineEdit_3->text();
        ((TStore *)store)->phnumber = ui->lineEdit_4->text();
        ((TStore *)store)->sublev = InitArray();

        int ps;bool fnd;
        FindElList(((TCity*)Start[pos])->sublev,((TStore*)store)->name,ps,fnd,cmpStore);
        addToSort(((TCity*)Start[pos])->sublev,store, ps);
    }

    if (tab == 13)
    {
        FindElList(Start, ui->comboBox->currentText(),pos,fnd,cmpCity);

        int poss;bool found;
        void ** s = (void**)(((TCity*)Start[pos])->sublev);
        FindElList(s,ui->comboBox_2->currentText(),poss,found,cmpStore);

        void * product = new TProduct;
        ((TProduct *)product)->name = ui->lineEdit_2->text();
        ((TProduct *)product)->id = ui->lineEdit_3->text().toUShort();
        ((TProduct *)product)->category = ui->lineEdit_4->text();
        ((TProduct *)product)->description = ui->lineEdit->text();
        ((TProduct *)product)->count = ui->lineEdit_5->text().toUShort();
        ((TProduct *)product)->price = ui->lineEdit_6->text().toFloat();

        int posProduct; bool fndProduct;
        FindElList(((TStore*)(s[poss]))->sublev, ((TProduct *)product)->name, posProduct, fndProduct, cmpProduct);

        addToSort(((TStore*)s[poss])->sublev, product, posProduct);
    }

    if (tab == 20)
    {
        if (ui->comboBox_2->currentText() == "null")
        {
            FindElList(Start, ui->comboBox->currentText(),pos, fnd, cmpCity);
            delEl(Start, pos, 3);
            ui->comboBox->removeItem(ui->comboBox->currentIndex());
        }
        else if (ui->comboBox_3->currentText() == "null")
        {
            FindElList(Start, ui->comboBox->currentText(),pos, fnd, cmpCity);
            int spos;bool sfound;
            FindElList(((TCity*)Start[pos])->sublev, ui->comboBox_2->currentText(), spos, sfound, cmpStore);
            delEl(((TCity*)Start[pos])->sublev, spos, 2);
            ui->comboBox_2->removeItem(ui->comboBox_2->currentIndex());
        }
        else
        {
            FindElList(Start, ui->comboBox->currentText(),pos, fnd, cmpCity);
            int spos;bool sfound;
            FindElList(((TCity*)Start[pos])->sublev, ui->comboBox_2->currentText(), spos, sfound, cmpStore);
            int ppos;bool pfound;
            FindElList(((TStore*)(((TCity*)Start[pos])->sublev[spos]))->sublev, ui->comboBox_3->currentText(), ppos, pfound, cmpProduct);
            delEl(((TStore*)(((TCity*)Start[pos])->sublev[spos]))->sublev, ppos, 1);
            ui->comboBox_3->removeItem(ui->comboBox_3->currentIndex());
        }
    }
    if (tab == 31)
    {
        QStringList item = ui->treeWidget->currentItem()->text(ui->treeWidget->currentColumn()).split("\n");
        FindElList(Start, item[0], pos, fnd, cmpCity);
        ((TCity*)Start[pos])->name = ui->lineEdit_2->text();
        ((TCity*)Start[pos])->region = ui->lineEdit_3->text();
        ((TCity*)Start[pos])->postcode = ui->lineEdit_4->text().toUInt();
    }
    if (tab == 32)
    {
        QStringList item = ui->treeWidget->currentItem()->text(ui->treeWidget->currentColumn()).split("\n");
        FindElList(Start, item[0],pos, fnd, cmpCity);
        void ** s = (void**)(((TCity*)Start[pos])->sublev); // go to the next level
        int spos;bool sfound;
        FindElList(((TCity*)Start[pos])->sublev, ui->lineEdit_2->text(), spos, sfound, cmpStore);

        ((TStore*)s[spos])->name = ui->lineEdit_2->text();
        ((TStore*)s[spos])->adress = ui->lineEdit_3->text();
        ((TStore*)s[spos])->phnumber = ui->lineEdit_4->text();

    }
    if (tab == 33)
    {
        QStringList item = ui->treeWidget->currentItem()->text(ui->treeWidget->currentColumn()).split("\n");

        FindElList(Start, ui->comboBox->currentText(),pos, fnd, cmpCity);
        int spos;bool sfound;

        FindElList(((TCity*)Start[pos])->sublev, ui->comboBox_2->currentText(), spos, sfound, cmpStore);
        int ppos;bool pfound;
        FindElList(((TStore*)(((TCity*)Start[pos])->sublev[spos]))->sublev, ui->lineEdit_2->text(), ppos, pfound, cmpProduct);

        void ** p = ((TStore*)(((TCity*)Start[pos])->sublev[spos]))->sublev;

        ((TProduct *)p[ppos])->name = ui->lineEdit_2->text();
        ((TProduct *)p[ppos])->id = ui->lineEdit_3->text().toUShort();
        ((TProduct *)p[ppos])->category = ui->lineEdit_4->text();
        ((TProduct *)p[ppos])->description = ui->lineEdit->text();
        ((TProduct *)p[ppos])->count = ui->lineEdit_5->text().toUShort();
        ((TProduct *)p[ppos])->price = ui->lineEdit_6->text().toFloat();
    }
    if (tab == 31 || tab == 32 || tab == 33)
    {
        makeInvisible();
        ui->treeWidget->setVisible(true);
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


void MainWindow::on_action_5_triggered()
{
    makeInvisible();

    tab = 20;

    ui->comboBox->setVisible(true);
    ui->comboBox_2->setVisible(true);
    ui->comboBox_3->setVisible(true);

    ui->treeWidget->setVisible(true);
    ui->comboBox->clear();
    for (int i = 0; i < ((int*)Start)[POS_CNT]; i++)
    {
        ui->comboBox->insertItem(i,QString(((TCity*)Start[i])->name));
    }

    ui->pushButton->setVisible(true);
    ui->pushButton->setText("Вилучити");
}

void MainWindow::on_comboBox_2_currentTextChanged(const QString &arg1)
{
    int pos; bool fnd;
    ui->comboBox_3->clear();
    FindElList(Start,ui->comboBox->currentText(),pos,fnd,cmpCity);
    if (tab == 20)
    {
        if (fnd)
        {
            void ** s = (void**)(((TCity*)Start[pos])->sublev);
            FindElList(s,ui->comboBox_2->currentText(), pos,fnd, cmpStore);
            if (fnd)
            {
                void ** p = (void**)(((TStore*)s[pos])->sublev);
                for (int i = 0; i < ((int*)(p))[POS_CNT]; i++)
                {
                    ui->comboBox_3->insertItem(i, ((TProduct*)(p[i]))->name);
                }
                ui->comboBox_3->addItem("null");
            }
        }
    }
}


//mode 1 - product
//mode 2 - store
//mode 3 - city
void delEl(void** &start,  int posFnd, int mode)
{
    if (mode == 3)
    {
        void ** s = (((TCity*)start[posFnd])->sublev);
        for (int j = 0; j < ((int*)(s))[POS_CNT];j++)
        {
            void * * p = (((TStore*)s[j])->sublev);
            for (int k = 0; k < ((int*)(p))[POS_CNT];k++)
            {
                delete ((TProduct*)p[k]);
            }
            delete (TStore*)s[j];
        }
    }
    else if (mode == 2)
    {
            void * * p = (((TStore*)start[posFnd])->sublev);
            for (int k = 0; k < ((int*)(p))[POS_CNT];k++)
            {
                delete ((TProduct*)p[k]);
            }
    }

    if (1 == ((int*)start)[POS_CNT])
    {
        delete *start;
        start = InitArray();
        return;
    }

    for (int i = posFnd; i < ((int*)start)[POS_CNT];i++)
    {
        ((void**)start)[i] = ((void**)start)[i+1];
    }
    ((int*)start)[POS_CNT]--;
}

void MainWindow::on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    if (item->text(column) == "")
    {
        QMessageBox *message = new QMessageBox(QMessageBox::Warning, "Warning",  "Спробуйте влучити по елементу");
        message->setStandardButtons(QMessageBox::Ok);
        message->exec();
        return;
    }
    if (column == 0)
    {
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

    QStringList data = item->text(column).split("\n");

    if (column == 1)
    {
        tab = 31;

        ui->lineEdit_2->setPlaceholderText("назва міста");
        ui->lineEdit_2->setText(data[0]);
        ui->lineEdit_3->setPlaceholderText("регіон");
        ui->lineEdit_3->setText(data[1]);
        ui->lineEdit_4->setPlaceholderText("поштовий індекс");
        ui->lineEdit_4->setText(data[2]);
    }
    else if (column == 2)
    {
        tab = 32;

        ui->lineEdit_2->setPlaceholderText("назва магазину");
        ui->lineEdit_2->setText(data[0]);
        ui->lineEdit_3->setPlaceholderText("адреса");
        ui->lineEdit_3->setText(data[1]);
        ui->lineEdit_4->setPlaceholderText("мобільний телефон");
        ui->lineEdit_4->setText(data[2]);
    }
    else if (column == 3)
    {
        tab = 33;

        ui->lineEdit->setVisible(true);
        ui->lineEdit_5->setVisible(true);
        ui->lineEdit_6->setVisible(true);

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

    for (int i = 0; i < ((int*)Start)[POS_CNT]; i++)
    {
        ui->comboBox->insertItem(i,QString(((TCity*)Start[i])->name));
    }


    ui->pushButton->setVisible(true);
    ui->pushButton->setText("Зберегти");
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    int pos; bool fnd;
    ui->comboBox_2->clear();
    FindElList(Start,arg1,pos,fnd,cmpCity);
    if (tab == 13 || tab == 20 || tab == 33)
    {
        if (fnd)
        {
            void ** s = (void**)(((TCity*)Start[pos])->sublev);
            for (int i = 0; i < ((int*)(s))[POS_CNT]; i++)
            {
                ui->comboBox_2->insertItem(i, ((TStore*)(s[i]))->name);
            }
            if (tab == 20)
            {
                ui->comboBox_2->addItem("null");
            }
        }
    }
}
