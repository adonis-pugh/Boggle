/*
 * CS 106B Boggle
 * This file defines values for constants used throughout the program.
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

#include "boggleconstants.h"

Vector<std::string> LETTER_CUBES {
    "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
    "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
    "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
    "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};

Vector<std::string> LETTER_CUBES_BIG {
    "AAAFRS", "AAEEEE", "AAFIRS", "ADENNN", "AEEEEM",
    "AEEGMU", "AEGMNN", "AFIRSY", "BJKQXZ", "CCNSTW",
    "CEIILT", "CEILPT", "CEIPST", "DDLNOR", "DDHNOT",
    "DHHLOR", "DHLNOR", "EIIITT", "EMOTTT", "ENSSSU",
    "FIPRSY", "GORRVW", "HIPRRY", "NOOTUW", "OOOTTU"
};

Vector<std::string> LETTER_CUBES_SUPER_BIG {
    "AAAFRS", "AAEEEE", "AAEEOO", "AAFIRS", "ABDEIO", "ADENNN",
    "AEEEEM", "AEEGMU", "AEGMNN", "AEILMN", "AEINOU", "AFIRSY",
    /* "AnErHeInQuTh" */ "AEHIQT", "BBJKXZ", "CCENST", "CDDLNN", "CEIITT", "CEIPST",
    "CFGNUY", "DDHNOT", "DHHLOR", "DHHNOW", "DHLNOR", "EHILRS",
    "EIILST", "EILPST", /* "EIO###" */ "EIOEIO", "EMTTTO", "ENSSSU", "GORRVW",
    "HIRSTV", "HOPRST", "IPRSYY", /* "JKQuWXZ" */ "JKQWXZ", "NOOTUW", "OOOTTU",
};
