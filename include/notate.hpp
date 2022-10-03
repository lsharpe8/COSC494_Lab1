/* COSC494 Lab 1 - Jgraph
 * Lily Sharpe
 * Due Oct 3, 2022
 */

using namespace std;

//Graphs five staff lines and decreases the y value by the height plus buffer of the staff
void graphStaff(double &y);

//Graphs the .eps specified by img at x y
void graphItem(const double &x, const double &y, const string &img);

//Graphs a note at x y given its n_value (calculated with its letter and number), lengthDivisor (e.g. 4 for a quarter note), 
//the beginning of its .eps name (up until but not including the .eps or _down.eps), and the current clef's corresponding integer.
bool graphNote(const double &x, const double &y, const int &n_value, const int &lengthDivisor, const string &imgPrefix, const int &clef);

//Graphs an accidental at x y given its value (calculated the same as n_value), .eps name, and the current clef.
bool graphAccidental(const double &x, const double &y, const int &value, const string &img, const int &clef);

//Graphs a time signature at x y given its upper and lower values.
void graphTimeSignature(const double &x, const double &y, const double &top, const int &bot);

//Graphs a key signature at x y given the number of accidentals; whether they are sharps, flats, or naturals (represented by 's', 'b', and 'n',
//respectively); whether they are to be placed at the sharp positions or the flat positions; and the current clef.
bool graphKeySignature(const double &x, const double &y, const int &num, const char &type, const char &positionType, const int &clef);

//Graphs a measure line spanning the whole staff height at the given x position
void graphMeasureLine(const double &x);
