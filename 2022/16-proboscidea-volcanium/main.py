from dataclasses import dataclass
from copy import copy

INPUT_FILENAME = "input.txt"
INF = int(1e9)

@dataclass
class Valve:
    name: str
    flow_rate: int
    children: list[str]


def parse_input():
    with open(INPUT_FILENAME, "r") as file:
        lines = [l.rstrip() for l in file.readlines()]

    valves = {}

    for line in lines:
        split = line.split(" ")
        valve_name = split[1]
        flow_rate = int(split[4][:-1].split("=")[1])
        children = [split[-1]] + [token[:-1] for token in split if token.endswith(",")]
        valves[valve_name] = Valve(valve_name, flow_rate, children)

    return valves


def floid_warshall(valves):
    dist = {v: {u: INF for u in valves} for v in valves}

    for v in valves:
        dist[v][v] = 0
        for u in valves[v].children:
            dist[v][u] = 1

    for k in valves:
        for i in valves:
            for j in valves:
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j])

    return dist


def main():
    valves = parse_input()
    distances = floid_warshall(valves)
    non_zero_valves = [v for v in valves if valves[v].flow_rate > 0]

    def generate_open_options(pos, open_valves, time_left):
        for next in non_zero_valves:
            if next not in open_valves and distances[pos][next] <= time_left:
                open_valves.append(next)
                yield from generate_open_options(
                    next, open_valves, time_left - distances[pos][next] - 1
                )
                open_valves.pop()

        yield copy(open_valves)

    def get_order_score(open_order, time_left):
        now, ans = "AA", 0
        for pos in open_order:
            time_left -= distances[now][pos] + 1
            ans += valves[pos].flow_rate * time_left
            now = pos
        return ans

    def solution_1():
        return max(get_order_score(o, 30) for o in generate_open_options("AA", [], 30))

    def solution_2():
        ways = list(generate_open_options("AA", [], 26))

        best_scores = {}

        for order in ways:
            tup = tuple(sorted(order))
            score = get_order_score(order, 26)
            best_scores[tup] = max(best_scores.get(tup, 0), score)

        best_scores = list(best_scores.items())
        
        print(len(best_scores))
        print(len(ways))

        ans = 0
        for human_idx in range(len(best_scores)):
            for elephant_idx in range(human_idx + 1, len(best_scores)):
                human_opens, human_score = best_scores[human_idx]
                elephant_opens, elephant_score = best_scores[elephant_idx]

                if len(set(human_opens).intersection(elephant_opens)) == 0:
                    ans = max(ans, human_score + elephant_score)

        return ans

    print("Answer 1:", solution_1())
    print("Answer 2:", solution_2())


if __name__ == "__main__":
    # INPUT_FILENAME = "Day 16/sample.txt"
    main()

