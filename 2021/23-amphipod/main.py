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
            

def solve(rooms, hallway) -> int:
    energy_used = 0
    while not done(rooms, hallway):
        
        # find cheapest mover that is not in own room
        for (room, slots) in rooms.items():
            print(room, slots)
        break


        # find cheapest slot in hallway that is closest to destination
    return 0

if __name__ == '__main__':
    rooms = {
        'A': [ 'A', 'B'  ],
        'B': [ 'D', 'C' ],
        'C': [ 'C', 'B' ],
        'D': [ 'A', 'D' ],
    }
    hallway = [ None for _ in range(len("...........")) ]
    pt1 = solve(rooms, hallway)
