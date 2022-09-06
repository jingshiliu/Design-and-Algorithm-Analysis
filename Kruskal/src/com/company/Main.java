package com.company;

import java.io.*;
import java.util.Scanner;


class Edge{
    int Ni, Nj, cost;
    Edge next;

    Edge(int n1, int n2, int cost){
        Ni = n1;
        Nj = n2;
        this.cost = cost;
        next = null;
    }

    String printEdge(){
        return "<" + Ni + ", " + Nj + ", " + cost + "> —> ";
    }
}


class KruskalMST{
    int num_nodes;
    int[] which_set;
    int num_sets;
    int total_MST_cost;
    Edge edge_head = new Edge(0,0,0);
    Edge mst_head = new Edge(0,0,0);

    KruskalMST(int numNodes){
        num_nodes = numNodes;
        which_set =  new int[num_nodes+1];
        for (int i = 0; i < which_set.length; i++) {
            which_set[i] = i;
        }
        num_sets = num_nodes;
    }

    void insert(Edge new_edge){
        Edge spot = edge_head;

        while(spot.next != null && spot.next.cost < new_edge.cost){
            spot = spot.next;
        }

        new_edge.next = spot.next;
        spot.next = new_edge;
    }

    Edge removeEdge(){
        if(edge_head.next == null)
            return null;

        Edge res = edge_head.next;
        edge_head.next = edge_head.next.next;
        res.next = null;

        return res;
    }

    void mergeSets(int Ni, int Nj){
        int set_of_Ni = which_set[Ni], set_of_Nj = which_set[Nj];

        if( set_of_Ni > set_of_Nj)
            for (int i = 0; i < which_set.length; i++) {
                if(which_set[i] == set_of_Ni)
                    which_set[i] = set_of_Nj;
            }
        else
            for (int i = 0; i < which_set.length; i++) {
                if(which_set[i] == set_of_Nj)
                    which_set[i] = set_of_Ni;
            }
    }

    void push(Edge nextEdge){
        nextEdge.next = mst_head.next;
        mst_head.next = nextEdge;
    }

    void printArray(BufferedWriter outFile) throws IOException {
        String res = "Which Set: ";

        for(int i: which_set)
            res += i + " ";
        outFile.write(res + "\n");
    }

    void printList(BufferedWriter outFile, Edge head) throws IOException {
        Edge pointer;
        String res = "";

        if(head == edge_head){
            pointer = edge_head;
            res += "Edge List: ";
        }else {
            pointer = mst_head;
            res += "MST List: ";
        }

        while(pointer != null){
            res += pointer.printEdge();
            pointer = pointer.next;
        }

        outFile.write(res + " null\n");
    }

}

public class Main {

    public static void main(String[] args) throws IOException {

	    Scanner input = new Scanner(new File(args[0]));

        BufferedWriter outFile1 = new BufferedWriter(new FileWriter(args[1], true));
        BufferedWriter outFile2 = new BufferedWriter(new FileWriter(args[2], true));



        int num_nodes = input.nextInt();
        KruskalMST kruskalMST = new KruskalMST(num_nodes);

        outFile2.write("Reading:\n");

        int n1, n2, cost;
        try{
            while(input.hasNextLine()){
                n1 = input.nextInt();
                n2 = input.nextInt();
                cost = input.nextInt();

                kruskalMST.insert(new Edge(n1, n2, cost));
                kruskalMST.printList(outFile2, kruskalMST.edge_head);
            }
        }catch (Exception ignored){
        }


        outFile2.write("\n\nConstructing Minimum Spanning Tree:\n");
        Edge nextEdge;
        while(kruskalMST.num_sets > 1){
            do{
                nextEdge = kruskalMST.removeEdge();
            }while(nextEdge != null && kruskalMST.which_set[nextEdge.Ni] == kruskalMST.which_set[nextEdge.Nj]);

            if(nextEdge == null)
                break;

            kruskalMST.push(nextEdge);
            kruskalMST.total_MST_cost += nextEdge.cost;
            kruskalMST.mergeSets(nextEdge.Ni, nextEdge.Nj);
            kruskalMST.num_sets--;

            kruskalMST.printArray(outFile2);
            kruskalMST.printList(outFile2, kruskalMST.edge_head);
            kruskalMST.printList(outFile2, kruskalMST.mst_head);
            outFile2.write("\n");
        }
        
        kruskalMST.printList(outFile1, kruskalMST.mst_head);
        outFile1.write("Total cost is: " + kruskalMST.total_MST_cost);

        outFile1.close();
        outFile2.close();
    }
}
