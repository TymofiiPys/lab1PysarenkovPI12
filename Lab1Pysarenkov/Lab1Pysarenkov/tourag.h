#pragma once

namespace tourag {
	struct User {
		int id;				//1 - 9999
		char _login[21];	
		char _password[21];
		char _sur[21];   //Прізвище
		char _name[21];	//Ім'я
		char _patr[21];  //По-батькові
		int purch;		//Кількість покупок на сайті для того щоб визначати, чи є клієнт постійним, чи ні
		bool isadmin;	//Адмін чи клієнт
		bool showflag;	//Замість видалення користувача зі списку просто не показуємо його при виводі інформації про користувачів
		User(int id, char const* login, char const* password, char const* sur, char const* name, char const* patr, int purch, bool isadmin, bool showflag);
	};
	struct Tour {
		int id;
		int type; //0 - відпочинок, 1 - екскурсія, 2 - шопінг
		char desttown[32]; //місто або міста
		char destcntry[32]; //країна
		char dob[11]; //дата початку ДД.ММ.РРРР
		char doe[11]; //дата кінця туру ДД.ММ.РРРР
		int price; //вартість
		Tour(int id, int type, char const* town, char const* cntry, char const* begin, char const* end, int price);
	};
	struct Trans {
		int id;
		char _sur[21];
		char _name[21];
		char _patr[21];
		int tid;
		char desttown[32];
		char destcntry[32];
		int price;
		Trans(int id, char const* sur, char const* name, char const* patr, int tourid, char const* town, char const* cntry, int price);
	};
	struct NodeList {
	private:
		struct Node {
			User* user;
			Node* next;
			Node(User* user);
		};
		Node* head = nullptr;
		Node* tail = nullptr;
	public:
		void signup(bool isadmin);
		bool logIn(char const *login, char const *password);
		bool checkAccess(char const* login);
		void getUserInfo();
		int discPercent; 
		void changeDiscPercent(int dsc);
		void printUsers();
		void deleteUser(int id/*char const* searchQuery*/);
		void printTours(int type, char const* login);
		void editTours();
		void checkout(int id, Trans* t);
		void printLastNTransactions(int N);
		char* name(char const* login);
	};
}