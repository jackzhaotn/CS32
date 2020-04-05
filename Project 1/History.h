//
//  History.hpp
//  Project 1
//
//  Created by Jack Zhao on 1/9/20.
//  Copyright © 2020 Jack Zhao. All rights reserved.
//

#ifndef History_h
#define History_h

#include <stdio.h>
#include "globals.h"

class History
{
  public:
    History(int nRows, int nCols);
    bool record(int r, int c);
    void display() const;
  private:
    char m_historyGrid[MAXROWS][MAXCOLS];
    int m_rows;
    int m_cols;
};

#endif /* History_hpp */
