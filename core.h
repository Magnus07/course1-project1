#ifndef CORE_H
#define CORE_H

#include <QApplication>

#define POS_CNT      -1
#define POS_SZ       -2
#define SIZE_AR_FIRST 5
#define DELTA         3

using namespace std;

typedef int (*TfuncCmp)(void *p, QString key);
// опис типу структури для елемента - City
typedef struct
{
    QString name;
    QString region;
    uint    postcode;
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
    ushort  id;
    QString category;
    QString description;
    ushort  count;
    float  price;
} TProduct;

// headers
void ** InitArray();
void addToSort(void** &start,  void* pnew, int posAdd);
void FindElList(void** start,  QString key, int &posFndEl, bool &findOK, TfuncCmp pFunc);
void delEl(void** &start,  int posFnd, int mode);

int cmpCity(void* p, QString key);
int cmpStore(void *p, QString key);
int cmpProduct(void *p, QString key);

#endif // CORE_H
