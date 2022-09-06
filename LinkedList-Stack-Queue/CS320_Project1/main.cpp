//
//  main.cpp
//  CS320_Project1
//
//  Created by Jingshi Liu on 2/8/22.
//

#include <stdio.h>
#include <fstream>
using namespace std;

class ListNode{
public:
    int data;
    ListNode* next;
    
    ListNode(int data){
        this->data = data;
        next = nullptr;
    }
    
    string printNode(){
        string answer = "";
        answer += "(" + to_string(data) + ", ";
        if(next == NULL){
            answer += "NULL)->";
        }else{
            answer += to_string(next->data) + ")->";
        }
        return answer;
    }
};


class Stack{
public:
    ListNode* top;
    
    Stack(){
        top = new ListNode(-99999);//top points to a new Node(Dummy) which holds -99999
    }
    
    void push(ListNode *newNode){
        newNode->next = top->next;
        top->next = newNode;
    }
    
    bool isEmpty(){
        return top->next == nullptr;
    }
    
    ListNode* pop(){
        if(!isEmpty()){
            ListNode *temp = top->next;
            top->next = temp->next;
            temp->next = NULL;
            return temp;
        }
        return NULL;
    }
    
    void buildStack(ifstream &inFile){
        int data;
        while(!inFile.eof()){
            inFile >> data;
            push(new ListNode(data));
        }
    }
};


class Queue{
public:
    ListNode* head;
    ListNode* tail;
    
    Queue(){
        head = new ListNode(-99999);
        tail = head;
    }
    
    bool isEmpty(){
        return head -> next == nullptr;
    }
    
    void insert(ListNode *newNode){
        tail->next = newNode;
        tail = newNode;
    }
    
    ListNode* deleteQ(ofstream &outFile){
            if(!isEmpty()){
                ListNode *temp = head->next;
                head->next = temp->next;
                temp->next = NULL;
                if (isEmpty())
                    tail = head;
                return temp;
            }else{
                outFile<<"Queue is empty."<<endl;
                return NULL;
            }
            
    }
    
    void buildQueue(Stack* stack, ofstream& outFile){
        ListNode *listhead;
        int data;
        while(!stack->isEmpty()){
            listhead = stack->pop();
            data = listhead->data;
            outFile << data << endl;
            insert(new ListNode(data));
        }
    }
};


class LinkedList{
public:
    ListNode* head;
    
    LinkedList(){
        head = new ListNode(-99999);
    }
    
    void insertOneNode(ListNode* spot, ListNode* newNode){
        newNode->next = spot->next;
        spot->next = newNode;
    }
    
    ListNode* findSpot(ListNode* newNode){
        ListNode *spot = head;
        
        while(spot->next != NULL && spot->next->data < newNode->data)
            spot = spot->next;
        return spot;
    }
    
    void buildList(Queue* queue, ofstream &outFile){
        
        ListNode *newNode;
        ListNode* spot;
        while(!queue->isEmpty()){
            newNode = queue->deleteQ(outFile);
            outFile << newNode->data<<endl;
            spot = findSpot(newNode);
            insertOneNode(spot, newNode);
        }
    }
    
    void printList(ofstream &outFile){
        ListNode* listHead = head;
        string nodeString = "";
        outFile << "ListHead ->";
        while(listHead != NULL){
            nodeString = listHead->printNode();
            outFile << nodeString;
            listHead = listHead->next;
        }
        outFile << "NULL";
    }
};



int main(int argc, const char * argv[]) {
    ifstream inFile;
    ofstream outFile1, outFile2, outFile3;
    if (argc != 5){
        printf("Need one inFile and three outfiles\n");
    }else{
        inFile.open(argv[1]);
        outFile1.open(argv[2]);
        outFile2.open(argv[3]);
        outFile3.open(argv[4]);
    }
    Stack *stack = new Stack();
    stack->buildStack(inFile);
    Queue *queue = new Queue();
    queue->buildQueue(stack, outFile1);
    LinkedList *list = new LinkedList();
    list->buildList(queue, outFile2);
    list->printList(outFile3);
    return 0;
}


