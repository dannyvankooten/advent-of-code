import unittest

from main import *

class PuzzleTestCase(unittest.TestCase):
    
    def test_matrix_rotations(self):
        values = [(1, 2, 3), (4, 5, 6)]
        new_values = matrix_rotations(values)
        
        print(f"{new_values} (length={len(new_values)})")
        self.assertEqual(len(new_values), 24)
        


if __name__ == '__main__':
    unittest.main()                     