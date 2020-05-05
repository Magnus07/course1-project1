#ifndef CORE_H
#define CORE_H

#include <QApplication>

#define POS_CNT      -1
#define POS_SZ       -2
#define SIZE_AR_FIRST 5
#define DELTA         3

using namespace std;

typedef int (*TfuncCmp)(void *p, string key);
// опис типу структури для елемента - City
typedef struct
{
    QString name;
    QString region;
    int    postcode;
    void** sublev;  // вказівник на підсписок stores
}TCity;

// опис типу структури для елемента - Store
typedef struct
{
    QString name;
    QString adress;
    QString phnumber;
    void** sublev;  // вказівник на підсписок products
} TStore;

// опис типу структури для елемента - Product
typedef struct
{
    QString name;
    short  id;
    QString category;
    QString description;
    short  count;
    short  price;
} TProduct;

#endif // CORE_H
