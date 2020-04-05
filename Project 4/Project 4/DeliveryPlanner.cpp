#include "provided.h"
#include <vector>

using namespace std;

class DeliveryPlannerImpl
{
public:
    DeliveryPlannerImpl(const StreetMap* sm);
    ~DeliveryPlannerImpl();
    DeliveryResult generateDeliveryPlan(
        const GeoCoord& depot,
        const vector<DeliveryRequest>& deliveries,
        vector<DeliveryCommand>& commands,
        double& totalDistanceTravelled) const;
    string calculateDirection(double angle, string type) const; //returns direction i.e. northeast for type proceed or right for type turn

private:
    const StreetMap* sm;
    PointToPointRouter* pp;
};

DeliveryPlannerImpl::DeliveryPlannerImpl(const StreetMap* sm)
{
    this->sm = sm;
    pp = new PointToPointRouter(sm);
}

DeliveryPlannerImpl::~DeliveryPlannerImpl()
{

    delete pp;
}

DeliveryResult DeliveryPlannerImpl::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    double dist = 0.0;
    double totalDist = 0.0;
    list<StreetSegment> route;
    double initalAngle = 0.0;
    vector<DeliveryCommand> commandsVector;
    

    //Checks bad coords condition
    vector<StreetSegment> ss;
    if (!sm->getSegmentsThatStartWith(depot, ss))
    {
        return BAD_COORD;
    }
    for (DeliveryRequest d : deliveries)
    {
        if (!sm->getSegmentsThatStartWith(d.location, ss))
        {
            return BAD_COORD;
        }
    }

    
    
    bool backToDepot = false;
    for(int i = 0; i <= deliveries.size() ; i++)
    {
            
        if(i==0) //generate route from depot to first delivery location
        {
            if(pp->generatePointToPointRoute(depot, deliveries[i].location, route, dist) == NO_ROUTE)
                return NO_ROUTE;
        }
        else if (i == deliveries.size()) //generate route from last deliver location to depot
        {
            if (pp->generatePointToPointRoute(deliveries[i-1].location, depot, route, dist) == NO_ROUTE)
                return NO_ROUTE;
            backToDepot = true;
        }
        else //generate route from one delivery location to the next
        {
            if(pp->generatePointToPointRoute(deliveries[i-1].location, deliveries[i].location, route, dist) == NO_ROUTE)
            {
                cerr << "No route at position"  << i << endl;
                return NO_ROUTE;
            }
        }
    
        string street;
        bool firstCommand = true;
        
        for (StreetSegment s: route) //for every route
        {
            totalDist += distanceEarthMiles(s.start, s.end);
            if (s.name != street && !firstCommand) //true when we need to turn
            {
                DeliveryCommand proceed; //creates proceed here since we know the full distance
                proceed.initAsProceedCommand(calculateDirection(initalAngle, "proceed"), street, dist);
                commandsVector.push_back(proceed); //push this command into the vector
                street = s.name;
                double dx = s.end.longitude - s.start.longitude;
                double dy = s.end.latitude - s.start.latitude;
                double thisAngle = atan2(dy, dx) * 180 / 3.14159;   //calculates angle
                double turnAngle = thisAngle - initalAngle; //calculates turn angle
                DeliveryCommand turn;
                turn.initAsTurnCommand(calculateDirection(turnAngle, "turn"), street);
                commandsVector.push_back(turn); //push command
                initalAngle = thisAngle; //inital angle is thisAngle since its the angle of the start of the new street
                
                dist = distanceEarthMiles(s.start, s.end); //resets dist since we moved onto a new street
            }
            else
            {
                if(firstCommand) //since calculatation of initalAngle only works
                {
                    double dx = s.end.longitude - s.start.longitude;
                    double dy = s.end.latitude - s.start.latitude;
                    initalAngle = atan2(dy, dx) * 180 / 3.14159;  //calculates initalAngle since it is the first street segment we are travelling on of the street
                    firstCommand = false;
                }
                street = s.name;
                dist += distanceEarthMiles(s.start, s.end); //adds to total distance
            }
            
        }
        if (!backToDepot) //actual deliver command
        {
            DeliveryCommand proceed;
            proceed.initAsProceedCommand(calculateDirection(initalAngle, "proceed"), street, dist);
            commandsVector.push_back(proceed);
            DeliveryCommand deliver;
            deliver.initAsDeliverCommand(deliveries[i].item);
            commandsVector.push_back(deliver);
        }
    }
    commands = commandsVector;
    totalDistanceTravelled = totalDist;
    return DELIVERY_SUCCESS;  // Delete this line and implement this function correctly
}


string DeliveryPlannerImpl::calculateDirection(double angle, string type) const
{
    //makes sure angle is in the rangle of 0-359
    while (angle < 0)
        angle += 360;
    if (type == "proceed")
    {
        if (angle >= 337.5)
            return "east";
        if (angle >= 292.5)
            return "southeast";
        if (angle >= 247.5)
            return "south";
        if (angle >= 202.5)
            return "southwest";
        if (angle >= 157.5)
            return "west";
        if (angle >= 112.5)
            return "northwest";
        if (angle >= 67.5)
            return "north";
        if (angle >= 22.5)
            return "northeast";
        if (angle >= 0)
            return "east";
    }
    else if (type == "turn")
    {
        if (angle >= 1 && angle < 180)
            return "left";
        if (angle >= 180 && angle < 360)
            return "right";
    }
    else
    {
        cerr << "Invalid type" << endl;
        return "";
    }
    return "";
}

//******************** DeliveryPlanner functions ******************************

// These functions simply delegate to DeliveryPlannerImpl's functions.
// You probably don't want to change any of this code.

DeliveryPlanner::DeliveryPlanner(const StreetMap* sm)
{
    m_impl = new DeliveryPlannerImpl(sm);
}

DeliveryPlanner::~DeliveryPlanner()
{
    delete m_impl;
}

DeliveryResult DeliveryPlanner::generateDeliveryPlan(
    const GeoCoord& depot,
    const vector<DeliveryRequest>& deliveries,
    vector<DeliveryCommand>& commands,
    double& totalDistanceTravelled) const
{
    return m_impl->generateDeliveryPlan(depot, deliveries, commands, totalDistanceTravelled);
}
