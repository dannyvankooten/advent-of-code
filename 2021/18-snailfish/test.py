import unittest

from main import add, reduce, explode, split, parse_digit

class PuzzleTestCase(unittest.TestCase):
    
    def test_explode(self):
        tests = [
            (explode("[[[[[9,8],1],2],3],4]"), ("[[[[0,9],2],3],4]", True)),
            (explode("[7,[6,[5,[4,[3,2]]]]]"), ("[7,[6,[5,[7,0]]]]", True)),
            (explode("[[6,[5,[4,[3,2]]]],1]"), ("[[6,[5,[7,0]]],3]", True)),
            (explode("[[3,[2,[1,[7,3]]]],[6,[5,[4,[3,2]]]]]"), ("[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]", True)),
            (explode("[[3,[2,[8,0]]],[9,[5,[4,[3,2]]]]]"), ("[[3,[2,[8,0]]],[9,[5,[7,0]]]]", True)),
        ]
        for (inp, exp) in tests:
            self.assertEqual(inp, exp)

        self.assertEqual(explode("[[[[0,7],4],[7,[[8,4],9]]],[1,1]]"), ("[[[[0,7],4],[15,[0,13]]],[1,1]]", True))


    def test_split(self):
        tests = [
            (split("10"), ("[5,5]", True)),
            (split("11"), ("[5,6]", True)),
            (split("[11]"), ("[[5,6]]", True)),
        ]
        for (inp, exp) in tests:
            self.assertEqual(inp, exp)


    def test_add(self):
        self.assertEqual(add("[1,2]", "[[3,4],5]"), "[[1,2],[[3,4],5]]")    
        self.assertEqual(add("[[[[4,3],4],4],[7,[[8,4],9]]]", "[1,1]"), "[[[[0,7],4],[[7,8],[6,0]]],[8,1]]")
        self.assertEqual(add("[[[0,[4,5]],[0,0]],[[[4,5],[2,6]],[9,5]]]", "[7,[[[3,7],[4,3]],[[6,3],[8,8]]]]"), "[[[[4,0],[5,4]],[[7,7],[6,0]]],[[8,[7,7]],[[7,9],[5,0]]]]")
        
    def test_parse_digit(self):
        self.assertEqual(parse_digit("1"), (1, 0, 1))
        self.assertEqual(parse_digit("10"), (10, 0, 2))
        self.assertEqual(parse_digit("123"), (123, 0, 3))
        self.assertEqual(parse_digit("123,5"), (123, 0, 3))
        self.assertEqual(parse_digit("123]"), (123, 0, 3))
        self.assertEqual(parse_digit("]]],123]"), (123, 4, 7))





if __name__ == '__main__':
    unittest.main()                     