import unittest

from main import *

class PuzzleTestCase(unittest.TestCase):
    
    def test_overlap(self):
        a = Cube(10, 12, 10, 12, 10, 12, 1)
        b = Cube(11, 13, 11, 13, 11, 13, 1)
        c = Cube(9, 11, 9, 11, 9, 11, 0)
        z = Cube(100, 105, 100, 105, 100, 105, 1)

        self.assertEqual(overlap(a, b), 8)
        self.assertEqual(overlap(b, a), 8)
        self.assertEqual(overlap(a, c), 8)
        self.assertEqual(overlap(a, z), 0)
        self.assertEqual(overlap(z, a), 0)

        


if __name__ == '__main__':
    unittest.main()                     