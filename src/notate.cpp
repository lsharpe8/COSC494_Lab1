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

#define CLEF_SPACE 5
#define QUARTER_SPACE 10
#define TS_SPACE 5
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

bool graphNote(const double &x, const double &y, const int &n_value, const int &lengthDivisor, const string &imgPrefix, const int &clef) {
    int offset, direction;
    string imgSuffix;
    int middleValue;

    switch (clef) {
	//Treble clef
	case 0: 
	    //b4 = 6 + 4 * 7 = 34
	    middleValue = 34;
	    break;
	default:
	    cerr << "Invalid clef.\n";
	    return false;
    }
    offset = (n_value - middleValue);
    if ((n_value > (middleValue + 12)) || (n_value < (middleValue - 12))) {
	cerr << "A note requiring more than 4 ledger lines is not supported.\n";
	return false;
    }
    imgSuffix = ((n_value >= middleValue) && (lengthDivisor != 1)) ? "_down.eps" : ".eps";
    graphItem(x, y + (offset / 2.0), imgPrefix + imgSuffix);

    //Graph ledger lines
    direction = (n_value < middleValue) ? 1 : -1;
    if (((offset & 0x1) && (direction == -1)) || (!(offset & 0x1) && (direction == 1))) {
    } else
	offset += direction;
    for (double i = y + (offset / 2.0);(i < (y - 2)) || (i > (y + 2));i += direction)
	cout << "newline color 0 0 0 pts " << x - 1 << " " << i << " " << x + 1 << " " << i << endl;
    return true;
}

void graphTimeSignature(const double &x, const double &y, const double &top, const int &bot) { 
    cout << "newstring vjc hjc x " << x << " y " << y + 1 << " : " << top << "\nfontsize " << TS_FONT_SIZE << endl
	 << "newstring vjc hjc x " << x << " y " << y - 1 << " : " << bot << "\nfontsize " << TS_FONT_SIZE << endl;
}

bool graphKeySignature(const double &x, const double &y, const int &num, const char &type, const int &clef) { 
    if (type == 'n')
	return true;
    
    if ((type != 's') && (type != 'b'))
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
    if (type == 's') {
	start = 0;
	direction = 1;
    } else {
	start = 6;
	direction = -1;
	keys[3] -= 3.5;
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
    double x = 5;
    double bpb = 0;
    int beatUnit = 0;
    double beatSum = 0;

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
	    if (item == "done")
		return 0;

	    if (x >= X_LIMIT) {
		graphStaff(staff_y);
		graphMeasureLine(0, staff_y);
		x = CLEF_SPACE;
		graphItem(x, staff_y, "src/eps/" + clefItr->first + ".eps");
		x += CLEF_SPACE;
		graphTimeSignature(x, staff_y, bpb, beatUnit);
		x += TS_SPACE;
	    }

	    //Item format examples: qc4 = middle c for a quarter note, qr = quarter rest, t4/4 = 4/4 time signature marking
	    if (item.length() < 2) {
		cerr << "Invalid command.\n";
		continue;
	    }
	    
	    if (item[0] == 'q') {
		if (!bpb) {
		    cerr << "Missing time signature - enter one.\n";
		    continue;
		}

		double beats = beatUnit / 4.0;

		if ((beats + beatSum) > bpb) {
		    cerr << "A quarter note/rest is too long for this measure.\n";
		    continue;
		}

		if (item[1] == 'r')
		    graphItem(x, staff_y, "src/eps/quarter_rest.eps");
		else {
		    if ((item[1] < 'a') || (item[1] > 'g') || (item.length() < 3)) {
			cerr << "Invalid quarter note.\n";
			return 1;
		    }
		    int note = (((item[1] - 'a') + 5) % 7) + (item[2] - '0') * 7;
		    if (!graphNote(x, staff_y, note, 4, "src/eps/quarter_note", clef)) {
			cerr << "Invalid quarter note.\n";
			return 1;
		    }
		}
		space = QUARTER_SPACE;

		beatSum += beats;
		cerr << beatSum << endl;
	    } else if (item[0] == 't') {
		if ((sscanf(item.c_str(), "t%lf/%d", &bpb, &beatUnit) < 2) || (bpb < 1) || (beatUnit < 1)) {
		    cerr << "Invalid time signature.\n";
		    continue;
		}
		graphTimeSignature(x, staff_y, bpb, beatUnit);
		space = TS_SPACE;
	    } else if (item[0] == 'k') {
		int num;
		char type;
		if ((item.length() == 2) && (item[1] == '0')) {
		    num = 0;
		    type = 'n';
		} else if (item.length() == 3) {
		    num = item[1] - '0';
		    type = item[2];

		    if (!graphKeySignature(x, staff_y, num, type, clef)) {
			cerr << "Invalid key signature.\n";
			continue;
		    }
		} else {
		    cerr << "Invalid key signature.\n";
		    continue;
		}
		    
		space = KEY_SPACE * num;
	    } else if (item == "changeclef") {
		break;
	    } else {
		cerr << "Invalid command.\n";
		continue;
	    }

	    x += space;

	    if (beatSum == bpb) {
		graphMeasureLine(x - (space / 2.0), staff_y);
		beatSum = 0;
	    }
	}
    }
    return 0;
}
