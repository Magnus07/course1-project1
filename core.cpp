#include "core.h"


// functions for comparing things
// comparing among cities
int cmpCity(void* p, QString key){
    if (((TCity*)p)->name == key) // if it's our key
    { return 0; }
    else { // in other situation
        if (((TCity*)p)->name > key)
        { return 1; }
        else {return -1;}
    }
}

// comparing among stores
int cmpStore(void *p, QString key){
    if (((TStore*)p)->name == key)// if it's our key
    { return 0; }
    else {// in other situation
        if (((TStore*)p)->name > key)
        { return 1; }
        else {return -1;}
    }
}

// comparing among products
int cmpProduct(void *p, QString key){
    if (((TProduct*)p)->name == key)// if it's our key
    { return 0; }
    else {// in other situation
        if (((TProduct*)p)->name > key)
        { return 1; }
        else { return -1; }
    }
}


/* Функцiя iнiцiалiзацiї.*/
void ** InitArray()
{  /* SIZE_AR_FIRST -  константа, початковий розмір масиву*/
   void ** p=new void* [SIZE_AR_FIRST+2];	/* виділення пам'яті для нового масиву*/
   p=p+2;  		/* вказівник вниз на 2 елементи щоб він вказував на перший вказівник а не на службовий елемент*/
((int*)p)[POS_CNT]  = 0; 	/*заповнюємо службовий елемент масиву –
            кількість елементів списку*/
   ((int*)p)[POS_SZ]  = SIZE_AR_FIRST; 	/*заповнюємо службовий елемент масиву – розмір масиву */
   return p;
}


/* Функцiя розширення. ar – вказівник на масив вказівників*/
void ExpandArrPtr (void** &ar, int &SizeAr, int Cnt)
{
  int	   SizeW = SizeAr;   /*збереження старого розміру масиву*/
  SizeAr = SizeAr + DELTA;   /*визначення нового розміру масиву*/
  void**   arW;              /*допоміжний безтиповий покажчик на
                             початок масиву покажчиків*/
  arW = new void*[SizeAr+2]; /*виділення місця для нового масиву*/
  /*копіювання старого масиву вказівників в новий масив*/
  for (int i=0; i<SizeW; i++)
    { arW[i+2] = ar[i]; }
  ar=ar-2;      // ставимо вказівник на початок старого масиву
  delete []ar;  //звільняємо пам'ять
  ar=arW+2;  //ставимо вказівник масиву на перший вказівник в новому масиві
  ((int*)ar)[POS_CNT]  = Cnt;    /* заповнюємо службові елементи */
  ((int*)ar)[POS_SZ]  = SizeAr;
}


/* Функція пошуку половинним діленням.
start – вказівник на список,
key – ключ пошуку,
posFndEl – індекс знайденого елементу
findOk – результат пошуку (true – якщо  елемент знайдено,
         false – якщо не знайдено елемент з таким ключем)
pFunc – вказівник на функцію порівняння */
void FindElList(void** start,  QString key, int &posFndEl, bool &findOK, TfuncCmp pFunc)
{
 int	middl;    /* індекс середнього елемента масиву*/
 int	hi, low;  /* поточні межі - верхня, нижня*/
 findOK = false;  /* елемент не знайдено*/
 int resultCompare;
 if (((int*)start)[POS_CNT] == 0)
 {      /* якщо список пустий */
        posFndEl = 0;
        return;
 }
 /* Встановлюємо межі пошуку на межі масиву*/
 low = 0;
 hi = ((int*)start)[POS_CNT] - 1;
 do
 {  middl = (hi + low) / 2 ; /*індекс середнього елементу*/
    /*Виклик функції порівняння через вказівник на функцію*/
    resultCompare = pFunc(start[middl], key);
    if (resultCompare== 0) /*Елемент знайдено*/
        { posFndEl = middl;
          findOK = true;
          return;
        }
     if (resultCompare== -1) /*зміна меж пошуку*/
        { hi = middl - 1; }
     else
        { low = middl + 1; }
 } while  (low <= hi);
 posFndEl = low;    /*Перебрано весь масив. Елемент не знайдено.
                    Визначена позиція можливої вставки нового елемента*/
}



/* Функцiя додавання у вiдсортований список. Отримує:
start – вказiвник на список
pnew – вказiвник на новий елемент списку
posAdd – індекс в масиві, куди необхідно вставити вказiвник на новий елемент
*/
void addToSort(void** &start,  void* pnew, int posAdd)
{
    int cnt=((int*)start)[POS_CNT];
    int sz= ((int*)start)[POS_SZ];
    //якщо масив переповнений, то викликаємо розширення масиву
    if (cnt == sz)
    {
        ExpandArrPtr (start, sz, cnt);
    }
    if (posAdd == cnt) //елемент додається в кінець списку
    {
        start[posAdd] = pnew;
    }
    else
    {     // або зсув елементів масиву вправо
        for (int k = cnt-1; k >= posAdd; k--)
            { start[k+1] = start[k]; }
        start[posAdd] = pnew;
    }
    cnt++;
    ((int*)start)[POS_CNT]=cnt;
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
        //delete []((void**)start);//(((TCity*)start[0])->sublev);
        start = InitArray();
        return;
    }

    for (int i = posFnd; i < ((int*)start)[POS_CNT];i++)
    {
        ((void**)start)[i] = ((void**)start)[i+1];
    }
    ((int*)start)[POS_CNT]--;
}

