//
//  main.cpp
//  320_Project2
//
//  Created by Jingshi Liu on 2/15/22.
//

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fstream>
using namespace std;

class ListNode{
public:
    int data;
    ListNode* next;
    
    ListNode(int data){
        this->data = data;
        next = NULL;
    }
    
    string printNode(){
        string res = "(" + to_string(this->data) + ", ";
        if (this->next == NULL)
            res += "NULL) -> ";
        else
            res += to_string(this->next->data) + ") -> ";
        return res;
    }
};

class HashTable{
public:
    char op;
    int data;
    int bucketSize;
    ListNode** hashTable;
    
    HashTable(int bucketSize){
        this->bucketSize = bucketSize;
        hashTable = new ListNode*[this->bucketSize];
        for(int i = 0; i < this->bucketSize; i++)
            hashTable[i] = new ListNode(-99999);
    }
    
    int hashInt(int number){
        int i = 1;
        int sum = 0;
        while(number > 0){
            sum += number % 10 * i;
            number /= 10;
            i++;
        }
        return sum%this->bucketSize;
    }
    
    void informationProcess(ifstream &inFile, ofstream &outFile, ofstream &debuggingFile){
        debuggingFile << "*** enter informationProcessing method."<<endl;
        int index = 0;
        while(inFile >> op && inFile >> data){
                
            debuggingFile <<"<op int> " << op << " " << data << "    ";
            
            index = hashInt(data);
            debuggingFile <<"<index> " << index << endl;
            printList(index, outFile);
            if(op == '-')
                hashDelete(index, outFile, debuggingFile);
            else if(op == '+')
                hashInsert(index, outFile, debuggingFile);
            else if(op == '?')
                hashRetrieval(index, outFile, debuggingFile);
            else
                outFile << "op is an unknown operation."<<endl;
            
            outFile << "After one op."<<endl;
            printList(index, outFile);
        }
    }
    
    void hashInsert(int index, ofstream& outFile, ofstream& debuggingFile){
        debuggingFile << "*** enter hashInsert method." << endl;
        ListNode* spot = findSpot(index);
        if(spot->next != NULL && spot->next->data == data)
            outFile << "HashTable already contains this data, no insertion needed."<<endl;
        else{
            ListNode* newNode = new ListNode(data);
            newNode->next = spot->next;
            spot->next = newNode;
        }
        debuggingFile << "Returning after hashInsert operation ..."<<endl;
        
    }
    
    void hashDelete(int index, ofstream& outFile, ofstream& debuggingFile){
        debuggingFile << "** Enter hashDelete method." << endl;
        ListNode* spot = findSpot(index);
        if(spot->next == NULL || spot->next->data != data){
            // if spot->next is NULL or ( spot->next is not NULL and next's data is not data
            outFile<< "***Warning*** Data is not in the Database"<<endl;
        }else{
            ListNode* newNode = spot->next;
            spot->next = newNode->next;
            newNode->next = nullptr;
        }
        debuggingFile << "Returning after hashDelete operation ... "<<endl;
    }
    
    void hashRetrieval(int index, ofstream& outFile, ofstream& debuggingFile){
        debuggingFile << "** Enter hashRetrieval."<<endl;
        ListNode* spot = findSpot(index);
        if(spot->next == NULL || spot->next->data != data){
            outFile<< "***Warning*** Data is not in the Database"<<endl;
        }else{
            outFile<<"*** Yes, the data is in the hashTable!"<<endl;
        }
        debuggingFile << "Returning after hashRetrieval operation ...  "<<endl;
    }
    
    ListNode* findSpot(int index){
        ListNode* spot = hashTable[index];
        while(spot->next != NULL && spot->next->data < data){
            spot = spot->next;
        }
        return spot;
    }
    
    void printList(int index, ofstream &outFile){
        ListNode* head = hashTable[index];
        outFile << "HashTable["<<index<<"]: ";
        while(head != NULL){
            outFile << head->printNode();
            head = head->next;
        }
        outFile << "NULL"<<endl;
    }
    
    void printHashTable(ofstream& outFile){
        for(int i = 0; i < bucketSize; i++)
            printList(i, outFile);
    }
    
};

int main(int argc, const char * argv[]) {
    int bucketSize;
    ifstream inFile;
    ofstream outFile, debuggingFile;
    
    if(argc != 5){
        cout<<"Please pass in two inFiles and two outFiles."<<endl;
    }else{
        inFile.open(argv[1]);
        bucketSize = atoi(argv[2]);
        outFile.open(argv[3]);
        debuggingFile.open(argv[4]);
    }
    HashTable* hashTable = new HashTable(bucketSize);
    hashTable->informationProcess(inFile, outFile, debuggingFile);
    hashTable->printHashTable(outFile);
    inFile.close();
    outFile.close();
    debuggingFile.close();
        
}
