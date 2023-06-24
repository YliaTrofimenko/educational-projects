#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <windows.h>
#define N 200000

using namespace std;

/* Объявление счетчиков сравнений */
int each;
int all;

#pragma pack(push,1) //выравнивание в 1 байт
struct node /* Структура для представления узлов дерева */
{
    struct node* left, * right;
    int key;
    unsigned height;
};
#pragma pack(pop)

struct node2 /* Структура для представления узлов дерева */
{
    int key;
    struct node2* left, * right;
};

typedef struct node avl_node;
typedef struct node2 nesb_node;
typedef struct node* tree;
typedef struct node2* tree2;

int bfactor(tree p) // вычисление баланса
{
    return (p->left ? p->left->height : 0) - (p->right ? p->right->height : 0);
}

node* make_node(int k)/* Создание нового дерева */
{
    node* n = new node;
    if (n == NULL)
        return NULL;
    n->key = k;
    n->left = n->right = NULL;
    n->height = 1;
    return n;
}

node2* make_node2(int k)/* Создание нового дерева */
{
    node2* n = new node2;
    if (n == NULL)
        return NULL;
    n->key = k;
    n->left = n->right = NULL;
    return n;
}

void fixheight(avl_node* p) // корректировка высоты после добавления/удаления узлов
{
    unsigned char hl = p->left ? p->left->height : 0;
    unsigned char hr = p->right ? p->right->height : 0;
    p->height = (hl > hr ? hl : hr) + 1;
}

avl_node* rotateright(avl_node* p) // правый поворот вокруг p
{
    avl_node* q = p->left;
    p->left = q->right;
    q->right = p;
    fixheight(p);
    fixheight(q);
    return q;
}

avl_node* rotateleft(avl_node* q) // левый поворот вокруг q
{
    avl_node* p = q->right;
    q->right = p->left;
    p->left = q;
    fixheight(q);
    fixheight(p);
    return p;
}

avl_node* balance(avl_node* p) /* Балансировка узла p */
{
    fixheight(p);
    if (bfactor(p) == -2)
    {
        if (bfactor(p->right) > 0)
            p->right = rotateright(p->right);
        return rotateleft(p);
    }
    if (bfactor(p) == 2)
    {
        if (bfactor(p->left) < 0)
            p->left = rotateleft(p->left);
        return rotateright(p);
    }
    return p; // балансировка не нужна
}

tree avl_insert(tree p, int k) /* Вставка ключа k в дерево с корнем p */
{
    if (!p) return make_node(k);
    if (k == p->key) return p;
    if (k < p->key)
        p->left = avl_insert(p->left, k);
    else
        p->right = avl_insert(p->right, k);
    return balance(p);
}

avl_node* findmin(avl_node* p) /* Поиск узла с минимальным ключом в дереве p */
{
    return p->left ? findmin(p->left) : p;
}

avl_node* removemin(avl_node* p) /* Удаление узла с минимальным ключом из дерева p */
{
    if (p->left == 0)
        return p->right;
    p->left = removemin(p->left);
    return balance(p);
}

tree2 nesb_insert(tree2 p, int k) /* Вставка ключа k в дерево с корнем p */
{
    if (!p) return make_node2(k);
    if (k == p->key) return p;
    if (k < p->key)
        p->left = nesb_insert(p->left, k);
    else
        p->right = nesb_insert(p->right, k);
    return p;
}

void del_tree(tree t) /* Удаление дерева */
{
    if (t->left) del_tree(t->left);
    if (t->right) del_tree(t->right);
    delete t;
}

void del_tree2(tree2 t) /* Удаление дерева */
{
    if (t->left) del_tree2(t->left);
    if (t->right) del_tree2(t->right);
    delete t;
}
node* find(node*, int); /* Нахождение ключа */
node2* find2(node2*, int); /* Нахождение ключа */

int main()
{
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    ifstream f;
    int keys[100];
    int x;
    tree avl_tree = NULL;
    tree2 nesb_tree = NULL;

    f.open("test.txt");
    if (!f.is_open())
    {
        perror("Ошибка открытия файла."); return -1;
    }
    for (int i = 0; i < N; i++) /* считываем значения из файла и создаем дерево */
    {
        f >> x; avl_tree = avl_insert(avl_tree, x);
    }
    f.close();

    f.open("test_keys.txt");
    if (!f.is_open())
    {
        perror("Ошибка открытия файла с ключами."); return -1;
    }
    for (int i = 0; i < 100; i++) /* считываем 100 ключей из файла */
    {
        f >> x; keys[i] = x;
    }
    f.close();

    cout << "АВЛ-дерево" << endl << endl;
    for (int i = 0; i < 100; i++) /* поиск по АВЛ-дереву */
    {
        each = 0;
        avl_node* temp = find(avl_tree, keys[i]);
        if (!temp)
            cout << "Ключ " << keys[i] << " не найден. Количество сравнений: " << each << endl;
        else
            cout << "Ключ " << keys[i] << " найден. Количество сравнений: " << each << endl;

    }
    cout << endl;
    cout << "Всего сравнений: " << all << endl;
    cout << "Среднее количество сравнений для одного ключа: " << all / 100 << endl;
    cout << "Количество требуемой памяти для реализации структуры: " << endl << sizeof(avl_node) * N << " байт" << endl;
    del_tree(avl_tree); /* удаляем АВЛ-дерево */

    cout << "---------------------------------------------------------------------------------" << endl;

    f.open("test.txt");
    if (!f.is_open())
    {
        perror("Ошибка открытия файла."); return -1;
    }
    for (int i = 0; i < N; i++) /* считываем значения из файла и создаем дерево */
    {
        f >> x; nesb_tree = nesb_insert(nesb_tree, x);
    }
    f.close();

    f.open("test_keys.txt");
    if (!f.is_open())
    {
        perror("Ошибка открытия файла с ключами."); return -1;
    }
    for (int i = 0; i < 100; i++) /* считываем 100 ключей из файла */
    {
        f >> x; keys[i] = x;
    }
    f.close();

    cout << "Несбалансированное бинарное дерево поиска " << endl << endl;
    for (int i = 0; i < 100; i++) /* поиск по несбалансированное бинарное дерево поиска */
    {
        each = 0;
        nesb_node* temp = find2(nesb_tree, keys[i]);
        if (!temp)
            cout << "Ключ " << keys[i] << " не найден. Количество сравнений: " << each << endl;
        else
            cout << "Ключ " << keys[i] << " найден. Количество сравнений: " << each << endl;

    }
    cout << endl;
    cout << "Всего сравнений: " << all << endl;
    cout << "Среднее количество сравнений для одного ключа: " << all / 100 << endl;
    cout << "Количество требуемой памяти для реализации структуры: " << endl << sizeof(nesb_node) * N << " байт" << endl;
    del_tree2(nesb_tree); /* удаляем несбалансированное бинарное дерево поиска */
    return 0;
}

node* find(node* p, int key) /* Нахождение ключа */
{
    while (p)
    {
        each++; all++;
        if (key == p->key)
            return p;
        each++; all++;
        if (key < p->key)
            p = p->left;
        else
            p = p->right;
    }
    return p;
}

node2* find2(node2* p, int key) /* Нахождение ключа */
{
    while (p)
    {
        each++; all++;
        if (key == p->key)
            return p;
        each++; all++;
        if (key < p->key)
            p = p->left;
        else
            p = p->right;
    }
    return p;
}
