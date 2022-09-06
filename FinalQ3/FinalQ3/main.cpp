//
//  main.cpp
//  FinalQ3
//
//  Created by Jingshi Liu on 5/19/22.
//

#include <iostream>
#include <fstream>
using namespace std;

class Q3{
public:
    int numNodes;
    int **adjMatrix;
    int *valAry;
    
    Q3(int numberOfNodes){
        this->numNodes = numberOfNodes;
        adjMatrix = new int*[this->numNodes+1];
        for(int i = 0; i < this->numNodes+1; i++){
            adjMatrix[i] = new int[numNodes+1];
            for(int j = 0; j <= numNodes; j++){
                adjMatrix[i][j] = 0;
            }
            adjMatrix[i][i] = 1;
        }
        valAry = new int[numNodes+1];
        valAry[0] = 1;
        for (int i = 1; i <= numNodes; i++) {
            valAry[i] = 0;
        }
    }
    
    void loadMatrix(ifstream& inFile){
        int row = 0;
        int col = 0;
        while(inFile >> row && inFile >> col){
            adjMatrix[row][col] = 1;
        }
    }
    
    bool validCheck(int nextNode, int newVal){
        for (int i = 0; i <= numNodes+1; i++) {
            if(adjMatrix[nextNode][i] == 1 && valAry[i] == newVal)
                return false;
        }
        return true;
    }
    
    bool isDone(){
        for (int i = 1; i <= numNodes; i++) {
            if(valAry[i] == 0)
                return false;
        }
        return true;
    }
    
    void printAry(ofstream& outFile){
        for(int i = 1; i <= numNodes; i++){
            outFile<< i <<" "<<(char)valAry[i]<<" "<<endl;
        }
        outFile<<endl<<endl<<endl;
    }
    
};

int main(int argc, const char * argv[]) {
    ifstream inFile;
    ofstream outFile, debug;
    inFile.open(argv[1]);
    outFile.open(argv[2]);
    debug.open(argv[3]);
    
    int numNodes = 0;
    inFile >> numNodes;
    Q3* q3 = new Q3(numNodes);
    
    int newVal = 65;
    
    q3->loadMatrix(inFile);
    
    while(!q3->isDone()){
        for(int nextNode = 1; nextNode <= q3->numNodes; nextNode++)
            if(q3->valAry[nextNode] == 0 && q3->validCheck(nextNode, newVal))
                    q3->valAry[nextNode] = newVal;
        q3->printAry(debug);
        newVal++;
    }
    
    q3->printAry(outFile);
    
    inFile.close();
    outFile.close();
    debug.close();
    
    return 0;
}
