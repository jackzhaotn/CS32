#include "provided.h"
#include <list>
#include <set>
#include <cmath>
#include "ExpandableHashMap.h"
using namespace std;

struct GeoCoordNode //new struct for A* use
{
    GeoCoordNode(GeoCoord startLoc) //starting node constructor
    {
        parent = nullptr;
        currentLoc = startLoc;
        f = 0;
        g = 0;
        h = 0;
    }
    GeoCoordNode(GeoCoord currentLoc, GeoCoord destination, GeoCoordNode* parent)
    {
        this->parent = parent;
        this->currentLoc = currentLoc;
        
        g = parent->g + distanceEarthMiles(currentLoc, parent->currentLoc);
        h = distanceEarthMiles(currentLoc, destination);
        f = g + h;
    }
    
    GeoCoordNode* parent; //prev waypoint
    GeoCoord currentLoc;
    //f = g + h
    //g = parent's g + cost between current and parent
    //h = heuristic , euclidiean  distance from current to end
    double f, g, h;
};

inline bool operator < (const GeoCoordNode& lhs, const GeoCoordNode& rhs)
{
    return lhs.f < rhs.f;
}
    
inline
bool operator==(const GeoCoordNode& lhs, const GeoCoordNode& rhs)
{
    return lhs.currentLoc == rhs.currentLoc  &&  lhs.currentLoc == rhs.currentLoc;
}
    
struct APtrComp //Comparing function to help sort GeoCoordNode* in a set
{
    bool operator()(const GeoCoordNode* lhs, const GeoCoordNode* rhs) const
    {
        return lhs->f < rhs->f;
    }
};

class PointToPointRouterImpl
{
public:
    PointToPointRouterImpl(const StreetMap* sm);
    ~PointToPointRouterImpl();
    
    DeliveryResult generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const;
    
    list<StreetSegment> pathFinder(GeoCoordNode* end)  const;
private:
    const StreetMap* sm;
};

PointToPointRouterImpl::PointToPointRouterImpl(const StreetMap* sm)
{
    this->sm = sm;
}

PointToPointRouterImpl::~PointToPointRouterImpl()
{

}

DeliveryResult PointToPointRouterImpl::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    vector<StreetSegment> ss;
    if(!(sm->getSegmentsThatStartWith(start, ss)) || !(sm->getSegmentsThatStartWith(end, ss)))
       return BAD_COORD;
    if (start == end)
    {
        route.clear();
        totalDistanceTravelled = 0;
        return DELIVERY_SUCCESS;
    }
     
    set<GeoCoordNode*, APtrComp> openList; //open list for that orders GeoCoordNode* in increasing f values
    ExpandableHashMap<GeoCoord, double> closedList; //"closed list" for already visited nodes. O(1) search
    
    GeoCoordNode* startPoint = new GeoCoordNode(start);
    openList.insert(startPoint);
    
    vector<GeoCoordNode*> deleteMe;

    while(!openList.empty())
    {
        GeoCoordNode* current = *openList.begin();
        openList.erase(openList.begin()); //pops off least f valued GeoCoordNode
        deleteMe.push_back(current);
        closedList.associate(current->currentLoc, current->f); //stores popped off GeoCoord as already visited
        
        vector<GeoCoord> neighbors;
        vector<StreetSegment> segs;
        sm->getSegmentsThatStartWith(current->currentLoc, segs);
    
        for(int i = 0; i != segs.size(); i++)
            neighbors.push_back(segs[i].end);
    
        for (int i = 0; i != neighbors.size(); i++ ) //creates successors
        {
            GeoCoordNode* successor = new GeoCoordNode(neighbors[i], end, current);
            if(successor->h == 0)  //if  h ==  0, successor is at the destination
            {
                route = pathFinder(successor);
                totalDistanceTravelled = successor->g;
                //cleanup
                delete successor;
                for (GeoCoordNode* node : openList)
                    delete node;
                for (GeoCoordNode* node : deleteMe)
                    delete node;
                return DELIVERY_SUCCESS;
            }
            bool isOptimal = true;  //optimal  if  it doesn't exist on the open or closed list
            double* test = closedList.find(neighbors[i]);
            if (test != nullptr && (*test < successor->f)) //can still be optimal if its f is less than its same key's f
                isOptimal = false;
    
            for (GeoCoordNode* g : openList)
                if (g == successor && g->f < successor->f) //can still be optimal if its f is less than its same key's f
                    isOptimal = false;
    
            if(isOptimal)
            {
                openList.insert(successor); //if it is optimal than insert it into the openlist
            }
            else
                deleteMe.push_back(successor); //if not delete it soon
        }
        
        closedList.associate(current->currentLoc, current->f); //current goes into closed list since its been explored
            
    }
    
    //CLEAN UP IF NO ROUTE IS FOUND
    for (GeoCoordNode* node : openList)
        delete node;
    for (GeoCoordNode* node : deleteMe)
        delete node;
    
    return NO_ROUTE;  // Delete this line and implement this function correctly
}

list<StreetSegment> PointToPointRouterImpl::pathFinder(GeoCoordNode* end) const
{
    list<StreetSegment> path;
    GeoCoordNode* p = end;
    
    StreetSegment ss;
    vector<StreetSegment> temp;
    while(p->parent != nullptr)
    {
        sm->getSegmentsThatStartWith(p->parent->currentLoc, temp); //gets street segments
        for (int i = 0; i!=temp.size(); i++)
        {
            if(temp[i].end.latitude == p->currentLoc.latitude && temp[i].end.longitude == p->currentLoc.longitude)
                path.push_front(temp[i]); //finds matching street segment that matches p and its parents geocoord
        }
        p = p->parent;
    }
    return path;
}
    
//******************** PointToPointRouter functions ***************************

// These functions simply delegate to PointToPointRouterImpl's functions.
// You probably don't want to change any of this code.

PointToPointRouter::PointToPointRouter(const StreetMap* sm)
{
    m_impl = new PointToPointRouterImpl(sm);
}

PointToPointRouter::~PointToPointRouter()
{
    delete m_impl;
}



DeliveryResult PointToPointRouter::generatePointToPointRoute(
        const GeoCoord& start,
        const GeoCoord& end,
        list<StreetSegment>& route,
        double& totalDistanceTravelled) const
{
    return m_impl->generatePointToPointRoute(start, end, route, totalDistanceTravelled);
}





