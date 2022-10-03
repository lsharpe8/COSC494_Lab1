# COSC494 Lab1

This program graphs sheet music with Jgraph based on user commands read from stdin.
To compile the program and run it with the five test inputs, enter 'make' on the command line. The
resulting PDFs will be titled test[1-5].txt and will be in the pdf directory.
To compile and run the program to read from stdin, enter 'make run file=foo.bar' on the command line. It will
name the resulting PDF whatever you replaced foo.bar with.

In case you are not familiar with the meaning of these musical terms or use a different meaning than I,
here is a personal glossary of terms:
    Accidental - Modifiers for notes that raise or lower the pitch, represented by symbols next to the notes.
    Bar - See Measure.
    Clef - Dictates which notes the lines on the staff represent, represented by a symbol which is
	    usually at the left end of the staff.
    Key signature - The representation of the accidentals which are part of the key, shown at the left end of the staff.
    Ledger lines - Small, horizontal lines that "extend" the staff temporarily to fit especially high or low notes
    Measure - A small section of a song, separated by vertical lines based on the rhythm
    Staff - The grouped horizontal lines that all the notes, clefs, etc. are drawn on.
    Time signature - Dictates what the length of a beat is and how many beats per measure/bar there are, represented
			by two numbers, one on top of the other, at the beginning of the staff. The top number is
			the number of beats per measure, and the bottom number is the length of a beat represented
			as the divisor of the fraction of a whole note that represents the length.

**All the commands must be entirely in lowercase.**
The first thing a user must enter is the clef they wish the first staff to have. Currently, the only acceptable clef is 'treble'.

Next, the user must enter the time signature with a command in the format 't#/#' where the #s are replaced by the upper and
lower values of the time signature, respectively.

To enter a key signature, enter a command in the format 'k#c' where # is replaced by the number of sharps or flats in the key
and c is replaced by 's' for sharps and 'b' for flats. If the number is 0, c is optional but can be replaced with 'n'.
Numbers less than 0 or greater than 7 are invalid.

To enter a note, enter a command in the format 'ln#' where l is replaced by the first character of the word which describes its
length and n# is replaced by the note's scientific pitch notation. Wikipedia has a full explanation of this notation, but basically
each octave, starting from C, has a number. The octave from Middle C to the B a 7th up from it has the number 4.
Currently, the only acceptable inputs for l are 'q' for quarter and 'h' for half. 

To enter an accidental before a note, you must enter a separate command before the note in the format 'cn#' where c is replaced by
the desired accidental in the same way as for key signature and n# is replaced by the note's scientific pitch notation.

To enter a rest, enter a command in the format 'lr' where l is replaced in the same way as a note.

Enter 'done' to finish.
