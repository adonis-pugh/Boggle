/*
 * CS 106B/X Boggle
 * This file declares a window that contains the graphical user interface for Boggle.
 * See boggleguiwindow.cpp for the implementation of each member.
 *
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * @author Marty Stepp and Julie Zelenski;
 *         previous modifications by Eric Roberts
 * @version 2019/04/30
 * - added Ctrl +/- hotkeys to scale GUI fonts
 * @version 2019/04/25
 * - initial 19sp version; added INTERACTIVE_GUI flag to allow for console
 *   to drive overall program UI if so desired
 * @version 2018/10/25
 * - additional decomp of human player handling code
 * @version 2018/10/20
 * - 18au 106B initial version
 * - refactored for new Qt-based GUI library subsystem
 * - refactored to make most behavior non-static, except for a few functions
 *   to be called by student (don't want to have to pass them a GUI reference)
 * @version 2016/10/25
 * - 16au initial version; heavily refactored into fully graphical UI
 */

#ifndef _boggleguiwindow_h
#define _boggleguiwindow_h

#include <string>
#include "boggleconstants.h"
#include "grid.h"
#include "lexicon.h"
#include "gevents.h"
#include "ginteractors.h"
#include "gobjects.h"
#include "gtypes.h"
#include "gwindow.h"
#include "vector.h"

class BoggleGuiWindow {
public:
    /**
     * This enumeration distinguishes the human and computer players.
     */
    enum Player { HUMAN, COMPUTER };

    /**
     * Returns the singleton GUI object.
     */
    static BoggleGuiWindow* instance();

    /**
     * Restores any highlighted cubes to their normal color.
     */
    void clearHighlighting();

    /**
     * Closes and discards the graphical window.
     * This differs from shutdown() in that you can call close() and then call
     * init() again to make a new window (perhaps of a different size)
     * while shutdown() totally closes down the Stanford C++ graphics subsystem.
     */
    void close();

    /**
     * Initializes the Boggle graphics package and its graphics window,
     * using the given number of rows and columns on the board.
     * If no number of rows or columns is passed, uses the default size of 4.
     * An error is raised if either dimension is <= BOARD_SIZE_MIN or > BOARD_SIZE_MAX.
     */
    void initialize(int rows = BOARD_SIZE, int cols = BOARD_SIZE);

    /**
     * Returns true if the Boggle GUI has already been initialized.
     */
    bool isInitialized() const;

    /**
     * Draws the specified letter on the face of the cube at position (row, col).
     * The cubes are numbered from top to bottom left to right starting with zero.
     * The upper left corner is is (0, 0); the lower right is (rowCount-1, columnCount-1).
     *
     * An error is raised if row or col is out of range for the board.
     */
    void labelCube(int row, int col, char letter);

    /**
     * Draws the specified letters on the face of all cubes of the board.
     * You can pass a 4x4 Grid, a 16-letter string,
     * or a string with line breaks after each four characters.
     * An error is raised if the string is not the right length.
     */
    void labelCubes(const Grid<char>& letters);
    void labelCubes(const std::string& letters);

    /**
     * Plays a sound effect located in the given file.
     */
    void playSound(const std::string& filename);

    /**
     * Records the specified word by adding it to the screen display for
     * the specified player.
     * If the given word has already been recorded, adds it again (so don't do that).
     */
    void recordWord(Player player, const std::string& word);

    /**
     * Sets the Boggle game to pause for the given number of milliseconds
     * after toggling the highlight on any letter cube.
     * If set to <= 0, there will be no delay.
     * The default is 100ms, as defined in DEFAULT_ANIMATION_DELAY below.
     */
    void setAnimationDelay(int ms);

    /**
     * Sets whether the letter cube at the given row/column should be displayed
     * with a bright highlight color.  This is used when the human player is
     * searching for words on the game board.
     */
    void setHighlighted(int row, int col, bool highlighted = true, bool animate = true);

    /**
     * Sets the score label for the given player to display the given
     * number of points.
     */
    void setScore(Player player, int score);

    /**
     * Closes the GUI window and frees all associated memory.
     */
    void shutdown();

private:
    /**
     * Creates a new GUI.  Private to prevent unwanted construction.
     */
    BoggleGuiWindow();

    /**
     * Frees up memory allocated by the GUI.
     */
    virtual ~BoggleGuiWindow();

    /**
     * Internal helper to compute sizes of various GUI shapes.
     */
    void calculateGeometry();

    /**
     * Number of columns in the game board.
     */
    int columnCount() const;

    /**
     * Number of total cubes in the game board.
     * Equal to rowCount * columnCount.
     */
    int cubeCount() const;

    /**
     * Internal helper to make sure GUI is initialized.
     * Throws an ErrorException if the GUI has not been initialized.
     */
    void ensureInitialized();

    /**
     * Clears the GUI's state, including all letters, highlighting, and scores.
     * After a call to reset(), the cubes will be blank, scores will show as 0,
     * and no words will be recorded.
     */
    void reset();

    /**
     * Number of rows in the game board.
     */
    int rowCount() const;

    /**
     * Updates fonts of all onscreen widgets in response to a font size change.
     */
    void scaleAllFonts(int amount);

    /**
     * Scales up/down the font to an appropriate size for the user's computer.
     */
    std::string scaleFont(const std::string& fontName, int rawFontSize);

    /**
     * Sets the Boggle board to be size x size.
     * Returns true if size is valid, false if invalid.
     */
    bool setBoardSize(int size);

    /**
     * Sets the Boggle GUI window to display the given text as a status message.
     */
    void setStatusMessage(const std::string& status, bool isError = false);

    /**
     * Internal helper to draw letter cubes as rounded rectangles.
     * Creates all cubes in position, including a blank letter centered in
     * the middle of each cube.  Initially the cubes are not highlighted.
     */
    void setupLetterCubes();

    /**
     * Internal helper to draw player labels.
     * Labels player word list with specified name and draws a line underneath.
     * Also sets up the score label for the specified player.
     */
    void setupPlayerLabels(Player player, const std::string& name);

    /*
     * Constants
     * ---------
     * To configure fixed spacing, fonts, etc.
     */
    static const bool BIG_BOGGLE_SUPPORT = false;           // true to ask board size and allow Big Boggle
    static const bool DOCK_WINDOWS = true;                  // if true, tries to arrange GUI window above console window on screen
    static const int BOGGLE_WINDOW_HEIGHT = 450;            // window size in px
    static const int BOGGLE_WINDOW_WIDTH  = 850;
    static const int DEFAULT_ANIMATION_DELAY = 100;         // delay in ms
    static const int FONT_SIZE = 20;                        // default font size for UI widgets
    static const int SCORE_FONT_SIZE = 18;
    static const int WORD_FONT_SIZE = 10;
    static const int FONT_SIZE_INCREMENT = 2;               // how much to change font size when Ctrl +/- are pressed

    static constexpr const double BOARD_BORDER = 8;         // width of border around the cubes
    static constexpr const double FONT_PERCENTAGE = 0.6;    // percentage of cube used for letter
    static constexpr const double HUMAN_PERCENTAGE = 0.38;  // percentage of space for word lists to apportion to human
    static constexpr const double INDENT = 20;              // indent at vertical edges of window
    static constexpr const double LABEL_DY = -5;            // adjustment for label text
    static constexpr const double LABEL_HEIGHT = 40;        // space at top of window for labels
    static constexpr const double WORD_DY = 5;              // adjustment for word list

    static const std::string BOGGLE_WINDOW_TITLE;
    static const std::string CUBE_FONT;           // font for letters on cube face
    static const std::string SCORE_FONT;          // font for scoreboard names & scores
    static const std::string WORD_FONT;

    // colors used on the GUI, as hex strings
    static const int BOARD_COLOR       = 0x326BF6;
    static const int DIE_COLOR         = 0xE6E6E6;
    static const int LETTER_COLOR      = 0x023B46;
    static const int DIE_COLOR_H       = 0xFFEE66;   // _H = highlighted colors
    static const int LETTER_COLOR_H    = 0x023BD6;
    static const int WORD_COLOR        = 0x02805D;
    static const int LABEL_COLOR       = 0x028040;
    static const int LABEL_COLOR_ERROR = 0xA22020;

    // (static) member variables

    /**
     * A static struct is used to gather the state that is local to this module.
     * Mostly this is geometry calculated to fit the cubes, word lists,
     * and labels neatly within the current size of the graphics window.
     * In addition, the module tracks the count of words listed for each player
     * (to place additional words in the correct location in the displayed word
     * list) and the scores for each player (which must be saved and erased
     * before updating). There is also a 2-d array of the letters currently
     * showing on the cubes, to enable drawing them inverted for the
     * highlighting function.
     *
     * We'd like to use GRectangle instead, but it doesn't support setter methods.
     */
    struct RectStruct {
        double x, y, w, h;
    };

    /**
     * Maintains information about a single letter cube on the screen.
     * Used so that the letter cube's color and letter can be updated without
     * needing to redraw them over the existing one.
     */
    struct CubeInfo {
        GRoundRect* rect;
        GText* label;
        CubeInfo() {
            rect = nullptr;
            label = nullptr;
        }
    };

    // pointer to the singleton GUI object
    static BoggleGuiWindow* _instance;

    // Global pointers to graphical objects such as the graphics window,
    // and collections of objects for letter cubes and words recorded.
    // These are used to update/clear the letters and words as the game goes on.
    GWindow* _window;
    GText* _humanScoreLabel;
    GText* _computerScoreLabel;
    GText* _statusLabel;
    Grid<CubeInfo> _letterCubes;
    Vector<GText*> _recordedWordLabels;
    int _animationDelay;

    int _totalScore[2];        // points scored by each player
    int _wordCount[2];         // words displayed for each player
    RectStruct _scoreBox[2];   // rectangle enscribing label + score for each player
    RectStruct _statusBox;     // rectangle enscribing current game status
    double _wordColumnWidth;   // width of word list columns
    double _cubeSize;          // size of cube
    double _cubeFontSize;      // font size used for labeling cubes
    double _fontSizeAdjust;    // font size adjustment based on Ctrl +/-
    RectStruct _boardRect;     // rectangle enscribed the cubes
};

#endif // _boggleguiwindow_h
