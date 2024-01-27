from pathlib import Path
import functools 

def parse(input):
    rates = {}
    on = {} 
    valve_exits = {}
    for line in input.split('\n'):
        desc, exits = line.split(';' )
        _, name, _, _, rate = desc.split()
        exits = exits.replace(',', '').split(" ")[5:]
        rate = int(rate.split('=')[1])
        rates[name] = rate 
        on[name] = False 
        valve_exits[name] = exits

    return rates, on, valve_exits

def solve(rates, exits, on, cur: str, total_pressure: int, time_left: int):
    if time_left == 0:
        return total_pressure  

    # add pressure for each open valve
    for name in on: 
        if on[name]:
            total_pressure += rates[name]
   
    # if already on, do not recurse into exits?
    #if on[cur]:
     #   return total_pressure
    
    results = []
    if not on[cur] and rates[cur] > 0:
        on_copy = on.copy() 
        on_copy[cur] = True 
        results.append(solve(rates, exits, on_copy, cur, total_pressure, time_left=time_left - 1))

    for e in exits[cur]:
        results.append(solve(rates, exits, on, e, total_pressure, time_left=time_left - 1))

    return max(results)
        

if __name__ == '__main__':
    input = Path("input_test.txt").read_text().strip()
    rates, on, exits = parse(input)
    print("rates: ", rates)
    print("statuses: ", on) 
    print("exits: ", exits)
    print("pt1: ", solve(rates, exits, on, 'AA', 0, 30))

