#include <iostream>
#include <GL/glut.h>
#include <list>
#include <fstream>
#include <string>
#include <Windows.h>

using namespace std;

class MainMenu {
public:
	list <string> sb; //list

	int display();
	void displaysb();
	void scoreboard();
	void addScore(int score);

private:
	int choice;
	int counter; //counter for outputting list
};

int MainMenu::display() { //inital menu
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
	cout << "~\t   Scoreboard \t\t~" << endl;
	for (int i = 0; i < 33; i++)
		cout << "_"; //seperator

	cout << endl;
	//Color First Place red
	list<string>::iterator itr; //list iterator
	for (itr = sb.begin(); itr != sb.end(); itr++) {
		if (counter == 0) {
			//Color First Place red //white background
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | (BACKGROUND_RED| BACKGROUND_GREEN| BACKGROUND_BLUE));
			cout << "\t1. " << *itr << endl;
		}
		counter++;
	}

	counter = 0; //reset counter

	//color 2nd place Blue //white background
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE));
	for (itr = sb.begin(); itr != sb.end(); itr++) {
		if (counter == 1)
			cout << "\t2. " << *itr << endl;
		counter++;
	}

	counter = 0; //reset counter

	//color 3rd place green //white background
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN | (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE));
	for (itr = sb.begin(); itr != sb.end(); itr++) {
		if (counter == 2) //only display 3rd element
			cout << "\t3. " << *itr << endl;
		counter++;
	}

	counter = 0; //reset counter

	//4th -> infinity are white text
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED| FOREGROUND_GREEN | FOREGROUND_BLUE); //reset output to white
	for (itr = sb.begin(); itr != sb.end(); itr++) {
		if (counter > 2) //only display 4th element and on
			cout << "\t" << counter + 1 << ". " << *itr << endl; //output '#. Name - Score'
		counter++;
	}

	for (int i = 0; i < 33; i++)
		cout << "_"; //seperator

	cout << endl;

}

void MainMenu::scoreboard() {
	string l;
	ifstream board;
	try { //exception handling

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
	}
	catch (...) {
		cout << "Error opening file\n";
	}

	displaysb();

	board.close();
}
