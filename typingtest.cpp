#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <string>
#include <vector>
#include <chrono>
#include <fstream>
#include <sstream>
#include <conio.h>
#include <set>
#include <thread>

using namespace std;
using namespace chrono;
using namespace std;

int main(int argc, char** argv) {
	ifstream file;
	file.open("C:\\Users\\cchha\\OneDrive\\Desktop\\Programming\\Capstone\\typingtest\\typingtest\\tests.txt");

	vector<string>sentences;
	set<int>used;
	steady_clock::time_point t1, t2;

	while (true) {
		string s;
		getline(file, s);
		if (file.eof())break;
		sentences.push_back(s);
	}
	srand(time(0));
	cout << "------------------\n";
	cout << "Type 'g' to start\n";
	cout << "Type 's' to skip\n";
	cout << "Press esc during test to exit \n";
	cout << "------------------\n";

	for (int i = 0; i < sentences.size(); i++) {
		//int rnd = rand() % sentences.size();
		int rnd = i; //normal
		while (used.count(rnd))rnd = rand() % sentences.size();
		used.insert(rnd);
		cout << "\x1b[0;33m" << sentences[rnd] << "\x1b[0m" << endl;
		int ite = 0, wrong = 0, acc = sentences[rnd].size();
		bool cancel = 0;
		char ch;
		string md; cin >> md;
		if (md == "s")continue;
		else {
			while (md != "g" && md != "f") {
				cout << "Invalid input, try again. \n";
			}
		}
		while (ite < sentences[rnd].size()) { 
			if(md!="f")ch = _getch();
			if (ite == 0) {
				t1 = steady_clock::now();
			}
			if (md == "f") {
				cout << "\x1b[0;32m" << sentences[rnd][ite] << "\x1b[0m";
				cout.flush();
				ite++;
				this_thread::sleep_for(milliseconds(10));
				continue;
			}
			if (ch == 27) {
				cancel = 1;
				break;
			}
			if (ch == 8) {
				cout << '\b' << " " << '\b';
				cout.flush();
				if (wrong > 0)wrong--;
				else if (ite > 0)ite--;
			}
			else if (wrong > 0) {
				cout << "\x1b[0;31m" << ch << "\x1b[0m";
				cout.flush();
				wrong++;
			}
			else if (ch == sentences[rnd][ite]) {
				cout << "\x1b[0;32m" << ch << "\x1b[0m";
				cout.flush();
				ite++;
			}
			else if (ch != sentences[rnd][ite]) {
				cout << "\x1b[0;31m" << ch << "\x1b[0m";
				cout.flush();
				acc--;
				wrong++;
			}
		}
		t2 = steady_clock::now();
		if (cancel) {
			cout << "N/A\n";
		}
		else {
			cout << "\n";
			double totaltime = duration_cast<duration<double>>(t2 - t1).count();
			cout << "WPM: " << round((sentences[rnd].size() / 5.0) / (totaltime / 60)) << "\n";
			cout << "Accuracy: " << round(((double)acc / (double)sentences[rnd].size()) * 100) << "%\n";
		}


	}
}
