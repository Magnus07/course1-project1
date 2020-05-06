#include "core.h"


////Функцiя iнiцiалiзацiї файлу.
//void initFile(FILE *&f)
//{
// TAuditoryF newRec;    //Структура для файлу
// newRec.next=-1;   //Ініціалізувати файл - значить записати в початок
//                   //файлу 2 елементи з посиланням на наступний -1.
// fwrite(&newRec,sizeof(TAuditoryF),1,f);  //Перша - заголовок списка "дірок"
// fwrite(&newRec,sizeof(TAuditoryF),1,f);  //Другий - заголовок списка елементів
// cout<<"File was initialized."<<endl;
//}


////Функція відкриття файлу зі списком
////Параметри: ім'я файлу fileName і змінна початку списку St
////Функція повертає файлову змінну f
//FILE* openFile(char fileName[15], int &St)
//{   FILE* f;
//    TAuditoryF stud;
//    if (!QFile::exists(fileName)) {    //Якщо файл НЕ існує
//        f=fopen(fileName,"w+b");       //створюємо його
//        initFile(f);                   //і ініціалізуємо
//        St=-1;                         //змінна початку пустого списку
//        }
//    else {      //Файл існує. відкриваємо для читання та запису
//        f=fopen(fileName,"r+b");
//        //Ініціалізуємо змінну початку списку5
//        fseek(f,sizeof(TAuditoryF),SEEK_SET);
//    //Заголовок списку - друга запис у файлі,
//  //тому зсув у файлі sizeof(TStudF)
//        fread(&stud,sizeof(TAuditoryF),1,f); //Читаємо заголовок списку з файлу
//        St=stud.next;
//        }
// return f;
//}


//// functions for comparing things
//// comparing among cities
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


//void addStore(void ** start)
//{
//    string  keyStore; /*ключ для пошуку міста*/
//    /*запитуємо назву університету, якому будемо додавати факультет*/
//    cout<<"\nEnter city name:\n";
//    cin>>keyStore;
//    /*Викликаємо функцію пошуку міста*/
//    bool findOk;
//    int  posFndCity;  // індекс знайденого університету
//    FindElList(start, keyStore, posFndCity, findOk, cmpCity);
//    /* Чи знайдено місто?*/
//    if (findOk==false)
//    {
//        cout<<"\nNo such city!\n";
//        return;
//    }
//    /*Створюємо новий магазин*/
//    string name;
//    string adress;
//    string telnumber;
//    cout<<"\nEnter store name:\n";
//    cin>>name;
//    cout<<"\nEnter adress:\n";
//    cin>>adress;
//    cout<<"\nEnter phone number:\n";
//    cin>>telnumber;
//    void * pNew=new TStore;
//    ((TStore *)pNew)->name=name;
//    ((TStore *)pNew)->adress=adress;
//    ((TStore *)pNew)->phnumber=telnumber;
//    /*У нового магазину ініціалізуємо підсписок товарів.*/
//    ((TStore *)pNew)->sublev=InitArray();
//    /*Викликаємо функцію пошуку місця вставки в масив вказівника нового
//    магазину*/
//    int posFndFct;  /*позиція вставки магазину*/
//    FindElList(((TCity *)start[posFndCity])->sublev, ((TStore *) pNew)->name , posFndFct, findOk, cmpStore);
//    /*Викликаємо функцію додавання магазину*/
//    addToSort(((TCity *)start[posFndCity])->sublev, pNew, posFndFct);
//}


//void addProduct(void ** start)
//{
//    string  keyCity; /*ключ для пошуку міста*/
//    string  keyStore; // key for searching among stores
//    /*запитуємо назву міста, якому будемо додавати продукт*/
//    cout<<"\nEnter city name:\n";
//    cin>>keyCity;
//    cout<<"\nEnter store name:\n";
//    cin>>keyStore;
//    /*Викликаємо функцію пошуку міста*/
//    bool findOk;
//    int  posFndCity;  // індекс знайденого міста
//    FindElList(start, keyCity, posFndCity, findOk, cmpCity);
//    /* Чи знайдено місто?*/
//    if (findOk==false)
//    {
//        cout<<"\nNo such city!\n";
//        return;
//    }

//    int posFndStore;
//    FindElList(((TCity*)start[posFndCity])->sublev, keyStore, posFndStore, findOk, cmpStore);
//    /* Чи знайдено магазин?*/
//    if (findOk==false)
//    {
//        cout<<"\nNo such store!\n";
//        return;
//    }
//    /*Створюємо новий product*/
//    string name;
//    short  id;
//    string category;
//    string description;
//    short  count;
//    short  price;
//    cout<<"\nEnter product name:\n";
//    cin>>name;
//    cout<<"\nEnter product id:\n";
//    cin>>id;
//    cout<<"\nEnter category:\n";
//    cin>>category;
//    cout<<"\nEnter product description:\n";
//    cin>>description;
//    cout<<"\nEnter count:\n";
//    cin>>count;
//    cout<<"\nEnter price:\n";
//    cin>>price;
//    void * pNew=new TProduct;
//    ((TProduct *)pNew)->name=name;
//    ((TProduct *)pNew)->id=id;
//    ((TProduct *)pNew)->category=category;
//    ((TProduct *)pNew)->description=description;
//    ((TProduct *)pNew)->count=count;
//    ((TProduct *)pNew)->price=price;

//    /*Викликаємо функцію пошуку місця вставки в масив вказівника нового
//    продукта*/
//    int posFndPct;  /*позиція вставки продукта*/
//    FindElList((((TStore*)((TCity *)start[posFndCity])->sublev[posFndStore]))->sublev, ((TProduct *) pNew)->name , posFndPct, findOk, cmpProduct);
//    /*Викликаємо функцію додавання продукта*/
//    addToSort(((TStore*)((TCity *)start[posFndCity])->sublev[posFndStore])->sublev, pNew, posFndPct);
//}


//// output function
//void outputList(void** start)
//{   // how much elements do we have
//    int cnt=((int*)start)[POS_CNT];
//    for (int i = 0; i <cnt; i++) // loop
//    {   // output third level
//        cout<<((TCity*)start[i])->name<< " | "<< ((TCity*)start[i])->region<< " | " << ((TCity*)start[i])->postcode <<"\n";
//        void ** s = (void**)(((TCity*)start[i])->sublev); // go to the next level
//        for (int j = 0; j < ((int*)(s))[POS_CNT];j++) // loop
//        {   // output
//            cout << "      |-----" << ((TStore*)s[j])->name << " | " << ((TStore*)s[j])->adress << " | " << ((TStore*)s[j])->phnumber << endl;
//            void ** p = (void**)(((TStore*)s[j])->sublev); // go to the next level
//            for (int k = 0; k < ((int*)(p))[POS_CNT];k++) // loop
//            {   // output
//                cout << "            |-----" << ((TProduct*)p[k])->name << " | " << ((TProduct*)p[k])->id << " | " <<  ((TProduct*)p[k])->category << " | " << ((TProduct*)p[k])->description << " | " << ((TProduct*)p[k])->count << " | " << ((TProduct*)p[k])->price << endl;
//            }
//        }
//    }
//}

////mode 1 - product
////mode 2 - store
////mode 3 - city
//void delEl(void** &start,  int posFnd, int mode)
//{
//    if (mode == 3)
//    {
//        void ** s = (((TCity*)start[posFnd])->sublev);
//        for (int j = 0; j < ((int*)(s))[POS_CNT];j++)
//        {
//            void * * p = (((TStore*)s[j])->sublev);
//            for (int k = 0; k < ((int*)(p))[POS_CNT];k++)
//            {
//                delete ((TProduct*)p[k]);
//            }
//            delete (TStore*)s[j];
//        }
//    }
//    else if (mode == 2)
//    {
//            void * * p = (((TStore*)start[posFnd])->sublev);
//            for (int k = 0; k < ((int*)(p))[POS_CNT];k++)
//            {
//                delete ((TProduct*)p[k]);
//            }
//    }

//    if (1 == ((int*)start)[POS_CNT])
//    {
//        delete *start;
//        //delete []((void**)start);//(((TCity*)start[0])->sublev);
//        start = InitArray();
//        return;
//    }

//    for (int i = posFnd; i < ((int*)start)[POS_CNT];i++)
//    {
//        ((void**)start)[i] = ((void**)start)[i+1];
//    }
//    ((int*)start)[POS_CNT]--;
//}


//void showCityRating(void** start)
//{   // how much elements do we have
//    int cnt=((int*)start)[POS_CNT];
//    int currentMax = -1;
//    int all[(((int*)start)[POS_CNT])];
//    for (int i = 0; i <cnt; i++) // loop
//    {
//        void ** s = (void**)(((TCity*)start[i])->sublev); // go to the next level
//        int tmp = 0;
//        for (int j = 0; j < ((int*)(s))[POS_CNT];j++) // loop
//        {
//            void ** p = (void**)(((TStore*)s[j])->sublev); // go to the next level
//            for (int k = 0; k < ((int*)(p))[POS_CNT];k++) // loop
//            {
//                tmp+=((TProduct*)p[k])->count;
//            }
//        }
//        if (tmp > currentMax)
//            currentMax = tmp;
//        all[i] = tmp;
//    }

//    for (int i = 0; i < cnt; i++)
//    {
//        if (all[i] == currentMax)
//        {
//            cout << (((TCity*)start[i])->name) << "   " << (((TCity*)start[i])->region) << "   " << (((TCity*)start[i])->postcode) << "   " << currentMax << endl;
//            all[i] = -1;
//            i = -1;

//            currentMax = -1;
//            for (int j = 0; j < cnt;j++)
//            {
//                if (all[j] > currentMax)
//                    currentMax = all[j];
//            }
//            if (currentMax == -1)
//                return;
//        }
//    }
//}



//// output function
//void findStoresWithStuff(void** start)
//{
//    string stuff;
//    cout << "Enter stuff you search for: ";
//    cin >> stuff;
//    // how much elements do we have
//    int cnt=((int*)start)[POS_CNT];
//    for (int i = 0; i <cnt; i++) // loop
//    {
//        void ** s = (void**)(((TCity*)start[i])->sublev); // go to the next level
//        for (int j = 0; j < ((int*)(s))[POS_CNT];j++) // loop
//        {
//            bool found = false;
//            void ** p = (void**)(((TStore*)s[j])->sublev); // go to the next level
//            for (int k = 0; k < ((int*)(p))[POS_CNT];k++) // loop
//            {
//                if (stuff == ((TProduct*)p[k])->name)
//                {
//                    // output
//                    cout << "|-----" << ((TProduct*)p[k])->name << " | " << ((TProduct*)p[k])->id << " | " <<  ((TProduct*)p[k])->category << " | " << ((TProduct*)p[k])->description << " | " << ((TProduct*)p[k])->count << " | " << ((TProduct*)p[k])->price;
//                    found = true;
//                }
//            }
//            if (found)
//                // output
//                cout << " --- in store --- " << ((TStore*)s[j])->name << " | " << ((TStore*)s[j])->adress << " | " << ((TStore*)s[j])->phnumber << endl;
//        }
//    }
//}

