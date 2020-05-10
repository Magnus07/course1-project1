﻿#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <core.h>
#include <QMessageBox>
#include <QTreeWidgetItem>
#include <QTableWidgetItem>
#include <QCheckBox>
#include <Qt>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_action_6_triggered();

    void on_pushButton_clicked();

    void on_action_7_triggered();

    void on_action_8_triggered();

    void on_action_9_triggered();

//    void on_comboBox_currentIndexChanged(const QString &arg1);

    void on_action_5_triggered();

    void on_comboBox_2_currentTextChanged(const QString &arg1);

    void on_treeWidget_itemDoubleClicked(QTreeWidgetItem *item, int column);

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_action_3_triggered();

    void on_action_4_triggered();

    void on_action_10_triggered();

    void on_action_11_triggered();

    void on_action_12_triggered();

    void on_action_13_triggered();

private:
    Ui::MainWindow *ui;
    void makeInvisible();
    void refreshTreeView();
    void cleanUp();

    template <typename T>
    void sortThings();
};
#endif // MAINWINDOW_H
