
import java.io.*;
import java.util.Scanner;

class Node{
    int jobID;
    int jobTime;
    Node next;

    public Node(int jobID, int jobTime, Node next) {
        this.jobID = jobID;
        this.jobTime = jobTime;
        this.next = next;
    }

    String getInfo(){
        return "("+ jobID + ", " + jobTime + ") --> ";
    }
}

class Scheduler{
    int numNodes, numProcs, procUsed, currentTime, totalJobTimes;
    int[] jobTimeAry;
    int[][] matrix, table;
    Node OPEN;

    public Scheduler(int numNodes, int numProcs) {
        this.numNodes = numNodes;
        this.numProcs = numProcs;
        jobTimeAry = new int[numNodes+1];
        matrix = new int[numNodes+1][numNodes+1];
        OPEN = new Node(0, 0, null);
    }

    void allocateTable(){
        table = new int[numProcs+1][this.totalJobTimes+1];
    }

    void loadMatrix(Scanner graph){
        int node1, node2;
        while(graph.hasNext()){
            node1 = graph.nextInt();
            node2 = graph.nextInt();
            matrix[node1][node2] = 1;

            // following lines of code increment the parent count and dependent count
            matrix[node1][0]++;
            matrix[0][node2]++;
        }
        // here initialize the state of the node: matrix[i][i] = 1
        matrix[0][0] = numNodes;
        for (int i = 1; i < numNodes+1; i++) {
            matrix[i][i] = 1;
        }
    }

    int loadJobTimeAry(Scanner inFile){
        inFile.nextInt();
        int node, time;
        while(inFile.hasNext()){
            node = inFile.nextInt();
            time = inFile.nextInt();
            jobTimeAry[node] = time;
            totalJobTimes += time;
        }
        return totalJobTimes;
    }

    void printMatrix(BufferedWriter outFile) throws IOException {
        outFile.write("\n\n\n\nMatrix:\n        ");
        for (int i = 0; i < matrix.length; i++) {
            if(i!=0){
                outFile.write("Node " + i + " :" );
            }

            for (int j = 0; j < matrix.length; j++) {
                outFile.write(" " + matrix[i][j] + " ");
            }
            outFile.write("\n");
        }
    }

    int findOrphan(){
        for (int i = 0; i < matrix.length; i++) {
            if(matrix[0][i] == 0 && matrix[i][i] == 1){
                matrix[i][i] = 2;
                return i;
            }
        }
        return -1;
    }

    void openInsert(Node node){
        int val = node.jobTime;
        Node spot = OPEN;

        //if use spot.next.jobTime < val as condition, a new job has same job time
        // compared to a JOB in open can cut the line in front of JOB
        while(spot.next != null && spot.next.jobTime <= val){
            spot = spot.next;
        }

        node.next = spot.next;
        spot.next = node;
    }

    // format: (jobID, jobTime) -->
    void printOpen(BufferedWriter outFile) throws IOException {
        Node pointer = OPEN;
        outFile.write("\nOPEN: ");
        while(pointer != null){
            outFile.write(pointer.getInfo());
            pointer = pointer.next;
        }
        outFile.write("null");
    }

    int getNextProc(){
        for (int i = 1; i < table.length; i++) {
            if(table[i][currentTime] == 0) return i;
        }
        return -1;
    }

    void fillOpen(){
        int jobID = findOrphan();
        while(jobID != -1){
            openInsert(new Node(jobID, jobTimeAry[jobID], null));
            jobID = findOrphan();
        }
    }

    void fillTable(){
        int availableProc = getNextProc();
        Node newJob;
        while(availableProc >= 0 && OPEN.next != null && procUsed <= numProcs){
            newJob = removeOpen();
            putJobOnTable(availableProc, newJob);
            if(availableProc > procUsed)
                procUsed++;
            availableProc = getNextProc();
        }
    }

    void putJobOnTable(int availableProc, Node newJob) {
        int endTime = currentTime + newJob.jobTime;
        for (int time = currentTime; time < endTime; time++) {
            table[availableProc][time] = newJob.jobID;
        }
    }

    Node removeOpen(){
        Node node = OPEN.next;
        OPEN.next = OPEN.next.next;
        node.next = null;
        return node;
    }

    void printTable(BufferedWriter outFile) throws IOException{
        outFile.write("\n\n\n\n==================================================\n\n" +
                "ProcUsed: " + procUsed + "     currentTime: " + currentTime +
                "\n\nTime:  ");
        for (int i = 0; i < totalJobTimes; i++) {
            if(i < 10)
                outFile.write(" |  " + i);
            else
                outFile.write(" | " + i);
        }

        for (int procNumber = 1; procNumber <= procUsed; procNumber++) {
            outFile.write("\n\nProc: " + procNumber);
            for (int time = 0; time < totalJobTimes; time++) {
                if(table[procNumber][time] == 0)
                    outFile.write(" | --");
                else if(table[procNumber][time] < 10)
                    outFile.write(" |  " + table[procNumber][time]);
                else
                    outFile.write(" | " + table[procNumber][time]);
            }
        }
    }

    boolean containsCycle(){
        boolean allProcsResting = true;
        for (int i = 1; i <= procUsed; i++) {
            if(table[i][currentTime] > 0) {
                allProcsResting = false;
                break;
            }
        }

        return allProcsResting && OPEN.next == null && (!graphIsEmpty());
    }

    boolean graphIsEmpty(){
        return matrix[0][0] == 0;
    }

    void deleteDoneJobs(){
        for (int proc = 1; proc <= procUsed; proc++) {
            if(table[proc][currentTime] == 0 && table[proc][currentTime-1] != 0) {
                deleteJob(table[proc][currentTime - 1]);
            }
        }
    }

    void deleteJob(int jobID){
        matrix[jobID][jobID] = 0;
        matrix[0][0]--;
        matrix[jobID][0] = 0;
        for (int i = 1; i <= numNodes; i++) {
            if(matrix[jobID][i] > 0){
                matrix[jobID][i] = 0;
                matrix[0][i]--;
            }
        }
    }
}

public class Main {

    public static void main(String[] args) throws IOException {
        Scanner graph, jobTime;
        BufferedWriter outFile, debugFile;
        try{
            graph = new Scanner(new File(args[0]));
            jobTime = new Scanner(new File(args[1]));
            outFile = new BufferedWriter(new FileWriter(args[2], true));
            debugFile = new BufferedWriter(new FileWriter(args[3], true));
        }catch (IOException exception){
            return;
        }

        int numNodes = graph.nextInt();
        int numProcs = new Scanner(args[4]).nextInt();

        try{
            if (numProcs <= 0)
                throw new Exception("Invalid input from args[4] for numProcs.");
        }catch (Exception ignored){}

        // Constructor and other methods to fully initialize the Scheduler
        Scheduler scheduler = new Scheduler(numNodes,numProcs);
        scheduler.loadMatrix(graph);
        scheduler.printMatrix(debugFile);
        scheduler.loadJobTimeAry(jobTime);
        scheduler.allocateTable();
        scheduler.printTable(outFile);


        while(!scheduler.graphIsEmpty()){
            scheduler.fillOpen();
            scheduler.printOpen(debugFile);

            // Has to check after fillOpen
            // Otherwise, nodes that might not make a cycle are not yet pushed into OPEN
            if(scheduler.containsCycle()) {
                outFile.write("\n\nFound a cycle in the graph!!!");
                break;
            }

            scheduler.fillTable();
            scheduler.printTable(outFile);

            scheduler.currentTime++;
            scheduler.deleteDoneJobs();

        }
        scheduler.printTable(outFile);
        scheduler.printMatrix(debugFile);

        graph.close();
        jobTime.close();
        outFile.close();
        debugFile.close();
    }
}
