

import java.util.Scanner;

class PrimeNum extends Thread {
    private int numb;
 
    public PrimeNum(int numb) {
        this.numb = numb;
     }
 
    @Override
    public void run() {
        for (int i = 0; i <= numb; i++) {
            if (isPrime(i)) {
                System.out.printf("%d ", i);
            }
        }
    }
    
    private boolean isPrime(int n) {
        if (n <= 1) {
            return false;
        }
        else {
            for (int i = 2; i <= Math.sqrt(n); i++) {
                if (n % i == 0) {
                    return false;
                }
            }
        }
 
        return true;
    }
}
 
public class D2_Q1 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("Écris un nombre : ");
        int num = scanner.nextInt();
        scanner.close();
 
        PrimeNum primeNumbers = new PrimeNum(num);
        primeNumbers.start();
    }
}
 
