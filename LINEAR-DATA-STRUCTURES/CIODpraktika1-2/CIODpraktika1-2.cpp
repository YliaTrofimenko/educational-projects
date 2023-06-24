#include <iostream>
#include <string>
#include <Windows.h>
using namespace std;


template<typename T>
class interfaceSt 
{
public:
	bool virtual isEmpty() = 0; //виртуальный метод проверки пустоты стека
	virtual T top() = 0; //виртуальный метод неразрушающего чтения с вершины стека
	virtual T pop() = 0; //виртуальный метод извлечения элемента из стека
	virtual int push(T) = 0; //виртуальный метод добавления элемента в стек
	virtual ~interfaceSt() {}; //виртуальный деструктор
};

//векторная структура хранения
template<typename T>
class StVector: public interfaceSt<T> 
{
private:
	int _top = 0; //индекс верхнего элемента стека
	T* data; //элемент стека
	int maxlength = 0; //размер стека

public:
	virtual bool isEmpty() override //проверка пустоты стека
	{
		if (_top < 0) 
		{
			return true;
		}
		else 
		{
			return false;
		}
	}

	virtual T top() override //метод неразрушающего чтения с вершины стека
	{
		return data[_top];
	}

	virtual T pop() override //метод извлечения элемента из стека
	{
		if (_top > -1)
		{
			return data[_top--];
		}
	}

	virtual int push(T data) override //метод добавления элемента в стек
	{
		if (_top == maxlength - 1) return 0;
		this->data[++_top] = data;
		return 1;
	}

	virtual ~StVector() override //деструктор
	{
		delete[]data;
	}

	StVector(int n) //конструктор
	{
		maxlength = n;
		data = new T[maxlength];
		_top = -1;
	}
};

//класс элемента стека для связной структуры хранения
template<typename T>
class Element {
private:
	T* data; //элемент стека
	Element* next; //указатель на следующий элемент стека

public:
	T getData() //метод получения элемента
	{
		return *data;
	}

	void setData(T data) //метод инициализации элемента
	{
		this->data = new T;
		*this->data = data;
	}

	Element* getNext() //метод получения адреса на следующия элемент
	{
		return next;
	}

	void setNext(Element* next) //метод инициалиализации указателя на следующий элемент
	{
		this->next = next;
	}
};


typedef Element<float>* p_stack;

//связная структуа хранения
template<typename T>
class St_list: public interfaceSt<T>
{
private:
	p_stack pstack; //верхний элемент стека

public:
	virtual bool isEmpty() override //проверка пустоты
	{
		if (pstack == NULL) 
		{
			return true;
		}

		else 
		{
			return false;
		}
	}

	virtual T top() override //метод неразрушающего чтения с вершины стека
	{
		return pstack->getData();
	}

	virtual T pop() override //метод извлечения элемента из стека
	{
		p_stack del = pstack;
		T temp = pstack->getData();
		pstack = pstack->getNext();
		delete del;
		return temp;
	}

	virtual int push(T data) override //метод добавления элемента в стек
	{
		p_stack ins = new Element<T>;
		if (ins == NULL) 
		{
			return 0;
		}
		ins->setData(data);
		ins->setNext(pstack);
		pstack = ins;
		return 1;
	}

	virtual ~St_list() override //деструктор
	{
		p_stack temp;
		while (pstack) 
		{
			temp = pstack;
			pstack = pstack->getNext();
			delete temp;
		}
	}

	St_list() //конструктор
	{
		pstack = NULL;
	}
};

int addInStack(interfaceSt<float>* st); //сигнатура функции работы со стеком

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251); //подключения русского языка

	bool f = 1;

	while (f == 1)
	{
		char menu;

		interfaceSt<float>* st;
		cout << "Выберите реализацию стека:" << endl;
		cout << "1 - векторная" << endl;
		cout << "2 - связанная" << endl;
		cin >> menu;
		system("CLS");

		if (menu == '1')
		{
			st = new StVector<float>(50); //создаём векторный стек
		}
		else
		{
			st = new St_list<float>(); //создаём связный стек
		}

		f = addInStack(st); //работа со стеком
		delete st; //удаление стека
	}
	return 0;
}

int addInStack(interfaceSt<float>* st) //функция работы со стеком
{
	cin.ignore();
	string str, bufStr;
	float s = 0;
	int f = 0;

	cout << "Введите выражение в постфиксной форме: " << endl;
	getline(cin, str);

	for (int i = 0; i < str.length(); i++) //перебираем все символы строки
	{
		bufStr = "";
		if (isdigit(str[i])) //если элемент строки цифра
		{
			while ((str[i] == '0' || str[i] == '1' || str[i] == '2' || str[i] == '3' || str[i] == '4' || str[i] == '5' || str[i] == '6' || str[i] == '7' || str[i] == '8' || str[i] == '9' || str[i] == '.' || str[i] == ',') && i < str.length())
			{
				if (str[i] == '.' || str[i] == ',')
				{
					bufStr = bufStr + '.';
					i++;
					continue;
				}
				bufStr = bufStr + str[i];
				i++;
			}
			st->push(stof(bufStr)); //добавляем элемент в стек
		}
		else
		{
			if (str[i] != ' ')
			{
				switch (str[i])
				{
				case '+':
				{
					float a, b;
					if (!st->isEmpty()) a = st->pop(); //если стек не пуст, извлекаем элемент
					if (!st->isEmpty()) b = st->pop();
					else
					{
						cout << "Неверный ввод выражения" << endl << endl;
						return 1;
					}
					s = b + a;
					st->push(s); //добавление в стек
					break;
				}
				case '-':
				{
					float a, b;
					if (!st->isEmpty()) a = st->pop();
					if (!st->isEmpty()) b = st->pop();
					else
					{
						cout << "Неверный ввод выражения" << endl << endl;
						return 1;
					}
					s = b - a;
					st->push(s);
					break;
				}
				case '/':
				{
					float a, b;
					if (!st->isEmpty()) a = st->pop();
					if (!st->isEmpty()) b = st->pop();
					else
					{
						cout << "Неверный ввод выражения" << endl << endl;
						return 1;
					}
					if (a == 0)
					{
						system("CLS");
						cout << "Происходит деление на 0" << endl << endl;
						return 1;
					}
					s = b / a;
					st->push(s);
					break;
				}
				case '*':
				{
					float a, b;
					if (!st->isEmpty()) a = st->pop();
					if (!st->isEmpty()) b = st->pop();
					else
					{
						cout << "Неверный ввод выражения" << endl << endl;
						return 1;
					}
					s = a * b;
					st->push(s);
					break;
				}
				default:
					break;
				}

			}
		}
	}
	float result;
	if (!st->isEmpty())
	{
		result = st->pop();
		if (st->isEmpty()) //если в стеке больше ничего нет
		{
			cout << endl << "Значение выражения равно: " << result << endl;
			return 0;
		}
	}
	else
	{
		system("CLS");
		cout << "Неверный ввод выражения" << endl << endl;
		return 1;
	}
}