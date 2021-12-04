import lab2 as m
import unittest


class TestCase(unittest.TestCase):
    def test_version(self):
        self.assertEqual(m.__version__, '0.0.1')

    def test_correct_results(self):
        self.assertEqual(m.multiply_factors_to_string({1:0, 2:1, 3:3}), '54')
        self.assertEqual(m.multiply_factors_to_string({10:100}), str(10**100))
        self.assertEqual(m.multiply_factors_to_string({9000000:2}), '81000000000000')
        self.assertEqual(m.multiply_factors_to_string({9000000:2, 10:3}), '81000000000000000')
        self.assertEqual(m.multiply_factors_to_string({2:51, 3:4, 5:100}), str(2**51 * 3**4 * 5**100))
        self.assertEqual(m.multiply_factors_to_string({999999999:10}), str(999999999**10))
        self.assertEqual(m.multiply_factors_to_string({999999999:100, 4294967295:100}), str(999999999**100 * 4294967295**100))

    def test_invalid_args(self):
        self.assertRaises(ValueError, m.multiply_factors_to_string, {0:0})
        self.assertRaises(ValueError, m.multiply_factors_to_string, {0:1})

    def test_invalid_types(self):
        self.assertRaises(TypeError, m.multiply_factors_to_string, {-1: 5})
        self.assertRaises(TypeError, m.multiply_factors_to_string, {1: -5})
        self.assertRaises(TypeError, m.multiply_factors_to_string, {-5: -5})
        self.assertRaises(TypeError, m.multiply_factors_to_string, 'not even a map')

unittest.main()
