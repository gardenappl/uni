public class Task2 {
    public static void main(String[] args) {
        long a = 160272279849904686L;
        long b = 27342113381205372L;
        long c = 20268L;

        System.out.println("a^5 % 23 = " + binaryModPow(a, 5, 23));
        System.out.println("b^7 % 23 = " + binaryModPow(b, 7, 23));
        System.out.println("c^9 % 23 = " + binaryModPow(c, 9, 23));
    }

    static long binaryModPow(long a, long power, long mod) {
        long result = 1;
        a = a % mod;

        if (a == 0)
            return 0;
        while (power > 0) {
            if ((power & 1) != 0) result = (result * a) % mod;
            power = power >> 1;
            a = (a * a) % mod;
        }
        return result;
    }
}