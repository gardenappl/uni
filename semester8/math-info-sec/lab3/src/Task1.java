public class Task1 {
    public static GCDExtendedResult gcdExtended(long a, long b) {
        if (a == 0) {
            return new GCDExtendedResult(b, 0, 1);
        }

        GCDExtendedResult result = gcdExtended(b % a, a);

        return new GCDExtendedResult(
                result.gcd,
                result.y - (b / a) * result.x, //new x
                result.x //new y
        );
    }

    static class GCDExtendedResult {
        public final long gcd;
        public final long x;
        public final long y;

        public GCDExtendedResult(long gcd, long x, long y) {
            this.gcd = gcd;
            this.x = x;
            this.y = y;
        }
    }


    public static void main(String[] args) {
        long a = 39;
        long b = 30;
        long c = 18;

        GCDExtendedResult result = gcdExtended(a, b);
        long d = result.gcd;
        long s = result.x * (c / d);
        long t = result.y * (c / d);

        System.out.println("d = " + d);
        System.out.println("u = " + result.x);
        System.out.println("v = " + result.y);
        System.out.println("s = " + s);
        System.out.println("t = " + t);
    }
}