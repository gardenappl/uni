import java.math.BigInteger;

public class Task6 {
    static void printPrimes(BigInteger n) {
        System.out.println("Primes for " + n + ':');
        BigInteger c = BigInteger.TWO;

        while (n.compareTo(BigInteger.ONE) > 0) { // n > 1
            if (n.mod(c).compareTo(BigInteger.ZERO) == 0) { // n % c == 0
                System.out.print(c + ", ");
                n = n.divide(c);
            } else {
                c = c.add(BigInteger.ONE);
            }
        }
        System.out.println();
    }

    public static void main(String[] args) {
        for (BigInteger x : new BigInteger[]{
                BigInteger.valueOf(366),
                BigInteger.valueOf(431),
                BigInteger.valueOf(739),

                BigInteger.valueOf(3666),
                BigInteger.valueOf(4313),
                BigInteger.valueOf(7399),

                Task5.factorial(66),
                Task5.factorial(31),
                Task5.factorial(39),
        }) {
            printPrimes(x);
        }
    }
}
