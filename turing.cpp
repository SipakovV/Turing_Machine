/*******************************/
/* File Name: turing.cpp       */
/* Abstraction: Turing machine */
/* Description: Course work    */
/* Date: 26.04.2020            */
/* Author: Viacheslav Sipakov  */
/*******************************/

#include <iostream>
#include <fstream>
#include <string.h>

#define TAPE_LEN 256

using std::cout;
using std::endl;
using std::cin;
using std::cerr;
using std::ifstream;

int last = 0, condition = 1;
bool detailed_mode = 0;

struct Command{ //структурный тип данных, реализующий команду машине
	int q; //состояние
	char a; //символ
	char move; //шаг
};

void tape_print(char * tape, int cursor, char zero_symbol){ //функция вывода ленты на консоль
	bool b = 0;
	for (int j = 0; j <= last; j++){
		if (b || tape[j] != zero_symbol){
			if (cursor == j)
				cout << "\e[1m" << tape[j] << "\e[0m";
			else cout << tape[j]; 
			b = 1;
		}
	}
    if (detailed_mode) cout << "  \tCondition: " << condition;
    cout << endl;
}

int main(int argc, char ** argv){ 
	int cursor = -1,m,n,command,steps,j = 0;
	short int symbol_num = -1;//cout << n << '-' << endl
	char tape_ini[TAPE_LEN],tape[TAPE_LEN],buf[3]; 

	char logo[] = "=============================================================\n"
                  "= ***     **** ****  | Nizhny Novgorod Technical University =\n"
                  "=  * *   * ** **  ** | Course Work                          =\n"
                  "=   * * * **   **    | 'Turing Machine'                     =\n"
                  "=    * * **     **   | Performed by student                 =\n"
                  "=     * **   **  **  | Viacheslav Sipakov                   =\n"
                  "=      **     ****   | Group number 19-IVT-3                =\n"
                  "=============================================================\n";
	
	cout << logo;

    if (argc < 2 || argc > 3 || argc == 3 && argv[2][0] != '-'){
        cerr << "Error: Invalid arguments\n";
        exit(1);
    }

	ifstream in;
	in.open(argv[1]);
    if (!in.is_open()){
        cerr << "Error: Could not open file\n";
        exit(1);
    }

    if (argc == 3 && argv[2][1] == 'd'){
        cout << "= Detailed mode =\n";
        detailed_mode = 1;
    }
	
	in >> tape_ini; //ввод начальной ленты из файла
	in >> m; //ввод к-ва состояний из файла
	in >> n; //ввод к-ва символов из файла
	
	Command * c = new Command[m*n]; //создание массива структурного типа Command
	char * symbol = new char[n]; //создание массива символов, используемых в данной машине
	
	for (int i = 0; i < m; i++) //
		in >> buf;
	
	for (int i = 0; i < n; i++){
		in >> symbol[i]; //заполнение массива символов
		for (j = 0; j < m; j++){ //заполнение массива команд
			int k = i*m+j;
			in >> c[k].q;
			in >> c[k].a;
			in >> c[k].move;
		}
	}
	
	in.close();
	
	for (int i = 0; i <= 261; i++)
		tape[i] = symbol[0]; //заполнение ленты нулевым символом
	
	j = 0;
	for (int i = 0; i < strlen(tape_ini); i++){
		if (tape_ini[i] == '!')
			cursor = i + 119;
		else {
			tape[120+j] = tape_ini[i];
			j++;
		}
		last = 119 + i; //вспомогательная переменная для обозначения последнего значащего символа
	}
	
	if (cursor < 0){ //проверка на наличие курсора
		cerr << "Error: no cursor!" << endl;
		exit(1);
	}

	tape_print(tape,cursor,symbol[0]);
	
	for (int i = 0; condition > 0; i++){ //основной цикл
		
		for (j = 0; j < n; j++){ //определение номера символа
			if (tape[cursor] == symbol[j]){
				symbol_num = j;
				break;
			}
		}
		
		command = m*symbol_num + condition - 1; //определение номера команды
		
		tape[cursor] = c[command].a; //получение нового символа
		
		condition = c[command].q; //получение нового состояния
		
		switch (c[command].move){
			case 'L': {cursor--; break;}
			case 'R': {cursor++; break;}
			case 'N': {break;}
		}
		
		if (cursor > last && tape[cursor] != 0) //изменение переменной last
			last = cursor;
		while (tape[last] == symbol[0] && cursor < last)
			last -= 1;
		
		steps = i; //счетчик шагов машины
		
		tape_print(tape,cursor,symbol[0]);

		if (i > 500){
			cerr << "Error: infinite cycle! Terminating the program...\n";
			break;
		}
    }
	
	cout << "Steps = " << steps + 1 << endl;
	
	return 0;
}