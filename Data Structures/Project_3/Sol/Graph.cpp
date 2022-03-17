#include "Graph.h"
#include "GraphExceptions.h"

#include <iostream>
#include <iomanip>
#include <queue>
#include <fstream>
#include <sstream>
#include <cstdlib>

// Literally do nothing here
// default constructors of the std::vector is enough
Graph::Graph()
{}

Graph::Graph(const std::string& filePath)
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    // Tokens
    std::string tokens[3];

    std::ifstream mapFile(filePath.c_str());
    // Read line by line
    std::string line;
    while (std::getline(mapFile, line))
    {
        // Empty Line Skip
        if(line.empty()) continue;
        // Comment Skip
        if(line[0] == '#') continue;

        // Tokenize the line
        int i = 0;
        std::istringstream stream(line);
        while(stream >> tokens[i]) i++;

        // Single token (Meaning it is a vertex)
        if(i == 1)
        {
            InsertVertex(tokens[0]);
        }
        // Exactly three tokens (Meaning it is an edge)
        else if(i == 3)
        {
            int weight = std::atoi(tokens[0].c_str());
            if(!ConnectVertices(tokens[1], tokens[2], weight))
            {
                std::cerr << "Duplicate edge on "
                          << tokens[0] << "-"
                          << tokens[1] << std::endl;
            }
        }
        else std::cerr << "Token Size Mismatch" << std::endl;
    }
}

void Graph::InsertVertex(const std::string& vertexName)
{
    int s = vertexList.size() ;
    for(int i = 0; i < s; i++)
    {
        if(vertexList[i].name == vertexName) throw DuplicateVertexNameException() ;
    }
    GraphVertex hey;
    hey.name = vertexName;
    hey.edgeCount = 0;
    vertexList.push_back(hey);
}

bool Graph::ConnectVertices(const std::string& fromVertexName,
                            const std::string& toVertexName,
                            int weight)
{

    if(fromVertexName == toVertexName) return false ;
    int s = vertexList.size(), found = 0, s2 = edgeList.size(), vertids[2] ;
    GraphVertex* yurt7[2] ;
    for(int i = 0; i < s; i++)
    {
        if(vertexList[i].name == fromVertexName) 
        { 
            yurt7[0] = &vertexList[i] ; 
            vertids[0] = i ;
            found++ ; 
        }
        if(vertexList[i].name == toVertexName) 
        { 
            yurt7[1] = &vertexList[i] ; 
            vertids[1] = i ;
            found++ ; 
        }
    }
    
    if (found != 2) throw VertexNotFoundException() ;
    if (yurt7[0] -> edgeCount == MAX_EDGE_PER_VERTEX || yurt7[1] -> edgeCount == MAX_EDGE_PER_VERTEX) throw TooManyEdgeOnVertexExecption() ;
    for(int i = 0; i < s2; i++)
    {
        if( (edgeList[i].vertexId0 == vertids[0] && edgeList[i].vertexId1 == vertids[1]) || (edgeList[i].vertexId1 == vertids[0] && edgeList[i].vertexId0 == vertids[1]) )
            return false;
    }
    GraphEdge finally;
    finally.vertexId0 = vertids[0] ;
    finally.vertexId1 = vertids[1] ;
    finally.weight = weight ;
    finally.masked = false ;
    edgeList.push_back(finally) ;
    yurt7[0] -> edgeIds[yurt7[0] -> edgeCount++] = edgeList.size() - 1 ;
    yurt7[1] -> edgeIds[yurt7[1] -> edgeCount++] = edgeList.size() - 1 ;
    
    
}

bool Graph::ShortestPath(std::vector<int>& orderedVertexIdList,
                         const std::string& from,
                         const std::string& to) const
{
    
    int s = vertexList.size(), found = 0, vertids[2], s2 = edgeList.size(), new_weight, id ;
    std::priority_queue<DistanceVertexIdPair> yes ;
    std::vector<int> previous ;
    std::vector<int>pathh ;
    std::vector<int> w ;

    for(int i = 0; i < s; i++)
    {
        w.push_back(LARGE_NUMBER) ;
        if(vertexList[i].name == from) 
        {
            
            found++ ;
            vertids[0] = i ;
            w[i] = 0 ;
        }
        
        if(vertexList[i].name == to) 
        {

            found++ ;
            vertids[1] = i ;
        }
    }
    

    if(found != 2) throw VertexNotFoundException() ;
    DistanceVertexIdPair a (vertids[0], 0) ;
    yes.push(a) ;
    
    for(int i = 0; i < s; i++)
    {
        previous.push_back(-1) ;
        if(i)
        {
            a.vId = i ;
            a.currentWeight = LARGE_NUMBER ;
            yes.push(a) ;
        }
    }

    while(!yes.empty())
    {
        DistanceVertexIdPair x = yes.top() ;
        if(x.currentWeight == LARGE_NUMBER && previous[x.vId] == -1) return false;
        if(x.vId == vertids[1])
            break ;
        yes.pop() ;
        for(int j = 0; j < s2; j++)
        {
            if( ( edgeList[j].vertexId0 == x.vId || edgeList[j].vertexId1 == x.vId ) && ( edgeList[j].masked == false ) )
            {
                if (edgeList[j].vertexId0 == x.vId)
                    id = edgeList[j].vertexId1 ;
                if (edgeList[j].vertexId1 == x.vId)
                    id = edgeList[j].vertexId0 ;
                new_weight = x.currentWeight + edgeList[j].weight ;
                if (new_weight < w[id])
                {    
                    DistanceVertexIdPair c (id, new_weight) ;
                    w[id] = new_weight ;
                    previous[id] = x.vId ;
                    yes.push(c) ;
                }
            }
        }
    }

    id = vertids[1] ;
    while(1)
    {
        pathh.push_back(id) ;
        if(id == vertids[0])
            break ;
        id = previous[id] ;
    }
    
    s = pathh.size() ;
    for(int i = s-1; i >= 0; i--)
    {
        orderedVertexIdList.push_back(pathh[i]) ;
    }
    
    return true ;



}

int Graph::MultipleShortPaths(std::vector<std::vector<int> >& orderedVertexIdList,
                              const std::string& from,
                              const std::string& to,
                              int numberOfShortestPaths)
{

    int k = 1, s2, max, s = edgeList.size(), id  ;
    std::vector<int> ok ;
    this -> UnMaskAllEdges() ;
    if (!this -> ShortestPath(ok, from, to))
        return 0 ;
    orderedVertexIdList.push_back(ok) ;
    for(k = 1; k < numberOfShortestPaths; k++)
    {
        s2 = orderedVertexIdList[k-1].size() ;
        max = - LARGE_NUMBER ;
        for(int i = 0; i < s; i++)
        {
            for(int j = 0; j < s2 - 1; j++)
            {
                
                if( (edgeList[i].vertexId0 == orderedVertexIdList[k-1][j] && edgeList[i].vertexId1 == orderedVertexIdList[k-1][j+1]) || (edgeList[i].vertexId0 == orderedVertexIdList[k-1][j+1] && edgeList[i].vertexId1 == orderedVertexIdList[k-1][j]) )
                {
                    if (edgeList[i].weight > max) 
                    {
                        max = edgeList[i].weight ;
                        id = i ;
                        break ;
                    }
                }
            }
        }
        edgeList[id].masked = true ;
        ok.clear() ;
        id = this -> ShortestPath(ok, from, to) ;
        if(!id)
            break ;
      
        else
            orderedVertexIdList.push_back(ok) ;
    }
    this -> UnMaskAllEdges() ;
    return k ;
}

void Graph::MaskEdges(const std::vector<StringPair>& vertexNames)
{

    int vertids[2] ;
    int s = vertexNames.size(), s2 = vertexList.size(), s3 = edgeList.size(), found;
    for(int i = 0; i < s; i++)
    {
        found = 0 ;
        for(int j = 0; j < s2; j++)
        {
            if(vertexList[j].name == vertexNames[i].s0 || vertexList[j].name == vertexNames[i].s1) 
            {
                vertids[found] = j ;
                found++ ;
            }
        }
        if(found != 2) throw VertexNotFoundException() ; 

        for(int i = 0; i < s3; i++)
        {
            if( (edgeList[i].vertexId0 == vertids[0] && edgeList[i].vertexId1 == vertids[1]) || (edgeList[i].vertexId1 == vertids[0] && edgeList[i].vertexId0 == vertids[1]) )
                edgeList[i].masked = true ;
        }    
    }
}

void Graph::UnMaskEdges(const std::vector<StringPair>& vertexNames)
{

    int vertids[2] ;
    int s = vertexNames.size(), s2 = vertexList.size(), s3 = edgeList.size(), found;
    for(int i = 0; i < s; i++)
    {
        found = 0 ;
        for(int j = 0; j < s2; j++)
        {
            if(vertexList[j].name == vertexNames[i].s0 || vertexList[j].name == vertexNames[i].s1) 
            {
                vertids[found] = j ;
                found++ ;
            }
        }
        if(found != 2) throw VertexNotFoundException() ; 

        for(int i = 0; i < s3; i++)
        {
            if( (edgeList[i].vertexId0 == vertids[0] && edgeList[i].vertexId1 == vertids[1]) || (edgeList[i].vertexId1 == vertids[0] && edgeList[i].vertexId0 == vertids[1]) )
                edgeList[i].masked = false ;
        }    
    }
}

void Graph::UnMaskAllEdges()
{
    
    int s = edgeList.size() ;
    for(int i = 0; i < s; i++)
    {
        edgeList[i].masked = false ;
    }
}

void Graph::MaskVertexEdges(const std::string& name)
{

    int s = vertexList.size(), found = 0, vertid ;
    GraphVertex puck ;
    for(int i = 0; i < s; i++)
    {
        if(vertexList[i].name == name) 
        {
            puck = vertexList[i] ;
            found++ ;
            vertid = i ;
        }
    }

    if(!found) throw VertexNotFoundException() ;

    int s2 = edgeList.size() ;
    for(int j = 0; j < s2; j++)
    {
        if(edgeList[j].vertexId0 == vertid || edgeList[j].vertexId1 == vertid)
            edgeList[j].masked = true ;
    }
}

void Graph::UnMaskVertexEdges(const std::string& name)
{

    int s = vertexList.size(), found = 0, vertid ;
    GraphVertex puck ;
    for(int i = 0; i < s; i++)
    {
        if(vertexList[i].name == name) 
        {
            puck = vertexList[i] ;
            found++ ;
            vertid = i ;
        }
    }

    if(!found) throw VertexNotFoundException() ;

    int s2 = edgeList.size() ;
    for(int j = 0; j < s2; j++)
    {
        if(edgeList[j].vertexId0 == vertid || edgeList[j].vertexId1 == vertid)
            edgeList[j].masked = false ;
    }
}

void Graph::ModifyEdge(const std::string& vName0,
                       const std::string& vName1,
                       float newWeight)
{
    
    int s = edgeList.size(), s2 = vertexList.size(), found = 0, vertids[2] ;
    GraphVertex bambam[2] ; 
    for(int a = 0; a < s2; a++)
    {
        if(vertexList[a].name == vName0)
        {
            bambam[0] = vertexList[a] ;
            vertids[0] = a ;
            found++ ;
        }
        if(vertexList[a].name == vName1)
        {
            bambam[1] = vertexList[a] ;
            vertids[1] = a ;
            found++ ;
        }
    }

    if(found != 2) throw VertexNotFoundException() ;

    for(int i = 0; i < s; i++)
    {
        if( (edgeList[i].vertexId0 == vertids[0] && edgeList[i].vertexId1 == vertids[1]) || (edgeList[i].vertexId1 == vertids[0] && edgeList[i].vertexId0 == vertids[1]) )
            edgeList[i].weight = newWeight ;
    }
}

void Graph::ModifyEdge(int vId0, int vId1,
                       float newWeight)
{
    
    int s = edgeList.size() ;
    for(int i = 0; i < s; i++)
    {
        if( (edgeList[i].vertexId0 == vId0 && edgeList[i].vertexId1 == vId1) || (edgeList[i].vertexId1 == vId1 && edgeList[i].vertexId0 == vId0) )
            edgeList[i].weight = newWeight ;
    }
}

void Graph::PrintAll() const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this function !   //
    // ============================= //
    for(size_t i = 0; i < vertexList.size(); i++)
    {
        const GraphVertex& v = vertexList[i];
        std::cout << v.name << "\n";
        for(int j = 0; j < v.edgeCount; j++)
        {
            int edgeId = v.edgeIds[j];
            const GraphEdge& edge = edgeList[edgeId];
            // Skip printing this edge if it is masked
            if(edge.masked)
                continue;

            // List the all vertex names and weight
            std::cout << "-" << std::setfill('-')
                             << std::setw(2) << edge.weight
                             << "-> ";
            int neigVertexId = (static_cast<int>(i) == edge.vertexId0)
                                 ? edge.vertexId1
                                 : edge.vertexId0;
            std::cout << vertexList[neigVertexId].name << "\n";
        }
    }
    // Reset fill value because it "sticks" to the std out
    std::cout << std::setfill(' ');
    std::cout.flush();
}

void Graph::PrintPath(const std::vector<int>& orderedVertexIdList,
                      bool sameLine) const
{
    // ============================= //
    // This function is implemented  //
    // Do not edit this file !       //
    // ============================= //
    for(size_t i = 0; i < orderedVertexIdList.size(); i++)
    {
        int vertexId = orderedVertexIdList[i];
        if(vertexId >= static_cast<int>(vertexList.size()))
            throw VertexNotFoundException();

        const GraphVertex& vertex = vertexList[vertexId];
        std::cout << vertex.name;
        if(!sameLine) std::cout << "\n";
        // Only find and print the weight if next is available
        if(i == orderedVertexIdList.size() - 1) break;
        int nextVertexId = orderedVertexIdList[i + 1];
        if(nextVertexId >= static_cast<int>(vertexList.size()))
            throw VertexNotFoundException();

        // Find the edge between these two vertices
        int edgeId = INVALID_INDEX;
        if(vertexId     < static_cast<int>(vertexList.size()) &&
           nextVertexId < static_cast<int>(vertexList.size()))
        {
            // Check all of the edges of vertex
            // and try to find
            const GraphVertex& fromVert = vertexList[vertexId];
            for(int i = 0; i < fromVert.edgeCount; i++)
            {
                int eId = fromVert.edgeIds[i];
                // Since the graph is not directional
                // check the both ends
                if((edgeList[eId].vertexId0 == vertexId &&
                    edgeList[eId].vertexId1 == nextVertexId)
                ||
                   (edgeList[eId].vertexId0 == nextVertexId &&
                    edgeList[eId].vertexId1 == vertexId))
                {
                    edgeId = eId;
                    break;
                }
            }
        }
        if(edgeId != INVALID_INDEX)
        {
            const GraphEdge& edge = edgeList[edgeId];
            std::cout << "-" << std::setfill('-')
                      << std::setw(2)
                      << edge.weight << "->";
        }
        else
        {
            std::cout << "-##-> ";
        }
    }
    // Print endline on the last vertex if same line is set
    if(sameLine) std::cout << "\n";
    // Reset fill value because it "sticks" to the std out
    std::cout << std::setfill(' ');
    std::cout.flush();
}

int Graph::TotalVertexCount() const
{
    return vertexList.size() ;
}

int Graph::TotalEdgeCount() const
{
    return edgeList.size() ;
}

std::string Graph::VertexName(int vertexId) const
{
    if(vertexId >= vertexList.size()) return "" ;
    else return vertexList[vertexId].name ;
}

int Graph::TotalWeightInBetween(std::vector<int>& orderedVertexIdList)
{

    int total = 0, s = edgeList.size(), s2 = orderedVertexIdList.size() ;
    for(int i = 0; i < s; i++)
    {
        for(int j = 0; j < s2 - 1; j++)
        {
            if( (edgeList[i].vertexId0 == orderedVertexIdList[j] && edgeList[i].vertexId1 == orderedVertexIdList[j+1]) || (edgeList[i].vertexId0 == orderedVertexIdList[j+1] && edgeList[i].vertexId1 == orderedVertexIdList[j]) )
            {
                total += edgeList[i].weight ;
                break ;
            }
        }
    }

    return total ;
}