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
                print(f"Checking ({row}, {col}) in board {i}: {board[row][col].drawn}")
                if board[row][col].drawn == False:
                    break

                # if entire row is crossed
                if col == 4:
                    return i
        
        for col in range(0, 5):
            for row in range(0, 5):
                print(f"Checking ({row}, {col}) in board {i}: {board[row][col].drawn}")

                if board[row][col].drawn == False:
                    break

                # if entire column is crossed
                if row == 4:
                    return i

    return None                        

def part_one():
    input = open("input.txt").read()
    lines = input.split("\n")
    numbers = lines.pop(0).split(",")
    numbers = [int(n) for n in numbers]
    print("Numbers: ", numbers)

    boards = []
    num_boards = int(len(lines) / 6)
    print("Number of boards: ", num_boards)

    for _ in range(0, num_boards):
        # skip empty row
        lines.pop(0)

        board = [None, None, None, None, None]
        for row in range(0, 5):
            board[row] = [Number(int(n)) for n in lines.pop(0).rstrip().split()]

        boards.append(board)
        print("")

    print(boards)

    for idx, n in enumerate(numbers):
        print(f"Drawing number {n} ({idx} / {len(numbers)-1})")
        print(f"{len(boards)} boards remaining.")

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
                print(f"Found last winner! Index {winner}.")
            else:
                # remove winner from list
                print(f"Board #{winner} won. Removing from players.")
                boards.pop(winner)
                winner = find_winning_board(boards)
                continue

            winner = boards[winner]
            print("Last winner: ", winner)

            sum = 0
            for row in range(0, 5):
                for col in range(0, 5):
                    if winner[row][col].drawn == False:
                        sum += winner[row][col].value

            sum *= n
            print("Score: ", sum)
            return



part_one()