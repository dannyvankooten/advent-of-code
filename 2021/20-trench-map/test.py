import unittest

from main import *

class PuzzleTestCase(unittest.TestCase):
    
    def test_encoding_to_binary(self):
        self.assertEqual(encoding_to_binary("...#...#."), 34)
    
    def test_pad(self):
        image = [[ '#']] # 1x1
        image = pad(image, 1)

        self.assertEqual(len(image), 3)
        self.assertEqual(len(image[0]), 3)
        self.assertEqual(image[0], ['.', '.', '.'])
        self.assertEqual(image[1], ['.', '#', '.'])
        self.assertEqual(image[2], ['.', '.', '.'])


if __name__ == '__main__':
    unittest.main()                     