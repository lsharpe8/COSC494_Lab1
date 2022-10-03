/* COSC494 Lab 1 - Jgraph
 * Lily Sharpe
 * Due Oct 3, 2022
 */

#include <iostream>
#include <map>
#include <stdio.h>
#include "notate.hpp"

#define STAFF_HEIGHT 20
#define X_LIMIT 95
#define X_MAX 100

#define CLEF_SPACE 5
#define TS_SPACE 5
#define BAR_SPACE 5
#define QUARTER_SPACE 10
#define HALF_SPACE 20
#define KEY_SPACE 2

#define STANDARD_SIZE_X 10
#define STANDARD_SIZE_Y 10
#define TS_FONT_SIZE 22
#define KEY_FONT_SIZE 20

using namespace std;

void graphStaff(double &y) {
    y -= STAFF_HEIGHT;
    cout << "newcurve eps src/eps/staff.eps marksize 100 5" << endl
	 << "pts 50 " << y << endl;
}

void graphItem(const double &x, const double &y, const string &img) {
    cout << "newcurve eps " << img << " marksize " << STANDARD_SIZE_X << " " << STANDARD_SIZE_Y << endl
	 << "pts " << x << " " << y << endl;
}

int getMiddleValue(const double &y, const int &n_value, const int &clef) {
    int middleValue;

    switch (clef) {
	//Treble clef
	case 0: 
	    //b4 = 6 + 4 * 7 = 34
	    middleValue = 34;
	    break;
	default:
	    return 1000;
    }
    
    return middleValue;
}

bool graphNote(const double &x, const double &y, const int &n_value, const int &lengthDivisor, const string &imgPrefix, const int &clef) {
    string imgSuffix;

    int middleValue = getMiddleValue(y, n_value, clef);
    if (middleValue == 1000) {
	cerr << "Invalid clef.\n";
	return false;
    }
    int offset = (n_value - middleValue);
    if ((n_value > (middleValue + 12)) || (n_value < (middleValue - 12))) {
	cerr << "A note requiring more than 4 ledger lines is not supported.\n";
	return false;
    }

    imgSuffix = ((n_value >= middleValue) && (lengthDivisor != 1)) ? "_down.eps" : ".eps";
    graphItem(x, y + (offset / 2.0), imgPrefix + imgSuffix);

    //Graph ledger lines
    int direction = (n_value < middleValue) ? 1 : -1;
    if (offset & 0x1) {
	offset += direction;
    }
    for (double i = y + (offset / 2.0);(i < (y - 2)) || (i > (y + 2));i += direction)
	cout << "newline color 0 0 0 pts " << x - 1 << " " << i << " " << x + 1 << " " << i << endl;
    return true;
}

bool graphAccidental(const double &x, const double &y, const int &value, const string &img, const int &clef) {
    int middleValue = getMiddleValue(y, value, clef);
    if (middleValue == 1000)
	return false;
    if ((value > (middleValue + 12)) || (value < (middleValue - 12))) {
	cerr << "A note requiring more than 4 ledger lines is not supported.\n";
	return false;
    }
    int offset = (value - middleValue);
    graphItem(x, y + (offset / 2.0), img);
    return true;
}

void graphTimeSignature(const double &x, const double &y, const double &top, const int &bot) { 
    cout << "newstring vjc hjc x " << x << " y " << y + 1 << " : " << top << "\nfontsize " << TS_FONT_SIZE << endl
	 << "newstring vjc hjc x " << x << " y " << y - 1 << " : " << bot << "\nfontsize " << TS_FONT_SIZE << endl;
}

bool graphKeySignature(const double &x, const double &y, const int &num, const char &type, const char &positionType, const int &clef) { 
    if ((type != 's') && (type != 'b') && (type != 'n'))
	return false;

    int temp_y;
    double keys[7] = {2, .5, 2.5, 1, -.5, 1.5, 0}; //Positions of sharps relative to middle of treble clef in order
    switch (clef) {
	case 0:
	    temp_y = y;
	    break;
	case 1:
	    temp_y = y - 2;
	    break;
	default:
	    cerr << "Invalid clef.\n";
	    return false;
    }

    int start, direction;
    if (positionType == 's') {
	start = 0;
	direction = 1;
    } else {
	start = 6;
	direction = -1;
    }

    if (type == 'b') {
	keys[0] -= 3.5;
	keys[2] -= 3.5;
    }
    
    const char typeCStr[2] = {type, '\0'};

    for (int i = start, j = 0;j < num;i += direction, j++) {
	graphItem(x + j * KEY_SPACE, keys[i] + temp_y, "src/eps/" + string(typeCStr) + ".eps");
    }

    return true;
}

void graphMeasureLine(const double &x, const double &y) {
    cout << "newline color 0 0 0 pts " << x << " " << y + 1.9 << " " << x << " " << y - 1.9 << endl;
}

void graphEndBar(const double &x, const double &y) {
    graphMeasureLine(x - .4, y);
    cout << "newline poly pfill 0 pts " << x - .2 << " " << y + 1.9 << " " << x << " " << y + 1.9 << " " << x << " " << y - 1.9 << " " << x - .2 << " " << y - 1.9 << endl;
}

int main() {
    double top = 100.0;
    double staff_y = top;

    cout << "newgraph" << endl;
    cout << "xaxis min 0 max 100 size 8 nodraw" << endl
	 << "yaxis min 0 max 100 size 11.5 nodraw" << endl;

    graphStaff(staff_y);
    
    graphMeasureLine(0, staff_y);

    map<string, int> clefs = {{"treble", 0}};
    string str;
    string item;

    double space = 0;
    double x = CLEF_SPACE;

    double bpb = 0;
    int beatUnit = 0;
    double beatSum = 0;
    int num = 0;
    char type = 'n';

    while (cin >> str) {
	map<string, int>::iterator clefItr = clefs.find(str);
	if (clefItr == clefs.end()) {
	    cerr << "Invalid command.\n";
	    return 1;
	}

	int clef = clefItr->second;
	graphItem(x, staff_y, "src/eps/" + clefItr->first + ".eps");
	x += CLEF_SPACE;

	while (cin >> item) {
	    if (item == "done") {
		if (beatSum == 0) {
		    cout << "newline poly pfill 1 color 1 1 1 pts " << (x - space / 2.0 - .2) << " " << staff_y + 2 << " " << X_MAX << " " << staff_y + 2
			 << " " << X_MAX << " " << (staff_y - 2) << " " << (x - space / 2.0 - .2) << " " << (staff_y - 2) << endl;
		    graphEndBar(x - space / 2.0 - .2, staff_y);
		} else 
		    graphEndBar(X_MAX, staff_y);
		return 0;
	    }

	    //Extend staff when necessary
	    if (x >= X_LIMIT) {
		if (staff_y - STAFF_HEIGHT < (STAFF_HEIGHT / 2.0)) {
		    cerr << "Graphing more than one page of music at a time is not supported.\n";
		    return 1;
		}
		graphStaff(staff_y);
		graphMeasureLine(0, staff_y);

		x = CLEF_SPACE;
		graphItem(x, staff_y, "src/eps/" + clefItr->first + ".eps");
		x += CLEF_SPACE;

		graphKeySignature(x, staff_y, num, type, type, clef);
		x += KEY_SPACE * num;
		if (num > 0) {
		    x += KEY_SPACE;
		}
	    }

	    if (item.length() < 2) {
		cerr << "Invalid command.\n";
		continue;
	    }
	    
	    if ((item[0] == 'q') || (item[0] == 'h')) { //Graph note
		if (!bpb) {
		    cerr << "Missing time signature - enter one.\n";
		    continue;
		}

		string imgPrefix;
		int lengthDivisor;
		int noteSpace;
		switch (item[0]) {
		    case 'q':
			imgPrefix = "src/eps/quarter_";
			lengthDivisor = 4;
			noteSpace = QUARTER_SPACE;
			break;
		    case 'h':
			imgPrefix = "src/eps/half_";
			lengthDivisor = 2;
			noteSpace = HALF_SPACE;
			break;
		    //Error checking already done, no need for default
		}

		double beats = beatUnit / (double) lengthDivisor;

		if ((beats + beatSum) > bpb) {
		    cerr << "Specified note/rest is too long for this measure.\n";
		    continue;
		}

		if (item[1] == 'r')
		    graphItem(x, staff_y, imgPrefix + "rest.eps");
		else {
		    if ((item[1] < 'a') || (item[1] > 'g') || (item.length() < 3)) {
			cerr << "Invalid note.\n";
			return 1;
		    }
		    int note = (((item[1] - 'a') + 5) % 7) + (item[2] - '0') * 7;
		    if (!graphNote(x, staff_y, note, lengthDivisor, imgPrefix + "note", clef)) {
			cerr << "Invalid note.\n";
			return 1;
		    }
		}
		space = noteSpace;

		beatSum += beats;
	    } else if (item[0] == 't') { //Graph time signature
		if (beatSum != 0) {
		    cerr << "Cannot change time signature in the middle of a measure.\n";
		    continue;
		}

		if ((sscanf(item.c_str(), "t%lf/%d", &bpb, &beatUnit) < 2) || (bpb < 1) || (beatUnit < 1)) {
		    cerr << "Invalid time signature.\n";
		    continue;
		}
		graphTimeSignature(x, staff_y, bpb, beatUnit);
		space = TS_SPACE;
	    } else if (item[0] == 'k') {
		if (beatSum != 0) {
		    cerr << "Cannot change key signature in the middle of a measure.\n";
		    continue;
		}

		//Graph key signature transition lines and naturalize previous key
		if (x > (CLEF_SPACE * 2 + TS_SPACE * 2)) {
		    x -= (space / 2.0);
		    graphMeasureLine(x + .5, staff_y);
		    x += KEY_SPACE + .5;
		    
		    graphKeySignature(x, staff_y, num, 'n', type, clef);
		    x += KEY_SPACE * num;
		    if (num > 0)
			x += KEY_SPACE;
		}

		if ((item.length() == 2) && (item[1] == '0')) {
		    num = 0;
		    type = 'n';
		} else if (item.length() == 3) {
		    num = item[1] - '0';
		    type = item[2];

		    if (!graphKeySignature(x, staff_y, num, type, type, clef)) {
			cerr << "Invalid key signature.\n";
			continue;
		    }
		} else {
		    cerr << "Invalid key signature.\n";
		    continue;
		}
		    
		space = KEY_SPACE * num;
		if (num > 0)
		    space += KEY_SPACE;
	    } else if ((item[0] == 's') || (item[0] == 'b') || (item[0] == 'n')) {
		char type;
		char pos_c;
		int pos;
		if (sscanf(item.c_str(), "%c%c%d", &type, &pos_c, &pos) != 3) {
		    cerr << "Could not read accidental command.\n";
		    continue;
		}

		pos *= 7;
		pos += ((pos_c - '0' + 5) % 7);
		const char typeCStr[2] = {type, '\0'};
		if (!graphAccidental(x, staff_y, pos, "src/eps/" + string(typeCStr) + ".eps", clef)) {
		    cerr << "Invalid accidental.\n";
		    continue;
		}

		space = KEY_SPACE;
	    } else if (item == "changeclef") {
		break;
	    } else {
		cerr << "Invalid command.\n";
		continue;
	    }

	    x += space;

	    //Put bar at the end of each measure
	    if (beatSum == bpb) {
		double bar_x;
		if (x >= X_LIMIT)
		    bar_x = X_MAX - .1;
		else {
		    bar_x = x - space / 2.0;
		    x -= space / 2.0;
		    x += BAR_SPACE;
		}
		graphMeasureLine(bar_x, staff_y);
		beatSum = 0;
	    }
	}
    }
    return 0;
}
