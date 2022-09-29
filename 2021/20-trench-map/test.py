import unittest

from main import *

class PuzzleTestCase(unittest.TestCase):
    
    def test_pad(self):
        image = [[ 1 ]] # 1x1
        image = pad(image, 1)

        self.assertEqual(len(image), 3)
        self.assertEqual(len(image[0]), 3)
        self.assertEqual(image[0], [ 0, 0, 0])
        self.assertEqual(image[1], [ 0, 1, 0])
        self.assertEqual(image[2], [ 0, 0, 0])


if __name__ == '__main__':
    unittest.main()                     