public class Task3 {
    static long modInverse(long x, long mod) {
        Task1.GCDExtendedResult result = Task1.gcdExtended(x, mod);

        if (result.gcd != 1) {
            throw new ArithmeticException("No inverse");
        } else {
            long inverse = result.x % mod;
            if (inverse > 0)
                return inverse;
            else
                return (inverse + mod) % mod;
        }
    }

    public static void main(String[] args) {
        long mod = 17;

        for (long x : new long[]{39, 30, 18, 5811988, 4964974, 6174, 160272279849904686L, 27342113381205372L, 20268}) {
            System.out.printf("%d ^ -1 = %d (mod %d)\n", x, modInverse(x, mod), mod);
        }
    }
}
