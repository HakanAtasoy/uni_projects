#include "METUMaps.h"
#include "GraphExceptions.h"

#include <iostream>

void METUMaps::PrintNotInJourney() const
{
    std::cout << "Device is not in a journey!" << std::endl;
}

void METUMaps::PrintUnableToChangeDestination() const
{
    std::cout << "Cannot change Destination during journey!" << std::endl;
}

void METUMaps::PrintUnableToChangeStartingLoc() const
{
    std::cout << "Cannot change Starting Location during journey!" << std::endl;
}

void METUMaps::PrintAlreadyInJourney() const
{
    std::cout << "Device is already in a journey!" << std::endl;
}

void METUMaps::PrintJourneyIsAlreadFinished() const
{
    std::cout << "Journey is already finished!" << std::endl;
}

void METUMaps::PrintLocationNotFound() const
{
    std::cout << "One (or both) of the locations are not found in the maps!" << std::endl;
}

void METUMaps::PrintJourneyCompleted() const
{
    std::cout << "Journey Completed!" << std::endl;
}

void METUMaps::PrintCachedLocationFound(const std::string& location0,
                                        const std::string& location1) const
{
    std::cout << "Route between \""
              << location0 << "\" and \""
              << location1 << "\" is in cache, using that..."
              << std::endl;
}

void METUMaps::PrintCalculatingRoutes(const std::string& location0,
                                      const std::string& location1) const
{
    std::cout << "Calculating Route(s) between \""
              << location0 << "\" and \""
              << location1 << "\"..."
              << std::endl;
}

std::string METUMaps::GenerateKey(const std::string& location0,
                                  const std::string& location1)
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    return location0 + "/" + location1;
}

METUMaps::METUMaps(int potentialPathCount,
                   const std::string& mapFilePath)
{

    this -> potentialPathCount = potentialPathCount ;
    Graph hey (mapFilePath) ;
    map = hey ;
    KeyedHashTable wow (map.TotalVertexCount()*potentialPathCount) ;
    cachedPaths = wow ;
    inJourney = false ;
}

void METUMaps::SetDestination(const std::string& name)
{
    if (inJourney) 
        this -> PrintUnableToChangeDestination() ;
    else 
        destination = name ;
}

void METUMaps::SetStartingLocation(const std::string& name)
{

    if (inJourney) 
        this -> PrintUnableToChangeStartingLoc() ;
    else 
        startingLoc = name ;
}

void METUMaps::StartJourney()
{
    if (inJourney)
        this -> PrintAlreadyInJourney() ;
    else
    {
        std::vector< std::vector<int> > another_one ;
        std::vector<int> somebody ;
        this -> PrintCalculatingRoutes(startingLoc, destination) ;
        if(!map.MultipleShortPaths(another_one, startingLoc, destination, potentialPathCount))
            this -> PrintLocationNotFound() ;
        else
        {
            currentLoc = startingLoc ;
            currentRoute = another_one[0] ;
            inJourney = true ;
            std::string hoop ;
            int s = another_one.size(), s2 ;
            for(int i = 0; i < s; i++)
            {
                s2 = another_one[i].size() ;
                for(int l = 0; l < s2 - 1; l++)
                {
                    hoop = GenerateKey(map.VertexName(another_one[i][l]), map.VertexName(another_one[i][s2-1])) ;
                    somebody.clear() ;
                    for(int k = l; k < s2; k++)
                    {
                        somebody.push_back(another_one[i][k]) ;
                    }
                    cachedPaths.Insert(hoop, somebody) ;
                }
            }
        }
    }
    
}

void METUMaps::EndJourney()
{
    if(!inJourney)
        this -> PrintJourneyIsAlreadFinished() ;
    else
    {
        inJourney = false ;
        destination = "" ;
        startingLoc = "" ;
        currentLoc = "" ;
        cachedPaths.ClearTable() ;
    }
}

void METUMaps::UpdateLocation(const std::string& name)
{
    if(!inJourney)
    {
        this -> PrintNotInJourney() ;
        return ;
    }
    if(name == destination)
    {   
        this -> PrintJourneyCompleted() ;
        return ;
    }
    std::vector<int> searching ;
    std::string apo = GenerateKey(name, destination) ;
    if ( cachedPaths.Find(searching, apo) )
    {
        this -> PrintCachedLocationFound(name, destination) ;
        currentRoute = searching ;
        currentLoc = name ;
    }
    else
    {
        
        this -> PrintCalculatingRoutes(name, destination) ;
        std::vector< std::vector<int> > another_one ;
        std::vector<int> somebody ;
        if(map.MultipleShortPaths(another_one, startingLoc, destination, potentialPathCount)) 
        {
            currentLoc = name ;
            currentRoute = another_one[0] ;

            std::string hoop ;
            int s = another_one.size(), s2 ;
            for(int i = 0; i < s; i++)
            {
                s2 = another_one[i].size() ;
                for(int l = 0; l < s2 - 1; l++)
                {
                    hoop = GenerateKey(map.VertexName(another_one[i][l]), map.VertexName(another_one[i][s2-1])) ;
                    somebody.clear() ;
                    for(int k = l; k < s2; k++)
                    {
                        somebody.push_back(another_one[i][k]) ;
                    }
                    cachedPaths.Insert(hoop, somebody) ;
                }
            }
        }
        else
            this -> PrintLocationNotFound() ;
        
    }
    
}

void METUMaps::Display()
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    if(!inJourney)
    {
        // We are not in journey, this function
        // shouldn't be called
        PrintNotInJourney();
        return;
    }

    int timeLeft = map.TotalWeightInBetween(currentRoute);

    // Print last known location
    std::cout << "Journey         : " << startingLoc << "->"
              << destination << "\n";
    std::cout << "Current Location: " << currentLoc << "\n";
    std::cout << "Time Left       : " << timeLeft << " minutes\n";
    std::cout << "Current Route   : ";
    map.PrintPath(currentRoute, true);

    std::cout.flush();
}