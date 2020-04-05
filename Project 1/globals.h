//
//  Utilities.hpp
//  Project 1
//
//  Created by Jack Zhao on 1/9/20.
//  Copyright © 2020 Jack Zhao. All rights reserved.
//

#ifndef globals_h
#define globals_h

#include <stdio.h>

class Arena;

bool attemptMove(const Arena& a, int dir, int& r, int& c);
int computeDanger(const Arena& a, int r, int c);
int randInt(int lowest, int highest);
bool decodeDirection(char ch, int& dir);
bool recommendMove(const Arena& a, int r, int c, int& bestDir);
void clearScreen();

const int MAXROWS = 20;                // max number of rows in the arena
const int MAXCOLS = 20;                // max number of columns in the arena
const int MAXVAMPIRES = 100;           // max number of vampires allowed
const int INITIAL_VAMPIRE_HEALTH = 2;  // initial vampire health
const int POISONED_IDLE_TIME = 1;      // poisoned vampire idles this many turns
                                       //   between moves

const int NORTH = 0;
const int EAST  = 1;
const int SOUTH = 2;
const int WEST  = 3;
const int NUMDIRS = 4;

const int EMPTY      = 0;
const int HAS_POISON = 1;

#endif /* Utilities_hpp */
