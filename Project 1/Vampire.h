//
//  Vampire.hpp
//  Project 1
//
//  Created by Jack Zhao on 1/9/20.
//  Copyright © 2020 Jack Zhao. All rights reserved.
//

#ifndef Vampire_h
#define Vampire_h

#include "globals.h"

#include <stdio.h>

class Arena;

class Vampire
{
  public:
      // Constructor
    Vampire(Arena* ap, int r, int c);

      // Accessors
    int  row() const;
    int  col() const;
    bool isDead() const;

      // Mutators
    void move();

  private:
    Arena* m_arena;
    int    m_row;
    int    m_col;
    int    m_health;
    int    m_idleTurnsRemaining;
};

#endif /* Vampire_hpp */
