import java.math.BigDecimal;
import java.math.BigInteger;
import java.math.MathContext;
import java.math.RoundingMode;

public class Task5 {
    static BigInteger euler(BigInteger n) {
        MathContext context = new MathContext(1000, RoundingMode.FLOOR);

        BigDecimal result = new BigDecimal(n, context);

        // p = 2; p * p <= n; p++
        for (BigInteger p = BigInteger.TWO; p.multiply(p).compareTo(n) <= 0; p = p.add(BigInteger.ONE)) {
            // n % p == 0
            if (n.mod(p).compareTo(BigInteger.ZERO) == 0) {
                while (n.mod(p).compareTo(BigInteger.ZERO) == 0)
                    n = n.divide(p);
                // result *= (1 - 1 / p)
                result = result.multiply(BigDecimal.ONE.subtract(BigDecimal.ONE.divide(new BigDecimal(p), context)));
            }
        }
        // if (n > 1) result *= 1 - 1 / n
        if (n.compareTo(BigInteger.ONE) > 0)
            result = result.multiply(BigDecimal.ONE.subtract(BigDecimal.ONE.divide(new BigDecimal(n), context)));
        return result.toBigInteger();
    }

    static BigInteger factorial(int n) {
        BigInteger result = BigInteger.ONE;
        for (int i = 2; i <= n; i++)
            result = result.multiply(BigInteger.valueOf(i));
        return result;
    }

    public static void main(String[] args) {
        for (BigInteger x : new BigInteger[]{
                BigInteger.valueOf(366),
                BigInteger.valueOf(431),
                BigInteger.valueOf(739),

                BigInteger.valueOf(636),
                BigInteger.valueOf(1401),
                BigInteger.valueOf(3974),

                factorial(13),
                factorial(8),
                factorial(21),

                factorial(36),
                factorial(43),
                factorial(73),
        }) {
            System.out.printf("Euler(%d) = \n   %d\n", x, euler(x));
        }
    }
}
