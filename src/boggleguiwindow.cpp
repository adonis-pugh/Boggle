/*
 * CS 106B/X Boggle
 * This file implements a window that contains the graphical user interface for Boggle.
 * See boggleguiwindow.h for the declaraction of the class and its members.
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
 * @version 2019/04/26
 * - bug fix for WORD_FONT constant declaration on Windows
 * @version 2019/04/25
 * - initial 19sp version; added INTERACTIVE_GUI flag to allow for console
 *   to drive overall program UI if so desired
 * @version 2018/10/25
 * - added threading to improve animation responsiveness on Mac systems
 * - additional decomp of human player handling code
 * @version 2018/10/22
 * - added repaint() calls to ask___() methods to remove stale GUI appearance
 * @version 2018/10/20
 * - 18au 106B initial version
 * @version 2016/10/25
 * - 16au initial version; heavily refactored into fully graphical UI
 *
 * Known open issue: Leaks memory for all GObjects created.  Oops.
 */

#include "boggleguiwindow.h"
#include "error.h"
#include "filelib.h"
#include "gconsolewindow.h"
#include "gevents.h"
#include "gfont.h"
#include "gthread.h"
#include "sound.h"
#include "strlib.h"
#include "boggle.h"

// declare/initialize static variables and constants
const std::string BoggleGuiWindow::BOGGLE_WINDOW_TITLE = "CS 106B Boggle";
const std::string BoggleGuiWindow::CUBE_FONT = "Helvetica-Bold";
const std::string BoggleGuiWindow::SCORE_FONT = "Helvetica";
#ifdef _WIN32
const std::string BoggleGuiWindow::WORD_FONT = "Arial";
#else
const std::string BoggleGuiWindow::WORD_FONT = "Geneva";
#endif

BoggleGuiWindow* BoggleGuiWindow::_instance = nullptr;

// declare/initialize member variables
BoggleGuiWindow::BoggleGuiWindow()
        : _window(nullptr),
          _humanScoreLabel(nullptr),
          _computerScoreLabel(nullptr),
          _statusLabel(nullptr),
          _animationDelay(DEFAULT_ANIMATION_DELAY),
          _totalScore {0, 0},
          _wordCount {0, 0},
          _scoreBox {{0, 0, 0, 0}, {0, 0, 0, 0}},
          _statusBox {0, 0, 0, 0},
          _wordColumnWidth(0),
          _cubeSize(0),
          _cubeFontSize(0),
          _fontSizeAdjust(0),
          _boardRect {0, 0, 0, 0} {
    // empty
}

BoggleGuiWindow::~BoggleGuiWindow() {
    // TODO: delete
    _window = nullptr;
    _humanScoreLabel = nullptr;
    _computerScoreLabel = nullptr;
    _statusLabel = nullptr;
}

/*static*/ BoggleGuiWindow* BoggleGuiWindow::instance() {
    if (!_instance) {
        _instance = new BoggleGuiWindow();
    }
    return _instance;
}

/*
 * This internal helper does all the messy math to work out how to divide
 * up the space within the current graphics window to neatly fit the board,
 * the cubes, and the word lists.
 * This function does not actually add any GObjects to the GWindow;
 * but it does set the state of the ' structure to be used later.
 */
void BoggleGuiWindow::calculateGeometry() {
//    std::cout << "calculateGeometry: width = " << _window->getWidth()
//              << ", height = " << _window->getHeight()
//              << ", canvas width = " << _window->getCanvasWidth()
//              << ", canvas height = " << _window->getCanvasHeight() << std::endl;
    double boardSize = std::min(_window->getWidth() / 3 - 2 * INDENT,
                                _window->getHeight() - 2 * LABEL_HEIGHT);
    _cubeSize = std::min((boardSize - BOARD_BORDER) / rowCount(),
                        (boardSize - BOARD_BORDER) / columnCount());
    _boardRect.w = _boardRect.h = boardSize;
    _boardRect.y = LABEL_HEIGHT + LABEL_HEIGHT;
    double leftover = _window->getWidth() - _boardRect.w - 2 * INDENT;
    _scoreBox[HUMAN].x = INDENT;
    _scoreBox[HUMAN].y = _scoreBox[COMPUTER].y = _boardRect.y;
    _scoreBox[HUMAN].h = _scoreBox[COMPUTER].h = LABEL_HEIGHT;
    _scoreBox[HUMAN].w = leftover * HUMAN_PERCENTAGE;
    _boardRect.x = _scoreBox[HUMAN].x + _scoreBox[HUMAN].w + INDENT;
    _scoreBox[COMPUTER].x = _boardRect.x + _boardRect.w + INDENT;
    _scoreBox[COMPUTER].w = _window->getWidth() - _scoreBox[COMPUTER].x - INDENT;
    _statusBox.x = INDENT;
    _statusBox.y = LABEL_HEIGHT;
    _statusBox.w = _window->getWidth() - 2 * INDENT;
    _statusBox.h = LABEL_HEIGHT;

    _cubeFontSize = _cubeSize / 2 + _fontSizeAdjust;
    _wordColumnWidth = _cubeSize * 1.45;
}

void BoggleGuiWindow::clearHighlighting() {
    ensureInitialized();
    for (int row = 0; row < rowCount(); row++) {
        for (int col = 0; col < columnCount(); col++) {
            setHighlighted(row, col, false, /* animate */ false);
        }
    }
}

void BoggleGuiWindow::close() {
    if (isInitialized()) {
        _window->setVisible(false);
        _window->close();
        delete _window;
        _window = nullptr;
        _humanScoreLabel = nullptr;
        _computerScoreLabel = nullptr;
        _statusLabel = nullptr;
        _letterCubes.resize(0, 0);
        _recordedWordLabels.clear();
    }
}

int BoggleGuiWindow::columnCount() const {
    return _letterCubes.numCols();
}

int BoggleGuiWindow::cubeCount() const {
    return rowCount() * columnCount();
}

void BoggleGuiWindow::ensureInitialized() {
    if (!isInitialized()) {
        error("BoggleGUI has not been initialized yet");
    }
}

void BoggleGuiWindow::initialize(int rows, int cols) {
    if (rows < BOARD_SIZE_MIN || rows > BOARD_SIZE_MAX
            || cols < BOARD_SIZE_MIN || cols > BOARD_SIZE_MAX) {
        error("BoggleGuiWindow::initialize: invalid dimensions: "
              + std::to_string(rows) + "x" + std::to_string(cols));
    }

    _letterCubes.resize(rows, cols);

    if (!_window) {
        // create entire GUI
        _window = new GWindow(BOGGLE_WINDOW_WIDTH, BOGGLE_WINDOW_HEIGHT, /* visible */ false);
        _window->setWindowTitle(BOGGLE_WINDOW_TITLE);
        _window->setResizable(true);
        _window->setExitOnClose(true);

        _window->setKeyListener([this](GEvent event) {
            if (event.getEventType() != KEY_PRESSED || !event.isCtrlOrCommandKeyDown()) {
                return;
            }
            // std::cout << "event code=" << event.getKeyCode() << ", char=" << event.getKeyChar() << std::endl;
            if (event.getKeyChar() == '=' || event.getKeyChar() == '+') {
                // reduce font size
                _fontSizeAdjust += FONT_SIZE_INCREMENT;
                scaleAllFonts(FONT_SIZE_INCREMENT);
            } else if (event.getKeyChar() == '-' || event.getKeyChar() == '_') {
                // increase font size
                _fontSizeAdjust -= FONT_SIZE_INCREMENT;
                scaleAllFonts(-FONT_SIZE_INCREMENT);
            }
        });
    }

    calculateGeometry();

    _wordCount[HUMAN] = 0;
    _wordCount[COMPUTER] = 0;

    // draw the Boggle board's blue background
    GRect* boardBackground = new GRect(_boardRect.x, _boardRect.y, _boardRect.w, _boardRect.h);
    boardBackground->setColor(BOARD_COLOR);
    boardBackground->setFilled(true);
    boardBackground->setFillColor(BOARD_COLOR);
    _window->add(boardBackground);

    if (!_humanScoreLabel || _computerScoreLabel) {
        // draw the labels of player names and score labels
        setupPlayerLabels(HUMAN, "Human");
        setupPlayerLabels(COMPUTER, "Computer");
    }

    if (!_statusLabel) {
        // set up status label
        _statusLabel = new GText("");
        _statusLabel->setFont(scaleFont(SCORE_FONT, SCORE_FONT_SIZE));
        _statusLabel->setColor(LABEL_COLOR);
        _window->add(_statusLabel, _statusBox.x, _statusBox.y);
    }

    // draw the NxN grid of Boggle letter cubes
    setupLetterCubes();

    // clear out any in-progress game state
    reset();

    if (!_window->isVisible() && DOCK_WINDOWS) {
        // put GUI to left of console
        GConsoleWindow* console = GConsoleWindow::instance();
        if (console) {
            console->setSize(850, 350);
            GDimension screen = GWindow::getScreenSize();
            console->setLocation((screen.getWidth() - console->getWidth()) / 2,
                                 (screen.getHeight() + console->getHeight() / 2) / 2);
            _window->setLocation(console->getX(),
                                 console->getY() - _window->getHeight() - 2 * INDENT);
            _window->setVisible(true);

            // put visibility / keyboard focus back on the console
            console->toFront();
            console->requestFocus();
        }
    }
}

bool BoggleGuiWindow::isInitialized() const {
    return _window != nullptr;
}

void BoggleGuiWindow::labelCubes(const Grid<char>& letters) {
    ensureInitialized();
    if (letters.size() != cubeCount()
            || letters.numRows() != rowCount()
            || letters.numCols() != columnCount()) {
        error("BoggleGuiWindow::labelCubes: incorrectly sized grid; expected "
              + std::to_string(rowCount()) + "x" + std::to_string(columnCount())
              + " but saw " + std::to_string(letters.numRows()) + "x" + std::to_string(letters.numCols()));
    }
    std::string boardText = letters.toString2D("", "", "", "");
    boardText = stringReplace(boardText, "'", "");
    labelCubes(boardText);
}

void BoggleGuiWindow::labelCubes(const std::string& letters) {
    ensureInitialized();
    if (letters.length() < cubeCount()) {
        error("BoggleGuiWindow::labelCubes: too few letters in string: \"" + letters + "\""
              "; need " + std::to_string(cubeCount()) + " letters");
    }
    int row = 0;
    int col = 0;
    bool labeledAll = false;
    for (int i = 0; i < letters.length(); i++) {
        char letter = letters[i];
        if (!isalpha(letter)) {
            continue;
        }
        labelCube(row, col, letter);
        col++;
        if (col == columnCount()) {
            col = 0;
            row++;
            if (row == rowCount()) {
                labeledAll = true;
                break;
            }
        }
    }
    if (!labeledAll) {
        error("BoggleGuiWindow::labelAllCubes: didn't label every cube from letters string: \"" + letters + "\"");
    }
}

void BoggleGuiWindow::labelCube(int row, int col, char letter) {
    ensureInitialized();
    if (!_letterCubes.inBounds(row, col)) {
        error("BoggleGuiWindow::labelCube: invalid row or column");
    }
    if (!isalpha(letter) && letter != ' ') {
        error("BoggleGuiWindow::labelCube: invalid non-alphabetic character");
    }

    GText* label = _letterCubes[row][col].label;
    label->setText(std::string(1, letter));
    if (isalpha(letter)) {
        label->setLocation(
                _letterCubes[row][col].rect->getX() + _cubeSize/2 - 0.5 * label->getWidth(),
                _letterCubes[row][col].rect->getY() + _cubeSize/2 + 0.36 * _cubeFontSize);
    }
}

void BoggleGuiWindow::playSound(const std::string& filename) {
    Sound s(filename);
    s.play();
}

void BoggleGuiWindow::recordWord(Player player, const std::string& word) {
    ensureInitialized();
    if (player != HUMAN && player != COMPUTER) {
        error("BoggleGuiWindow::recordWord: invalid player argument");
    }
    GText* label = new GText(toLowerCase(trim(word)));
    label->setFont(scaleFont(WORD_FONT, WORD_FONT_SIZE));
    label->setColor(WORD_COLOR);
    int wordsPerRow = int((_scoreBox[player].w + 50) / _wordColumnWidth);
    int row = _wordCount[player] / wordsPerRow;
    int col = _wordCount[player] % wordsPerRow;
    _wordCount[player]++;
    int rowHeight = WORD_FONT_SIZE + 3;
    _window->add(label, _scoreBox[player].x + col * _wordColumnWidth,
                  _scoreBox[player].y + (row + 1) * rowHeight + WORD_DY);
    _recordedWordLabels.add(label);
}

void BoggleGuiWindow::reset() {
    ensureInitialized();

    // clear out the counts of words recorded so far and scores
    setStatusMessage("Initializing ...");
    _window->setRepaintImmediately(false);
    _wordCount[HUMAN] = 0;
    _wordCount[COMPUTER] = 0;
    setScore(HUMAN, 0);
    setScore(COMPUTER, 0);

    // clear the word cubes
    for (int row = 0; row < rowCount(); row++) {
        for (int col = 0; col < columnCount(); col++) {
            labelCube(row, col, ' ');
        }
    }

    // clear out the words found by each player
    for (GText* label : _recordedWordLabels) {
        _window->remove(label);
    }
    _recordedWordLabels.clear();
    _window->setRepaintImmediately(true);
    _window->repaint();
    setStatusMessage("");
}

int BoggleGuiWindow::rowCount() const {
    return _letterCubes.numRows();
}

void BoggleGuiWindow::scaleAllFonts(int amount) {
    _window->setAutoRepaint(false);   // delay repaint until all labels are updated
    for (int i = 0; i < _window->getCanvas()->getElementCount(); i++) {
        GObject* obj = _window->getCanvas()->getElement(i);
        if (obj->getType() == "GText" || obj->getType() == "GString") {
            GText* label = reinterpret_cast<GText*>(obj);
            QFont qfont = GFont::toQFont(label->getFont());
            qfont = GFont::changeFontSize(qfont, amount);
            label->setFont(qfont);
        }
    }
    _window->setAutoRepaint(true);
    _window->repaint();
}

std::string BoggleGuiWindow::scaleFont(const std::string& fontName, int rawFontSize) {
    // TODO
    return fontName + "-" + integerToString(rawFontSize);
}

void BoggleGuiWindow::setAnimationDelay(int ms) {
    _animationDelay = ms;
}

bool BoggleGuiWindow::setBoardSize(int size) {
    if (size < BOARD_SIZE_MIN || size > BOARD_SIZE_MAX) {
        return false;
    }
    initialize(size, size);
    setStatusMessage("");
    return true;
}

void BoggleGuiWindow::setHighlighted(int row, int col, bool highlighted, bool animate) {
    ensureInitialized();
    if (!_letterCubes.inBounds(row, col)) {
        error("BoggleGuiWindow::setHighlighted: invalid row or column");
    }
    if (GConsoleWindow::instance()
            && GConsoleWindow::instance()->isVisible()
            && GConsoleWindow::instance()->hasInputScript()) {
        // optimization: skip highlighting if we are running an input
        // such as those run with Ctrl+1, etc.
        return;
    }

    _letterCubes[row][col].rect->setFillColor(highlighted ? DIE_COLOR_H : DIE_COLOR);
    _letterCubes[row][col].label->setColor(highlighted ? LETTER_COLOR_H : LETTER_COLOR);
    if (animate && highlighted && _animationDelay > 0) {
        _window->pause(_animationDelay);
    }
}

void BoggleGuiWindow::setScore(Player player, int points) {
    ensureInitialized();
    _totalScore[player] = points;
    GText* scoreLabel = player == COMPUTER ? _computerScoreLabel : _humanScoreLabel;
    scoreLabel->setText(integerToString(points));
}

void BoggleGuiWindow::setStatusMessage(const std::string& status, bool isError) {
    ensureInitialized();
    _statusLabel->setLabel(status);
    _statusLabel->setColor(isError ? LABEL_COLOR_ERROR : LABEL_COLOR);
}

void BoggleGuiWindow::setupLetterCubes() {
    double lineWidth = 2;
    double cubeSize = BoggleGuiWindow::_cubeSize - lineWidth;
    double cubeY = _boardRect.y + BOARD_BORDER / 2;

    // if (cubeCount() != letterCubes.size()) {
        // clear out any old rounded rectangles from the screen
        for (int row = 0; row < _letterCubes.numRows(); row++) {
            for (int col = 0; col < _letterCubes.numCols(); col++) {
                if (_letterCubes[row][col].rect) {
                    _window->remove(_letterCubes[row][col].rect);
                    _window->remove(_letterCubes[row][col].label);
                    delete _letterCubes[row][col].rect;
                    delete _letterCubes[row][col].label;
                    _letterCubes[row][col].rect = nullptr;
                    _letterCubes[row][col].label = nullptr;
                }
            }
        }
    // }

    _letterCubes.resize(rowCount(), columnCount());
    for (int row = 0; row < _letterCubes.numRows(); row++) {
        double cubeX = _boardRect.x + BOARD_BORDER / 2;
        for (int col = 0; col < _letterCubes.numCols(); col++) {
            // display the letter cubes as rounded rectangles
            double cubeRoundRadius = cubeSize / 6;
            GRoundRect* rect = new GRoundRect(cubeX, cubeY, cubeSize,
                                              cubeSize, cubeRoundRadius * 2);
            rect->setLineWidth(lineWidth);
            rect->setColor("Black");
            rect->setFilled(true);
            rect->setFillColor(DIE_COLOR);
            _window->add(rect);
            _letterCubes[row][col].rect = rect;

            // draw the letter on the cube
            GText* label = new GText("M");   // start as "M" for getWidth
            label->setFont(scaleFont(CUBE_FONT, int(_cubeFontSize)));
            label->setColor(LETTER_COLOR);
            label->setLocation(cubeX + cubeSize / 2 - label->getWidth() / 2,
                               cubeY + cubeSize / 2 + 0.4 * _cubeFontSize);
            label->setLabel(" ");
            _window->add(label);
            _letterCubes[row][col].label = label;
            cubeX += cubeSize + lineWidth;
        }
        cubeY += cubeSize + lineWidth;
    }
}

void BoggleGuiWindow::setupPlayerLabels(Player player, const std::string& name) {
    GLine* playerLine = new GLine(_scoreBox[player].x, _scoreBox[player].y,
            _scoreBox[player].x + _scoreBox[player].w,
            _scoreBox[player].y);
    playerLine->setColor(LABEL_COLOR);
    _window->add(playerLine);

    GText* label = new GText(name);
    label->setFont(scaleFont(SCORE_FONT, SCORE_FONT_SIZE + _fontSizeAdjust));
    _window->add(label, _scoreBox[player].x, _scoreBox[player].y + LABEL_DY);

    GText*& scoreLabel = player == COMPUTER ? _computerScoreLabel : _humanScoreLabel;
    if (scoreLabel) {
        scoreLabel->setText("0");
    } else {
        scoreLabel = new GText("0");
    }
    scoreLabel->setFont(scaleFont(SCORE_FONT, SCORE_FONT_SIZE + _fontSizeAdjust));
    scoreLabel->setColor(LABEL_COLOR);
    int offset = 32;
    _window->add(scoreLabel,
                _scoreBox[player].x + _scoreBox[player].w - offset,
                _scoreBox[player].y + LABEL_DY);
}

void BoggleGuiWindow::shutdown() {
    if (isInitialized()) {
        close();
    }
}
