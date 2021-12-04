import lab2 as m
import unittest


class TestCase(unittest.TestCase):
    def test_version(self):
        self.assertEqual(m.__version__, '0.0.1')

    def test_correct_results(self):
        self.assertEqual(m.multiply_factors_to_string({1:0, 2:1, 3:3}), '54')
        self.assertEqual(m.multiply_factors_to_string({10:100}), '10000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000')
        self.assertEqual(m.multiply_factors_to_string({9000000:2}), '81000000000000')
        self.assertEqual(m.multiply_factors_to_string({9000000:2, 10:3}), '81000000000000000')
        self.assertEqual(m.multiply_factors_to_string({2:51, 3:4, 5:100}), '1438849039914202876389026641845703125000000000000000000000000000000000000000000000000000')
        self.assertEqual(m.multiply_factors_to_string({999999999:10}), '999999990000000044999999880000000209999999748000000209999999880000000044999999990000000001')
        self.assertEqual(m.multiply_factors_to_string({999999999:1, 4294967295:1}), '4294967290705032705')

    def test_invalid_args(self):
        self.assertRaises(ValueError, m.multiply_factors_to_string, {0:0})
        self.assertRaises(ValueError, m.multiply_factors_to_string, {0:1})

unittest.main()
