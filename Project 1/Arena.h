//
//  Arena.hpp
//  Project 1
//
//  Created by Jack Zhao on 1/9/20.
//  Copyright © 2020 Jack Zhao. All rights reserved.
//



#ifndef Arena_h
#define Arena_h

#include "globals.h"
#include "History.h"
#include <string>
using namespace std;

class Vampire;
class Player;

class Arena
{
  public:
      // Constructor/destructor
    Arena(int nRows, int nCols);
    ~Arena();

      // Accessors
    int     rows() const;
    int     cols() const;
    Player* player() const;
    int     vampireCount() const;
    int     getCellStatus(int r, int c) const;
    int     numberOfVampiresAt(int r, int c) const;
    void    display(string msg) const;

      // Mutators
    void setCellStatus(int r, int c, int status);
    bool addVampire(int r, int c);
    bool addPlayer(int r, int c);
    void moveVampires();
    History& history();

  private:
    int      m_grid[MAXROWS][MAXCOLS];
    int      m_rows;
    int      m_cols;
    Player*  m_player;
    Vampire* m_vampires[MAXVAMPIRES];
    int      m_nVampires;
    int      m_turns;
    History h;
    
      // Helper functions
    void checkPos(int r, int c, string functionName) const;
    bool isPosInBounds(int r, int c) const;
};

#include <stdio.h>

#endif /* Arena_hpp */


