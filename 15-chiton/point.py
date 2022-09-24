
from dataclasses import dataclass, field
from typing import List

INT_MAX = int(pow(2, 64))

@dataclass
class Point():
    x: int
    y: int
    risk_factor: int 
    tent_distance: int = INT_MAX
    visited: bool = False 
    
    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __hash__(self):
        return hash((self.x, self.y))

    def __lt__(self, other):
        return self.tent_distance < other.tent_distance

    def __repr__(self):
        return "({}, {}): {}".format(self.x, self.y, self.tent_distance)