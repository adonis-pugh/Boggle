/*
 * CS 106B Boggle
 * This file declares the prototypes for your required functions.
 *
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 *
 * (If you want to declare prototypes for your other functions,
 *  do that near the top of your .cpp file, not here.)
 *
 * @author Marty Stepp and Julie Zelenski
 * @version 2019/04/25
 * - initial 19sp version
 */

#ifndef _boggle_h
#define _boggle_h

#include <string>
#include "grid.h"
#include "lexicon.h"
#include "set.h"
#include "vector.h"
#include "boggleconstants.h"


// function prototype declarations
// (DO NOT CHANGE THESE HEADINGS; your functions must match EXACTLY)

Set<std::string> computerWordSearch(Grid<char>& board, Lexicon& dictionary,
                                    Set<std::string>& humanWords);
bool humanWordSearch(Grid<char>& board, std::string word);

// (DO NOT ADD OTHER FUNCTION PROTOTYPES HERE;
//  instead add any needed prototypes at the top of your .cpp file)


#endif // _boggle_h

/*
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 * !!! DO NOT MODIFY THIS FILE !!!
 */
