//
//  main.cpp
//  CS320_Project3
//
//  Created by Jingshi Liu on 2/28/22.
//

#include <iostream>
#include <fstream>
using namespace std;

class HuffmanTreeNode{
public:
    string char_Str;
    int prob;
    string bin_code;
    HuffmanTreeNode *left;
    HuffmanTreeNode *right;
    HuffmanTreeNode *next;
    
    HuffmanTreeNode(string char_str, int prob, HuffmanTreeNode *left, HuffmanTreeNode *right, HuffmanTreeNode *next){
        this->left = left;
        this->right = right;
        this->next = next;
        this->char_Str = char_str;
        this->prob = prob;
    }
    
    // print char, prob, bin_code, next_char, left_char, right_char
    string print_huffman_node(){
        string node_info = "(" + char_Str + ", " + to_string(prob) + ", ";
        
        if(this->bin_code.length() == 0){
            node_info += " ";
        }else{
            node_info += this->bin_code + ", ";
        }
        
        if(this->next == NULL){
            node_info += "NULL, ";
        }else{
            node_info += next->char_Str + ", ";
        }
        
        if(this->left == NULL){
            node_info += "NULL, ";
        }else{
            node_info += left->char_Str + ", ";
        }
        
        if(this->right == NULL){
            node_info += "NULL)->";
        }else{
            node_info += right->char_Str + ")->";
        }
        
        return node_info;
    }
};

class HuffmanTree{
public:
    HuffmanTreeNode* listhead;
    HuffmanTreeNode* root;
    
    HuffmanTree(){
        this->listhead = new HuffmanTreeNode("dummy", 0, NULL, NULL, NULL);
        this->root = listhead;
    }
    
    HuffmanTreeNode* findSpot(HuffmanTreeNode* new_node){
        HuffmanTreeNode* spot = this->listhead;
        
        while(spot->next != NULL && spot->next->prob < new_node->prob){
            spot = spot->next;
        }
        return spot;
    }
    
    void listInsert(HuffmanTreeNode* new_node){
        HuffmanTreeNode* spot = findSpot(new_node);
        new_node->next = spot->next;
        spot->next = new_node;
    }
    
    HuffmanTreeNode* construct_huffman_linked_list(ifstream& inFile, ofstream& outFile){
        string character = "";
        int prob;
        while(inFile >> character && inFile >> prob){
            listInsert(new HuffmanTreeNode(character, prob, NULL, NULL, NULL));
            print_list(outFile);
            outFile << "\n\n";
        }
        return this->listhead;
    }
    
    HuffmanTreeNode* construct_huffman_tree(ofstream& outFile){
        while(listhead->next != NULL && listhead->next->next!= NULL){
            HuffmanTreeNode* new_node = new HuffmanTreeNode(
                                listhead->next->char_Str + listhead->next->next->char_Str,
                                listhead->next->prob + listhead->next->next->prob,
                                listhead->next,
                                listhead->next->next,
                                NULL);
            listInsert(new_node);
            listhead->next = listhead->next->next->next;
            new_node->left->next = nullptr;
            new_node->right->next = nullptr;
            print_list(outFile);
            outFile << "\n\n";
        }
        root = listhead->next;
        return root;
    }
    
    void construct_char_code(HuffmanTreeNode* current_node, string code_of_parent, ofstream& outFile){
        if(isLeaf(current_node)){
            current_node->bin_code = code_of_parent;
            outFile << current_node->char_Str + " " + current_node->bin_code<< endl;
        }else{
            construct_char_code(current_node->left, code_of_parent + "0", outFile);
            construct_char_code(current_node->right, code_of_parent + "1", outFile);
        }
    }
    
    void preorder_traversal(HuffmanTreeNode* current_node, ofstream& outFile){
        if(isLeaf(current_node))
            outFile << current_node->print_huffman_node();
        else{
            outFile << current_node->print_huffman_node();
            preorder_traversal(current_node->left, outFile);
            preorder_traversal(current_node->right, outFile);
        }
    }
    
    void inorder_traversal(HuffmanTreeNode* current_node, ofstream& outFile){
        if(isLeaf(current_node))
            outFile << current_node->print_huffman_node();
        else{
            inorder_traversal(current_node->left, outFile);
            outFile << current_node->print_huffman_node();
            inorder_traversal(current_node->right, outFile);
        }
    }
    
    void postorder_traversal(HuffmanTreeNode* current_node, ofstream& outFile){
        if(isLeaf(current_node))
            outFile << current_node->print_huffman_node();
        else{
            postorder_traversal(current_node->left, outFile);
            postorder_traversal(current_node->right, outFile);
            outFile << current_node->print_huffman_node();
        }
    }
    
    void print_list(ofstream& outFile){
        HuffmanTreeNode* head = listhead;
        
        while(head != NULL){
            outFile << head->print_huffman_node();
            head = head->next;
        }
        outFile << endl;
    }
    
    bool isLeaf(HuffmanTreeNode* node){
        return node->left == NULL && node->right == NULL;
    }
    
    
};


int main(int argc, const char * argv[]) {
    ifstream inFile;
    ofstream outFile1, outFile2;
    if(argc != 4){
        cout<< "Please pass in one data file and 2 output file"<<endl;
    }else{
        inFile.open(argv[1]);
        outFile1.open(argv[2]);
        outFile2.open(argv[3]);
    }
    HuffmanTree* huffman_tree = new HuffmanTree();
    outFile2 << "LL\n\n";
    huffman_tree->construct_huffman_linked_list(inFile, outFile2);
    outFile1 << "\n\nHuffman Linked List\n\n";
    huffman_tree->print_list(outFile1);
    
    outFile2 << "\n\nTree\n";
    huffman_tree->construct_huffman_tree(outFile2);
    
    outFile1 << "\n\nBinary Code of Each Character\n\n";
    huffman_tree->construct_char_code(huffman_tree->root, "", outFile1);
    
    outFile1 << "\n\nPreorder Traversal\n\n";
    huffman_tree->preorder_traversal(huffman_tree->root, outFile1);
    
    outFile1 << "\n\nInorder Traversal\n\n";
    huffman_tree->inorder_traversal(huffman_tree->root, outFile1);
    
    outFile1 << "\n\nPostorder Traversal\n\n";
    huffman_tree->postorder_traversal(huffman_tree->root, outFile1);
    
    inFile.close();
    outFile1.close();
    outFile2.close();
    
    return 0;
}
