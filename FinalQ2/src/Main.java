import java.io.*;
import java.util.Scanner;

public class Main {

    public static void main(String[] args) throws Exception {
        Scanner inFile, inFile2;
        BufferedWriter outFile, debugFile;
        inFile = new Scanner(new File(args[0]));
        inFile2 = new Scanner(new File(args[1]));
        outFile = new BufferedWriter(new FileWriter(args[2], true));
        debugFile = new BufferedWriter(new FileWriter(args[3], true));

        int data1 = 0, data2 = 0;

        data1 = inFile.nextInt();
        data2 = inFile2.nextInt();

        while(inFile.hasNext() && inFile2.hasNext()){
            debugFile.write("\nData1: " + data1 + "  Data2: " + data2 + "\n");

            if(data1 <= data2){
                outFile.write(data1 + "\n");
                data1 = inFile.nextInt();
            }else{
                outFile.write(data2 + "\n");
                data2 = inFile2.nextInt();
            }
        }

        if(data1 <= data2 && data1!=0) {
            outFile.write(data1 + "\n");
            outFile.write(data2 + "\n");
        }else{
            outFile.write(data2 + "\n");
            outFile.write(data1 + "\n");
        }

        while(inFile.hasNext()){
            data1 = inFile.nextInt();
            outFile.write(data1 + "\n");
        }

        while(inFile2.hasNext()){
            data2 = inFile2.nextInt();
            outFile.write(data2);
        }

        inFile.close();
        inFile2.close();
        outFile.close();
        debugFile.close();
    }
}
