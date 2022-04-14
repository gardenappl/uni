import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;

public class Lab4 {
    private static class FermaFactorizationResult {
        //n = p^2 - q^2
        public final int p;
        public final int q;
        //n = r * s
        public final int r;
        public final int s;

        FermaFactorizationResult(int p, int q, int r, int s) {
            this.p = p;
            this.q = q;
            this.r = r;
            this.s = s;
        }
    }

    public static FermaFactorizationResult fermaFactorizeStep(int n) {
        double s = Math.ceil(Math.sqrt(n));

        for (int k = 0; k < n; k++) {
            double y = (s + k) * (s + k) - n;

            double ySqrt = Math.sqrt(y);
            int q = (int) ySqrt;
            if (ySqrt == q) {
                int p = (int) (s + k);
                return new FermaFactorizationResult(
                        p,
                        q,
                        p + q,
                        p - q
                );
            }
        }
        throw new ArithmeticException("Not a difference of squares (" + n + " is an even number?)");
    }

    public static void fermaFactorizeRecursive(int n, List<Integer> factors) {
        while (n % 2 == 0) {
            n /= 2;
            factors.add(2);
        }
        FermaFactorizationResult result = fermaFactorizeStep(n);
//        System.out.printf("n = %d, r = %d, s = %d\n", n, result.r, result.s);
        if (result.s == 1) {
            factors.add(result.r);
        } else {
            fermaFactorizeRecursive(result.r, factors);
            fermaFactorizeRecursive(result.s, factors);
        }
    }

    public static int euler(int n) {
        ArrayList<Integer> factorsList = new ArrayList<>();
        fermaFactorizeRecursive(n, factorsList);

        HashSet<Integer> factorsSet = new HashSet<>(factorsList);
//        System.out.println(factorsSet);

        double result = n;
        for (int factor : factorsSet) {
            result *= 1. - 1. / factor;
        }
        return (int) result;
    }

    public static void main(String[] args) {
        for (int n : new int[]{14873, 284350889}) {
            FermaFactorizationResult result = fermaFactorizeStep(n);
            System.out.printf("%d = %d^2 - %d^2 = %d * %d\n", n, result.p, result.q, result.r, result.s);
        }

        for (int n : new int[]{3560, 4520, 116424, 1002001, 1294700, 1294699}) {
            System.out.printf("phi(%d) = %d\n", n, euler(n));
        }

        double task18 = 1792 / (1 - 1./2) / (1 - 1./5) / (1 - 1./113);
        ArrayList<Integer> factors = new ArrayList<>();
        fermaFactorizeRecursive((int)task18, factors);

        System.out.println(task18);
        System.out.println(factors);

        for (int[] tuple : new int[][]{{5, 26}, {2, 45}, {3, 40}}) {
            int result = (int)Math.pow(tuple[0], euler(tuple[1])) % tuple[1];
            System.out.printf("%d^phi(%d) = %d (mod %d)\n", tuple[0], tuple[1], result, tuple[1]);
        }
    }
}
