#include <iostream>
#include <fstream>
using namespace std;

class AstarNode{
public:
    string configuration;
    int gStar;
    int hStar;
    int fStar;
    AstarNode* parent = NULL;
    AstarNode* next = NULL;

    AstarNode(string configuration, AstarNode* parent, AstarNode* next){
        this->configuration = configuration;
        this->parent = parent;
        this->next = next;
        this->gStar = 0;
        this->hStar = 0;
        this->fStar = 9999999;
    }

    string printNode(){
        string res = "(" + to_string(fStar) + " :: " + configuration + " :: ";
        if(parent == NULL){
            res += "NULL)";
        }else
            res += parent->configuration + ")";
        return res;
    }
};

class AStar{
public:
AstarNode *startNode;
AstarNode *goalNode;
AstarNode *open;
AstarNode *close;
AstarNode *childList;
int table[9][9] = {
    {0, 1, 2, 1, 2, 3, 2, 3, 4},
    {1, 0, 1, 2, 1 ,2, 3, 2, 3},
    {2, 1, 0, 3, 2, 1, 4, 3, 2},
    {1, 2, 3, 0, 1, 2, 1, 2, 3},
    {2, 1, 2, 1, 0, 1, 2, 1, 2},
    {3, 2, 1, 2, 1, 0, 3, 2, 1},
    {2, 3, 4, 1, 2, 3, 0, 1, 2},
    {3, 2, 3, 2, 1, 2, 1, 0, 1},
    {4, 3, 2, 3, 2 ,1, 2, 1, 0}
    };

AStar(AstarNode* startNode, AstarNode* goalNode){
    this->startNode = startNode;
    this->goalNode = goalNode;
    this->open = new AstarNode("open", NULL, NULL);
    this->close = new AstarNode("close", NULL, NULL);
    this->childList = new AstarNode("childList", NULL, NULL);

}

int computeGstar(AstarNode *node){
    return node->parent->gStar+1;
}

int computeHstar(AstarNode *node){
    int totalMoves = 0;
    for(int i = 0; i < 9; i++){
        if(node->configuration[i] != goalNode->configuration[i]){
            for(int j = 0; j < 9; j++){
                if(goalNode->configuration[j] == node->configuration[i]){
                    totalMoves += table[i][j];
                    break;
                }
            }
        }
    }

    return totalMoves;

}

bool isGoalNode(AstarNode *node){
    return node->configuration == goalNode->configuration;
}

void openInsert(AstarNode *node){
    if(!open->next){
        open->next = node;
        return;
    }
        
    AstarNode *spot = open;
    
    while(spot->next && spot->next->fStar < node->fStar)
        spot = spot->next;
    
    if(!spot->next){
        spot->next = node;
        return;
    }
        
    node->next = spot->next;
    spot->next = node;
}

void closeInsert(AstarNode *node){
    AstarNode *spot = close;
    while(spot->next != nullptr && spot->next->fStar < node->fStar)
        spot = spot->next;
    
    node->next = spot->next;
    spot->next = node;
}

AstarNode* removeOpen(){
    if(open->next == NULL)
        return NULL;
    AstarNode* res = open->next;
    open->next = open->next->next;
    return res;
}

// search if child is in CLOSE list. If true, return the node before child
// else return null
AstarNode* findChildInCloseList(AstarNode *child){
    AstarNode *spot = this->childList;
    while(spot->next){
        if(spot->configuration == child->configuration)
            return spot;
        spot = spot->next;
    }
    return NULL;
}

void closeDelete(AstarNode *spot){
    if(spot->next == NULL)
        return;
    AstarNode* target = spot->next;
    spot->next = spot->next->next;
    target->next = NULL;
}

bool match(string configuration, string configuration2){
    return configuration == configuration2;
}

bool checkAncestors(AstarNode *currentNode){
    AstarNode *ancestor = currentNode->parent;
    while(true){
        if(ancestor->configuration == currentNode->configuration)
            return true;
        if(ancestor->configuration == startNode->configuration)
            return false;
        ancestor = ancestor->parent;
    }
}

AstarNode* constructChildList(AstarNode *currentNode){
    int index;
    for(int i = 0; i < currentNode->configuration.length(); i++){
        if(currentNode->configuration[i] == toascii('0')){
            index = i;
            break;
        }
    }
    int sizeOfArray = sizeof(table[index])/sizeof(table[index][0]);
    string configuration;
    AstarNode *newNode;
    for(int i = 0; i < sizeOfArray; i++){
        if(table[index][i] == 1){
            configuration = "";
            for(int j = 0; j < sizeOfArray; j++){
                if(j == i)
                    configuration += "0";
                else if(j == index)
                    configuration += (char)currentNode->configuration[i];
                else
                    configuration += (char)currentNode->configuration[j];
            }

            newNode = new AstarNode(configuration, currentNode, NULL);
            if(!checkAncestors(newNode)){
                newNode->next = childList->next;
                childList->next = newNode;
            }
        }
    }

    return childList;
}

AstarNode* popChildList(){
    if(childList->next == NULL)
        return NULL;
    AstarNode* res = childList->next;
    childList->next = childList->next->next;
    res->next = NULL;
    return res;
}

void printList(string list, ofstream& outFile){
    AstarNode* pointer;
    if(list == "open")
        pointer = this->open;
    else
        pointer = this->close;

    while(pointer){
        outFile << pointer->printNode() + " --> ";
        pointer = pointer->next;
    }
    outFile << "\n";
}

void printSolution(AstarNode* currentNode, ofstream& outFile){
    
    string out = "";
    AstarNode *cur = currentNode;
    
    while(cur->parent){
        out = cur->printNode() + " --> " + out;
        cur = cur->parent;
    }
    
    out = "Make moves by following bottom configurations would give the shortest path.\n " + out;
    
    outFile << out;
}



};

int main(int argc, const char *argv[])
{
    ifstream inFile1, inFile2;
    ofstream outFile1, outFile2;
    inFile1.open(argv[1]);
    inFile2.open(argv[2]);
    outFile1.open(argv[3]);
    outFile2.open(argv[4]);

    string initalConfiguration = "", goalConfiguration = "";
    string val = "";
    while(inFile1 >> val){
        initalConfiguration += val;
    }
    while(inFile2 >> val)
        goalConfiguration += val;
    
    
    AstarNode *startNode = new AstarNode(initalConfiguration, NULL, NULL);
    AstarNode *goalNode = new AstarNode(goalConfiguration, NULL, NULL);
    AStar* aStarSearch = new AStar(startNode, goalNode);

    startNode->gStar = 0;
    startNode->hStar = aStarSearch->computeHstar(startNode);
    startNode->fStar = startNode->gStar + startNode->hStar;
    aStarSearch->openInsert(startNode);

    int debugPrints = 0;
    AstarNode* currentNode;
    AstarNode *child, *spot;
    while(aStarSearch->open->next){
        
        currentNode = aStarSearch->removeOpen();
        if(aStarSearch->isGoalNode(currentNode)){
            outFile2 << "Found a solution" << endl;
            aStarSearch->printSolution(currentNode, outFile2);
            break;
        }

        //OPEN INSERT:  add currentNode's children that are not visted or has smaller fStar if visited
        aStarSearch -> constructChildList(currentNode);

        while(aStarSearch->childList->next){
            child = aStarSearch->popChildList();
            child->gStar = aStarSearch->computeGstar(child);
            child->hStar = aStarSearch->computeHstar(child);
            child->fStar = child->gStar + child->hStar;

            spot = aStarSearch->findChildInCloseList(child);
            if(spot && spot->next && spot->next->fStar > child->fStar){
                aStarSearch->closeDelete(spot);
            }
            aStarSearch->openInsert(child);
        }

        aStarSearch->closeInsert(currentNode);
        if(debugPrints < 20){
            
            outFile1 << "\n\n\nOpen List: ";
            aStarSearch->printList("open", outFile1);

            outFile1 << "\n\nClose List: ";
            aStarSearch->printList("close", outFile1);
            debugPrints++;
        }
    }

    if(aStarSearch->open->next == NULL && !aStarSearch->isGoalNode(currentNode))
        outFile1 << "No Solution Found!!! \n";
    
    inFile1.close();
    inFile2.close();
    outFile1.close();
    outFile2.close();

    return 0;
}
