public class Task4 {
    static long jacobi(long a, long n) {
        if (a == 0)
            return 0;
        if (a == 1)
            return 1;
        long e = 0;
        while (a % 2 == 0) {
            e++;
            a /= 2;
        }
        long s;
        if (e % 2 == 0 || n % 8 == 1 || n % 8 == 7)
            s = 1;
        else
            s = -1;

        if (n % 4 == 3 && a % 4 == 3)
            s = -s;

        n = n % a;
        if (a == 1)
            return s;
        else
            return s * jacobi(n, a);
    }

    static long squareRootMod(long a, long mod) {
        a = a % mod;
        if (a == 0)
            throw new ArithmeticException("0 does not have square root modulo " + mod);

        long legendre = jacobi(a, mod);
        if (legendre == -1)
            throw new ArithmeticException("a does not have square root modulo " + mod);

        long b;
        do {
            b = (long) (Math.random() * (mod - 2) + 1);
        } while (jacobi(b, mod) != -1);

        long s = 0;
        long t = mod - 1;
        while (t % 2 == 0) {
            s++;
            t /= 2;
        }
        long aInverse = Task3.modInverse(a, mod);
        long c = Task2.binaryModPow(b, t, mod);
        long r = Task2.binaryModPow(a, (t + 1) / 2, mod);

        for (int i = 1; i <= s - 1; i++) {
            //dPow = 2 ** (s-i-1)
            long dPow = 1;
            for (int i2 = 1; i2 <= s - i - 1; i2++)
                dPow *= 2;
            long d = Task2.binaryModPow((r * r * aInverse) % mod, dPow, mod);
            if (d % mod == mod - 1) {
                r = (r * c) % mod;
            }
            c = c*c % mod;
        }
        return r;
    }
    public static void main(String[] args) {
        int mod = 29;

        for (long x : new long[]{39, 30, 18, 5811988, 4964974, 6174, 160272279849904686L, 27342113381205372L, 20268}) {
            try {
                long sqrt = squareRootMod(x, mod);
                System.out.printf("sqrt(%d) mod %d: %d and %d\n", x, mod, sqrt, mod - sqrt);
            } catch (ArithmeticException e) {
                System.out.println("Error for " + x + ": " + e.getMessage());
            }
        }
        // Extra examples
        int x = 4;
        mod = 5;
        long sqrt = squareRootMod(x, mod);
        System.out.printf("sqrt(%d) mod %d: %d and %d\n", x, mod, sqrt, mod - sqrt);

        x = 81;
        mod = 11;
        sqrt = squareRootMod(x, mod);
        System.out.printf("sqrt(%d) mod %d: %d and %d\n", x, mod, sqrt, mod - sqrt);

        x = 81;
        mod = 5;
        sqrt = squareRootMod(x, mod);
        System.out.printf("sqrt(%d) mod %d: %d and %d\n", x, mod, sqrt, mod - sqrt);
    }
}
