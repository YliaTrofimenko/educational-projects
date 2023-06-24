#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <time.h>
#define N1 70000
#define N2 110000
#define N3 150000
#define N4 190000
//#define DEBUG
using namespace std;

typedef struct
{
    int* beg; /* адрес начала последовательности */
    int len; /* длина последовательности */
}segment;

//вывод массива
void PrintArray(int* arr, int N)
{
    for (int i = 0; i < N; i++)
        cout << arr[i] << " ";
}

//переворот массива
void ArrBack(int* a, int i)
{
    int k, t;
    for (k = 0; k < (i / 2); k++) {
        t = a[k];
        a[k] = a[i - 1 - k];
        a[i - 1 - k] = t;
    }
}

//считываниие в список из файла
void IFStream(string path, int* arr, int N)
{
    ifstream file; //класс для считывание из файла
    file.open(path); //открытие файла (путь path)
    if (file.is_open()) //если получилось открыть файл
    {
        for (int i = 0; i < N; i++)
        {
            int n;
            file >> n;
            *(arr + i) = n;
        }
        file.close(); //закрытие файла
    }
    else
    {
        cout << "Ошибка открытия файла!" << endl;
    }
}

//сортировка прямым слиянием нерекурсивная
void MergeSortNonRecursive(int* a, int n, int* count, int* countpb)
{
    int* temp = (int*)malloc(sizeof(int) * n);	/* вспомогательный массив */
    int* p1, 	/* адрес элемента в одной серии */
        * p2, 	/* адрес элемента в другой серии */
        * p3, 	/* адрес элемента в формируемом массиве */
        length = 1, 		/* длина серии */
        len1 = 0, len2 = 0, 	/* текущие длины серий */
        * end_a,				/* адрес конца массива a */
        * end_temp;			/* адрес конца массива temp */
    char flag = 0;			/* признак того, откуда куда идет перезапись: 0 - из сортируемого массива во вспомогательный, 1 - из вспомогательного в исходный */
    *countpb += 5;

    while (length < n)	/* пока длина серии меньше длины массива */
    {
        p1 = a; p2 = a + length; p3 = temp; end_a = a + n; end_temp = temp + n; /* указатели ставим на стартовые позиции */
        *countpb += 5;
        while (p3 < end_temp)			/* пока переписаны не все элементы массива */
        {
            len1 = 0;			/* в первой серии всегда будут элементы, устанавливаем счетчик в 0 */
            *countpb += 1;
            if (p2 < end_a) 	/* если на вторую серию элементов в массиве хватает, */
            {
                len2 = 0;		/* то счетчик тоже устанавливаем в 0 */
                *countpb += 1;
            }
            else
            {
                len2 = length;	/* а если элементов не хватает, то серия не будет принимать участие в переписывании */
                *countpb += 1;
            }
            while (len1 < length && len2 < length && p2 < end_a)   /* пока в обеих сериях есть элементы */
            {
                if (*p1 <= *p2)						/* если значение в первой серии не больше, чем во второй, */
                {
                    *p3++ = *p1++, len1++;			/* то переписываем его в другой массив, увеличиваем счетчик изъятых элементов */
                    *countpb += 3;
                    *count += 1;
                }
                else
                {
                    *p3++ = *p2++, len2++;			/* иначе переписываем значение из второй серии */
                    *count += 1;
                    *countpb += 3;
                }
            }
            while (len1 < length && p3 < end_temp)  	/* пока первая серия не пуста */
            {
                *p3++ = *p1++, len1++;					/* переписываем ее до конца во второй массив */
                *count += 1;
                *countpb += 3;
            }
            while (len2 < length && p3 < end_temp)   	/* пока вторая серия не пуста */
            {
                *p3++ = *p2++, len2++;					/* переписываем ее до конца во второй массив */
                *count += 1;
                *countpb += 3;
            }
            p1 += length; 			/* переставляем указатели на первые элементы следующих серий */
            p2 += length;
            *countpb += 2;
        }
        length *= 2;			/* увеличиваем длину серии вдвое */
        p3 = a;			/* меняем местами входной и выходной массивы */
        a = temp;
        temp = p3;
        flag = !flag; 	/* меняем значение флага */
        *countpb += 5;
    }
    if (flag)	/* если флаг установлен, то упорядоченная последовательность находится во вспомогательном массиве */
    {
        *countpb += 2;
        for (p1 = temp, p2 = a; p1 < temp + n; *p1++ = *p2++, *count += 1, *countpb += 2);	/* переписываем ее в исходный */
        free(a);												/* освобождаем память от вспомогательного массива */
    }
    else
        free(temp);	/* иначе на вспомогательный массив указыает temp, освобождаем память от вспомогательного массива */
}

//сортировка Хоара
void QuickSort(int* a, int n, int* count, int* countpb)
{
    int x, w, i, j;
    x = a[n / 2];
    i = 0;
    j = n - 1;
    *countpb += 3;
    do
    {
        while (a[i] < x)
        {
            i++;
            *countpb += 1;
        }
        while (x < a[j])
        {
            j--;
            *countpb += 1;
        }
        if (i <= j)
        {
            if (a[i] != a[j])
            {
                w = a[i];
                a[i] = a[j];
                a[j] = w;
                *count += 3;
            }
            i++;
            j--;
            *countpb += 2;
        }
    } while (i <= j);
    if (j > 0)
    {
        QuickSort(a, j + 1, count, countpb);
    }
    if (i < n - 1)
    {
        QuickSort(a + i, n - i, count, countpb);
    }
}

//вычисление длины последовательности в зависимости от N (ответ должен быть в диапазоне (32,64])
int get_min_size(int n, int* count, int* countpb)
{
    int r = 0;
    *countpb += 1;
    while (n >= 64)
    {
        n >>= 1;
        r |= n & 1;
        *countpb += 2;
    }
    return n + r;
}

//разворот массива
void reverse(int* a, int n, int* count, int* countpb)
{
    int i, j, tmp;
    *countpb += 2;
    for (i = 0, j = n - 1; i < j; i++, j--, *countpb += 2)
    {
        if (a[i] != a[j])
        {
            tmp = a[i];
            a[i] = a[j];
            a[j] = tmp;
            *count += 3;
        }
    }
}

//сортировкa вставками
void insertion_sort(int* a, int n, int i, int* count, int* countpb) /*адрес начала массива, его размер, размер уже упорядоченной части*/
{
    int j, x;
    for (; i < n; ++i, *countpb += 1)
    {
        x = a[i];
        *countpb += 2;
        for (j = i; j && a[j - 1] > x; --j, *countpb += 1)
        {
            if (a[j] != a[j - 1])
            {
                a[j] = a[j - 1];
                *count += 1;
            }
        }
        if (a[j] != x)
        {
            a[j] = x;
            *count += 2;
            *countpb -= 1;
        }
    }
}

//слияние с предварительным копированием во вспомогательный массив более короткой последовательности
void merge_copy_less(segment* seg, int* count, int* countpb)
{
    int* a = seg[0].beg; /*адрес начала первой из сливаемых последовательностей*/
    int split = seg[0].len; /*длина первой последовательности*/
    int n = split + seg[1].len; /*длина результирующей последовательности*/
    int pos1 = 0;	/* текущая позиция чтения из первой последовательности a[0]...a[split] */
    int pos2 = split;	/*текущая позиция чтения из второй последовательности a[split]...a[n]*/
    int pos3 = 0;  /*текущая позиция записи в результирующей последовательности*/
    int* temp;
    *countpb += 6;

    if (seg[0].len < seg[1].len)
    {
        temp = (int*)malloc(sizeof(int) * split);
        *countpb += 1;
        /*копируем первую последовательность во вспомогательный массив*/
        for (pos1 = 0; pos1 < split; pos1++, *countpb += 1)
        {
            temp[pos1] = a[pos1];
            *count += 1;
        }
        pos1 = 0;
        *countpb += 1;

        while (pos1 < split && pos2 < n) 	/*идет слияние, пока есть хоть один элемент в каждой последовательности*/
            if (temp[pos1] <= a[pos2])
            {
                a[pos3++] = temp[pos1++];
                *countpb += 2;
                *count += 1;
            }
            else
            {
                a[pos3++] = a[pos2++];
                *countpb += 2;
                *count += 1;
            }
        /* одна последовательность закончилась - копировать остаток другой в конец буфера */
        /* даже если вторая последовательность не пуста, она уже на месте */
        while (pos1 < split)  		/* пока первая последовательность не пуста */
        {
            a[pos3++] = temp[pos1++];
            *countpb += 2;
            *count += 1;
        }
    }
    else
    {
        temp = (int*)malloc(sizeof(int) * seg[1].len);
        *countpb += 2;
        /*копируем вторую последовательность во вспомогательный массив*/
        for (pos1 = 0, pos2 = split; pos2 < n; )
        {
            temp[pos1++] = a[pos2++];
            *countpb += 2;
            *count += 1;
        }
        /*в этом случае слияние производится справа налево*/
        pos1 = split - 1;
        pos2 = seg[1].len - 1;
        pos3 = n - 1;
        *countpb += 3;
        while (pos1 >= 0 && pos2 >= 0) 	/*идет слияние, пока есть хоть один элемент в каждой последовательности*/
            if (temp[pos2] > a[pos1])
            {
                a[pos3--] = temp[pos2--];
                *countpb += 2;
                *count += 1;
            }
            else
            {
                a[pos3--] = a[pos1--];
                *count += 1;
                *countpb += 2;
            }
        /* одна последовательность закончилась - копировать остаток другой в конец буфера */
        /* даже если первая последовательность не пуста, она уже на месте */
        while (pos2 >= 0)  		/* пока вторая последовательность не пуста */
        {
            a[pos3--] = temp[pos2--];
            *countpb += 2;
            *count += 1;
        }
    }
    free(temp);
    seg[0].len = n; /*фиксируем длину полученной последовательности в стеке*/
    *countpb += 1;
}

//слияние близких по размеру последовательностей
int try_merge(segment* seg, int top, int* count, int* countpb)
{
    int x, y, z;
    while (top > 0) /*пока в стеке больше одного элемента*/
    {
        x = seg[top].len;
        y = seg[top - 1].len;
        z = top > 1 ? seg[top - 2].len : 0;
        *countpb += 3;

        if (top > 1 && z <= x + y) /*если в стеке не меньше трех элементов и третий сверху маловат*/
        {
            if (z < x)
            {
                merge_copy_less(&seg[top - 2], count, countpb); /*сливаем ZY*/
                seg[top - 1] = seg[top]; /*корректируем стек*/
                *countpb += 1;
            }
            else
            {
                merge_copy_less(&seg[top - 1], count, countpb); /*сливаем YX*/
            }
            top--;  /*стало на одну последовательность меньше*/
            *countpb += 1;
        }
        else
            if (y <= x) /*если предпоследний не больше последнего*/
            {
                merge_copy_less(&seg[top - 1], count, countpb); /*сливаем YX*/
                top--;
                *countpb += 1;
            }
            else
                break;
    }
    return top;
}

//сортировка методом timsort
void tim_sort(int* a, int n, int* count, int* countpb)
{
    int min_size = get_min_size(n, count, countpb);
    int size;
    int i, j = 0;
    /*стек координат последовательностей*/
    segment* seg = (segment*)malloc(((n - 1) / min_size + 1) * sizeof(segment));
    int t = -1; /*вершина стека*/
    *countpb += 4;

    /*формирование упорядоченных последовательностей*/
    for (i = 0; i < n; i += size, *countpb += 1)
    {
        for (j = i + 1; j < n && a[j - 1] >= a[j]; j++, *countpb += 1); /*поиск обратно упорядоченной последовательности*/
            
        if (j != i + 1)
        {
            reverse(a + i, j - i, count, countpb);          /*переворачиваем найденную обратную последовательность*/
        }
        if (j != n && j - i < min_size)
        {
            size = n - i < min_size ? n - i : min_size;
            insertion_sort(a + i, size, j - i, count, countpb); /*адрес начала фрагмента, его размер, размер упорядоченной части*/
            j = i + size;
            *countpb += 2;
        }
        for (; j < n && a[j - 1] <= a[j]; j++, *countpb += 1); /*ищем конец последовательности, если она не закончилась*/

            /*запоминаем адрес начала и длину последовательности в стеке*/
            seg[++t].beg = a + i;

        size = seg[t].len = j - i;
        t = try_merge(seg, t, count, countpb);
        *countpb += 5;
    }
    /*слияние всех оставшихся последовательностей*/
    while (t > 0)
    {
        merge_copy_less(&seg[t - 1], count, countpb); /*сливаем две последние последовательности*/
        t--; /*количество последовательностей уменьшилось*/
        *countpb += 1;
    }
    free(seg);
}

typedef unsigned char uchar;
typedef unsigned short int ushort;

template <class T>
void swap(T& a, T& b, int* count, int* countpb)
{
    T tmp = a;
    a = b;
    b = tmp;
    *count += 3;
}

// Создать счетчики. data-сортируемый массив, counters-массив для счетчиков, N-число элементов в data
template<class T>
void createCounters(T* data, long* counters, long N, int* count, int* countpb) {
    // i-й массив count расположен, начиная с адреса counters+256*i
    memset(counters, 0, 256 * sizeof(T) * sizeof(long));

    uchar* bp = (uchar*)data;
    uchar* dataEnd = (uchar*)(data + N);
    ushort i;
    *countpb += 2;

    while (bp != dataEnd) 
    {
        // увеличиваем количество байт со значением *bp
        // i - текущий массив счетчиков
        *countpb += 1;
        for (i = 0; i < sizeof(T);i++, *countpb += 1)
        {
            counters[256 * i + *bp++]++;
            *countpb += 2;
        }
    }
}

// Функция radixPass принимает в качестве параметров номер байта Offset, число элементов N, исходный массив source, массив dest, куда будут записываться числа, отсортированные по байту Offset массив счетчиков count, соответствующий текущему проходу.
template<class T>
void radixPass(short Offset, long N, T* source, T* dest, long* count, int* countt, int* countpb) {
    // временные переменные
    T* sp;
    long s, c, i, * cp;
    uchar* bp;


    // шаг 3
    s = 0; 	// временная переменная, хранящая сумму на данный момент
    cp = count;
    *countpb += 3;

    for (i = 256; i > 0; --i, ++cp, *countpb += 2)
    {
        c = *cp;
        *cp = s;
        s += c;
        *countpb += 3;
    }

    // шаг 4
    bp = (uchar*)source + Offset;
    sp = source;
    *countpb += 3;

    for (i = N; i > 0; --i, bp += sizeof(T), ++sp, *countpb += 3)
    {
        cp = count + *bp;
        dest[*cp] = *sp;
        ++(*cp);
        *countt += 1;
        *countpb += 2;
    }
}

// проход поразрядной сортировки по старшим байтам, для целых чисел со знаком Offset = sizeof(T)-1.
template<class T>
void signedRadixLastPass(short Offset, long N, T* source, T* dest, long* count, int* countt, int* countpb) {
    T* sp;
    long s, c, i, * cp;
    uchar* bp;

    // подсчет количества отрицательных чисел
    long numNeg = 0;
    *countpb += 2;
    for (i = 128;i < 256;i++, *countpb += 1)
    {
        numNeg += count[i];
        *countpb += 1;
    }

    // первые 128 элементов count относятся к положительным числам.
    // отсчитываем номер первого числа, начиная от numNeg
    s = numNeg;
    cp = count;
    *countpb += 3;

    for (i = 0; i < 128; ++i, ++cp, *countpb += 2)
    {
        c = *cp;
        *cp = s;
        s += c;
        *countpb += 3;
    }

    // номера для отрицательных чисел отсчитываем от начала массива
    s = 0;
    cp = count + 128;
    *countpb += 3;

    for (i = 0; i < 128; ++i, ++cp, *countpb += 2)
    {
        c = *cp;
        *cp = s;
        s += c;
        *countpb += 3;
    }


    bp = (uchar*)source + Offset;
    sp = source;
    *countpb += 3;

    for (i = N; i > 0; --i, bp += sizeof(T), ++sp, *countpb += 3)
    {
        cp = count + *bp;
        dest[*cp] = *sp;
        ++(*cp);
        *countpb += 2;
        *countt += 1;
    }
}

template<class T>
void signedRadixSort(T*& in, long N, int* countt, int* countpb) 
{
    T* out = new T[N];
    ushort i;

    long* counters = new long[sizeof(T) * 256], * count;
    createCounters(in, counters, N, countt, countpb);

    *countpb += 1;
    for (i = 0; i < sizeof(T) - 1; i++, *countpb += 1)
    {
        *countpb += 1;
        count = counters + 256 * i;
        if (count[0] == N) continue;
        radixPass(i, N, in, out, count, countt, countpb);
        swap(in, out);
    }
    count = counters + 256 * i;
    *countpb += 1;
    signedRadixLastPass(i, N, in, out, count, countt, countpb);

    delete[] in;
    in = out;
    *countpb += 1;
    delete[] counters;
}


int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

#ifdef DEBUG
    const int Size = 10;
    string path = "test.txt";
#else
    const int Size = N4;
    //string path = "test_numbers.txt";
    string path = "test1000.txt";
#endif
    //cout << "Путь к файлу: ";
    //cin >> path;


    bool cikl = true;
    while (cikl)
    {
        clock_t TimeStart, TimeEnd; //переменные для подсчёта времени
        int* arr = new int[Size]; //массив
        int count = 0, countpb = 0; //переменные для количества присваиваний

        IFStream(path, arr, Size); //заполняем список из файла

        cout << "\tВыберите метод сортировки:" << endl
            << "1. Хоара" << endl
            << "2. Timsort" << endl
            << "3. Сортировка прямым слиянием" << endl
            << "4. Поразрядная сортировка" << endl
            << "5. Выход" << endl;

        char menu;
        cin >> menu;
        cin.ignore(cin.rdbuf()->in_avail());

        switch (menu)
        {
        case '1':
        {
            system("CLS");
            cout << "\tМетод Хоара:" << endl << endl;

            cout << "Неупорядоченный массив:" << endl;
#ifdef DEBUG
            cout << "До: ";
            PrintArray(arr, Size);
#endif
            TimeStart = clock();
            QuickSort(arr, Size, &count, &countpb);
            TimeEnd = clock() - TimeStart;
#ifdef DEBUG
            cout << endl << "После: ";
            PrintArray(arr, Size);
            cout << endl;
#endif
            cout << "Количество основных присваиваний: " << count << endl;
            cout << "Количество дополнительных присваиваний: " << countpb << endl;
            cout << "Затраченное время: " << TimeEnd << " милисек" << endl;
            count = countpb = 0;



            cout << endl << "Упорядоченный массив:" << endl;
#ifdef DEBUG
            cout << "До: ";
            PrintArray(arr, Size);
#endif
            TimeStart = clock();
            QuickSort(arr, Size, &count, &countpb);
            TimeEnd = clock() - TimeStart;
#ifdef DEBUG
            cout << endl << "После: ";
            PrintArray(arr, Size);
            cout << endl;
#endif
            cout << "Количество основных присваиваний: " << count << endl;
            cout << "Количество дополнительных присваиваний: " << countpb << endl;
            cout << "Затраченное время: " << TimeEnd << " милисек" << endl;
            count = countpb = 0;



            cout << endl << "Упорядоченный в обратном порядке массив:" << endl;
            ArrBack(arr, Size);
#ifdef DEBUG
            cout << "До: ";
            PrintArray(arr, Size);
#endif
            TimeStart = clock();
            QuickSort(arr, Size, &count, &countpb);
            TimeEnd = clock() - TimeStart;
#ifdef DEBUG
            cout << endl << "После: ";
            PrintArray(arr, Size);
            cout << endl;
#endif
            cout << "Количество основных присваиваний: " << count << endl;
            cout << "Количество дополнительных присваиваний: " << countpb << endl;
            cout << "Затраченное время: " << TimeEnd << " милисек" << endl << endl;
            count = countpb = 0;

            system("pause"); system("CLS");
            break;
        }

        case '2':
        {
            system("CLS");
            cout << "\tTimsort:" << endl << endl;

            cout << "Неупорядоченный массив:" << endl;
#ifdef DEBUG
            cout << "До: ";
            PrintArray(arr, Size);
#endif
            TimeStart = clock();
            tim_sort(arr, Size, &count, &countpb);
            TimeEnd = clock() - TimeStart;
#ifdef DEBUG
            cout << endl << "После: ";
            PrintArray(arr, Size);
            cout << endl;
#endif
            cout << "Количество основных присваиваний: " << count << endl;
            cout << "Количество дополнительных присваиваний: " << countpb << endl;
            cout << "Затраченное время: " << TimeEnd << " милисек" << endl;
            count = countpb = 0;



            cout << endl << "Упорядоченный массив:" << endl;
#ifdef DEBUG
            cout << "До: ";
            PrintArray(arr, Size);
#endif
            TimeStart = clock();
            tim_sort(arr, Size, &count, &countpb);
            TimeEnd = clock() - TimeStart;
#ifdef DEBUG
            cout << endl << "После: ";
            PrintArray(arr, Size);
            cout << endl;
#endif
            cout << "Количество основных присваиваний: " << count << endl;
            cout << "Количество дополнительных присваиваний: " << countpb << endl;
            cout << "Затраченное время: " << TimeEnd << " милисек" << endl;
            count = countpb = 0;



            cout << endl << "Упорядоченный в обратном порядке массив:" << endl;
            ArrBack(arr, Size);
#ifdef DEBUG
            cout << "До: ";
            PrintArray(arr, Size);
#endif
            TimeStart = clock();
            tim_sort(arr, Size, &count, &countpb);
            TimeEnd = clock() - TimeStart;
#ifdef DEBUG
            cout << endl << "После: ";
            PrintArray(arr, Size);
            cout << endl;
#endif
            cout << "Количество основных присваиваний: " << count << endl;
            cout << "Количество дополнительных присваиваний: " << countpb << endl;
            cout << "Затраченное время: " << TimeEnd << " милисек" << endl << endl;
            count = countpb = 0;

            system("pause"); system("CLS");
            break;
        }

        case '3':
        {
            system("CLS");
            cout << "\tСортировка прямым слиянием:" << endl << endl;

            cout << "Неупорядоченный массив:" << endl;
#ifdef DEBUG
            cout << "До: ";
            PrintArray(arr, Size);
#endif
            TimeStart = clock();
            MergeSortNonRecursive(arr, Size, &count, &countpb);
            TimeEnd = clock() - TimeStart;
#ifdef DEBUG
            cout << endl << "После: ";
            PrintArray(arr, Size);
            cout << endl;
#endif
            cout << "Количество основных присваиваний: " << count << endl;
            cout << "Количество дополнительных присваиваний: " << countpb << endl;
            cout << "Затраченное время: " << TimeEnd << " милисек" << endl;
            count = countpb = 0;



            cout << endl << "Упорядоченный массив:" << endl;
#ifdef DEBUG
            cout << "До: ";
            PrintArray(arr, Size);
#endif
            TimeStart = clock();
            MergeSortNonRecursive(arr, Size, &count, &countpb);
            TimeEnd = clock() - TimeStart;
#ifdef DEBUG
            cout << endl << "После: ";
            PrintArray(arr, Size);
            cout << endl;
#endif
            cout << "Количество основных присваиваний: " << count << endl;
            cout << "Количество дополнительных присваиваний: " << countpb << endl;
            cout << "Затраченное время: " << TimeEnd << " милисек" << endl;
            count = countpb = 0;



            cout << endl << "Упорядоченный в обратном порядке массив:" << endl;
            ArrBack(arr, Size);
#ifdef DEBUG
            cout << "До: ";
            PrintArray(arr, Size);
#endif
            TimeStart = clock();
            MergeSortNonRecursive(arr, Size, &count, &countpb);
            TimeEnd = clock() - TimeStart;
#ifdef DEBUG
            cout << endl << "После: ";
            PrintArray(arr, Size);
            cout << endl;
#endif
            cout << "Количество основных присваиваний: " << count << endl;
            cout << "Количество дополнительных присваиваний: " << countpb << endl;
            cout << "Затраченное время: " << TimeEnd << " милисек" << endl << endl;
            count = countpb = 0;

            system("pause"); system("CLS");
            break;
        }

        case '4':
        {
            system("CLS");
            cout << "\tПоразрядная сортировка:" << endl << endl;

            cout << "Неупорядоченный массив:" << endl;
#ifdef DEBUG
            cout << "До: ";
            PrintArray(arr, Size);
#endif
            TimeStart = clock();
            signedRadixSort(arr, Size, &count, &countpb);
            TimeEnd = clock() - TimeStart;
#ifdef DEBUG
            cout << endl << "После: ";
            PrintArray(arr, Size);
            cout << endl;
#endif
            cout << "Количество основных присваиваний: " << count << endl;
            cout << "Количество дополнительных присваиваний: " << countpb << endl;
            cout << "Затраченное время: " << TimeEnd << " милисек" << endl;
            count = countpb = 0;



            cout << endl << "Упорядоченный массив:" << endl;
#ifdef DEBUG
            cout << "До: ";
            PrintArray(arr, Size);
#endif
            TimeStart = clock();
            signedRadixSort(arr, Size, &count, &countpb);
            TimeEnd = clock() - TimeStart;
#ifdef DEBUG
            cout << endl << "После: ";
            PrintArray(arr, Size);
            cout << endl;
#endif
            cout << "Количество основных присваиваний: " << count << endl;
            cout << "Количество дополнительных присваиваний: " << countpb << endl;
            cout << "Затраченное время: " << TimeEnd << " милисек" << endl;
            count = countpb = 0;



            cout << endl << "Упорядоченный в обратном порядке массив:" << endl;
            ArrBack(arr, Size);
#ifdef DEBUG
            cout << "До: ";
            PrintArray(arr, Size);
#endif
            TimeStart = clock();
            signedRadixSort(arr, Size, &count, &countpb);
            TimeEnd = clock() - TimeStart;
#ifdef DEBUG
            cout << endl << "После: ";
            PrintArray(arr, Size);
            cout << endl;
#endif
            cout << "Количество основных присваиваний: " << count << endl;
            cout << "Количество дополнительных присваиваний: " << countpb << endl;
            cout << "Затраченное время: " << TimeEnd << " милисек" << endl << endl;
            count = countpb = 0;

            system("pause"); system("CLS");
            break;
        }

        case '5':
        {
            cikl = false;
            break;
        }

        default:
        {
            system("CLS");
            cout << "Выберете пункт меню введя число от 1 до 5!" << endl << endl;
            break;
        }
        }
        delete[] arr;
    }
    return 0;
}