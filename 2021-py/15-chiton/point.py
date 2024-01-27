
from dataclasses import dataclass

INT_MAX = int(pow(2, 64))

@dataclass
class Point():
    __slots__ = ['x', 'y', 'risk_factor', 'tent_distance', 'visited']
    x: int
    y: int
    risk_factor: int 
    tent_distance: int 
    visited: bool  
    
    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __hash__(self):
        return hash((self.x, self.y))

    def __lt__(self, other):
        return self.tent_distance < other.tent_distance