/*******************************/
/* File Name: turing.cpp      */
/* Abstraction: Turing machine */
/* Description:         */
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

int last = 0;

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
	cout << endl;
}

int main(int argc, char ** argv){ 
	int cursor = -1,condition = 1,m,n,command,steps,j = 0;
	short int symbol_num = -1;
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

    /*if (argc == 3 && argv[2][1] == 'd'){
        cout << "= Detailed mode =\n";
        detailed_mode = 1;
    }
	*/
	in >> tape_ini; //ввод начальной ленты из файла
	in >> m; //ввод к-ва состояний из файла
	in >> n; //ввод к-ва символов из файла
	
	//cout << n << '-' << endl;
	
	Command * c = new Command[m*n]; //создание массива структурного типа Command
	char * symbol = new char[n]; //создание массива символов, используемых в данной машине
	
	for (int i = 0; i < m; i++){ //
		in >> buf;
		cout << buf << " \n";
	}
	
	for (int i = 0; i < n; i++){
		in >> symbol[i]; //заполнение массива символов
		for (j = 0; j < m; j++){ //заполнение массива команд
			int k = i*m+j;
			in >> c[k].q;
			cout << c[k].q << ' ';
			in >> c[k].a;
			cout << c[k].a << ' ';
			in >> c[k].move;
			cout << c[k].move << " - command #" << k << endl;
		}
	}
	
	in.close();
	
	for (int i = 0; i < n; i++)
		cout << symbol[i] << ' ';
	
	
	for (int i = 0; i <= 261; i++)
		tape[i] = symbol[0]; //заполнение ленты нулевым символом
	
	j = 0;
	for (int i = 0; i < strlen(tape_ini); i++){
		if (tape_ini[i] == '!')
			cursor = i + 119;
		else {
			tape[120+j] = tape_ini[i];
			//cout << tape[120+j];
			j++;
		}
		last = 119 + i; //вспомогательная переменная для обозначения последнего значащего символа
	}
	//cout << endl;
	
	//cout << tape << endl;
	
	//cout << cursor << "---\n";
	
	if (cursor < 0){ //проверка на наличие курсора
		cerr << "Error: no cursor!" << endl;
		exit(1);
	}
	
	//cout << "tape_ini=" << tape_ini << endl;
	//cout << tape[120] << endl;
	//cout << tape << endl;
	//cout << last << endl;
	tape_print(tape,cursor,symbol[0]);
	
	for (int i = 0; condition > 0; i++){ //основной цикл
		
		//tape_print(tape,cursor,symbol[0]);

		for (j = 0; j < n; j++){ //определение номера символа
			//cout << "tape[cursor]=" << tape[cursor] << " symbol[j]=" << symbol[j] << endl;
			if (tape[cursor] == symbol[j]){
				symbol_num = j;
				//cout << '=' << symbol_num << '=' << endl;
				break;
			}
		}
		
		cout << '=' << symbol_num << '=' << endl;
		
		cout << "Command_old=" << command << ' ';
		command = m*symbol_num + condition - 1; //определение номера команды
		cout << "Command_new=" << command << ' ';
		cout << "Symbol_old=" << tape[cursor] << ' ';
		tape[cursor] = c[command].a; //получение нового символа
		cout << "Symbol_new=" << tape[cursor] << ' ';
		cout << "Symbol_num_old=" << symbol_num << ' '; 
		cout << "Condition_from=" << condition << ' ';
		condition = c[command].q; //получение нового состояния
		cout << "Condition_to=" << condition << '\n';
			
		switch (c[command].move){
			case 'L': {cursor--; break;}
			case 'R': {cursor++; break;}
			case 'N': {break;}
		}
		
		if (cursor > last && tape[cursor] != 0) //изменение переменной last
			last = cursor;
		while (tape[last] == symbol[0] && cursor < last)
			last -= 1;
			
		//cout << "Cursor=" << cursor << ' ';
		//cout << "Last=" << last << endl;
		
		
		steps = i; //счетчик шагов машины
		
		//cout << "}}}}}}}}}}}}}}}}}}}}}}}}}}\n";
		//cout << last << endl;
		//cout << tape << endl;
		tape_print(tape,cursor,symbol[0]);
		/*
		for (j = 0; j <= last; j++){
			if (b || tape[j] != symbol[0]){
				//cout << tape[j];
				if (cursor == j)
					cout << "\e[1m" << tape[j] << "\e[0m";
					else cout << tape[j];
				//cout << '(' << j << ')';
				b = 1;
			}
		}
		/*
		for (j = 0; j < last+1; j++){ //вывод, начиная с первого значащего символа
			cout << "j=" << j << ' ';
			if (b || tape[j] != symbol[0]){
				
				cout << tape[j] << '=' << symbol[0] << ' ';
				if (j = cursor){
					cout << "\e[1m" << tape[j] << "\e[0m";
					//cout << j << '_' << cursor << ' ';
					//cout << tape[j] << '!';
				}
				else cout << tape[j];
				b = 1;
			}
		}
		*/
	}
	
	cout << "Steps = " << steps + 1 << endl;
	
	return 0;
}