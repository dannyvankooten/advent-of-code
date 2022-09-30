from dataclasses import dataclass
from functools import lru_cache
from itertools import product
from pathlib import Path

@dataclass
class Player():
    name: int
    position: int 
    score: int = 0

@dataclass 
class DetermisticDie():
    next: int = 1

    def roll(self):
        result = self.next 
        self.next = ( self.next + 100 ) % 100 + 1
        return result 


def part1(input) -> int:
    die = DetermisticDie()
    cur = Player(1, input[0])
    other = Player(2, input[1])

    for number_of_rolls in range(3, 2 << 32, 3):
        roll = [die.roll(), die.roll(), die.roll()]
        
        cur.position, cur.score = play(cur.position, cur.score, sum(roll))
        if cur.score >= 1000:
            return other.score * number_of_rolls

        cur, other = other, cur


def play(pos, score, roll):
    new_pos = ((pos - 1 + roll) % 10) + 1
    new_score = score + new_pos
    return new_pos, new_score


@lru_cache(maxsize=None)
def count_wins(player, pos0, score0, pos1, score1):
    if score0 >= 21:
        return 1, 0
    elif score1 >= 21:
        return 0, 1

    wins = [0, 0]
    for rolls in product(range(1, 4), repeat=3):
        if player == 0:
            new_pos, new_score = play(pos0, score0, sum(rolls))
            wins0, wins1 = count_wins(1, new_pos, new_score, pos1, score1)
        else:
            new_pos, new_score = play(pos1, score1, sum(rolls))
            wins0, wins1 = count_wins(0, pos0, score0, new_pos, new_score)

        wins[0] += wins0
        wins[1] += wins1
        
    return wins



if __name__ == '__main__':
    input = Path("input.txt").read_text()
    starting_positions = (8, 6)
    pt1 = part1(starting_positions)
    print(pt1, pt1 == 503478)

    pt2 = max(count_wins(0, 8, 0, 6, 0))
    print(pt2, pt2 == 716241959649754)
