#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <Windows.h>
using namespace std;

int Proverka() //функция для проверки интового числа
{
	int n;
	for (;;)
	{
		if ((cin >> n).good()) return n;
		if (cin.fail())
		{
			cin.clear();
			cout << "!!!Неверный ввод!!!" << endl;
		}
		cin.ignore();
	}
};

//класс списка
class List
{
public:
	List();
	~List();

	//удаление первого элемента в списке
	void pop_front();

	//добавление элемента в конец списка
	void push_back(char ch, int countPassengers, int train_number, string station_of_destination, int time_of_departure, int travel_time, bool availability_of_tickets);

	//очистить список
	void clear();

	//получить количество елементов в списке
	int GetSize() { return Size; }

	//добавление элемента в начало списка
	void push_front(char ch, int countPassengers, int train_number, string station_of_destination, int time_of_departure, int travel_time, bool availability_of_tickets);

	//добавление элемента в список по указанному номеру
	void insert(char ch, int countPassengers, int train_number, string station_of_destination, int time_of_departure, int travel_time, bool availability_of_ticketsa);

	//удаление элемента в списке по указанному номеру
	void removeAt(int index);

	//удаление последнего элемента в списке
	void pop_back();

	//вывод расписания в прямом направлении
	int PrintList1();

	//вывод расписания в обратном направлении
	int PrintList2();

	//поиск поездов в определенный город в заданном временном интервале
	void Seek(string station, int timeLeft, int timeRight);

	//получить информацию о наличии билетов на поезд с определенным номером
	void GetInfo(int number);

	//запись списка в файл
	void OFStream(string path);

	//считываниие в список из файла
	void IFStream(string path);

private:

	//класс элемента списка
	class Node
	{
	public:
		Node* pNext; //указатель на следующий элемент списка
		Node* pPrev; //указатель на предыдущий элемент списка
		int train_number; //номер поезда
		string station_of_destination; //станция назначения
		int time_of_departure; //время отправления
		int travel_time; //время в пути
		bool availability_of_tickets;// 0 - нет билетов 1 - билеты есть
		union {
			char ch; // Билетов нет
			int countPassengers; // Кол-во пассажиров
		}my;


		//конструктор
		Node(char ch, int countPassengers, int train_number, string station_of_destination, int time_of_departure, int travel_time, bool availability_of_tickets, Node* pNext = nullptr, Node* pPrev = nullptr) //конструктор элемента списка
		{
			this->train_number = train_number;
			this->station_of_destination = station_of_destination;
			this->time_of_departure = time_of_departure;
			this->travel_time = travel_time;
			this->availability_of_tickets = availability_of_tickets;
			this->pNext = pNext;
			this->pPrev = pPrev;
			if (this->availability_of_tickets) { this->my.countPassengers = countPassengers; }
			else { this->my.ch = ch; }
		}
	};

	int Size; //размер списка
	Node* head; //указатель на начало списка
	Node* tail; //указатель на конец списка
};

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251); //подключение русского языка

	string path; //строка для хранения пути к файлу
	cout << "Путь к файлу: ";
	cin >> path;


	List Train; //создаём пустой список
	Train.IFStream(path); //заполняем список из файла

	while (true)
	{
		cout << "\tМеню:" <<
			endl << "1. Показать расписание" <<
			endl << "2. Добавить поезд" <<
			endl << "3. Удалить поезд" <<
			endl << "4. Поиск поездов в определенный город в заданном временном интервале" <<
			endl << "5. Получить информацию о наличии билетов на поезд с определенным номером" <<
			endl << "6. Выход" << endl;

		char menu;
		cin >> menu;
		cin.ignore(cin.rdbuf()->in_avail()); //ингнорирует все символы кроме первого

		switch (menu)
		{
		case '1':
		{
			system("CLS");
			if (Train.GetSize() == 0)
			{
				cout << "Расписание пока пустое" << endl;
				break;
			}
			while (true)
			{
				bool f = false;
				cout << "\tПоказать расписание:" <<
					endl << "1. В прямом направлении" <<
					endl << "2. В обратном направлении" <<
					endl << "3. Выход" << endl;

				char menu2;
				cin >> menu2;
				cin.ignore(cin.rdbuf()->in_avail());

				switch (menu2)
				{
				case '1':
					system("CLS");
					Train.PrintList1(); //вывод списка в прямом направлении
					break;

				case '2':
					system("CLS");
					Train.PrintList2(); //вывод списка в обратном направлении
					break;

				case '3':
					system("CLS");
					f = true;
					break;

				default:
					system("CLS");
					cout << "Выберете пункт меню введя число от 1 до 3" << endl;
					break;
				}
				if (f) break;
			}
			break;
		}

		case '2':
		{
			system("CLS");
			bool f = false;
			cout << "\tДобавление поезда:" << endl;

			int trainNumber, timeOfDeparture, travelTime, h, m;
			string stationOfDestination;
			bool availabilityOfTickets;
			char ch; int countPassengers;

			cout << "Введите параметры поезда:" << endl;
			cout << "Номер поезда: ";
			trainNumber = Proverka(); //все интовые переменные заполняем с учётом проверки
			cout << "Станция назначения: ";
			cin >> stationOfDestination;
			cout << "Время отправления: ";
			h = Proverka();
			cin.get(); //пропускаем символ чтобы вводить время через любой символ например 12-30
			m = Proverka();
			timeOfDeparture = h * 100 + m; //для хранения преобразовываем время
			cout << "Время в пути: ";
			h = Proverka();
			cin.get();
			m = Proverka();
			travelTime = h * 100 + m;
			cout << "Есть ли в наличии билеты? (0 - Нет 1 - Есть): ";
			cin >> availabilityOfTickets;
			if (availabilityOfTickets)
			{
				cout << "Количество билетов: ";
				countPassengers = Proverka();
				ch = '\0';
			}
			else
			{
				ch = '-';
				countPassengers = -1;
			}

			Train.insert(ch, countPassengers, trainNumber, stationOfDestination, timeOfDeparture, travelTime, availabilityOfTickets); //вставка поезда в список
			break;
		}

		case '3':
		{
			system("CLS");
			while (true)
			{
				bool f = false;
				if (Train.GetSize() == 0) //если список пуст то пункт меню не откроется
				{
					cout << "Расписание пустое, нельзя ничего удалить" << endl;
					break;
				}
				cout << "\tМеню:" <<
					endl << "1. Удалить первый поезд" <<
					endl << "2. Удалить последний поезд" <<
					endl << "3. Удалить поезд по номеру" <<
					endl << "4. Выход" << endl;

				char menu2;
				cin >> menu2;
				cin.ignore(cin.rdbuf()->in_avail());

				switch (menu2)
				{
				case '1':
					system("CLS");
					Train.pop_front(); //удаление первого поезда
					f = true;
					break;

				case '2':
					system("CLS");
					Train.pop_back(); //удаление последнего поезда
					f = true;
					break;

				case '3':
					system("CLS");
					int indeks;
					f = true;
					while (true)
					{
						cout << "С каким номером удалить поезд: ";
						indeks = Proverka();
					}
					Train.removeAt(indeks); //удаление поезда по номеру
					break;

				case '4':
					system("CLS");
					f = true;
					break;

				default:
					system("CLS");
					cout << "Выберете пункт меню введя число от 1 до 4" << endl;
					break;
				}
				if (f) break;
			}
			break;
		}

		case '4':
		{
			system("CLS");
			if (Train.GetSize() == 0) //если расписание пустое, пункт менб не откроется
			{
				cout << "Расписание пока пустое" << endl;
				break;
			}
			int h1, h2, m1, m2;
			string siti;
			cout << "Введите параметры для поиска поездов:" << endl;
			cout << "Станция назначения: ";
			cin >> siti;
			cout << "Временной интервал (без пробелов): ";
			h1 = Proverka();
			cin.get();
			m1 = Proverka();
			cin.get();
			h2 = Proverka();
			cin.get();
			m2 = Proverka();
			h1 = h1 * 100 + m1;
			h2 = h2 * 100 + m2;
			Train.Seek(siti, h1, h2); //поиск поезда
			break;
		}

		case '5':
		{
			system("CLS");
			if (Train.GetSize() == 0) //если расписание пустое, пкнкт меню не откроется
			{
				cout << "Расписание пока пустое" << endl;
				break;
			}
			cout << "Введите номер поезда: ";
			int number;
			number = Proverka();
			Train.GetInfo(number); //поиск
			break;
		}

		case '6':
		{
			system("CLS");
			Train.OFStream(path);  //при выходе записываем список в файл
			return 0;
		}

		default:
		{
			system("CLS");
			cout << "Выберете пункт меню введя число от 1 до 6" << endl;
			break;
		}
		}
	}
}

//конструктор списка
List::List()
{
	Size = 0; //размер списка
	head = nullptr; //указатель на начало списка
	tail = nullptr; //указатель на конец списка
}

//деструктор списка
List::~List()
{
	clear(); //очистка списка
}

//удаление первого элемента в списке
void List::pop_front()
{
	Node* temp = head;

	if (Size == 1)
	{
		head = nullptr;
		tail = nullptr;
	}
	else
	{
		head = head->pNext;
		head->pPrev = temp->pPrev;
	}
	delete temp;
	Size--;
}

//добавление элемента в конец списка
void List::push_back(char ch, int countPassengers, int train_number, string station_of_destination, int time_of_departure, int travel_time, bool availability_of_tickets)
{
	if (Size == 0)
	{
		tail = new Node(ch, countPassengers, train_number, station_of_destination, time_of_departure, travel_time, availability_of_tickets); //вызов конструктора элемента списка
		head = tail;
	}
	else
	{
		Node* current = this->tail;
		tail = new Node(ch, countPassengers, train_number, station_of_destination, time_of_departure, travel_time, availability_of_tickets);
		current->pNext = tail;
		tail->pPrev = current;
	}
	Size++;
}

//очистить список
void List::clear()
{
	while (Size)
	{
		Node* temp = head;

		head = head->pNext;

		delete temp;

		Size--;
	}
	tail = nullptr;
}

//добавление элемента в начало списка
void List::push_front(char ch, int countPassengers, int train_number, string station_of_destination, int time_of_departure, int travel_time, bool availability_of_tickets)
{
	if (Size == 0)
	{
		head = new Node(ch, countPassengers, train_number, station_of_destination, time_of_departure, travel_time, availability_of_tickets);
		tail = head;
	}
	else
	{
		Node* current = this->head;
		head = new Node(ch, countPassengers, train_number, station_of_destination, time_of_departure, travel_time, availability_of_tickets);
		current->pPrev = head;
		head->pNext = current;
	}
	Size++;
}

//добавление элемента в список по указанному номеру
void List::insert(char ch, int countPassengers, int train_number, string station_of_destination, int time_of_departure, int travel_time, bool availability_of_tickets)
{
	Node* next = this->head;
	Node* temp2 = this->tail;
	if ((Size == 0) || (train_number <= next->train_number))  //сравниваем введённый номер с новером первого элемента
	{
		push_front(ch, countPassengers, train_number, station_of_destination, time_of_departure, travel_time, availability_of_tickets);
	}
	else
	{
		if (train_number >= temp2->train_number) //сравниваем введённый номер с новером последнего элемента
		{
			push_back(ch, countPassengers, train_number, station_of_destination, time_of_departure, travel_time, availability_of_tickets);
		}
		else
		{
			while (train_number > next->train_number)
			{
				next = next->pNext; //переход на следующий элемент списка
			}
			Node* previous = next->pPrev;
			Node* newNode = new Node(ch, countPassengers, train_number, station_of_destination, time_of_departure, travel_time, availability_of_tickets, next, previous);
			previous->pNext = newNode;
			next->pPrev = newNode;
			Size++;
		}
	}
}

//удаление элемента в списке по указанному номеру
void List::removeAt(int index)
{
	Node* temp = this->head;
	Node* temp2 = this->tail;

	if (index == temp->train_number)
	{
		pop_front();
	}
	else
	{
		if (index == temp2->train_number)
		{
			pop_back();
		}
		else
		{
			Node* toDelete = this->head; //указатель на элемент для удаления
			while ((toDelete->train_number != index) && (toDelete->pNext != nullptr)) //перебираем все элементы и вравниваем с введённым номером
			{
				toDelete = toDelete->pNext; //переход на следующий элемент списка
			}
			if (toDelete->train_number == index) //если нужный элемент был найден
			{
				Node* previous = toDelete->pPrev;
				Node* next = toDelete->pNext;

				previous->pNext = next;
				next->pPrev = previous;

				delete toDelete;
				Size--;
			}
			else cout << "Нет такого номера" << endl;
		}
	}
}

//удаление последнего элемента в списке
void List::pop_back()
{
	Node* temp = tail;

	if (Size == 1)
	{
		head = nullptr;
		tail = nullptr;
	}
	else
	{
		tail = temp->pPrev;
		tail->pNext = nullptr;
	}
	delete temp;
	Size--;
}

//вывод расписания в прямом направлении
int List::PrintList1()
{
	Node* temp = head;
	int page = 1;
	int last = ((Size - 1) / 5) + 1; //количество страниц
	while (true)
	{
		cout << "\tРасписание:" << endl
			<< "--------------------------------------------------------------------------------------------------------------------" << endl
			<< "|  №  | Номер поезда | Станция назначения | Время отправления | Время в пути | Наличие билетов | Кол-во пассажиров |" << endl
			<< "--------------------------------------------------------------------------------------------------------------------" << endl;
		int count = 0;
		for (int i = ((page - 1) * 5) + 1; i < (((page - 1) * 5) + 1) + 5; i++) //считаем порядковые номера в зависимости от номера страницы
		{
			string str;
			if (temp->availability_of_tickets == 1) str = "Есть";
			if (temp->availability_of_tickets == 0) str = "Нет";
			int timeH1 = temp->time_of_departure / 100;  //приедение времени к красивому виду
			int timeM1 = temp->time_of_departure % 100;
			int timeH2 = temp->travel_time / 100;
			int timeM2 = temp->travel_time % 100;
			if (temp->availability_of_tickets == 1)
				cout << "| " << left << setw(3) << i << " | " << setw(12) << temp->train_number << " | " << setw(18) << temp->station_of_destination << " | " << right << setw(2) << timeH1 << ":" << left << setw(14) << timeM1 << " | " << right << setw(2) << timeH2 << ":" << left << setw(9) << timeM2 << " | " << setw(15) << str << " | " << setw(17) << temp->my.countPassengers << " |" << endl
				<< "--------------------------------------------------------------------------------------------------------------------" << endl;
			else
				cout << "| " << left << setw(3) << i << " | " << setw(12) << temp->train_number << " | " << setw(18) << temp->station_of_destination << " | " << right << setw(2) << timeH1 << ":" << left << setw(14) << timeM1 << " | " << right << setw(2) << timeH2 << ":" << left << setw(9) << timeM2 << " | " << setw(15) << str << " | " << setw(17) << temp->my.ch << " |" << endl
				<< "--------------------------------------------------------------------------------------------------------------------" << endl;
			if (temp->pNext == nullptr) break; //если элементы кончились то прерываем цикл
			temp = temp->pNext; //переход на следующий элемент списка
			count++;
		}
		char menu;

		cout << page << "/" << last;
		if (page == last && last != 1) //если мы на последней но не единственной стрранице
		{
			cout << "\t\t2-назад" << endl << "3. Выход" << endl;

			bool s = true;
			while (s == true)
			{
				cin >> menu;
				cin.ignore(cin.rdbuf()->in_avail());

				switch (menu)
				{
				case '2': //листаем назад
				{
					for (int i = 0; i < (5 + count); i++)
					{
						temp = temp->pPrev; //переход на предыдущий элемент списка
					}
					page--;
					s = false;
					break;
				}
				case '3':
				{
					return 0;
				}
				default:
				{
					break;
				}
				}
			}
			system("CLS");
		}
		else if (page == 1 && page != last) //если мы на первой но не единственной странице
		{
			cout << "\t1-вперёд" << endl << "3. Выход" << endl;

			bool s = true;
			while (s == true)
			{
				cin >> menu;
				cin.ignore(cin.rdbuf()->in_avail());

				switch (menu)
				{
				case '1': //листание вперёд
				{
					page++;
					s = false;
					break;
				}
				case '3':
				{
					return 0;
				}
				default:
				{
					break;
				}
				}
			}
			system("CLS");
		}
		else if (page != 1 && page != last) //если мы не на первой и не на последней странице
		{
			cout << "\t1-вперёд\t2-назад" << endl << "3. Выход" << endl;

			bool s = true;
			while (s == true)
			{
				cin >> menu;
				cin.ignore(cin.rdbuf()->in_avail());

				switch (menu)
				{
				case '1': //листание вперёд
				{
					page++;
					s = false;
					break;
				}
				case '2': //листание назад
				{
					for (int i = 0; i < (5 + count); i++)
					{
						temp = temp->pPrev; //переход на предыдущий элемент списка
					}
					page--;
					s = false;
					break;
				}
				case '3':
				{
					return 0;
				}
				default:
				{
					break;
				}
				}
			}
			system("CLS");
		}
		else if (page == 1 && page == last) //если страница единственная
		{
			cout << endl << "3. Выход" << endl;

			while (true)
			{
				cin >> menu;
				cin.ignore(cin.rdbuf()->in_avail());

				switch (menu)
				{
				case '3':
				{
					return 0;
				}
				default:
				{
					break;
				}
				}
			}
			system("CLS");
		}
	}
}

//вывод расписания в обратном направлении
int List::PrintList2()
{
	Node* temp = tail;
	int page = 1;
	int last = ((Size - 1) / 5) + 1; //количество страниц
	while (true)
	{
		cout << "\tРасписание:" << endl
			<< "--------------------------------------------------------------------------------------------------------------------" << endl
			<< "|  №  | Номер поезда | Станция назначения | Время отправления | Время в пути | Наличие билетов | Кол-во пассажиров |" << endl
			<< "--------------------------------------------------------------------------------------------------------------------" << endl;
		int count = 0;
		for (int i = ((page - 1) * 5) + 1; i < ((page - 1) * 5) + 6; i++)
		{
			string str;
			if (temp->availability_of_tickets == 0) str = "Нет";
			if (temp->availability_of_tickets == 1) str = "Есть";
			int timeH1 = temp->time_of_departure / 100; //приведение времени к стандартному виду
			int timeM1 = temp->time_of_departure % 100;
			int timeH2 = temp->travel_time / 100;
			int timeM2 = temp->travel_time % 100;
			if (temp->availability_of_tickets == 1)
				cout << "| " << left << setw(3) << i << " | " << setw(12) << temp->train_number << " | " << setw(18) << temp->station_of_destination << " | " << right << setw(2) << timeH1 << ":" << left << setw(14) << timeM1 << " | " << right << setw(2) << timeH2 << ":" << left << setw(9) << timeM2 << " | " << setw(15) << str << " | " << setw(17) << temp->my.countPassengers << endl
				<< "--------------------------------------------------------------------------------------------------------------------" << endl;
			else
				cout << "| " << left << setw(3) << i << " | " << setw(12) << temp->train_number << " | " << setw(18) << temp->station_of_destination << " | " << right << setw(2) << timeH1 << ":" << left << setw(14) << timeM1 << " | " << right << setw(2) << timeH2 << ":" << left << setw(9) << timeM2 << " | " << setw(15) << str << " | " << setw(17) << temp->my.ch << endl
				<< "--------------------------------------------------------------------------------------------------------------------" << endl;
			if (temp->pPrev == nullptr) break; //если поезда закончились
			temp = temp->pPrev; //переход на предыдущий элемент списка
			count++;
		}
		char menu;
		cin.ignore(cin.rdbuf()->in_avail());

		cout << page << "/" << last;
		if (page == last && last != 1) //если мы на последнеё но не единственной странице
		{
			cout << "\t\t2-назад" << endl << "3. Выход" << endl;

			bool s = true;
			while (s == true)
			{
				cin >> menu;
				cin.ignore(cin.rdbuf()->in_avail());

				switch (menu)
				{
				case '2': //листание назад
				{
					for (int i = 0; i < (5 + count); i++)
					{
						temp = temp->pNext; //переход на следующий элемент списка
					}
					page--;
					s = false;
					break;
				}
				case '3':
				{
					return 0;
				}
				default:
				{
					break;
				}
				}
			}
			system("CLS");
		}
		else if (page == 1 && page != last) //если мы на первое но не единственной странице
		{
			cout << "\t1-вперёд" << endl << "3. Выход" << endl;

			bool s = true;
			while (s == true)
			{
				cin >> menu;
				cin.ignore(cin.rdbuf()->in_avail());

				switch (menu)
				{
				case '1': //листание вперёд
				{
					page++;
					s = false;
					break;
				}
				case '3':
				{
					return 0;
				}
				default:
				{
					break;
				}
				}
			}
			system("CLS");
		}
		else if (page != 1 && page != last) //если мы не на первой и не на последней странице
		{
			cout << "\t1-вперёд\t2-назад" << endl << "3. Выход" << endl;

			bool s = true;
			while (s == true)
			{
				cin >> menu;
				cin.ignore(cin.rdbuf()->in_avail());

				switch (menu)
				{
				case '1': //листание вперёд
				{
					page++;
					s = false;
					break;
				}
				case '2': //листание назад
				{
					for (int i = 0; i < (5 + count); i++)
					{
						temp = temp->pNext; //переход на следующий элемент списка
					}
					page--;
					s = false;
					break;
				}
				case '3':
				{
					return 0;
				}
				default:
				{
					break;
				}
				}
			}
			system("CLS");
		}
		else if (page == 1 && page == last) //если мы на первой и единственной странице
		{
			cout << endl << "3. Выход" << endl;

			while (true)
			{
				cin >> menu;
				cin.ignore(cin.rdbuf()->in_avail());

				switch (menu)
				{
				case '3':
				{
					return 0;
				}
				default:
				{
					break;
				}
				}
			}
			system("CLS");
		}
	}
}

//поиск поездов в определенный город в заданном временном интервале
void List::Seek(string station, int timeLeft, int timeRight)
{
	Node* temp = head;
	cout << "Поезда идущие на станцию \"" << station << "\" c " << timeLeft / 100 << ":" << timeLeft % 100 << " по " << timeRight / 100 << ":" << timeRight % 100 << endl
		<< "------------------------------------" << endl
		<< "| Номер поезда | Время отправления |" << endl
		<< "------------------------------------" << endl;
	while (!(temp == nullptr)) //цикл до конца списка
	{
		if (temp->station_of_destination == station && temp->time_of_departure >= timeLeft && temp->time_of_departure <= timeRight) //если поезд в нужном интервале
		{
			int timeH1 = temp->time_of_departure / 100;
			int timeM1 = temp->time_of_departure % 100;
			cout << "| " << left << setw(12) << temp->train_number << " | " << right << setw(2) << timeH1 << ":" << left << setw(14) << timeM1 << " |" << endl
				<< "------------------------------------" << endl;
		}
		temp = temp->pNext; //переход на следующий элемент списка
	}
}

//получить информацию о наличии билетов на поезд с определенным номером
void List::GetInfo(int number)
{
	Node* temp = head;
	cout << "----------------------------------" << endl
		<< "| Номер поезда | Наличие билетов |" << endl
		<< "----------------------------------" << endl;
	while (!(temp == nullptr)) //цикл до конца списка
	{
		if (temp->train_number == number) //сравниваем номера
		{
			string str;
			if (temp->availability_of_tickets == 1) str = "Есть";
			if (temp->availability_of_tickets == 0) str = "Нет";
			cout << "| " << left << setw(12) << temp->train_number << " | " << setw(15) << str << " |" << endl
				<< "----------------------------------" << endl;
		}
		temp = temp->pNext; //переход на следующий элемент списка
	}
}

//запись списка в файл
void List::OFStream(string path) //сохранение в файл
{
	ofstream file; //класс для сохранения текста в файл
	file.open(path, ios::binary); //открытие файла

	if (file.is_open()) //если получилось открыть файл
	{
		if (Size != 0) //если список не пуст
		{
			Node* temp = head;
			while (temp->pNext != nullptr) //цикл без последнего элемента
			{
				file << temp->station_of_destination << " "; //запись в файл переменных через пробел
				file << temp->time_of_departure << " ";
				file << temp->travel_time << " ";
				file << temp->availability_of_tickets << " ";
				if (temp->availability_of_tickets == 0)
					file << temp->my.ch << " ";
				else
					file << temp->my.countPassengers << " ";
				file << temp->train_number << " ";

				temp = temp->pNext;
			}
			file << temp->station_of_destination << " ";
			file << temp->time_of_departure << " ";
			file << temp->travel_time << " ";
			file << temp->availability_of_tickets << " ";
			if (temp->availability_of_tickets == 0)
				file << temp->my.ch << " ";
			else
				file << temp->my.countPassengers << " ";
			file << temp->train_number; //в последнем элементе заносим без последнего пробела

		}
		file.close(); //закрытие файла
	}
	else
	{
		cout << "Ошибка открытия файла!" << endl;
	}
}

//считываниие в список из файла
void List::IFStream(string path)
{
	ifstream file; //класс для считывание из файла
	file.open(path); //открытие файла (путь path)
	if (file.is_open()) //если получилось открыть файл
	{
		while (!file.eof()) //пока не настанет конец файла
		{
			int train_number; //номер поезда
			string station_of_destination; //станция назначения
			int time_of_departure; //время отправления
			int travel_time; //время в пути
			bool availability_of_tickets; //наличие билетов
			char ch; //отсутствие билетов
			int countPassengers; //количество билетов
			file >> station_of_destination; //считывание из файла до пробела
			file >> time_of_departure;
			file >> travel_time;
			file >> availability_of_tickets;
			if (availability_of_tickets == 0)
			{
				file >> ch;
				countPassengers = 0;
			}
			else
			{
				file >> countPassengers;
				ch = '\0';
			}
			file >> train_number;

			this->push_back(ch, countPassengers, train_number, station_of_destination, time_of_departure, travel_time, availability_of_tickets); //добавляем поезд в конец списка
		}
		file.close(); //закрытие файла
	}
	else
	{
		cout << "Ошибка открытия файла!" << endl;
	}
}