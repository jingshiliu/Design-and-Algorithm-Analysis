//
//  main.cpp
//  Exam1_coding
//
//  Created by Jingshi Liu on 3/17/22.
//

#include <iostream>
#include <fstream>
using namespace std;

class BucketSort{
public:
    int* array;
    int maxInt;
    int data;
    BucketSort(int maxNum){
        maxInt = maxNum;
        array = new int[maxInt+1];
        for(int i = 0; i < maxInt+1; i++)
            array[i] = 0;
    }
    
    static int findLargest(ifstream& inFile){
        int data, maxInt = 0;
        while(inFile >> data){
            if(data > maxInt)
                maxInt = data;
        }
        return maxInt;
    }
    
    void printBucket(ofstream& outFile, int index){
        for(int i = 0; i < array[index]; i++){
            outFile << index << " ";
        }
    }
    
};



int main(int argc, const char * argv[]) {
    ifstream inFile;
    ofstream outFile;
    inFile.open(argv[1]);
    outFile.open(argv[2]);
    
    int maxInt = BucketSort::findLargest(inFile);
    
    BucketSort *bucketSort = new BucketSort(maxInt);
    
    inFile.close();
    inFile.open(argv[1]);
    
    int data;
    while(inFile >> data){
        bucketSort->array[data]++;
    }
    
    for(int index = 0; index <= bucketSort->maxInt; index++){
        if(bucketSort->array[index] > 0){
            bucketSort->printBucket(outFile, index);
        }
    }
    
    inFile.close();
    outFile.close();
    
    return 0;
}
