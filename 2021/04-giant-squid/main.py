from pathlib import Path


class Number:
    value = None
    drawn = False

    def __init__(self, value):
        self.value = value

    def __repr__(self):
        if self.drawn:
            return f"_{self.value}_"
        return f"{self.value}"

def find_winning_board(boards):
    for i, board in enumerate(boards):
        for row in range(0, 5):
            for col in range(0, 5):
                if board[row][col].drawn == False:
                    break

                # if entire row is crossed
                if col == 4:
                    return i
        
        for col in range(0, 5):
            for row in range(0, 5):

                if board[row][col].drawn == False:
                    break

                # if entire column is crossed
                if row == 4:
                    return i

    return None                        

def part_one():
    lines = Path("input.txt").read_text().split("\n")
    numbers = lines.pop(0).split(",")
    numbers = [int(n) for n in numbers]
    boards = []
    num_boards = int(len(lines) / 6)
    for _ in range(0, num_boards):
        # skip empty row
        lines.pop(0)

        board = [[Number(int(n)) for n in lines.pop(0).rstrip().split()] for _ in range(0, 5)]
        boards.append(board)

    for n in numbers:
        # mark number
        for board in boards:
            for row in range(0, 5):
                for col in range(0, 5):
                    if board[row][col].value == n:
                        board[row][col].drawn = True
        
        # check for winners along horizontal axis
        winner = find_winning_board(boards)
        while winner is not None:
            if len(boards) == 1 or n == numbers[-1]:
                pass
            else:
                # remove winner from list
                boards.pop(winner)
                winner = find_winning_board(boards)
               
                continue

            winner = boards[winner]

            sum = 0
            for row in range(0, 5):
                for col in range(0, 5):
                    if winner[row][col].drawn == False:
                        sum += winner[row][col].value

            sum *= n
            return sum



pt1 = part_one()
print(pt1, pt1 == 2568)