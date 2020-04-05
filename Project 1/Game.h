//
//  Game.hpp
//  Project 1
//
//  Created by Jack Zhao on 1/9/20.
//  Copyright © 2020 Jack Zhao. All rights reserved.
//

#ifndef Game_h
#define Game_h

#include <stdio.h>
#include <string>
#include <iostream>
using namespace std;

class Arena;

class Game
{
  public:
      // Constructor/destructor
    Game(int rows, int cols, int nVampires);
    ~Game();

      // Mutators
    void play();

  private:
    Arena* m_arena;

      // Helper functions
    string takePlayerTurn();
};

#endif /* Game_hpp */
