#include "Actor.h"
#include "StudentWorld.h"



// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

class StudentWorld;


// **************************ACTOR**************************//

Actor::Actor(int ID, double startX, double startY, Direction startDir, int depth, StudentWorld* world): GraphObject(ID, startX, startY, startDir, depth), m_world(world), m_isAlive(true)
{}
Actor :: ~Actor()
{}
bool Actor::isAlive() const //returns alive status
{
    return m_isAlive;
}
bool Actor::isFood() const //returns not a food by default
{
    return false;
}
bool Actor::isProj() const //returns not a proj by default
{
    return false;
}
bool Actor::isPit() const //returns not a pit by default
{
    return false;
}
bool Actor::isDirt() const //returns not a dirt by default
{
    return false;
}
bool Actor::takeDamage(int damage) //return cant take damage by default
{
    return false;
}
bool Actor::preventsLevelCompleting() const //return doesnt prevent completing level by default
{
    return false;
}
void Actor::die() //changes alive status to false
{
    m_isAlive = false;
}
StudentWorld* Actor::getAPointerToMyWorld() const //get a pointer to StudentWorld
{
    return m_world;
}
//**************************AGENT**************************//

Agent::Agent(int imageID, double startX, double startY, int dir, int hitPoints,  StudentWorld* world):Actor(imageID, startX, startY, dir, 0, world), m_hp(hitPoints)
{}
int Agent::getHp() const
{
    return m_hp;
}
bool Agent::isDead() //if hp is zero, play die sound, !isAlive, and return true
{
    if (m_hp <= 0)
    {
        getAPointerToMyWorld()->playSound(soundWhenDie()) ;
        die();
        return true;
    }
    return false;
}
bool Agent::takeDamage(int damage) //decrements hp by damage
{
    m_hp -= damage;
    getAPointerToMyWorld()->playSound(soundWhenHurt());
    return true;
}
void Agent::restoreHealth() //restores health
{
    //sets to 100 because only Socrates restores health
    m_hp = 100;
}

//**************************SOCRATES**************************//

Socrates:: Socrates(StudentWorld* world): Agent(IID_PLAYER, 0, 128, 0, 100, world), m_nSpray(20), m_nFlamethrower(5), m_positionalDir(180)
{}
Socrates :: ~Socrates()
{}
void Socrates:: doSomething()
{
    if(isDead()) //checks if alive
        return;

    int ch;
    if (getAPointerToMyWorld()->getKey(ch)) //obtains input key data
    {
        switch (ch)
        {
            case KEY_PRESS_LEFT: //move counter cw
                m_positionalDir = m_positionalDir + 5; //increments 5 degrees (counter-clockwise)
                moveTo(VIEW_WIDTH/2 + VIEW_RADIUS * cos(m_positionalDir*3.14159/180), VIEW_HEIGHT/2 + VIEW_RADIUS * sin(m_positionalDir*3.14159/180)); //moves to new position on perimeter. uses formula: (origin + rcosx, origin + rsinx)
                setDirection(180 + m_positionalDir); //player faces + 180 degrees
                break;
            case KEY_PRESS_RIGHT: //move cw
                m_positionalDir = m_positionalDir - 5; //decrements 5 degrees (clockwise)
                moveTo(VIEW_WIDTH/2 + VIEW_RADIUS * cos(m_positionalDir*3.14159/180), VIEW_HEIGHT/2 + VIEW_RADIUS * sin(m_positionalDir*3.14159/180));
                setDirection(180 + m_positionalDir);
                break;
            case KEY_PRESS_SPACE: //shoots spray
                if (m_nSpray > 0)
                {
                    //creates spray at sprite width infront of player
                    getAPointerToMyWorld()->addActor(new Spray(getX() - SPRITE_WIDTH * cos(m_positionalDir*3.14159/180), getY() - SPRITE_WIDTH * sin(m_positionalDir*3.14159/180), getDirection() ,getAPointerToMyWorld()));
                    getAPointerToMyWorld()->playSound(SOUND_PLAYER_SPRAY);
                    m_nSpray--;
                }
                break;
            case KEY_PRESS_ENTER: //shots flames
                if (m_nFlamethrower > 0)
                {
                    for (int i = 0; i < 352; i += 22) //creates 16 flames radially around socrates (22 degree apart)
                    {
                        //adds flame SPRITE_WIDTH from player
                        getAPointerToMyWorld()->addActor(new Flame(getX() + SPRITE_WIDTH * cos((getDirection()+i)*3.14159/180), getY() + SPRITE_WIDTH * sin((getDirection()+i)*3.14159/180), getDirection()+i  ,getAPointerToMyWorld()));
                    }
                    getAPointerToMyWorld()->playSound(SOUND_PLAYER_FIRE);
                    m_nFlamethrower--;
                }
                break;
            default:
                break;
        }
    }
    else if (m_nSpray < 20) //if no keys are inputted, add spray until full
        m_nSpray++;
}
int Socrates::getSpray()
{
    return m_nSpray;
}
int Socrates::getFlame()
{
    return m_nFlamethrower;
}
void Socrates::incHealth(int howMuch) //increases health by howMuch
{
    m_health += howMuch;
}
void Socrates::incFlame(int howMuch) //increases flame by howMuch
{
    m_nFlamethrower += howMuch;
}
int Socrates::soundWhenHurt() const
{
    return SOUND_PLAYER_HURT;
}
int Socrates::soundWhenDie() const
{
    return SOUND_PLAYER_DIE;
}

//**************************DIRT**************************//

Dirt::Dirt(double startX, double startY, StudentWorld* world): Actor(IID_DIRT, startX, startY, 0,  1, world)
{}
Dirt::~Dirt()
{}
bool Dirt::takeDamage(int damage)
{
    die();
    return true;
}
bool Dirt::isDirt() const
{
    return true;
}
void Dirt:: doSomething()
{} //does nothing at all

//**************************Food**************************//

Food::Food(double startX, double startY, StudentWorld* world):Actor(IID_FOOD, startX, startY, 90, 1, world)
{}
Food::~Food()
{}
bool Food::isFood() const
{
    return true;
}
void Food::doSomething()
{} //does nothing at all

//**************************Projectile**************************//

Projectile::Projectile(int imageID, double startX, double startY, Direction startDir, int pixelsToTravel, int damage, StudentWorld* world): Actor(imageID, startX, startY, startDir, 1, world), m_pixelsLeft(pixelsToTravel), m_dir(startDir), m_damage(damage)
{}
Projectile::~Projectile()
{}
bool Projectile::isProj() const
{
    return true;
}
void Projectile::doSomething()
{
    if(getAPointerToMyWorld()->projOverlap(this, m_damage)) //if projectile overlaps a damagable object, deal that damage, and delete itself
        die();
    if(m_pixelsLeft>0) //if projectile still has moves left
    {
        moveAngle(m_dir, SPRITE_WIDTH); //move 8 pixels in its direction
        m_pixelsLeft -= 8;
    }
    else
        die(); //die when out of moves
}

//**************************SPRAY**************************//

Spray::Spray(double startX, double startY, Direction startDir, StudentWorld* world): Projectile(IID_SPRAY, startX, startY, startDir, 112, 2, world) //112 moves and 2 damage value
{}
Spray:: ~Spray()
{}

//**************************Flame**************************//

Flame::Flame(double startX, double startY, Direction startDir, StudentWorld* world): Projectile(IID_FLAME, startX, startY, startDir, 32, 5, world) //32 moves and 5 damage value
{}
Flame:: ~Flame()
{}

//**************************Goodies**************************//

Goodies::Goodies(int imageID, double startX, double startY, StudentWorld* world): Actor(imageID, startX, startY, 0, 1, world)
{
    m_timeLeft = max(randInt(0, 300 - 10 * (getAPointerToMyWorld()->getLevel() - 1)), 50); //randomizes time left
}
Goodies::~Goodies()
{}
bool Goodies::takeDamage(int damage)
{
    die(); //immediately die if goodie "takes" damage
    return true;
}
void Goodies::doSomething()
{
    if(!isAlive())
        return;
    if(getAPointerToMyWorld()->socrOverlap(this)) //if socrates overlaps with a goodie
        doDifferientiatedStuff();
    if (m_timeLeft <= 0) //die when time runs out
        die();
    m_timeLeft--; //decrement time every tick
}

//**************************Health Goodie**************************//

HealthGoodie::HealthGoodie(double startX, double startY, StudentWorld* world): Goodies(IID_RESTORE_HEALTH_GOODIE, startX, startY, world)
{}

HealthGoodie::~HealthGoodie()
{}
void HealthGoodie::doDifferientiatedStuff() //increase score, die, play sound, and restore player health
{
    getAPointerToMyWorld()->increaseScore(250);
    die();
    getAPointerToMyWorld()->playSound(SOUND_GOT_GOODIE);
    getAPointerToMyWorld()->playerRestoreHealth();
    return;
}

//**************************Flame Goodie**************************//

FlameGoodie::FlameGoodie(double startX, double startY, StudentWorld* world): Goodies(IID_FLAME_THROWER_GOODIE, startX, startY, world)
{}
FlameGoodie::~FlameGoodie()
{}
void FlameGoodie::doDifferientiatedStuff() //increase score, die, play sound, increase flame count
{
    getAPointerToMyWorld()->increaseScore(300);
    die();
    getAPointerToMyWorld()->playSound(SOUND_GOT_GOODIE);
    getAPointerToMyWorld()->playerIncFlame(5);
    return;
}

//**************************Extra Life Goodie**************************//

ExtraLifeGoodie::ExtraLifeGoodie(double startX, double startY, StudentWorld* world): Goodies(IID_EXTRA_LIFE_GOODIE, startX, startY, world)
{}
ExtraLifeGoodie::~ExtraLifeGoodie()
{}
void ExtraLifeGoodie::doDifferientiatedStuff() //increase score, die, play sound, increase lives
{
    getAPointerToMyWorld()->increaseScore(500);
    die();
    getAPointerToMyWorld()->playSound(SOUND_GOT_GOODIE);
    getAPointerToMyWorld()->incLives();
    return;
}

//**************************Fungus**************************//

Fungus::Fungus(double startX, double startY, StudentWorld* world): Goodies(IID_FUNGUS, startX, startY, world)
{}
Fungus::~Fungus()
{}
void Fungus::doDifferientiatedStuff() //decreases score, decreases health
{
    getAPointerToMyWorld()->increaseScore(-50);
    die();
    getAPointerToMyWorld()->playerIncHealth(-20);
    return;
}

//**************************Pit**************************//

Pit::Pit(double startX, double startY, StudentWorld* world): Actor(IID_PIT, startX, startY, 0, 1, world), m_nSamonella(5), m_nAggroSamonella(3), m_nEcoli(2) //inventory of 5 samonella, 3 aggro, 2 ecoli
{}
Pit::~Pit()
{}
bool Pit::isPit() const
{
    return true;
}
bool Pit::preventsLevelCompleting() const
{
    return true;
}
void Pit::doSomething()
{
    if(m_nSamonella <= 0 && m_nAggroSamonella <= 0 && m_nEcoli <= 0) //once inventory is out, die
    {
        die();
        return;
    }
    
    if (randInt(1, 50) == 1) // 1/50 chance to spawn a bacteria
    {
        for (;;)
        {
            //each bacteria has a 1/3 to be spawn; if inventory of that bacteria is gone, loop again until avaliable bacteria is choosen
            
            int chance = randInt(1,3);
        
            if (chance == 3 && m_nSamonella > 0)
            {
                RegularSamonella* s = new RegularSamonella(getX(), getY(), getAPointerToMyWorld());
                    getAPointerToMyWorld()->addActor(s);
                    m_nSamonella--;
                    return;
            }
            else if (chance == 2 && m_nAggroSamonella > 0)
            {
                AggroSamonella* ag = new AggroSamonella(getX(), getY(), getAPointerToMyWorld());
                getAPointerToMyWorld()->addActor(ag);
                m_nAggroSamonella--;
                return;
            }
            else if (chance == 1 && m_nEcoli > 0)
            {
                Ecoli* e = new Ecoli(getX(), getY(), getAPointerToMyWorld());
                getAPointerToMyWorld()->addActor(e);
                m_nEcoli--;
                return;
            }
        }
    }
}

//**************************Bacterium**************************//

Bacterium::Bacterium(int imageID, double startX, double startY, int hp, StudentWorld* world): Agent(imageID, startX, startY, 90, hp, world), m_movePlan(0), m_foodEaten(0)
{
    world->playSound(SOUND_BACTERIUM_BORN);
}

Bacterium::~Bacterium()
{}
int Bacterium::movePlan()
{
    return m_movePlan;
}
int Bacterium::food()
{
    return m_foodEaten;
}
void Bacterium::incMovePlan(int howMuch) //increase moveplan by howMuch
{
    m_movePlan += howMuch;
}
void Bacterium::setMovePlan(int num) //set moveplan
{
    m_movePlan = num;
}
void Bacterium::incFood(int howMuch) //increases food by howMuch
{
    m_foodEaten += howMuch;
}
bool Bacterium::preventsLevelCompleting() const
{
    return true;
}

//**************************Samonella**************************//

Samonella::Samonella(double startX, double startY, int hp, StudentWorld* world):Bacterium(IID_SALMONELLA, startX, startY, hp, world)
{}
Samonella::~Samonella()
{}
int Samonella::soundWhenHurt() const
{
    return SOUND_SALMONELLA_HURT;
}
int Samonella::soundWhenDie() const
{
    return SOUND_SALMONELLA_DIE;
}

//**************************RegularSamonella**************************//

RegularSamonella::RegularSamonella(double startX, double startY, StudentWorld* world): Samonella(startX, startY, 4, world), m_dps(1)
{}
RegularSamonella::~RegularSamonella()
{}
void RegularSamonella::doSomething()
{
    if (isDead()) //if killed, increase score, and have a 50% to spawn a pizza
    {
        getAPointerToMyWorld()->increaseScore(100);
        if(randInt(1,2) == 1)
        {
            Food* f = new Food(getX(), getY(), getAPointerToMyWorld());
            getAPointerToMyWorld()->addActor(f);
        }
            
        return;
    }
    
    Actor* socrates;
    //if overlaps with socrates, deal damage to him
    if(getAPointerToMyWorld()->getOverlappingSocrates(this, socrates))
    {
        socrates->takeDamage(m_dps);
    }
    else
    {
        //else check for food == 3 to multiply and for food overlap
        if(food() == 3)
            multiply();
        if(getAPointerToMyWorld()->foodOverlap(this))
            incFood(1);
    }
    //if theres a move plan
    if (movePlan() > 0)
    {
        incMovePlan(-1);
        
        //generate potential newX and newY coordinates to move to
        double newX = getX() + 3 * cos(getDirection() * 3.14159/180);
        double newY = getY() + 3 * sin(getDirection() * 3.14159/180);
        
        double radiusAway = sqrt(pow(newX - VIEW_WIDTH/2, 2) + pow(newY - VIEW_HEIGHT/2, 2));
        
        //see if these new coordinates are valid - doesnt overlap with dirt and is within the arena
        if(getAPointerToMyWorld()->dirtOverlap(this) || radiusAway >=  VIEW_RADIUS )
        {
            //not valid - generatre new direction and reset moveplan
            setDirection(randInt(0,359));
            setMovePlan(10);
            return;
        }
        else
        {
            moveTo(newX, newY); //move to new coords
            return;
        }
    } else {
        //no moveplan?
        int dir;
        //if theres food with 128, set direction for it
        if(getAPointerToMyWorld()->closetFood(this, dir, 128))
        {
            setDirection(dir);
            setMovePlan(10);
            return;
        }
        else
        {
            setDirection(randInt(0,359)); //else, generate a random direction
            setMovePlan(10);
            return;
        }
    }
}

void RegularSamonella::multiply()
{
    //generates coordinates for new samonella to be created at
    double newX;
    double newY;
    if (getX()>VIEW_WIDTH/2)
        newX = getX()-SPRITE_WIDTH/2;
    else if(getX() < VIEW_WIDTH/2)
        newX = getX()+SPRITE_WIDTH/2;
    else
        newX = getX();
    if(getY()>VIEW_WIDTH/2)
        newY = getY()-SPRITE_WIDTH/2;
    else if(getY() < VIEW_WIDTH/2)
        newY = getY()+SPRITE_WIDTH/2;
    else
        newY = getY();
    
    RegularSamonella* s = new RegularSamonella(newX, newY, getAPointerToMyWorld());
    getAPointerToMyWorld()->addActor(s);
    
    incFood(-3);
}

//**************************AggroSamonella**************************//

AggroSamonella::AggroSamonella(double startX, double startY, StudentWorld* world): Samonella(startX, startY, 10, world), m_dps(2)
{}
AggroSamonella::~AggroSamonella()
{}
void AggroSamonella::doSomething()
{
    if (isDead()) //if dead, increase points and have a 50% chance of spawning a pizza
    {
        getAPointerToMyWorld()->increaseScore(100);
        if(randInt(1,2) == 1)
        {
            Food* f = new Food(getX(), getY(), getAPointerToMyWorld());
            getAPointerToMyWorld()->addActor(f);
        }
        return;
    }

    int angle;
    bool alreadyMoved = false;
    if(getAPointerToMyWorld()-> getAngleToPlayer(this, 72, angle)) //aggressively finds a direction towards socrates if within 72 pixels
    {
        setDirection(angle);
        double newX = getX() + 3 * cos(getDirection() * 3.14159/180);
        double newY = getY() + 3 * sin(getDirection() * 3.14159/180);
        if(!getAPointerToMyWorld()->dirtOverlap(this)) //if no dirt blocks, move in direction of socrates
            moveTo(newX, newY);
        alreadyMoved = true; //cant move twice in one tick
    }
    
    Actor* socrates;
    //if overlaps with socrates, deal damage to him
    if(getAPointerToMyWorld()->getOverlappingSocrates(this, socrates))
        socrates->takeDamage(m_dps);
    else
    {
        //if food == 3, multiply
        if(food() == 3)
            multiply();
        //increase food if bacteria overlaps with food
        if(getAPointerToMyWorld()->foodOverlap(this))
            incFood(1);
    }
    if(alreadyMoved)
        return;
    //cant move if already moved toward socrates
    if (movePlan() > 0)
    {
        incMovePlan(-1);
        //calculate pontential new coords
        double newX = getX() + 3 * cos(getDirection() * 3.14159/180);
        double newY = getY() + 3 * sin(getDirection() * 3.14159/180);
        
        double radiusAway = sqrt(pow(newX - VIEW_WIDTH/2, 2) + pow(newY - VIEW_HEIGHT/2, 2));
        
        //see if coords are valid
        if(getAPointerToMyWorld()->dirtOverlap(this) || radiusAway >=  VIEW_RADIUS )
        {
            //if not, generate new direction and reset moveplan
            setDirection(randInt(0,359));
            setMovePlan(10);
            return;
        }
        else
        {
            moveTo(newX, newY); //move along its current path
            return;
        }
    } else {
        int dir;
        //find direction to closet food within 128 pixels
        if(getAPointerToMyWorld()->closetFood(this, dir, 128))
        {
            setDirection(dir);
            setMovePlan(10);
            return;
        }
        else
        {
            //if none, generate random direction
            setDirection(randInt(0,359));
            setMovePlan(10);
            return;
        }
    }
}
void AggroSamonella::multiply()
{
    //generates coordinates for new samonella to be created at
    double newX;
    double newY;
    if (getX()>VIEW_WIDTH/2)
        newX = getX()-SPRITE_WIDTH/2;
    else if(getX() < VIEW_WIDTH/2)
        newX = getX()+SPRITE_WIDTH/2;
    else
        newX = getX();
    if(getY()>VIEW_WIDTH/2)
        newY = getY()-SPRITE_WIDTH/2;
    else if(getY() < VIEW_WIDTH/2)
        newY = getY()+SPRITE_WIDTH/2;
    else
        newY = getY();
    
    AggroSamonella* ag = new AggroSamonella(newX, newY, getAPointerToMyWorld());
    getAPointerToMyWorld()->addActor(ag);
    
    incFood(-3);
}

//**************************Ecoli**************************//

Ecoli::Ecoli(double startX, double startY, StudentWorld* world): Bacterium(IID_ECOLI, startX, startY, 5, world), m_dps(4)
{}
Ecoli::~Ecoli()
{}
void Ecoli::doSomething()
{
    if (isDead()) //if dead, if increase player score and spawn pizza 50% of the time
    {
        getAPointerToMyWorld()->increaseScore(100);
        if(randInt(1,2) == 1)
        {
            Food* f = new Food(getX(), getY(), getAPointerToMyWorld());
            getAPointerToMyWorld()->addActor(f);
        }
        return;
    }
    
    Actor* socrates;
    if(getAPointerToMyWorld()->getOverlappingSocrates(this, socrates)) //deal damage to socrates if overlap
        socrates->takeDamage(m_dps);
    else
    {
        //multiply and eat food
        if(food() == 3)
            multiply();
        if(getAPointerToMyWorld()->foodOverlap(this))
            incFood(1);
    }
    
    int theta;
    //finds angle to socrates
    if (getAPointerToMyWorld()->getAngleToPlayer(this, 256, theta))
    {
        setDirection(theta);
        if(!getAPointerToMyWorld()->dirtOverlap(this))
        {
            //move toward socrates
            moveAngle(getDirection(), 2);
            return;
        }
        else
        {
            //if blocked by dirt, increase angle by 10 up to 10 times until unblocked
            for (int i =  0; i != 10; i++)
            {
                setDirection(getDirection()+10);
                if(!getAPointerToMyWorld()->dirtOverlap(this))
                {
                    moveAngle(getDirection(), 2);
                    return;
                }
            }
        }
    }
}
int Ecoli::soundWhenHurt() const
{
    return SOUND_ECOLI_HURT;
}
int Ecoli::soundWhenDie() const
{
    return SOUND_ECOLI_DIE;
}
void Ecoli::multiply()
{
    //generates coordinates for new ecoli to be created at
    double newX;
    double newY;
    if (getX()>VIEW_WIDTH/2)
        newX = getX()-SPRITE_WIDTH/2;
    else if(getX() < VIEW_WIDTH/2)
        newX = getX()+SPRITE_WIDTH/2;
    else
        newX = getX();
    if(getY()>VIEW_WIDTH/2)
        newY = getY()-SPRITE_WIDTH/2;
    else if(getY() < VIEW_WIDTH/2)
        newY = getY()+SPRITE_WIDTH/2;
    else
        newY = getY();
    
    Ecoli* e = new Ecoli(newX, newY, getAPointerToMyWorld());
    getAPointerToMyWorld()->addActor(e);
    
    incFood(-3);
}
