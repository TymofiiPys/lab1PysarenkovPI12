// Lab1Pysarenkov.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <Windows.h>
#include "tourag.h"

int main()
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);
	tourag::NodeList list;
	list.getUserInfo();
	std::cout << "Вітаємо в туристичному агентстві \"Їдь подалі!\"\n";
	std::string oper;
	for (;;) {
		std::cout << "Введіть \"вхід\" якщо хочете здійснити вхід у свій обліковий запис\n\
або \"реєстрація\" якщо у вас немає облікового запису і Ви хочете його створити\nДля виходу із програми - введіть \"вийти\"\n";
		std::cin >> oper;
		if (oper == "вхід")
		{
			std::string login, password;
			std::cout << "Введіть свій логін\n";
			std::cin >> login;
			std::cout << "Введіть пароль\n";
			std::cin >> password;
			if (list.logIn(login.c_str(), password.c_str())) {
				std::cout << "Вітаємо, " << list.name(login.c_str()) << "!\n";
				if (!list.checkAccess(login.c_str())) {
					for (;;) {
						std::cout << "Оберіть тип туру, який Вас цікавить:\n";
						std::cout << "Введіть 0, якщо відпочинок:\n";
						std::cout << "1 - екскурсія\n";
						std::cout << "2 - шопінг\n";
						std::cout << "3 - вийти з облікового запису\n";
						int type;
						std::cin >> type;
						if (type == 3) {
							std::cout << "Вихід з облікового запису.";
							break;
						}
						else {
							list.printTours(type, login.c_str());
						}
					}
				}
				else {
					for (;;) {
						std::cout << "Оберіть дію, яку ви хочете здійснити\n";
						std::cout << "Введіть 1, якщо бажаєте видалити користувача:\n";
						std::cout << "2 - продивитися останні N транзакцій\n";
						std::cout << "3 - визначити відсоток знижки для постійних клієнтів\n";
						std::cout << "4 - редагувати список турів\n";
						std::cout << "5 - переглянути список усіх користувачів\n";
						std::cout << "6 - вийти\n";
						int type;
						bool exit = false;
						std::cin >> type;
						switch (type) {
						case 1:
							int id;
							std::cout << "Введіть ID користувача, якого ви хочете видалити\n";
							std::cin >> id;
							list.deleteUser(id);
							break;
						case 2:
							int N;
							std::cout << "Введіть N\n";
							std::cin >> N;
							list.printLastNTransactions(N);
							break;
						case 3:
							int dsc;
							std::cout << "Поточний відсоток знижки - " << list.discPercent << "%\n";
							std::cout << "Введіть новий відсоток знижки для постійних клієнтів\n";
							std::cin >> dsc;
							list.changeDiscPercent(dsc);
							break;
						case 4:
							list.editTours();
							break;
						case 5: 
							list.printUsers();
							break;
						case 6:
							exit = true;
							break;
						default:
							std::cout << "Помилка.\n";
							break;
						}
						if (exit)
						{
							std::cout << "Вихід з облікового запису.\n";
							break;
						}
					}
				}

			}
			else {
				std::cout << "Помилка входу.\n";
			}
		}
		else if (oper == "реєстрація")
			list.signup(false);
		else if (oper == "вийти")
			return 0;
		else
			std::cout << "Помилка вводу.\n";
	}
}

