#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <cmath>
#include <sstream>

using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{}

StudentWorld:: ~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    //Player
    m_player = new Socrates(this);
    
    int nPits = getLevel(); //n pits for n level
    int i = 0;

    while (i != nPits) //for every pit that needs to be init
    {
        if (m_actors.empty()) //first pit can be spawned anywhere in the circular arena
        {
            double rads = randInt(0,360)*3.14159/180;
            double radius = randInt(0, 120);
            //polar coordinate randomization
            Pit* p = new Pit(VIEW_WIDTH/2 + radius * cos(rads), VIEW_HEIGHT/2 + radius * sin(rads), this);
            addActor(p);
            i++;
        } else {
            //
            double rads = randInt(0,360)*3.14159/180;
            double radius = randInt(0, 120);
            
            vector<Actor*>:: iterator it;
            it = m_actors.begin();
            
            bool validLoc = true;
            //iterates through m_actors to make sure coords dont overlap with another pit
            while (it != m_actors.end())
            {
                if (overlapCheck((*it), VIEW_WIDTH/2 + radius * cos(rads), VIEW_HEIGHT/2 + radius * sin(rads)))
                    validLoc = false;
                it++;
            }
            if (validLoc) //if not init Pit, else loop again with new coords
            {
                Pit* p = new Pit(VIEW_WIDTH/2 + radius * cos(rads), VIEW_HEIGHT/2 + radius * sin(rads), this);
                addActor(p);
                i++;
            }
        }
    }
    
    //Food
    int nFood = min(5 * getLevel(), 25);
    int j = 0;
    while (j != nFood)
    {
        double rads = randInt(0,360)*3.14159/180;
        double radius = randInt(0, 120);
        
        vector<Actor*>:: iterator it;
        it = m_actors.begin();
        
        bool validLoc = true;
        //iterates to make sure ocords dont overlap with another pit or food
        while (it != m_actors.end())
        {
            if (((*it)->isPit() || (*it)->isFood()) && overlapCheck((*it), VIEW_WIDTH/2 + radius * cos(rads), VIEW_HEIGHT/2 + radius * sin(rads)))
                validLoc = false;
            it++;
        }
        if (validLoc) //if valid init food, else loops over again
        {
            Food* d = new Food(VIEW_WIDTH/2 + radius * cos(rads), VIEW_HEIGHT/2 + radius * sin(rads), this);
            addActor(d);
            j++;
        }
    }
    
    //Dirt
    int nDirt = max(180 - 20*getLevel(), 20);
    int k = 0;

    
    while (k != nDirt)
    {
        //generates cartesian coords (polar clusters dirts around center)
        double x = VIEW_WIDTH/2 + randInt(-120, 120);
        double y = VIEW_HEIGHT/2 + randInt(-120, 120);
        double radius = sqrt(pow(x - VIEW_WIDTH/2 ,2) + pow(y - VIEW_HEIGHT/2,2));
        if (radius > 120) //if outside arena, rerun the loop until a valid coord is found
            continue;
        
        vector<Actor*>:: iterator it;
        it = m_actors.begin();
        
        bool validLoc = true;
        //makes sure coords dont overlap with pit or food
        while (it != m_actors.end())
        {
            if (((*it)->isPit() || (*it)->isFood()) && overlapCheck((*it), x, y))
                validLoc = false;
            it++;
        }
        if (validLoc) //if valid init dirt, else loop again for new coords
        {
            Dirt* d = new Dirt(x, y, this);
            addActor(d);
            k++;
        }
    }
    
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    //socrates does something
    m_player->doSomething();
    //deallocate !isAlive objects
    killDead();
    
    vector<Actor*>::iterator it;
    it = m_actors.begin();
    //iterate through every actor, calling its doSomething function
    while(it != m_actors.end())
    {
        (*it)->doSomething();
        it++;
    }
    //add goodies to the map
    addGoodies();
    //creates an oss to broadcast stats
    ostringstream oss;
    oss << "Score: " << oss.fill('0')  << setw(6) << getScore() << "  Level: " << getLevel()  << "  Lives: "  << getLives() << "  Health: "  <<  m_player->getHp() << "  Spray: "  << m_player->getSpray() << "  Flames: "  << m_player->getFlame();
    string s = oss.str();
    setGameStatText(s);
    
    if(!m_player->isAlive()) //if player dies, decrement lives
    {
        decLives();
        return GWSTATUS_PLAYER_DIED;
    }
    
    it = m_actors.begin();
    bool advanceToNextLevel = true;
    //iterate through actors to see if any bacteria or pits are left
    while (it != m_actors.end())
    {
        if((*it)->preventsLevelCompleting())
            advanceToNextLevel = false;
        it++;
    }
    //if not, finish level and advance to next level
    if(advanceToNextLevel)
        return GWSTATUS_FINISHED_LEVEL;
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    //solves a memory leak bug after game ends
    if (getLives() == 0 && !m_player)
        delete m_player;
    //deallocates socrates
    if(getLives() != 0)
        delete m_player;
    
    vector<Actor*>::iterator it;
    it = m_actors.begin();
    //iterates through actors and deallocates all of them
    while(it != m_actors.end())
    {
        delete *it;
        it++;
    }
    m_actors.clear();
}

void StudentWorld::killDead()
{
    vector<Actor*>::iterator it;
    it = m_actors.begin();
    
    //deallocate all actors that are !isAlive
    while(it != m_actors.end())
    {
        if(!(*it)->isAlive())
        {
            delete (*it);
            it = m_actors.erase(it);
        }
        else
            it++;
    }
}
//adds new actor to the vector
void StudentWorld::addActor(Actor* a)
{
    m_actors.push_back(a);
}
//init goodies
void StudentWorld::addGoodies()
{
    //init fungus around perimeter with given chance
    int chanceFungus = max(510 - getLevel() * 10, 200);
    if(randInt(0, chanceFungus) == 0)
    {
        double theta = randInt(0, 359) * 3.14159 / 180;
        Fungus* fungus = new Fungus(VIEW_WIDTH/2 + VIEW_RADIUS * cos(theta), VIEW_HEIGHT/2 + VIEW_RADIUS * sin(theta), this);
        addActor(fungus);
        
    }
    
    //init goodies around perimeter with given chances
    int chanceGoodie = max(510 - getLevel() * 10, 250);
    if (randInt(0, chanceGoodie) == 0)
    {
        
        double theta = randInt(0, 359) * 3.14159 / 180;
        int chance = randInt(1, 10);
        //chances to spawn health, flame, live good is 60%/30%/10%
        if (chance <=6)
        {
            HealthGoodie* health = new HealthGoodie(VIEW_WIDTH/2 + VIEW_RADIUS * cos(theta), VIEW_HEIGHT/2 + VIEW_RADIUS * sin(theta), this);
            addActor(health);
        
        }
        if (chance >= 7 && chance <= 9)
        {
            FlameGoodie* flame = new FlameGoodie(VIEW_WIDTH/2 + VIEW_RADIUS * cos(theta), VIEW_HEIGHT/2 + VIEW_RADIUS * sin(theta), this);
            addActor(flame);

        }
        if (chance == 10)
        {
            ExtraLifeGoodie* life = new ExtraLifeGoodie(VIEW_WIDTH/2 + VIEW_RADIUS * cos(theta), VIEW_HEIGHT/2 + VIEW_RADIUS * sin(theta), this);
            addActor(life);

        }
    }
}

bool StudentWorld::getOverlappingSocrates(Actor* a, Actor* &player)
{
    //returns bool and passes socrates by reference if A overlaps him
    if(checkDistanceBetween(a, m_player) <= SPRITE_WIDTH) //euclidian distance is less than SPRITE_WIDTH
    {
        player = m_player;
        return true;
    }
        
    return false;
    
}

void StudentWorld::playerRestoreHealth() //restores socrates health to full
{
    m_player->restoreHealth();
}

void StudentWorld::playerIncFlame(int flame) //increase flame inventory
{
    m_player->incFlame(flame);
}

void StudentWorld::playerIncHealth(int health) //increase socrates health

{
    m_player->incHealth(health);
}

bool StudentWorld::overlapCheck(Actor* a, double x, double y) //check to see if coords overlap with actor
{
    double dx = x - a->getX();
    double dy = y - a->getY();
    
    if (sqrt(pow(dx,2) + pow(dy,2)) <= SPRITE_WIDTH) //euclidian distance is less than SPRITE_WIDTH
        return true;
    else
        return false;
}

bool StudentWorld::projOverlap(Actor* a, int damage) //checks to see if projectile overlaps anything
{
    vector<Actor*>::iterator it;
    it = m_actors.begin();
    
    while(it != m_actors.end())
    {
        //if overlap and actor is damagable, that object loses 'damage' health
        if(checkDistanceBetween(a, (*it)) <=  SPRITE_WIDTH && (*it)->takeDamage(damage))
        {
            return true;
        }
        it++;
    }

    return false;

}

bool StudentWorld::socrOverlap(Actor* a)
{
    if(checkDistanceBetween(a, m_player) <= SPRITE_WIDTH) //checks if euclidian distance is less than SPRITE_WIDTH
    {
        return true;
    }
        
    return false;
}

bool StudentWorld::foodOverlap(Actor* a)
{
    vector<Actor*>:: iterator it;
    it = m_actors.begin();
    
    //check if any food overlap with 'a', if so food dies and return true
    while(it != m_actors.end())
    {
        if((*it)->isFood() && checkDistanceBetween(a,(*it))<=SPRITE_WIDTH)
        {
            (*it)->die();
            return true;
        }
        it++;
    }
    return false;
}

bool StudentWorld::dirtOverlap(Actor* a) //see if actor's next move overlaps with dirt
{
    vector<Actor*>:: iterator it;
    it = m_actors.begin();
    
    //actors next move
    double x = a->getX() + 3 * cos(a->getDirection()* 3.14159/180);
    double y = a->getY() + 3 * sin(a->getDirection()* 3.14159/180);

    while(it != m_actors.end())
    {
        double dx = (*it)->getX()  - x;
        double dy = (*it)->getY() - y;
        double distAway = sqrt(pow(dx,2) + pow(dy,2));
        if((*it)->isDirt() && distAway <=SPRITE_WIDTH/2)
            //if next move is less than SPRITE_WIDTH/2 distance away
            //means object is blocked by dirt
        {
            return true;
        }
        it++;
    }
    return false;
}

double StudentWorld::checkDistanceBetween(Actor* a , Actor* b) const
{
    //returns euclidian distance between two actors
    double dx = b->getX() - a->getX();
    double dy = b->getY() - a->getY();
    
    return sqrt(pow(dx,2) + pow(dy,2));
    
}

bool StudentWorld::closetFood(Actor* a, int &direction, double searchRadius)

{
    vector<Actor*>:: iterator it;
    it = m_actors.begin();
    
    
    double closestDistance = VIEW_WIDTH;
    Direction angle;
    
    //finds closet food
    while(it != m_actors.end())
    {
        if((*it)->isFood() && checkDistanceBetween(a, (*it)) <= searchRadius)
        {
            //iterates through food objects and saves the closest distance and its angle
            if (checkDistanceBetween(a, (*it)) < closestDistance)
            {
            
                closestDistance = checkDistanceBetween(a, (*it));
                double dx = (*it)->getX() - a->getX();
                double dy = a->getY() - (*it)->getY();
                angle = -(atan2(dy, dx) * 180 / 3.14159); //atan finds angle based on slope
            }
               
        }
        it++;
    }
    if (closestDistance != VIEW_WIDTH) //if closestDistance is still VIEW_WIDTH, no food was found nearby
    {
        direction = angle; //passes direction by reference
        return true;
    }
    else
        return false;
}

bool StudentWorld::getAngleToPlayer(Actor* a, int dist, int& angle) const
{
    //finds angle to player
    double dx = m_player->getX() - a->getX();
    double dy = a->getY() - m_player->getY(); //order is reversed, because going down in this system increases y
    
    //finds angle based on slope
    angle = -(atan2(dy, dx) * 180 / 3.14159); //passes by reference
    if(sqrt(pow(dx, 2) + pow(dy, 2)) < dist)
        return true;
    else
        return false;
}
