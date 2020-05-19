/* BOGGLE
 * Author: Adonis Pugh

 * ----------------------------
 * This program implements the classic vocabulary-enhancing game "Boggle." Random boards are
 * typically generated, but manual board configurations are allowed as well. The user enters
 * words one by one that they suspect to be able to be formed on the board, and the CPU verifies
 * that the words are valid. After the user has found all the words they can, the CPU exhaustively
 * finds ALL the words in the English dictionary that the player missed. The key features of
 * this program are the recursrive algorithms for verifying user words and locating the remaining
 * words that can be formed on the board. */

#include "boggle.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include "console.h"
#include "filelib.h"
#include "grid.h"
#include "lexicon.h"
#include "random.h"
#include "set.h"
#include "shuffle.h"
#include "simpio.h"
#include "strlib.h"
#include "vector.h"
#include "gui.h"
using namespace std;

/*************************************************
 *             PROTOTYPE FUNCTIONS               *
 ************************************************/
void intro();
void promptBoard(Grid<char>& board);
void generateRandomBoard(Grid<char>& board);
void generateManualBoard(Grid<char>& board);
string getWord(Lexicon& dictionary);
bool inBounds(int row, int col);
int getPoints(string word);
Set<string> humanTurn(Grid<char>& board, Lexicon& dictionary, int humanScore);
void computerTurn(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords, int humanScore);
bool humanWordSearch(Grid<char>& board, string word);
Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords);
bool searchForWord(Grid<char> board, string word, string potentialWord, int row, int col);
Set<string> exhaustiveSearch(Grid<char> board, Lexicon& dictionary, Set<string>& humanWords,
                             string potentialWord, int row, int col);


/*************************************************
 *                  FUNCTIONS                    *
 ************************************************/

int main() {
    Grid<char> board(BOARD_SIZE, BOARD_SIZE);
    Lexicon dictionary(DICTIONARY_FILE);
    intro();
    do {
        gui::initialize(BOARD_SIZE, BOARD_SIZE);
        cout << endl;
        promptBoard(board);
        int humanScore = 0;
        Set<string> humanWords = humanTurn(board, dictionary, humanScore);
        computerTurn(board, dictionary, humanWords, humanScore);
    } while (getYesOrNo("Play again? "));
    cout << "Have a nice day." << endl;
    return 0;
}

/* Prints a welcome message that introduces the program to the user.*/
void intro() {
    cout << "Welcome to CS 106B Boggle!" << endl;
    cout << "This game is a search for words on a 2-D board of letter cubes." << endl;
    cout << "The good news is that you might improve your vocabulary a bit." << endl;
    cout << "The bad news is that you're probably going to lose miserably to" << endl;
    cout << "this little dictionary-toting hunk of silicon." << endl;
    cout << "If only YOU had 16 gigs of RAM!" << endl;
    cout << endl;
    getLine("Press Enter to begin the game ...");
}

/* Prompts the user to have a random board generated or to enter a manual configuration. */
void promptBoard(Grid<char>& board) {
    if(getYesOrNo("Generate a random board? ")) {
        generateRandomBoard(board);
    } else {
        generateManualBoard(board);
    }
}

/* A random board layout is generated from the fixed cubes and board size. */
void generateRandomBoard(Grid<char>& board) {
    Vector<string> cubes = LETTER_CUBES;
    shuffle(cubes);
    int cubeCounter = 0;
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            string cube = cubes[cubeCounter];
            cube = shuffle(cube);
            board[i][j] = cube[0];
            cubeCounter++;
            cout << cube[0];
        }
        cout << endl;
    }
    cout << endl;
    gui::labelCubes(board);
}

/* A manual board configuration is accepted from the user and used as the game board. */
void generateManualBoard(Grid<char>& board) {
    string choices = getLine("Type the " + integerToString(NUM_CUBES) + " letters on the board: ");
    while(choices.length() != NUM_CUBES) {
        cout << "Invalid board string. Try again." << endl;;
        choices = getLine("Type the " + integerToString(NUM_CUBES) + " letters on the board: ");
    }
    int cubeCounter = 0;
    for(int i = 0; i < BOARD_SIZE; i++) {
        for(int j = 0; j < BOARD_SIZE; j++) {
            board[i][j] = toUpperCase(choices[cubeCounter]);
            cubeCounter++;
            cout << board[i][j];
        }
        cout << endl;
    }
    cout << endl;
    gui::labelCubes(toUpperCase(choices));
}

/* Each string the user enters is checked to make sure it is in the English dictionary and
 * meets requirements for minimum word length. */
string getWord(Lexicon& dictionary) {
    string word = toUpperCase(getLine("Type a word (or Enter to stop): "));
    while((word.length() < MIN_WORD_LENGTH && word != "") ||
          (!dictionary.contains(word) && word != "")) {
        if(word.length() < MIN_WORD_LENGTH && word != "") {
            cout << "The word must have at least " << MIN_WORD_LENGTH << " letters." << endl;
            word = getLine("Type a word (or Enter to stop): ");
        }
        if(!dictionary.contains(word) && word != "") {
            cout << "That word is not found in the dictionary." << endl;
            word = getLine("Type a word (or Enter to stop): ");
        }
    }
    return toUpperCase(word);
}

/* This fuction outputs a score based on the length of an input word. */
int getPoints(string word) {
    if(word.length() == 4) {
        return 1;
    }
    if(word.length() == 5) {
        return 2;
    }
    if(word.length() == 6) {
        return 3;
    }
    if(word.length() == 7) {
        return 5;
    }
    if(word.length() > 7) {
        return 11;
    }
    return 0;
}

/* This function returns true if the input row/column are in the bounds of the board. */
bool inBounds(int row, int col) {
    return row >= 0 && row < BOARD_SIZE && col >= 0 && col < BOARD_SIZE;
}

/* The user is allowed to enter words which are verified by the word search algorithm.
 * The user is notified and reprompted if the word cannot be formed on the board. The
 * words they find are displayed to the GUI along with their tallied score. */
Set<string> humanTurn(Grid<char>& board, Lexicon& dictionary, int humanScore) {
    Set<string> wordList;
    cout << "It's your turn!" << endl;
    string word = " ";
    while(word != "") {
        gui::clearHighlighting();
        cout << "Your words: " << wordList << endl;
        cout << "Your score: " << humanScore << endl;
        word = getWord(dictionary);
        if(wordList.contains(word)) {
            cout << "You have already found that word." << endl;
        } else if(humanWordSearch(board, word)) {
            cout << "You found a new word! \"" << word << "\"" << endl << endl;
            wordList.add(word);
            humanScore += getPoints(word);
            gui::setScore("human", humanScore);
            gui::recordWord("human", word);
        } else if (word != ""){
            cout << "That word can't be formed on this board." << endl;
        }
    }
    cout << endl;
    return wordList;
}

/* This function scans each row and column pair to see if the char mathces the
 * first letter of the user's input word. If a valid row/column pair is found,
 * the word search begins. */
bool humanWordSearch(Grid<char>& board, string word) {
    for(int row = 0; row < BOARD_SIZE; row++) {
        for(int col = 0; col < BOARD_SIZE; col++) {
            char start = board[row][col];
            if(start == word[0]) {
                if(searchForWord(board, word, charToString(start), row, col)) {
                    return true;
                }
            }
        }
    }
    return false;
}

/* The word search algorithm starts from a position on the board where the char matches
 * the first letter of the user input word. From there, it investigates all adjcaent
 * chars. If the current/adjacent char pair is the prefix for the user's word, the algorithm
 * continues its search. If not, the algorithm terminates that search path. If all paths
 * are explored and the word is not found, the function returns false. */
bool searchForWord(Grid<char> board, string word, string potentialWord, int row, int col) {
    gui::setHighlighted(row, col);
    board[row][col] = integerToChar(0); // ensures letters are used only once
    pause(400);
    if(potentialWord == word) {
        return true;
    } else {
        for(int i = -1; i <= 1; i++) {
            for(int j = -1; j <= 1; j++) {
                if(inBounds(row + i, col + j)) {
                    string searchWord = potentialWord + board[row + i][col + j];
                    if(startsWith(word, searchWord)) {
                        if(searchForWord(board, word, searchWord, row + i, col + j)) {
                            return true;
                        }
                    }
                }
            }
        }
    }
    gui::clearHighlighting();
    return false;
}

/* The CPU undergoes an exhaustive search of words that can be formed from the board
 * that the user had not found. After the CPU word search is completed, the collection
 * of words it found is displayed to the GUI along with its score. */
void computerTurn(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords, int humanScore) {
    cout << "It's my turn!" << endl;
    int computerScore = 0;
    Set<string> computerWords = computerWordSearch(board, dictionary, humanWords);
    cout << "My words: " << computerWords << endl;
    for(string word : computerWords) {
        gui::recordWord("computer", word);
        computerScore += getPoints(word);
    }
    gui::setScore("computer", computerScore);
    cout << "My score: " << computerScore << endl;
    if(computerScore > humanScore) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!" << endl;
    } else if(computerScore < humanScore) {
        cout << "WOW, you defeated me! Congratulations!" << endl;
    } else {
        cout << "It's a draw. You should play again!" << endl;
    }
    cout << endl;
}

/* The CPU word search is initiated at each row/column pair. */
Set<string> computerWordSearch(Grid<char>& board, Lexicon& dictionary, Set<string>& humanWords) {
    Set<string> words;
    for(int row = 0; row < BOARD_SIZE; row++) {
        for(int col = 0; col < BOARD_SIZE; col++) {
            string start = charToString(board[row][col]);
            words += exhaustiveSearch(board, dictionary, humanWords, start, row, col);
        }
    }
    return words;
}

/* From a start row/column pair, the CUP investigates all the adjacent chars. If the current/
 * adjacent pair is the prefix of a word in the English dictionary, the search is continued.
 * After a word as been found, the same word is checked to be a prefix for another word. If
 * that is the case, the search continues further. In this way, all possible words are found.
 * The words the user discovered are not included in the collection returned by this function. */
Set<string> exhaustiveSearch(Grid<char> board, Lexicon& dictionary, Set<string>& humanWords,
                             string potentialWord, int row, int col) {
    Set<string> foundWords;
    board[row][col] = integerToChar(0); // ensures letters are used only once
    if(dictionary.contains(potentialWord) && potentialWord.length() >= MIN_WORD_LENGTH &&
             !humanWords.contains(potentialWord)) {
        foundWords += potentialWord;
    }
    if (dictionary.containsPrefix(potentialWord)) {
        for(int i = -1; i <= 1; i++) {
            for(int j = -1; j <= 1; j++) {
                if(inBounds(row + i, col + j) && board[row + i][col + j] != integerToChar(0)) {
                    string searchWord = potentialWord + board[row + i][col + j];
                    if(dictionary.containsPrefix(searchWord)) {
                        foundWords += exhaustiveSearch(board, dictionary, humanWords,
                                                      searchWord, row + i, col + j);
                    }
                }
            }
        }
    }
    return foundWords;
}
