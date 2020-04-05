#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"


// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject
{
public:
    Actor(int ID, double startX, double startY, int startDir, int depth, StudentWorld* world);
    virtual ~Actor();
    //ACCESSOR FUNCTIONS
    virtual bool isAlive() const;
    virtual bool isFood() const;
    virtual bool isProj() const;
    virtual bool isPit() const;
    virtual bool isDirt() const;
    virtual bool takeDamage(int damage);
    virtual bool preventsLevelCompleting() const;
    //MUTATOR FUNCTIONS
    void die();
    //OTHER FUNCTIONS
    virtual void doSomething() = 0;
    StudentWorld* getAPointerToMyWorld() const;
private:
    StudentWorld* m_world;
    bool m_isAlive;
    bool m_isFood;
    bool m_isPit;
    bool m_isProj;
    bool m_isDirt;
    
};

class Agent : public Actor
{
public:
    Agent(int imageID, double startX, double startY, int dir, int hitPoints, StudentWorld* world);
    //ACCESSOR FUNCTIONS
    bool isDead();
    int getHp() const;
    //MUTATOR FUNCTIONS
    virtual bool takeDamage(int damage);
    void restoreHealth();
private:
    int m_hp;
    virtual int soundWhenHurt() const = 0;
    virtual int soundWhenDie() const = 0;
};

class Socrates : public Agent
{
public:
    Socrates(StudentWorld* world);
    virtual ~Socrates();
    virtual void doSomething();
    //ACCESSOR FUNCTIONS
    int getSpray();
    int getFlame();
    //MUTATOR FUNCTIONS
    void incHealth(int howMuch);
    void incFlame(int howMuch);
private:
    virtual int soundWhenHurt() const;
    virtual int soundWhenDie() const;
    int m_nSpray;
    int m_nFlamethrower;
    int m_health;
    int m_positionalDir;
};

class Dirt: public Actor
{
public:
    Dirt(double startX, double startY, StudentWorld* world);
    virtual ~Dirt();
    //ACCESSOR FUNCTIONS
    virtual bool takeDamage(int damage);
    virtual bool isDirt() const;
    
    virtual void doSomething();

private:
};

class Food: public Actor
{
public:
    Food(double startx, double startY, StudentWorld* world);
    virtual ~Food();
    //ACCESSOR FUNCTIONS
    virtual bool isFood() const;
    virtual void doSomething();
private:
};

class Projectile: public Actor
{
public:
    Projectile(int imageID, double startX, double startY, Direction startDir, int pixelsToTravel, int damage, StudentWorld* world);
    virtual ~Projectile();
    virtual void doSomething();
    //ACCESSOR FUNCTIONS
    virtual bool isProj() const;
private:
    int m_pixelsLeft;
    int m_damage;
    Direction m_dir;
};


class Spray: public Projectile
{
public:
    Spray(double startX, double startY, Direction startDir, StudentWorld* world);
    virtual ~Spray();
};

class Flame: public Projectile
{
public:
    Flame(double startX, double startY, Direction startDir, StudentWorld* world);
    virtual ~Flame();
};

class Goodies: public Actor
{
public:
    Goodies(int imageID, double startX, double startY, StudentWorld* world);
    virtual ~Goodies();
    //ACCESSOR FUNCTIONS
    virtual bool takeDamage(int damage);
    
    virtual void doSomething();
private:
    virtual void doDifferientiatedStuff() = 0;
    int m_timeLeft;
};

class HealthGoodie: public Goodies
{
public:
    HealthGoodie(double startX, double startY, StudentWorld* world);
    virtual ~HealthGoodie();
private:
    virtual void doDifferientiatedStuff();
};

class FlameGoodie: public Goodies
{
public:
    FlameGoodie(double startX, double startY, StudentWorld* world);
    virtual ~FlameGoodie();
private:
    virtual void doDifferientiatedStuff();
    
};

class ExtraLifeGoodie: public Goodies
{
public:
    ExtraLifeGoodie(double startX, double startY, StudentWorld* world);
    virtual ~ExtraLifeGoodie();
private:
    virtual void doDifferientiatedStuff();
};

class Fungus: public Goodies
{
public:
    Fungus(double startX, double startY, StudentWorld* world);
    virtual ~Fungus();
private:
    virtual void doDifferientiatedStuff();
};

//Pit
class Pit: public Actor
{
public:
    Pit(double startX, double startY, StudentWorld* world);
    virtual ~Pit();
    void doSomething();
    //ACCESSOR FUNCTIONS
    virtual bool isPit() const;
    virtual bool preventsLevelCompleting() const;
private:
    int m_nSamonella;
    int m_nAggroSamonella;
    int m_nEcoli;
};

class Bacterium: public Agent
{
public:
    Bacterium(int imageID, double startX, double startY, int hp, StudentWorld* world);
    virtual ~Bacterium();
    //ACCESSOR FUNCTIONS
    int movePlan();
    int food();
    virtual bool preventsLevelCompleting() const;
    //MUTATOR FUNCTIONS
    void incMovePlan(int howMuch);
    void setMovePlan(int num);
    void incFood(int howMuch);
private:
    virtual void multiply() = 0;
    int m_movePlan;
    int m_foodEaten;
};

class Samonella: public Bacterium
{
public:
    Samonella(double startX, double startY, int dps, StudentWorld* world);
    virtual ~Samonella();
private:
    virtual int soundWhenHurt() const;
    virtual int soundWhenDie() const;
};

class RegularSamonella: public Samonella
{
public:
    RegularSamonella(double startX, double startY, StudentWorld* world);
    virtual ~RegularSamonella();
    virtual void doSomething();
private:
    int m_dps;
    virtual void multiply();
};

class AggroSamonella: public Samonella
{
public:
    AggroSamonella(double startX, double startY, StudentWorld* world);
    virtual ~AggroSamonella();
    virtual void doSomething();
private:
    int m_dps;
    virtual void multiply();
};

class Ecoli: public Bacterium
{
public:
    Ecoli(double startX, double startY, StudentWorld* world);
    virtual ~Ecoli();
    virtual void doSomething();
private:
    int m_dps;
    virtual int soundWhenHurt() const;
    virtual int soundWhenDie() const;
    virtual void multiply();
};







#endif // ACTOR_H_
