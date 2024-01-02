#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <iostream>

//typing test libaries
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

using namespace cv;
using namespace std;
using namespace chrono;

Mat img;

vector<Point>fingers(8);
//leftIndex, leftMiddle, leftRing, leftPinky, rightIndex, rightMiddle, rightRing, rightPinky;
//  purple     orange      green     blue       yellow      pink         green      blue

//a-z then 1 to 0 next line then special characters
vector<int>keyXLocation{460, 280, 360, 380, 400, 340, 300, 260, 200, 220, 180, 140, 200, 240, 160, 120, 480, 360, 420, 320, 240, 320, 440, 400, 280, 440, 
490, 450, 410, 370, 330, 290, 250, 210, 170, 130,
100,  60,  160,  120,  80, 90, 50
};
//:;  "'   ,<   .>     /?  -_  +=
vector<int>keyYLocation{80, 50, 50, 80, 110, 80, 80, 80, 110, 80, 80, 80, 50, 50, 110, 110, 110, 110, 80, 110, 110, 50, 110, 50, 110, 50,
140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140,
80, 80, 50, 50, 50, 140, 140};


vector<vector<int>>preferredFingers(200, vector<int>(8));
vector<set<int>>goodFingers(8);

void trackPosition(Mat imgDil, int color) {
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;

	findContours(imgDil, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
	//imshow("test", imgDil);
	Point pos(0, 0);

	for (int i = 0; i < contours.size(); i++) {
		int area = contourArea(contours[i]);
		vector<vector<Point>>conPoly(contours.size());
		vector<Rect> boundRect(contours.size());
		string objectType;
		if (area > 100) {
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], conPoly[i], 0.02 * peri, true);
			boundRect[i] = boundingRect(conPoly[i]);
			pos.x = boundRect[i].x + boundRect[i].width / 2;
			pos.y = boundRect[i].y + boundRect[i].height / 2;
			drawContours(img, conPoly, i, Scalar(255, 0, 255), 3);
			//cout << pos.x << " " << pos.y << "||||";
			if (pos.x != 0 && pos.y != 0) {
				if (color == 0)fingers[4] = pos;
				if (color == 1 && pos.x < 280)fingers[7] = pos;  
				if (color == 1 && pos.x >= 280)fingers[3] = pos;
				if (color == 2 && pos.x < 280)fingers[6] = pos;
				if (color == 2 && pos.x >= 280)fingers[2] = pos;
				if (color == 3)fingers[1] = pos;
				if (color == 5)fingers[0] = pos;
				if (color == 4)fingers[5] = pos;
			}
		}
		//if (color == 5)cout << pos.x << " " << pos.y << "\n";
	}
	/*
	for (int i = 0; i < 8; i++) {
		if (i == 0)cout << "leftIndex: "; 
		if (i == 1)cout << "leftMiddle: ";
		if (i == 2)cout << "leftRing: ";
		if (i == 3)cout << "leftPinky: ";
		if (i == 4)cout << "rightIndex: ";
		if (i == 5)cout << "rightMiddle: ";
		if (i == 6)cout << "rightRing: ";
		if (i == 7)cout << "rightPinky: ";
		cout << fingers[i].x << " " << fingers[i].y << "\n";
	}
	*/
}

string findFinger(char ch) {
	int dist = 100000, fnl = -1;
	string ans = "grrr";
	for (int i = 0; i < 8; i++) {
		int cur = 100000;
		if ((int)ch >= 65 && int(ch) <= 91) cur = sqrt(pow(abs(fingers[i].x - keyXLocation[(int)ch - 65]), 2) + pow(abs(fingers[i].y - keyYLocation[(int)ch - 65]), 2));
		else if (ch >= '1' && ch <= '9')cur = sqrt(pow(abs(fingers[i].x - keyXLocation[(int)ch - 23]), 2) + pow(abs(fingers[i].y - keyYLocation[(int)ch - 23]), 2));
		else if (ch == '0')cur = sqrt(pow(abs(fingers[i].x - keyXLocation[35]), 2) + pow(abs(fingers[i].y - keyYLocation[35]), 2)); // 0
		else if (ch == ';')cur = sqrt(pow(abs(fingers[i].x - keyXLocation[36]), 2) + pow(abs(fingers[i].y - keyYLocation[36]), 2)); // : and ;
		else if ((int)ch == 39)cur = sqrt(pow(abs(fingers[i].x - keyXLocation[37]), 2) + pow(abs(fingers[i].y - keyYLocation[37]), 2)); // " and '
		else if (ch == ',')cur = sqrt(pow(abs(fingers[i].x - keyXLocation[38]), 2) + pow(abs(fingers[i].y - keyYLocation[38]), 2)); // , and <
		else if (ch == '.')cur = sqrt(pow(abs(fingers[i].x - keyXLocation[39]), 2) + pow(abs(fingers[i].y - keyYLocation[39]), 2)); // . and >
		else if (ch == '/')cur = sqrt(pow(abs(fingers[i].x - keyXLocation[40]), 2) + pow(abs(fingers[i].y - keyYLocation[40]), 2)); // / and ?
		else if (ch == '-')cur = sqrt(pow(abs(fingers[i].x - keyXLocation[41]), 2) + pow(abs(fingers[i].y - keyYLocation[41]), 2)); //- and _
		else if (ch == '=')cur = sqrt(pow(abs(fingers[i].x - keyXLocation[42]), 2) + pow(abs(fingers[i].y - keyYLocation[42]), 2)); // = and +

		if (cur < dist) {
			dist = cur;
			if (i == 0) {
				ans = "indexL";
				fnl = 0;
			}
			if (i == 1) {
				ans = "middleL";
				fnl = 1;
			}
			if (i == 2) {
				ans = "ringL";
				fnl = 2;
			}
			if (i == 3) {
				ans = "pinkyL";
				fnl = 3;
			}
			if (i == 4) {
				ans = "indexR";
				fnl = 4;
			}
			if (i == 5) {
				ans = "middleR";
				fnl = 5;
			}
			if (i == 6) {
				ans = "ringR";
				fnl = 6;
			}
			if (i == 7) {
				ans = "pinkyR";
				fnl = 7;
			}
		}
	}

	preferredFingers[(int)ch][fnl]++;
	//cout << ch << ": ";
	//cout << ans << "\n";
		
	return ans;
}

string curWord = "";
set<int>pressed;
set<int>sPressed;

void LOG(string input) {
	fstream LogFile;
	LogFile.open("C:\\Users\\cchha\\OneDrive\\Desktop\\Keylogger.txt", fstream::app);
	if (LogFile.is_open()) {
		LogFile << input;
		cout << input;
		LogFile.close();
	}
}
bool specialKeys() {
	if (GetKeyState(0xBE) & 0x8000) { 
		if (!sPressed.count(10000)) {
			if (GetAsyncKeyState(VK_SHIFT))curWord += ">";
			else curWord += ".";
			sPressed.clear();
			sPressed.insert(10000);
			//findFinger('.');
			return true;
		}
	}
	else if (GetKeyState(0xBA) & 0x8000) {
		if (!sPressed.count(10001)) {
			if (GetAsyncKeyState(VK_SHIFT))curWord += ":";
			else curWord += ";";
			sPressed.clear();
			sPressed.insert(10001);
			//findFinger(';');
			return true;
		}
	}
	else if (GetKeyState(0xBC) & 0x8000) {
		if (!sPressed.count(10002)) {
			if (GetAsyncKeyState(VK_SHIFT))curWord += "<";
			else curWord += ",";
			sPressed.clear();
			sPressed.insert(10002);
			//findFinger(',');
			return true;
		}
	}
	else if (GetKeyState(0xDE) & 0x8000) {
		if (!sPressed.count(10003)) {
			if (GetAsyncKeyState(VK_SHIFT))curWord += '"';
			else curWord += "'";
			sPressed.clear();
			sPressed.insert(10003);
			//findFinger(''');
			return true;
		}
	}
	else if (GetKeyState(0xBF) & 0x8000) {
		if (!sPressed.count(10004)) {
			if (GetAsyncKeyState(VK_SHIFT))curWord += "?";
			else curWord += "/";
			sPressed.clear();
			sPressed.insert(10004);
			//findFinger('/');
			return true;
		}
	}
	else if (GetKeyState(0xBD) & 0x8000) {
		if (!sPressed.count(10005)) {
			if (GetAsyncKeyState(VK_SHIFT))curWord += "_";
			else curWord += "-";
			sPressed.clear();
			sPressed.insert(10005);
			//findFinger('-');
			return true;
		}
	}
	else if (GetKeyState(0xBB) & 0x8000) {
		if (!sPressed.count(10006)) {
			if (GetAsyncKeyState(VK_SHIFT))curWord += "+";
			else curWord += "=";
			sPressed.clear();
			sPressed.insert(10006);
			//findFinger('=');
			return true;
		}
	}
	else {
		sPressed.clear();
	}


	return false;
}

void main() {

	VideoCapture cap(1);

	Mat imgHSV;
	bool cancel = 0;
	int totalClicks = 0;
	vector<int>overallPF(8);
	vector<vector<int>>allColors{ {19, 51, 123, 255, 191, 255}, //yellow 
								  {90, 121, 209, 255, 135, 255}, //blue
								  {57, 86, 165, 255, 0, 255}, //green
								  {7, 22, 104, 255, 156, 255}, //orange
								  {162, 180, 168, 255, 159, 255}, //pink
								  {118, 158, 49, 255, 69, 255} //purple
	};
	
	//for adjustments
	namedWindow("Trackbars", (840, 200));
	createTrackbar("Hue min", "Trackbars", &allColors[3][0], 179);
	createTrackbar("Hue max", "Trackbars", &allColors[3][1], 179);
	createTrackbar("Sat min", "Trackbars", &allColors[3][2], 255);
	createTrackbar("Sat max", "Trackbars", &allColors[3][3], 255);
	createTrackbar("Val min", "Trackbars", &allColors[3][4], 255);
	createTrackbar("Val max", "Trackbars", &allColors[3][5], 255);
	
	while (true) {
		if (cancel)break;
		cap.read(img);
		img = img(Range(200, 450), Range(50, 590));
		cvtColor(img, imgHSV, COLOR_BGR2HSV);
		imshow("original", imgHSV);
		for (int i = 0; i < allColors.size(); i++) {
			Scalar lower(allColors[i][0], allColors[i][2], allColors[i][4]);
			Scalar upper(allColors[i][1], allColors[i][3], allColors[i][5]);
			Mat mask;

			inRange(imgHSV, lower, upper, mask);
			//imshow(to_string(i), mask);
			trackPosition(mask, i);

		}
		imshow("contours", img);
		
		
		for (int i = 32; i <= 126; i++) {
			if (GetAsyncKeyState(VK_ESCAPE)) {
				cancel = 1;
				break;
			}
			if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
				if (pressed.count(999))continue;
				pressed.insert(999);
				fstream LogFile;
				LogFile.open("C:\\Users\\cchha\\OneDrive\\Desktop\\Keylogger.txt", fstream::app);
				if (LogFile.is_open()) {
					cout << curWord << " ";
					LogFile << curWord << " ";
					curWord = "";
					LogFile.close();
				}
				continue;
			}
			else pressed.erase(999);
			if (!specialKeys()) {
				if (GetAsyncKeyState(char(i)) & 0x8000) {
					if (pressed.count(i))continue;
					pressed.insert(i);
					bool lower = 1;
					if (GetAsyncKeyState(VK_SHIFT))lower = 0;
					char output = (char)i;
					if (lower)output = tolower(output);
					curWord += output;
					string curFinger = findFinger((char)i);
					cout << curFinger;
				}
				else {
					pressed.erase(i);
				}
			}
		}
		
		waitKey(1);
	}

	goodFingers[3].insert(49);goodFingers[3].insert(65);goodFingers[3].insert(81);goodFingers[3].insert(90);
	goodFingers[2].insert(50);goodFingers[2].insert(83);goodFingers[2].insert(87);goodFingers[2].insert(88);
	goodFingers[1].insert(51);goodFingers[1].insert(67);goodFingers[1].insert(68);goodFingers[1].insert(69);
	goodFingers[0].insert(52);goodFingers[0].insert(53);goodFingers[0].insert(82);goodFingers[0].insert(84);goodFingers[0].insert(70);goodFingers[0].insert(71);goodFingers[0].insert(56);goodFingers[0].insert(66);
	goodFingers[4].insert(54);goodFingers[4].insert(55);goodFingers[4].insert(89);goodFingers[4].insert(72);goodFingers[4].insert(78);goodFingers[4].insert(77);goodFingers[4].insert(74);goodFingers[4].insert(85);
	goodFingers[5].insert(56);goodFingers[5].insert(73);goodFingers[5].insert(75);goodFingers[5].insert(60);
	goodFingers[6].insert(57);goodFingers[6].insert(76);goodFingers[6].insert(79);goodFingers[6].insert(62);
	goodFingers[7].insert(48);goodFingers[7].insert(80);goodFingers[7].insert(58);goodFingers[7].insert(63);

	cout << "\n\n Summary of Session: \n";
	for (int i = 0; i < 200; i++) {
		int curTotal = 0;
		for (int j = 0; j < 8; j++) {
			overallPF[j] += preferredFingers[i][j];
			totalClicks += preferredFingers[i][j];
			curTotal += preferredFingers[i][j];
		}
		if (curTotal > 0) {
			cout << "Key: " << (char)i << "\n";
			cout << "Left Index: " << preferredFingers[i][0] << "/" << curTotal <<  "\n";
			cout << "Left Middle: " << preferredFingers[i][1] << "/" << curTotal << "\n";
			cout << "Left Ring: " << preferredFingers[i][2] << "/" << curTotal << "\n";
			cout << "Left Pinky: " << preferredFingers[i][3] << "/" << curTotal << "\n";
			cout << "Right Index: " << preferredFingers[i][4] << "/" << curTotal << "\n";
			cout << "Right Middle: " << preferredFingers[i][5] << "/" << curTotal << "\n";
			cout << "Right Ring: " << preferredFingers[i][6] << "/" << curTotal << "\n";
			cout << "Right Pinky: " << preferredFingers[i][7] << "/" << curTotal << "\n";
			for (int j = 0; j < 8; j++) {
				if (goodFingers[j].count(i)) {
					if (preferredFingers[i][j]*100.0 / curTotal < 80) {
						if (j == 0)cout << "Try using your left index finger more for typing " << (char)i << "\n";
						else if (j == 1)cout << "Try using your left middle finger more for typing " << (char)i << "\n";
						else if (j == 2)cout << "Try using your left ring finger more for typing " << (char)i << "\n";
						else if (j == 3)cout << "Try using your left pinky finger more for typing " << (char)i << "\n";
						else if (j == 4)cout << "Try using your right index finger more for typing " << (char)i << "\n";
						else if (j == 5)cout << "Try using your right middle finger more for typing " << (char)i << "\n";
						else if (j == 6)cout << "Try using your right ring finger more for typing " << (char)i << "\n";
						else if (j == 7)cout << "Try using your right pinky finger more for typing " << (char)i << "\n";
					}
				}
			}
			cout << "---------------\n";
		}
	}

	cout << "\n\nOverall preferred fingers:\n\n";
	cout << "Left Index: " << overallPF[0] << "/" << totalClicks << "\n";
	cout << "Left Middle: " << overallPF[1] << "/" << totalClicks << "\n";
	cout << "Left Ring: " << overallPF[2] << "/" << totalClicks << "\n";
	cout << "Left Pinky: " << overallPF[3] << "/" << totalClicks << "\n";
	cout << "Right Index: " << overallPF[4] << "/" << totalClicks << "\n";
	cout << "Right Middle: " << overallPF[5] << "/" << totalClicks << "\n";
	cout << "Right Ring: " << overallPF[6] << "/" << totalClicks << "\n";
	cout << "Right Pinky: " << overallPF[7] << "/" << totalClicks << "\n";

}
