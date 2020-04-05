#include "provided.h"
#include "ExpandableHashMap.h"
#include <string>
#include <vector>
#include <functional>
#include <fstream>
using namespace std;

//HASHER FUNCTIONS
unsigned int hasher(const GeoCoord& g)
{
    return hash<string>()(g.latitudeText + g.longitudeText);
}

unsigned int hasher(const string& s)
{
    return hash<string>()(s);
}



class StreetMapImpl
{
public:
    StreetMapImpl();
    ~StreetMapImpl();
    bool load(string mapFile);
    bool getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const;
private:
    ExpandableHashMap<GeoCoord, vector<StreetSegment*>>* coordsToStreetSegs;
    
};

StreetMapImpl::StreetMapImpl()
{
    coordsToStreetSegs = new ExpandableHashMap<GeoCoord, vector<StreetSegment*>>(.5);
}

StreetMapImpl::~StreetMapImpl()
{
    delete coordsToStreetSegs;
}

bool StreetMapImpl::load(string mapFile)
{
    ifstream infile(mapFile);
    if (!infile )
    {
        cerr << "Error: Cannot open mapdata.txt!" << endl;
        return false;
    }
    
    string segmentName;
    while(getline(infile, segmentName))
    {
        int numSegments;
        infile >> numSegments;
        infile.ignore(10000, '\n');
        
        
        for(int i = 0; i != numSegments; i++)
        {
            string coords[4]; //stores 4 coords, start lat and long, end lat and long
            for (int j = 0; j != 3; j++)
            {
                getline(infile, coords[j], ' '); //assigns the coords for first three by parsing the line until space char
            }
            getline(infile, coords[3], '\n'); //assigns last coord by parsing until '\n'
            
            //INITIALIZES GEOCOORD OBJECTS AND STREETSEGMENT OBJECT WITH APPROPIATE COORDINATES
            GeoCoord startCoords(coords[0], coords[1]);
            GeoCoord endCoords(coords[2], coords[3]);
            StreetSegment* seg = new StreetSegment(startCoords, endCoords, segmentName); //makes street segments with those GeoCoords
            
            vector<StreetSegment*>* temp = coordsToStreetSegs->find(startCoords);
            if (temp != nullptr) //if a vector of street segs already exists for the starting GeoCoord
                (*temp).push_back(seg); //just add on to the vector
            else
            {
                vector<StreetSegment*> *newVector = new vector<StreetSegment*>; //otherwise allocate a new vector of StreetSegment pointers
                (*newVector).push_back(seg);
                coordsToStreetSegs->associate((*newVector).back()->start, *newVector);
            }
            
            //MAKES REVERSE VERSION OF THE STREETSEGMENT
            StreetSegment* segRev = new StreetSegment(endCoords, startCoords, segmentName);
            temp = coordsToStreetSegs->find(endCoords);
            if (temp != nullptr)
                (*temp).push_back(segRev);
            else
            {
                vector<StreetSegment*> *newVector = new vector<StreetSegment*>;
                (*newVector).push_back(segRev);
                coordsToStreetSegs->associate((*newVector).back()->start, *newVector);
            }
        }
    }
    
    
    return true;
}

bool StreetMapImpl::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
    vector<StreetSegment*> *origSeg = coordsToStreetSegs->find(gc); //gets the vector, O(1)
    vector<StreetSegment> tempSeg;
    
    if (origSeg == nullptr) //if nothing is there, return false
        return false;
    
    //since segs is a vector<StreetSegment> and not vector<StreetSegment*>, we turn it into that
    tempSeg.reserve(origSeg->size());
    transform(origSeg->begin(), origSeg->end(), back_inserter(tempSeg), [](StreetSegment* item){return *item;});
    
    segs = tempSeg;
    return true;
    
}

//******************** StreetMap functions ************************************

// These functions simply delegate to StreetMapImpl's functions.
// You probably don't want to change any of this code.

StreetMap::StreetMap()
{
    m_impl = new StreetMapImpl;
}

StreetMap::~StreetMap()
{
    delete m_impl;
}

bool StreetMap::load(string mapFile)
{
    return m_impl->load(mapFile);
}

bool StreetMap::getSegmentsThatStartWith(const GeoCoord& gc, vector<StreetSegment>& segs) const
{
   return m_impl->getSegmentsThatStartWith(gc, segs);
}
