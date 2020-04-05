//
//  main.cpp
//  Project 1
//
//  Created by Jack Zhao on 1/9/20.
//  Copyright © 2020 Jack Zhao. All rights reserved.
//

#include <iostream>
#include "Game.h"

class Game;

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 5, 2);
    Game g(10, 12, 40);

      // Play the game
    g.play();
}
