//
//  main.cpp
//  323Final_Q1
//
//  Created by Jingshi Liu on 5/19/22.
//

#include <iostream>
#include <fstream>
using namespace std;

class ListNode{
public:
    string data;
    ListNode* next;
    
    ListNode(string data, ListNode*next){
        this->data = data;
        this-> next = next;
    }
    
    string printNode(){
        string res = "(";
        res += data + ", ";
        
        if(next){
            res += next->data + ") -> ";
        }else
            res += "NULL) -> ";
        
        return res;
    }
};

class HashTable{
public:
    string data;
    int bucketsize;
    ListNode** hashtable;
    
    HashTable(int bucketsize){
        this->data = "";
        this->bucketsize = bucketsize;
        hashtable = new ListNode*[this->bucketsize];
        for(int i = 0; i < this->bucketsize; i++)
            hashtable[i] = new ListNode("dummy", NULL);
    }
    
    int hashString(string data){
        unsigned int val = 1;
        
        for(int i = 0; i < data.length(); i++){
            val = val*32 + (int)data[i];
        }
        return val % bucketsize;
    }
    
    void hashInsert(int index, string data, ofstream& outFile1, ofstream& outFile2){
        outFile2 << "***enter hashInsert method ***" <<endl;
        ListNode* spot = findSpot(index, data);
        
        if(spot->next && spot->next->data == data)
            outFile1 << "*** Data is already in the hashtable, no insertion takes place" <<endl;
        else{
            ListNode* newNode = new ListNode(data, NULL);
            newNode->next = spot->next;
            spot->next = newNode;
        }
    }
    
    ListNode* findSpot(int index, string data){
        ListNode* spot = hashtable[index];
        
        while(spot->next && spot->next->data < data){
            spot = spot->next;
        }
        
        return spot;
    }
    
    void printList(int index, ofstream& outFile){
        outFile<<"HashTable["<<index<<"]: ";
        ListNode *pointer = hashtable[index];
        string printing = "";
        while(pointer){
            printing += pointer->printNode();
            pointer = pointer->next;
        }
        outFile << printing << endl;
    }
    
    void printHashTable(ofstream& outFile){
        for(int i = 0; i < bucketsize; i++){
            printList(i, outFile);
        }
        outFile<<endl;
    }
};

int main(int argc, const char * argv[]) {
    ifstream inFile;
    ofstream outFile, debuggingFile;
    inFile.open(argv[1]);
    outFile.open(argv[2]);
    debuggingFile.open(argv[3]);
    
    int bucketsize = 29;
    HashTable* hashtable = new HashTable(bucketsize);
    string currentData = "";
    int index = 0;
    
    while(inFile >> currentData){
        index = hashtable->hashString(currentData);
        hashtable->hashInsert(index, currentData, outFile, debuggingFile);
        hashtable->printList(index, debuggingFile);
    }
    outFile <<endl<<endl<<endl<< "Final print: "<<endl;
    hashtable->printHashTable(outFile);
    
    inFile.close();
    outFile.close();
    debuggingFile.close();

    return 0;
}
