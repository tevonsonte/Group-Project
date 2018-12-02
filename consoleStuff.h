#ifndef MainMenu

#include <iostream>
#include <GL/glut.h>
#include <list>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

class MainMenu {
public:
	int display();
	void displaysb();
	void scoreboard();
	void addScore(int score);
private:
	list <string> sb;
	int choice;
	int counter; //counter for outputting list
};

int MainMenu::display() {
	cout << "Welcome to Breakout's Menu\n";
	cout << "Your Challenge awaits...\n";
	cout << "__________________________\n";
	cout << "1. Play the game" << endl;
	cout << "2. See Scoreboard" << endl;
	cout << "3. Exit" << endl;

	//get user choice
	cout << "Please enter your selection: ";
	cin >> choice;

	return choice;
}

void MainMenu::displaysb() {
	counter = 0; //reset counter on call
	
	system("cls");
	
	//ouput title of scoreboar
	cout << "~\t    Scoreboard \t\t~" << endl;
	for (int i = 0; i < 33; i++)
		cout << "_"; //seperator
	
	cout << endl;
	//Color First Place red
	//SetConsoleTextAttribute(GetStdHandle(STD_INPUT_HANDLE), FOREGROUND_RED);
	list<string>::iterator itr;
	for (itr = sb.begin(); itr != sb.end(); itr++) {
		if (counter == 0) {
			//Color First Place red
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);
			cout << "\t1. " << *itr << endl;
		}
		counter++;
	}
	
	counter = 0; //reset counter
	
	//color 2nd place Blue
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
	for (itr = sb.begin(); itr != sb.end(); itr++) {
		if (counter == 1)
			cout <<"\t2. "<< *itr << endl;
		counter++;
	}

	counter = 0; //reset counter

	//color 3rd place green
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	for (itr = sb.begin(); itr != sb.end(); itr++) {
		if (counter == 2) //only display 3rd element
			cout <<"\t3. "<< *itr << endl;
		counter++;
	}

	counter = 0; //reset counter

	//4th -> infinity are standard white text
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_GREEN); //reset output to white
	for (itr = sb.begin(); itr != sb.end(); itr++) {
		if (counter > 2) //only display 4th element and on
			cout << "\t" << counter+1 << ". " <<*itr << endl; //output '#. Name - Score'
		counter++;
	}

	for (int i = 0; i < 33; i++)
		cout << "_"; //seperator

	cout << endl;

}

void MainMenu::scoreboard() {
	string l;
	ifstream board;
	board.open("Scores.txt");

	if (board.is_open()) {
		//clear list on call to prevent duplication of scores
		sb.clear();

		//populate list from scoreboard if it already exists
		while (!board.eof()) 
		{
			board >> l;
			sb.push_back(l);
		}
	}

	displaysb();

	board.close();
}

void addScore(int score) {
	string user, s;
	ofstream newScore;

	cout << "Please enter your name: ";
	cin >> user;

	s = "-";
	s += score;

	user.append(s); //add user and highscore in same line

	newScore.open("Scores.txt");

	if (newScore.is_open())
		newScore << user; //add score to file

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
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (BACKGROUND_RED | BACKGROUND_BLUE | BACKGROUND_GREEN) | 0);
	cout << "\ \ / / |_ _|((/ __||_   _|  / _ \ | _ \\ \ / / " << endl;
	cout << " \ V /   | |  | (__   | |   | (_) ||   / \ V /  " << endl;
	cout << "  \_/   |___|  \___|  |_|    \___/ |_|_\  |_| " << endl;
}
#endif