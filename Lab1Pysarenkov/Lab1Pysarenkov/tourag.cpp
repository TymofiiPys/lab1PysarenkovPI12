#include <iostream>
#include <string>
#include "tourag.h"
#include <conio.h>

tourag::User::User(int id, char const* login, char const* password, char const* sur, char const* name, char const* patr, int purch, bool isadmin, bool showflag) {
	this->id = id;
	strcpy_s(_login, login);
	strcpy_s(_password, password);
	strcpy_s(_sur, sur);
	strcpy_s(_name, name);
	strcpy_s(_patr, patr);
	this->purch = purch;
	this->isadmin = isadmin;
	this->showflag = showflag;
}

tourag::Tour::Tour(int id, int type, char const* town, char const* cntry, char const* begin, char const* end, int price){
	this->id = id;
	this->type = type;
	strcpy_s(desttown, town);
	strcpy_s(destcntry, cntry);
	strcpy_s(dob, begin);
	strcpy_s(doe, end);
	this->price = price;
}

tourag::Trans::Trans(int id, char const* sur, char const* name, char const* patr, int tourid, char const* town, char const* cntry, int price){
	this->id = id;
	this->price = price;
	this->tid = tourid;
	strcpy_s(desttown, town);
	strcpy_s(destcntry, cntry);
	strcpy_s(_sur, sur);
	strcpy_s(_name, name);
	strcpy_s(_patr, patr);
};

struct Userbuff {
	int id;
	char _login[21];
	char _password[21];
	char _sur[21];
	char _name[21];
	char _patr[21];
	int purch;
	bool isadmin;
	bool showflag;
};

struct Tourbuff {
	int id;
	int type;
	char desttown[32];
	char destcntry[32];
	char dob[11];
	char doe[11];
	int price;
};

struct Transbuff {
	int id;
	char _sur[21];
	char _name[21];
	char _patr[21];
	int tid;
	char desttown[32];
	char destcntry[32];
	int price;
};

void tourag::NodeList::getUserInfo() {
	//Ця процедура читатиме інформацію про користувачів з .dat файлів та записуватиме їх у список
	if (FILE* fp = fopen("user.dat", "r+b"))
	{
		fseek(fp, 0, SEEK_SET);
		Userbuff ub;
		while (fread(&ub, sizeof(User), 1, fp))
		{
			User* u = new User(ub.id, ub._login, ub._password, ub._sur, ub._name, ub._patr, ub.purch, ub.isadmin, ub.showflag);
			Node* node = new Node(u);
			if (head == nullptr) { head = node; tail = node; }
			else {
				tail->next = node;
				tail = node;
			}
		}
		fclose(fp);
	}
	else {
		//Вітаю, Ви адмін!
		std::cout << "Даних про користувачів не виявлено. Реєстрація облікового запису адміністратора.\n";
		signup(true);
	}
	if (FILE* fp = fopen("trans.dat", "r+b")) //Зчитуємо відсоток знижки для постійних клієнтів з файлу trans.dat (структура файлу описана в коментах до printLastNTransactions).
		//Якщо файлу не існує, то файл створюється, і туди записується значення за замовчанням - 0%.
	{
		fseek(fp, 0, SEEK_SET);
		fread(&discPercent, sizeof(int), 1, fp);
		fclose(fp);
	}
	else {
		FILE* fc = fopen("trans.dat", "w+b");
		discPercent = 0;
		fseek(fc, 0, SEEK_SET);
		fwrite(&discPercent, sizeof(int), 1, fc);
		fclose(fc);
	}
}

void tourag::NodeList::signup(bool isadmin) {
	int id;
	std::cout << "Реєстрація облікового запису клієнта\n";
	//char const* login, char const* password, char const* sur, char const* name, char const* patr, bool isadmin)
	std::string login, password, sur, name, patr;
	std::cout << "Введіть логін:\n";
	std::cin >> login;
	bool exist = false;
	Node* iter = head;
	while (iter != nullptr) {
		if (iter->user->_login == login && iter->user->showflag) {
			exist = true;
			break;
		}
		iter = iter->next;
	}
	if (exist) {
		std::cout << "Обліковий запис із цим логіном вже існує. Спробуйте зареєструватись під іншим.\n";
	}
	else {
		std::cout << "Пароль:\n";
		std::cin >> password;
		std::cout << "Прізвище:\n";
		std::cin >> sur;
		std::cout << "Ім'я:\n";
		std::cin >> name;
		std::cout << "По-батькові:\n";
		std::cin >> patr;
		if (FILE* fp = fopen("user.dat", "r+b")) {
			int i = 1;
			Userbuff ub;
			while (fread(&ub, sizeof(User), 1, fp)) {
				i++;
			}
			id = i;
			fclose(fp);
		}
		else { //якщо user.dat не існує, логічно припустити, що на момент реєстрації зареєстровано 0 користувачів
			id = 1;
		}
		User* u = new User(id, login.c_str(), password.c_str(), sur.c_str(), name.c_str(), patr.c_str(), 0, isadmin, true);
		Node* node = new Node(u);
		if (head == nullptr) { head = node; tail = node; tail->next = nullptr; }
		else {
			tail->next = node;
			tail = node;
			tail->next = nullptr;
		}
		if (FILE* fw = fopen("user.dat", "r+b")) {
			fseek(fw, 0, SEEK_END);
			fwrite(u, sizeof(User), 1, fw);
			fclose(fw);
		}
		else {
			FILE* fc = fopen("user.dat", "w+b");
			fwrite(u, sizeof(User), 1, fc);
			fclose(fc);
		}
		std::cout << "Обліковий запис створено успішно. Увійдіть у нього, щоб продовжити роботу.\n";
	}
}

bool tourag::NodeList::logIn(char const *login, char const *password) {
	Node* iter = head;
	while (iter != nullptr) {
		if (std::strcmp(iter->user->_login, login) == 0 && iter->user->showflag) {
			if (std::strcmp(iter->user->_password, password) == 0) {
				return true;
			}
			else {
				return false;
			}
		}
		iter = iter->next;
	}
	return false;
}

bool tourag::NodeList::checkAccess(char const *login) {
	Node* iter = head;
	while (iter != nullptr) {
		if (strcmp(iter->user->_login, login) == 0)
		{
			if (iter->user->isadmin) {
				return true;
			}
			else {
				return false;
			}
		}
		iter = iter->next;
	}
	return false;
}

void tourag::NodeList::changeDiscPercent(int dsc) {
	discPercent = dsc;
	FILE* fp = fopen("trans.dat", "r+b");
	FILE* ft = fopen("temp.dat", "wb");
	fseek(fp, sizeof(int), SEEK_SET);
	fseek(ft, 0, SEEK_SET);
	fwrite(&discPercent, sizeof(int), 1, ft);
	/*TransBuff tb;
	while(fread())*/
	int usize = sizeof(User);
	fseek(fp, 0-usize, SEEK_CUR);
	std::cout << "Успішно. Новий відсоток знижки для постійних покупців - " << discPercent << "%.\n";
}

void tourag::NodeList::printUsers() {
	FILE* fp = fopen("user.dat", "rb");
	std::cout << "ID/Логін/Пароль/Прізвище/Ім'я/По-батькові/Кількість покупок/Роль\n";
	Userbuff ub;
	while (fread(&ub, sizeof(User), 1, fp)) {
		if (ub.isadmin)
			std::cout << ub.id << '/' << ub._login << '/' << ub._password << '/' << ub._sur << '/' << ub._name << '/' << ub._patr << '/' << ub.purch << '/' << "Адміністратор" << "\n";
		else
			std::cout << ub.id << '/' << ub._login << '/' << ub._password << '/' << ub._sur << '/' << ub._name << '/' << ub._patr << '/' << ub.purch << '/' << "Клієнт" << "\n";
	}
}

void tourag::NodeList::deleteUser(int id/*char const* searchQuery*/) {
	//Пошук по айді
	FILE* fp = fopen("user.dat", "r+b");
	FILE* ft = fopen("temp.dat", "wb");
	Userbuff ub;
	Node* iter = head;
	while (fread(&ub, sizeof(User), 1, fp))
	{
		if (ub.id == id) {
			std::cout << "Ви впевнені, що хочете видалити користувача " << iter->user->_sur << " " << iter->user->_name << " " << iter->user->_patr << "?\n";
			std::cout << "Введіть 0, якщо ні, 1 - так\n";
			char c;
			std::cin >> c;
			switch (c) {
			case '1':
				iter->user->showflag = false;
				std::cout << "Користувач видалений з реєстру\n";
				break;
			case '0':
				fwrite(&ub, sizeof(User), 1, ft);
				std::cout << "Користувач не був видалений з реєстру\n";
				break;
			default:
				fwrite(&ub, sizeof(User), 1, ft);
				std::cout << "Помилка вводу. Користувач не був видалений з реєстру\n";
				break;
			}
		}
		else {
			fwrite(&ub, sizeof(User), 1, ft);
		}
		iter = iter->next;
	}
	fclose(fp);
	fclose(ft);
	remove("user.dat");
	rename("temp.dat", "user.dat");

	/* Пошук по імені
	std::string sur, name, patr;
	int c = 0;
	std::string d[3];
	for (int i = 0; i < std::strlen(searchQuery); i++) {
		switch (searchQuery[i]) {
		case ' ':
			c++;
			break;
		default:
			switch (c) {
			case 0:
				sur += searchQuery[i];
				break;
			case 1:
				name += searchQuery[i];
				break;
			case 2:
				patr += searchQuery[i];
				break;
			}
			break;
		}
	}
	Node* iter = head;
	while (iter != nullptr) {
		if (iter->user->_sur == sur) {
			if (iter->user->_name == name) {
				if (iter->user->_patr == patr) {

				}
			}
		}
		iter = iter->next;
	}*/
}

void tourag::NodeList::printTours(int type, char const* login) {
	bool fail = false;
	FILE* fp = fopen("tours.dat", "rb");
	Tourbuff tb;
	std::string tourtype;
	std::cout << "ID\tТип туру\tМісто(-а)\tКраїна(-и)\tДата початку туру\tДата завершення туру\tВартість, грн.\n";
	while (fread(&tb, sizeof(Tour), 1, fp) && !fail) {
		switch (type) {
		case 0:
			tourtype = "Відпочинок";
			if (tb.type == 0)
				std::cout << tb.id << "\t" << tourtype << "\t" << tb.desttown << "\t" << tb.destcntry << "\t" << tb.dob << "\t" << tb.doe << "\t" << tb.price << "\n";
			break;
		case 1:
			tourtype = "Екскурсія";
			if (tb.type == 1)
				std::cout << tb.id << "\t" << tourtype << "\t" << tb.desttown << "\t" << tb.destcntry << "\t" << tb.dob << "\t" << tb.doe << "\t" << tb.price << "\n";
			break;
		case 2:
			tourtype = "Шопінг";
			if (tb.type == 2)
				std::cout << tb.id << "\t" << tourtype << "\t" << tb.desttown << "\t" << tb.destcntry << "\t" << tb.dob << "\t" << tb.doe << "\t" << tb.price << "\n";
			break;
		default:
			fail = true;
			break;
		}
	}
	if (!fail) {
		Node* iter = head;
		bool cust = false;
		std::string s, n, p;
		while (iter != nullptr) {
			if (strcmp(iter->user->_login, login) == 0)
			{
				s = iter->user->_sur;
				n = iter->user->_name;
				p = iter->user->_patr;
				if (iter->user->purch >= 3) {
					std::cout << "Вам доступна знижка у розмірі " << discPercent << "% як постійному клієнту нашого турагентства!\n";
					cust = true;
					break;
				}
				else {
					break;
				}
			}
			iter = iter->next;
		}
		std::cout << "Введіть ID туру, який Ви хотіли б придбати.\n";
		int tourid;
		std::cin >> tourid;
		fseek(fp, 0, SEEK_SET);
		while (fread(&tb, sizeof(Tour), 1, fp) && !fail)
			if (tb.id == tourid)
			{
				std::cout << "Ви впевнені, що хочете оплатити тур:\n";
				int price;
				if (cust)
					price = (int)((1 - 0.01 * discPercent) * tb.price);
				else
					price = tb.price;
				std::cout << tb.desttown << "," << tb.destcntry << ", дата початку: " << tb.dob << ", дата завершення: " << tb.doe << ", вартість: " << price << " грн.?\n";
				std::string town, cntry;
				town = tb.desttown;
				cntry = tb.destcntry;
				std::cout << "Введіть 0, якщо ні, 1 - так\n";
				char c;
				std::cin >> c;
				switch (c) {
				case '1':
				{Trans* t = new Trans(1, s.c_str(), n.c_str(), p.c_str(), tourid, town.c_str(), cntry.c_str(), price);
				checkout(tourid, t);
				break; }
				case '0':
					std::cout << "Користувач не був видалений з реєстру\n";
					break;
				default:
					std::cout << "Помилка вводу. Повернення до головного меню\n";
					break;
				}
			}
	}
	else {
		std::cout << "Помилка. Повернення до головного меню.\n";
		fclose(fp);
	}
}

void tourag::NodeList::editTours() {
	std::cout << "0 - Ввести тур до списку\n";
	std::cout << "1 - Видалити тур зі списку\n";
	char c;
	std::cin >> c;
	switch (c) {
	case '0':
		Tourbuff tb;
		std::cout << "Введіть інформацію про тур:\n";
		std::cout << "Тип туру (0 - відпочинок, 1 - екскурсія, 2 - шопінг): \n";
		std::cin >> tb.type;
		std::cout << "Країна: \n";
		std::cin >> tb.destcntry;
		std::cout << "Місто: \n";
		std::cin >> tb.desttown;
		std::cout << "Дата початку туру: \n";
		std::cin >> tb.dob;
		std::cout << "Дата кінця туру: \n";
		std::cin >> tb.doe;
		std::cout << "Вартість туру, грн.: \n";
		std::cin >> tb.price;
		if (FILE* fp = fopen("tours.dat", "r+b")) {
			fseek(fp, 0, SEEK_SET);
			Tourbuff tb1;
			int id = 0;
			while (fread(&tb1, sizeof(Tour), 1, fp))
			{
				id++;
			}
			fseek(fp, 0, SEEK_END);
			tb.id = id;
			fwrite(&tb, sizeof(Tour), 1, fp);
			fclose(fp);
		}
		else {
			FILE* fc = fopen("tours.dat", "wb");
			tb.id = 1;
			fwrite(&tb, sizeof(Tour), 1, fc);
			fclose(fc);
		}
		break;
	case '1':
		std::cout << "Введіть id туру: \n";
		int id;
		std::cin >> id;
		if (FILE* fp = fopen("tours.dat", "r+b"))
		{
			Tourbuff tb2;
			FILE* ft = fopen("temp.dat", "w+b");
			bool found = false;
			while (fread(&tb2, sizeof(Tour), 1, fp)) {
				if (tb2.id == id)
				{
					found = true;
				}
				else {
					fwrite(&tb2, sizeof(Tour), 1, ft);
				}
				fclose(ft);
				fclose(fp);
			}
			if (found) { 
				std::cout << "Тур видалено.\n";
				remove("tours.dat");
				rename("temp.dat", "tours.dat");
			}
			else { std::cout << "Тур не знайдено. Список залишився незмінним.\n"; }
		}
		else {
			std::cout << "Список пустий.\n";
		}
	}
}

void tourag::NodeList::checkout(int id, Trans* t) {
	std::cout << "Оплата\n";
	std::string s;
	std::cout << "Введіть номер Вашої банківської картки\n";
	std::cin >> s;
	std::cout << "Введіть термін придатності\n";
	std::cin >> s;
	std::cout << "Введіть CVV (3 цифри ззаду)\n";
	std::cin >> s;
	std::cout << "Вітаємо! Ваш тур оплачений! Дякуємо, що користуєтесь послугами нашого турагентства\n";
	FILE* fp = fopen("trans.dat", "wb");
	fwrite(t, sizeof(Trans), 1, fp);
	fclose(fp);
}

char* tourag::NodeList::name(char const* login) {
	Node* iter = head;
	while (iter != nullptr) {
		if (std::strcmp(iter->user->_login, login) == 0) {
			return iter->user->_name;
		}
		iter = iter->next;
	}
	char na[4] = { 'Н','/','Д', '\0' };
	return na;
}

void tourag::NodeList::printLastNTransactions(int N) {
	//trans.dat:
	//[Відсоток знижки для постійних клієнтів]
	//[інформація про транзакцію 1]
	//[інформація про транзакцію 2]
	//...
	FILE* fp = fopen("trans.dat", "r+b");
	fseek(fp, sizeof(int), SEEK_SET); //Пропускаємо зчитування відсотку знижки, яке було зроблене в getUserInfo
	Transbuff tb;
	int rows = 0;
	while (fread(&tb, sizeof(Trans), 1, fp)) {
		rows++;
	}
	fseek(fp, -N*sizeof(Trans), SEEK_END);
	std::cout << "ID/Прізвище/Ім'я/По-батькові/IDтуру/Місто/Країна/Вартість\n";
	while (fread(&tb, sizeof(Trans), 1, fp)) {
		std::cout << tb.id << '/' << tb._sur << '/' << tb._name << '/' << tb._patr << '/' << tb.tid << '/' << tb.desttown << '/' << tb.destcntry << '/' << tb.price << "\n";
	}
	fclose(fp);
}

tourag::NodeList::Node::Node(User* user)
{
	this->user = user;
	this->next = nullptr;
}