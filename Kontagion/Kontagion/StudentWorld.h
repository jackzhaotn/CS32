#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;
class Socrates;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    virtual ~StudentWorld();
    //MAIN GAME FUNCTIONS
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    void killDead();
    //ACCESSOR FUNCTION
    bool getOverlappingSocrates(Actor* a, Actor* &player);
    //MUTATOR FUNCTIONS
    void addActor(Actor* a);
    void addGoodies();
    void playerRestoreHealth();
    void playerIncHealth(int health);
    void playerIncFlame(int flame);
    //FUNCTIONS TO CHECK OVERLAP
    bool overlapCheck(Actor* a, double x, double y);
    bool projOverlap(Actor* a, int damage);
    bool socrOverlap(Actor* a);
    bool foodOverlap(Actor* a);
    bool dirtOverlap(Actor* a);
    double checkDistanceBetween(Actor* a, Actor* b) const;
    bool closetFood(Actor* a, int &direction, double searchRadius);
    bool getAngleToPlayer(Actor* a, int dist, int& angle) const;
    

private:
    Socrates* m_player;
    vector<Actor*> m_actors;
};

#endif // STUDENTWORLD_H_
