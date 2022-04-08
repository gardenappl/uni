package ua.knu.yurihr.infosec;

import java.util.Arrays;

public class Main {
    private static class DoubleMatrix {
        private final double[][] rows;

        public DoubleMatrix(double[][] rows) {
            this.rows = rows;
        }

        public DoubleMatrix multiply(DoubleMatrix matrix2) {
            double[][] resultRows = new double[rows.length][];
            int resultWidth = matrix2.rows[0].length;

            for (int j = 0; j < rows.length; j++) {
                resultRows[j] = new double[resultWidth];

                for (int i = 0; i < resultWidth; i++) {
                    for (int k = 0; k < rows[j].length; k++) {
                        resultRows[j][i] += this.rows[j][k] * matrix2.rows[k][i];
                    }
                }
            }
            return new DoubleMatrix(resultRows);
        }

        public double[] multiplyByLeft(double[] vector) {
            double[] resultColumn = new double[rows.length];

            for (int i = 0; i < rows.length; i++) {
                for (int j = 0; j < rows.length; j++) {
                    resultColumn[i] += vector[j] * rows[j][i];
                }
            }
            return resultColumn;
        }

        @Override
        public String toString() {
            StringBuilder sb = new StringBuilder();
            for (int i = 0; i < rows.length; i++) {
                if (i == 0)
                    sb.append('[');
                else
                    sb.append(' ');
                sb.append(Arrays.toString(rows[i]));
                if (i == rows.length - 1)
                    sb.append(']');
                else
                    sb.append('\n');
            }
            return sb.toString();
        }
    }

    public static void main(String[] args) {
        double[] p0 = new double[]{1f/3, 1f/3, 1f/3};
        System.out.println(Arrays.toString(p0));

        DoubleMatrix pMatrix = new DoubleMatrix(new double[][]{
                new double[]{0.4, 0.1, 0.5},
                new double[]{0.1, 0.6, 0.3},
                new double[]{0.3, 0.5, 0.2}
        });
        System.out.println(pMatrix);

        System.out.println("P^2:");
        DoubleMatrix pSquared = pMatrix.multiply(pMatrix);
        System.out.println(pSquared);

        System.out.println("P^3:");
        DoubleMatrix pCubed = pSquared.multiply(pMatrix);
        System.out.println(pCubed);

        System.out.println("P^4:");
        DoubleMatrix pFourth = pCubed.multiply(pMatrix);
        System.out.println(pFourth);

        System.out.println("p3:");
        double[] p3 = pCubed.multiplyByLeft(p0);
        System.out.println(Arrays.toString(p3));

        System.out.println("p4:");
        double[] p4 = pFourth.multiplyByLeft(p0);
        System.out.println(Arrays.toString(p4));
    }
}
