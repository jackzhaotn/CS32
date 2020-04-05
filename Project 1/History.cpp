//
//  History.cpp
//  Project 1
//
//  Created by Jack Zhao on 1/9/20.
//  Copyright © 2020 Jack Zhao. All rights reserved.
//

#include "History.h"
#include <iostream>
using namespace std;

History::History(int nRows, int nCols) : m_rows(nRows), m_cols(nCols)
{
    for (int r = 1; r <= m_rows; r++)
           for (int c = 1; c <= m_cols; c++)
               m_historyGrid[r-1][c-1] = '.';
}

bool History::record(int nRows, int nCols)
{
    if (nRows < 1 || nCols < 1 || nRows > m_rows || nCols > m_cols)
        return false;
    if (m_historyGrid[nRows-1][nCols-1] == '.')
    {
        m_historyGrid[nRows-1][nCols-1] = 'A';
        return true;
    }
    else if (m_historyGrid[nRows-1][nCols-1] == 'Z')
        return true;
    else
    {
        m_historyGrid[nRows-1][nCols-1]++;
        return true;
    }
}

void History::display() const
{
    clearScreen();
    for (int r = 1; r <= m_rows; r++)
    {
        for (int c = 1; c <= m_cols; c++)
            cout << m_historyGrid[r-1][c-1];
        cout << endl;
    }
    cout << endl;

    
}
