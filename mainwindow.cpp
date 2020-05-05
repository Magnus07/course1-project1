#include "mainwindow.h"
#include "ui_mainwindow.h"

// 0 - hello screen
// 10 - add country screen
// 11 - add city screen
// 12 - add store screen
// 13 - add product screen
ushort tab = 0;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->subheader->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->treeWidget->setVisible(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::makeInvisible()
{
    ui->header->setVisible(false);
    ui->lineEdit->setVisible(false);
    ui->pushButton->setVisible(false);
    ui->treeWidget->setVisible(false);
    ui->subheader->setVisible(false);
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

}
