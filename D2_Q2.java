

import java.util.Scanner;

class Fibo extends Thread {
    private int num;
    private int[] fibArray;
 
    public Fibo(int num) {
        this.num = num;
        this.fibArray = new int[num];
    }
 
    @Override
    public void run() {
        fib();
    }
 
    private void fib() {
        fibArray[0] = 0;
 
        if (num > 1) {
            fibArray[1] = 1;
 
            for (int i = 2; i < num; i++) {
                fibArray[i] = fibArray[i-1] + fibArray[i-2];
            }
        }
    }
 
    public int[] getArray() {
        return fibArray;
    }
}
 
public class D2_Q2 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Écris un nombre : ");
        int num = scanner.nextInt();
        scanner.close();
 
        Fibo fibNumbers = new Fibo(num);
        fibNumbers.start();
 
        try {
            fibNumbers.join();
        } catch(InterruptedException e)
        {
            e.printStackTrace();
        }
 
        int[] fibArray = fibNumbers.getArray();
 
        for (int i = 0; i < num; i++) {
            System.out.printf("%d ", fibArray[i]);
        }
    }
}
 