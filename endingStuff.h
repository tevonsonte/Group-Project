#include <Windows.h>
#include <string>
#include <fstream>
using namespace std;

string user;

void addScore(int score) {
	string s;
	ofstream newScore;

	cout << "Please enter your name: ";
	cin >> user;

	string num = to_string(score); //typecast int to string

	s = "-";
	s += score;

	user.append(s); //add user and highscore in same line
	
	try { //exception handling
		newScore.open("Scores.txt");

		if (newScore.is_open())
			newScore << user; //add score to file
	}
	catch (...) {
		cout << "Error saving score..." << endl;
	}

	newScore.close();
}

void displayVictory() {
	system("cls");
	//display the text graph
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN);
	cout << "         (                      )   (        ) " << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	cout << "         )\ )   (     *   )  ( /(   )\ )  ( /(  " << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN);
	cout << " (   (  (()/(   )\  ` )  /(  )\()) (()/(  )\()) " << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
	cout << " )\  )\  /(_))(((_)  ( )(_))((_)\   /(_))((_)\  " << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_INTENSITY);
	cout << "((_)((_)(_))  )\___ (_(_())  ((__) (_)) __ ((_) " << endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | (BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN));
	cout << "\ \ / / |_ _|((/ __||_   _|  / _ \ | _ \\ \ / / " << endl;
	cout << " \ V /   | |  | (__   | |   | (_) ||   / \ V /  " << endl;
	cout << "  \_/   |___|  \___|  |_|    \___/ |_|_\  |_|   " << endl;
}
