/*
 * CS 106B Boggle
 * This file declares constants used throughout the program along with
 * the necessary function prototypes, so that other files
 * are able to call the recursive functions you will write.
 *
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp and Julie Zelenski
 * @version 2019/04/25
 * - initial 19sp version
 */

#ifndef _boggleconstants_h
#define _boggleconstants_h

#include <string>
#include "vector.h"


// global constants used by the program

/** Dictionary file where all English words can be found; used to populate a Lexicon. */
const std::string DICTIONARY_FILE = "dictionary.txt";

/**
 * Strings for the names of the two players,
 * suitable for passing to gui::recordWord and gui::setScore.
 */
const std::string PLAYER_COMPUTER = "computer";
const std::string PLAYER_HUMAN = "human";

/** Largest allowed board size. */
const int BOARD_SIZE_MAX = 6;

/** Smallest allowed board size. */
const int BOARD_SIZE_MIN = 4;

/** Default board size of 4 to indicate 4x4 board. */
const int BOARD_SIZE = BOARD_SIZE_MIN;

/** Minimum allowed word length. */
const int MIN_WORD_LENGTH = BOARD_SIZE;

/** Total number of cubes on the board. */
const int NUM_CUBES = BOARD_SIZE * BOARD_SIZE;

/** letters on all 6 sides of every cube in 4x4 standard Boggle. */
extern Vector<std::string> LETTER_CUBES;

/** letters on all 6 sides of every cube in 5x5 "Big Boggle" (for optional extra feature). */
extern Vector<std::string> LETTER_CUBES_BIG;

/** letters on all 6 sides of every cube in 6x6 "Super Big Boggle" (for optional extra feature). */
extern Vector<std::string> LETTER_CUBES_SUPER_BIG;


#endif // _boggleconstants_h
