/*
 * CS 106B/X Boggle
 * This file implements functions you can use to interact with the Boggle GUI.
 * See gui.h for the declarations of each function.
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

#include "gui.h"
#include <iostream>
#include "boggleguiwindow.h"
#include "error.h"
#include "strlib.h"

namespace gui {
void clearHighlighting() {
    BoggleGuiWindow::instance()->clearHighlighting();
}

void initialize(int rows, int cols) {
    BoggleGuiWindow::instance()->initialize(rows, cols);
}


void labelCubes(const Grid<char>& letters) {
    BoggleGuiWindow::instance()->labelCubes(letters);
}

void labelCubes(const std::string& letters) {
    BoggleGuiWindow::instance()->labelCubes(letters);
}

void playSound(const std::string& filename) {
    BoggleGuiWindow::instance()->playSound(filename);
}

void recordWord(const std::string& player, const std::string& word) {
    std::string playerTrim = toLowerCase(trim(player));
    if (playerTrim == "human") {
        BoggleGuiWindow::instance()->recordWord(BoggleGuiWindow::HUMAN, word);
    } else if (playerTrim == "computer") {
        BoggleGuiWindow::instance()->recordWord(BoggleGuiWindow::COMPUTER, word);
    } else {
        error("Invalid player \"" + player + "\"; must be \"human\" or \"computer\"");
    }
}

void setHighlighted(int row, int col, bool highlighted, bool animate) {
    BoggleGuiWindow::instance()->setHighlighted(row, col, highlighted, animate);
}

void setScore(const std::string& player, int points) {
    std::string playerTrim = toLowerCase(trim(player));
    if (playerTrim == "human") {
        BoggleGuiWindow::instance()->setScore(BoggleGuiWindow::HUMAN, points);
    } else if (playerTrim == "computer") {
        BoggleGuiWindow::instance()->setScore(BoggleGuiWindow::COMPUTER, points);
    } else {
        error("Invalid player \"" + player + "\"; must be \"human\" or \"computer\"");
    }
}

} // namespace gui
