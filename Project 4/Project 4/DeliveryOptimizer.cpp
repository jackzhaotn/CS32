#include "provided.h"
#include <vector>
#include <stdlib.h>
using namespace std;

class DeliveryOptimizerImpl
{
public:
    DeliveryOptimizerImpl(const StreetMap* sm);
    ~DeliveryOptimizerImpl();
    void optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const;
    double calcDist(vector<DeliveryRequest> requests, GeoCoord depot) const;
    double acceptance(vector<DeliveryRequest> neighbor, vector<DeliveryRequest> current, double temp, GeoCoord depot) const;
};

DeliveryOptimizerImpl::DeliveryOptimizerImpl(const StreetMap* sm)
{
}

DeliveryOptimizerImpl::~DeliveryOptimizerImpl()
{

}

void DeliveryOptimizerImpl::optimizeDeliveryOrder(
    const GeoCoord& depot,
    vector<DeliveryRequest>& deliveries,
    double& oldCrowDistance,
    double& newCrowDistance) const
{
    oldCrowDistance = 0.0;
    newCrowDistance = 0.0;
    oldCrowDistance = calcDist(deliveries, depot);
    

    //ATTEMPT AT SIMULATED ANNEALING
    double temperature = 10000;
    double coolingRate = .95;
    vector<DeliveryRequest> newDeliveries;
    int count = 0;
    
    double fRand(double fMin, double fMax);
    
    while(temperature != 0)
    {
        int swap1 = rand() % deliveries.size();
        int swap2 = rand() % deliveries.size();
        
        newDeliveries = deliveries;
        //randomly swap two deliveries
        DeliveryRequest temp = deliveries[swap1];
        deliveries[swap1] = deliveries[swap2];
        deliveries[swap2] = temp;
        
        double p = acceptance(newDeliveries, deliveries, temperature, depot); //p is acceptance value
        if (p >= fRand(0.0, 1.0)) //always true if newDeliveries' dist < deliveries' dist
            deliveries = newDeliveries;
        
        temperature *= pow(coolingRate, count); //cooling function
        count++;
    }
    
    
    newCrowDistance = calcDist(deliveries, depot);
    
}


double DeliveryOptimizerImpl::calcDist(vector<DeliveryRequest> requests, GeoCoord depot) const
{
    double distance =  0.0;
    
    distance += distanceEarthMiles(depot, requests[0].location); //ads dist between depot and first deliv
    for (int i = 1; i != requests.size(); i++) //adds dist between every deliv
    {
        distance += distanceEarthMiles(requests[i-1].location , requests[i].location);
    }
    distance += distanceEarthMiles(requests[requests.size()-1].location, depot); //adds dist between last deliv and depot
    return distance;
    
}

double DeliveryOptimizerImpl::acceptance(vector<DeliveryRequest> neighbor, vector<DeliveryRequest> current, double temp, GeoCoord depot) const
{
    double p; //p value
    double newDist = calcDist(neighbor, depot);
    double oldDist = calcDist(current, depot);
    
    if (newDist < oldDist)
        p = 1.0;
    else
        p = exp(-(newDist - oldDist)/temp); //Boltzmann equation
    return p;
}

double fRand(double fMin, double fMax)
{
    double f = (double)rand() / RAND_MAX;
    return fMin + f * (fMax - fMin);
}

//******************** DeliveryOptimizer functions ****************************

// These functions simply delegate to DeliveryOptimizerImpl's functions.
// You probably don't want to change any of this code.

DeliveryOptimizer::DeliveryOptimizer(const StreetMap* sm)
{
    m_impl = new DeliveryOptimizerImpl(sm);
}

DeliveryOptimizer::~DeliveryOptimizer()
{
    delete m_impl;
}

void DeliveryOptimizer::optimizeDeliveryOrder(
        const GeoCoord& depot,
        vector<DeliveryRequest>& deliveries,
        double& oldCrowDistance,
        double& newCrowDistance) const
{
    return m_impl->optimizeDeliveryOrder(depot, deliveries, oldCrowDistance, newCrowDistance);
}

