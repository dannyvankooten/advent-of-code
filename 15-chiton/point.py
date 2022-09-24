
class Point():
    def __init__(self, x,  y, risk_factor):
        self.x = x
        self.y = y
        self.visited = False
        self.tent_distance = int(pow(2, 64))
        self.risk_factor = risk_factor
        self.neighbors = []

    def __eq__(self, other):
        return self.x == other.x and self.y == other.y

    def __hash__(self):
        return hash((self.x, self.y))

    def __lt__(self, other):
        return self.tent_distance < other.tent_distance

    def __repr__(self):
        return "({}, {}): {}".format(self.x, self.y, self.tent_distance)