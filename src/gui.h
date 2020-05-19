/*
 * CS 106B/X Boggle
 * This file declares functions you can use to interact with the Boggle GUI.
 * See gui.cpp for the implementation of each function.
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
 * - consolidated methods to make Julie happy
 * - alphabetized methods to make Julie unhappy
 */

#ifndef _gui_h
#define _gui_h

#include <string>
#include "grid.h"
#include "boggleconstants.h"

namespace gui {
    /**
     * Restores any highlighted cubes to their normal color.
     */
    void clearHighlighting();

    /**
     * Initializes the Boggle graphics package and its graphics window,
     * using the given number of rows and columns on the board.
     * If no number of rows or columns is passed, uses the default size of 4.
     * An error is raised if either dimension is <= BOARD_SIZE_MIN or > BOARD_SIZE_MAX.
     */
    void initialize(int rows = BOARD_SIZE, int cols = BOARD_SIZE);

    /**
     * Draws the specified letters on the face of all cubes of the board.
     * You can pass a 4x4 Grid, a 16-letter string,
     * or a string with line breaks after each four characters.
     * An error is raised if the string or grid is not the right length.
     */
    void labelCubes(const Grid<char>& letters);
    void labelCubes(const std::string& letters);

    /**
     * Plays a sound effect located in the given file.
     */
    void playSound(const std::string& filename);

    /**
     * Records the specified word by adding it to the screen display for
     * the specified player, which must be either "human" or "computer".
     * If the given word has already been recorded, adds it again (so don't do that).
     * Throws an error if the player is not "human" or "computer".
     */
    void recordWord(const std::string& player, const std::string& word);

    /**
     * Sets whether the letter cube at the given row/column should be displayed
     * with a bright highlight color.  This is used when the human player is
     * searching for words on the game board.
     */
    void setHighlighted(int row, int col, bool highlighted = true, bool animate = true);

    /**
     * Sets the score to be the given number of points in the score label
     * for the specified player, which must be either "human" or "computer".
     * Throws an error if the player is not "human" or "computer".
     */
    void setScore(const std::string& player, int points);
};

#endif // _gui_h
