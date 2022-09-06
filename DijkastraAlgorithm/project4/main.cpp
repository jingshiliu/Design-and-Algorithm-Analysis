//
//  main.cpp
//  320Project 4
//
//  Created by Jingshi Liu on 3/7/22.
//

#include <iostream>
#include <fstream>
using namespace std;


class DijkstraSSS{
public:
    int numNodes;
    int sourceNode;
    int minNode;
    int currentNode;
    int newCost;
    int** costMatrix;
    int* parentArray;
    int* todoArray;
    int* bestArray;
    
    DijkstraSSS(int num_nodes){
        numNodes = num_nodes;
        // Allocate rows of costMatrix
        costMatrix = new int*[numNodes+1];
        // Allocate memory for 3 arrays below
        parentArray = new int[numNodes+1];
        todoArray = new int[numNodes+1];
        bestArray = new int[numNodes+1];
        for(int i = 0; i  < numNodes+1; i++){
            //Current pointer in 1D array now pointing to newly allocated 1D array, this makes costMatrix a 2D array
            costMatrix[i] = new int[numNodes+1];
            // Initialize arrays below
            todoArray[i] = 1;
            bestArray[i] = 99999;
            for(int j = 0; j < numNodes+1; j++){
                costMatrix[i][j] = 99999;
            }
            // Change the diagonal index in the matrix to 0
            costMatrix[i][i] = 0;
        }
    }
    
    void loadCostMatrix(ifstream& inFile){
        int startingVertex, endingVertex, cost;
        while(inFile >> startingVertex && inFile >> endingVertex && inFile >> cost){
            costMatrix[startingVertex][endingVertex] = cost;
        }
    }
    
    void setBestArray(){
        for(int i = 1; i < numNodes+1; i++)
            bestArray[i] = costMatrix[sourceNode][i];
        bestArray[0] = 99999;
    }
    
    void setParentArray(){
        for(int i = 0; i < numNodes+1; i++)
            parentArray[i] = sourceNode;
    }
    
    void setToDoArray(){
        for(int i = 1; i < numNodes+1; i++)
            todoArray[i] = 1;
        todoArray[sourceNode] = 0;
        todoArray[0] = 0;
    }
    
    int findMinNode(){
        // Min cost by default is 99999 bc best bestArray[0] was initialized to 99999
        int minNode = 0;
        for(int i = 1; i < numNodes+1; i++){
            if(todoArray[i] == 1 && bestArray[i] < bestArray[minNode])
                minNode = i;
        }
        todoArray[minNode] = 0;
        this->minNode = minNode;
        return minNode;
    }
    
    int computeCost(int node){
        this->newCost = bestArray[minNode] + costMatrix[minNode][node];
        return this->newCost;
    }
    
    bool checkToDoArray(){
        for(int i = 1; i < numNodes+1; i++)
            if(todoArray[i] == 1)
                return true;
        return false;
    }
    
    void debuggingPrint(ostream& debuggingFile){
        debuggingFile << "Source Node       : "<<sourceNode;
        debuggingFile << "\nParent Array    : ";
        for(int i = 1; i < numNodes+1; i++)
            debuggingFile << parentArray[i]<< " ";
        debuggingFile << "\nTo Do Array     : ";
        for(int i = 1; i < numNodes+1; i++)
            debuggingFile << todoArray[i] << " ";
        debuggingFile << "\nBest Cost Array : ";
        for(int i = 1; i < numNodes+1; i++)
            debuggingFile << bestArray[i] << " ";
        
        debuggingFile << "\n\n\n**************************************\n\n\n";
    }
    
    void printShortestPath(ostream& sssFile){
        int current_node = 1, cost = 0;
        string shortestPath = "";
        sssFile << "==================================================\nSource node = "<<sourceNode<<"\n\n";
        
        for(int i = 1; i < numNodes+1; i++){
            current_node = i;
            shortestPath = to_string(current_node);
            cost = 0;
            while(current_node != sourceNode){
                shortestPath = to_string(parentArray[current_node]) + " -> " + shortestPath;
                cost += costMatrix[parentArray[current_node]][current_node];
                current_node = parentArray[current_node];
            }
            sssFile << shortestPath << "   Cost = " << cost <<"\n";
            
        }
    }
};



int main(int argc, const char * argv[]) {
    ifstream inFile;
    ofstream sssFile, debuggingFile;
    DijkstraSSS* dijkstra;
    
    inFile.open(argv[1]);
    sssFile.open(argv[2]);
    debuggingFile.open(argv[3]);
    
    int numNodes = 0;
    inFile >> numNodes;
    
    dijkstra = new DijkstraSSS(numNodes);
    // Load Cost Matrix
    dijkstra->loadCostMatrix(inFile);

    for(int source_node = 1; source_node <= dijkstra->numNodes; source_node++){
        
        dijkstra->sourceNode = source_node;
        dijkstra->setBestArray();
        dijkstra->setParentArray();
        dijkstra->setToDoArray();
        
        while(dijkstra->checkToDoArray()){
            dijkstra->findMinNode();
            for(int i = 1; i <= dijkstra->numNodes; i++){
                if(dijkstra->todoArray[i] == 1 && dijkstra->bestArray[i] > dijkstra->computeCost(i)){
                    dijkstra->parentArray[i] = dijkstra->minNode;
                    dijkstra->bestArray[i] = dijkstra->newCost;
                    dijkstra->debuggingPrint(debuggingFile);
                }
            }
        }
        dijkstra->printShortestPath(sssFile);
        
    }
    inFile.close();
    sssFile.close();
    debuggingFile.close();
    
    
    return 0;
}
