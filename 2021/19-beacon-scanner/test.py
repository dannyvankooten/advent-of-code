import unittest

from main import *

class PuzzleTestCase(unittest.TestCase):
    
    def test_permute(self):
        new_values = []
        values = [(1, 2, 3), (4, 5, 6)]
        for (cx, cy, cz) in product([1, -1], repeat=3):
            new_values.append([(x*cx, y*cy, z*cz) for (x, y, z) in values])
            #new_values.append([(x*cx, y*cy, z*cz) for (x, z, y) in values])
            new_values.append([(x*cx, y*cy, z*cz) for (y, z, x) in values])
            #new_values.append([(x*cx, y*cy, z*cz) for (y, x, z) in values])
            new_values.append([(x*cx, y*cy, z*cz) for (z, x, y) in values])
            #new_values.append([(x*cx, y*cy, z*cz) for (z, y, x) in values])

   
        print(new_values)
        self.assertEqual(len(new_values), 24)
        


if __name__ == '__main__':
    unittest.main()                     