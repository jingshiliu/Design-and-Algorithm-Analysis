package com.company;

import java.io.*;
import java.util.Scanner;

class QuadTreeNode{
    int color, upperRow, upperCol, size;
    QuadTreeNode NW, NE, SW, SE;

    QuadTreeNode(int upperRow, int upperCol, int size,  QuadTreeNode NW, QuadTreeNode NE, QuadTreeNode SW, QuadTreeNode SE){
        this.upperRow = upperRow;
        this.upperCol = upperCol;
        this.size = size;
        this.NW = NW;
        this.NE = NE;
        this.SW = SW;
        this.SE = SE;
    }

    void printQuadTreeNode(BufferedWriter outFile) throws IOException {
        String res = color + ", " + upperRow + ", " + upperCol + ", ";
        if(NW == null) res += "null"; else res += NW.color;
        if(NE == null) res += "null"; else res += NE.color;
        if(SW == null) res += "null"; else res += SW.color;
        if(SE == null) res += "null"; else res += SE.color;
        outFile.write(res + "\n");
    }

}

class QuadTree{
    int numRows, numCols, minVal, maxVal, power2;
    int[][] imgArray;
    QuadTreeNode root;

    QuadTree(int numRows, int numCols, int minVal, int maxVal){
        this.numRows = numRows;
        this.numCols = numCols;
        this.minVal = minVal;
        this.maxVal = maxVal;
        imgArray = new int[computePower2()][computePower2()];
    }

    int computePower2(){
        int size = Math.max(numCols, numRows), power2 = 2;

        while(size > power2)
            power2 *= 2;
        return power2;
    }

    void loadImage(Scanner inFile){
        for (int row = 0; row < numRows; row++) {
            for (int col = 0; col < numCols; col++) {
                imgArray[row][col] = inFile.nextInt();
            }
        }
    }

    void zeroImgArray(){
        imgArray = new int[numRows][numCols];
    }

    QuadTreeNode buildQuadTree(int upperRow, int upperCol, int size, BufferedWriter outFile) throws IOException {
        QuadTreeNode currentNode = new QuadTreeNode(upperRow, upperCol, size, null, null, null,null);
        currentNode.printQuadTreeNode(outFile);
        if(size == 1)
            currentNode.color = imgArray[upperRow][upperCol];
        else{
            currentNode.NW = buildQuadTree(upperRow, upperCol, size/2, outFile);
            currentNode.NE = buildQuadTree(upperRow, upperCol+size/2, size/2, outFile);
            currentNode.SW = buildQuadTree(upperRow+size/2, upperCol, size/2, outFile);
            currentNode.SE = buildQuadTree(upperRow+size/2, upperCol+size/2, size/2, outFile);

            switch (sumKids(currentNode)) {
                case 0 -> {
                    currentNode.color = 0;
                    setLeaf(currentNode);
                }
                case 4 -> {
                    currentNode.color = 1;
                    setLeaf(currentNode);
                }
                default -> currentNode.color = 5;
            }
        }
        return currentNode;
    }

    int sumKids(QuadTreeNode currentNode){
        return currentNode.NW.color + currentNode.NE.color + currentNode.SW.color + currentNode.SE.color;
    }

    void setLeaf(QuadTreeNode currentNode){
        currentNode.NW = null;
        currentNode.NE = null;
        currentNode.SW = null;
        currentNode.SE = null;
    }

    boolean isLeaf(QuadTreeNode currentNode){
        if(currentNode.NW != null) return false;
        if(currentNode.NE != null) return false;
        if(currentNode.SW != null) return false;
        return currentNode.SE == null;
    }

    void preOrder(QuadTreeNode currentNode, BufferedWriter outFile) throws IOException {
        currentNode.printQuadTreeNode(outFile);
        if(!isLeaf(currentNode)){
            preOrder(currentNode.NW, outFile);
            preOrder(currentNode.NE, outFile);
            preOrder(currentNode.SW, outFile);
            preOrder(currentNode.SE, outFile);
        }
    }

    void postOrder(QuadTreeNode currentNode, BufferedWriter outFile) throws IOException{
        if(!isLeaf(currentNode)){
            preOrder(currentNode.NW, outFile);
            preOrder(currentNode.NE, outFile);
            preOrder(currentNode.SW, outFile);
            preOrder(currentNode.SE, outFile);
        }
        currentNode.printQuadTreeNode(outFile);
    }

    void printImgArray(BufferedWriter outFile) throws IOException{
        for(var row: imgArray){
            for(var pixel: row)
                outFile.write(pixel + " ");
            outFile.write("\n");
        }

    }
}

public class Main {

    public static void main(String[] args) throws IOException {
        Scanner inFile = new Scanner(new File(args[0]));
        BufferedWriter outFile1 = new BufferedWriter(new FileWriter(args[1], true));
        BufferedWriter outFile2 = new BufferedWriter(new FileWriter(args[2], true));

        int numRows, numCols, minVal, maxVal;
        numRows = inFile.nextInt();
        numCols = inFile.nextInt();
        minVal = inFile.nextInt();
        maxVal = inFile.nextInt();

        QuadTree quadTree = new QuadTree(numRows,numCols,minVal, maxVal);
        quadTree.loadImage(inFile);
        for (var row: quadTree.imgArray) {
            for(var pixel: row)
                System.out.print(pixel + " ");
            System.out.println();
        }
        outFile2.write("\n\n\n\nImage Array\n\n");
        quadTree.printImgArray(outFile2);

        outFile2.write("\n\n\n\nBuild Quad Tree\n\n");
        quadTree.root = quadTree.buildQuadTree(0,0,quadTree.computePower2(),outFile2);


        outFile1.write("PreOrder\n\n");
        quadTree.preOrder(quadTree.root, outFile1);

        outFile1.write("\n\n\n\nPostOrder\n\n");
        quadTree.postOrder(quadTree.root, outFile1);

        outFile1.close();
        outFile2.close();
        inFile.close();
    }
}
