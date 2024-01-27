from pathlib import Path

def energy_cost(type: str, steps: int) -> int:
    costs = {
        'A': 1,
        'B': 10,
        'C': 100,
        'D': 1000,
    }
    return costs[type] * steps

def done(rooms, hallway):
    if len(hallway) > 0:
        return False 

    for key in rooms:
        if len(rooms[key]) != 2:
            return False 

        for el in rooms[key]:
            if el != key:
                return False 
    
    return True

def room_ready(room, pod):
    for p in room:
        if p != '' and p != pod:
            return False

    return True


def solve(rooms, hallway, energy_cost) -> int:
    hallway[0] = 'A'
    # base case: if we're done or no possible moves
    if done(rooms, hallway):
        return energy_cost

    # perform all possible moves
    moves = []
    for pos, amphipod in enumerate(hallway):
        if amphipod is None:
            continue

        # amphipod at pos
        # it can only move if its destination room is empty
        if not room_ready(rooms[amphipod], amphipod):
            continue

        # add move for amphipod to room

    # add moves for all last amphipods in room to all available hallway spots


    # if no moves possible, return INT_MAX

    # return best energy cost
    return 0

if __name__ == '__main__':
    rooms = {
        'A': [ 'A', ''  ],
        'B': [ 'D', 'C' ],
        'C': [ 'C', 'B' ],
        'D': [ 'A', 'D' ],
    }
    hallway = [ None for _ in range(len("...........")) ]
    pt1 = solve(rooms, hallway, 0)

    print("--- Day 23: Amphipod ---")
    print("Part 1: ", pt1)