#[derive(Debug, Copy, Clone)]
struct Operation {
    operator: char,
    right: i64,
}

#[derive(Debug, Clone)]
struct Monkey {
    items: Vec<i64>,
    operation: Operation,
    divisor: i64,
    target_true: usize,
    target_false: usize, 
}

fn solve(mut monkeys: Vec<Monkey>, modulo_lcm: bool) -> i64 {
    let mut activities = [0; 9];
    
    let mut lcm = 1;
    if modulo_lcm {
        for m in &monkeys {
            lcm *= m.divisor;
        }
    };
       
    let n = if modulo_lcm { 10000 } else { 20 };

    for _ in 0..n {
        for idx in 0..monkeys.len() {
            let op = monkeys[idx].operation;

            for i in 0..monkeys[idx].items.len() {
                let old = monkeys[idx].items[i];
                let mut new = match op.operator {
                    '+' => old + op.right,
                    '*' => old * if op.right == 0 { old } else { op.right },
                    _ => panic!("Invalid operator"),
                };

                if modulo_lcm { 
                    new %= lcm;
                } else {
                    new /= 3;
                }

                if new % monkeys[idx].divisor == 0 {
                    let target_monkey = monkeys[idx].target_true;
                    monkeys[target_monkey].items.push(new)
                } else {
                    let target_monkey = monkeys[idx].target_false;
                    monkeys[target_monkey].items.push(new)
                }
            }

            activities[idx] += monkeys[idx].items.len() as i64;
            monkeys[idx].items.clear();
        }
    }

    activities.sort();
    return activities[activities.len() - 1] * activities[activities.len() - 2];
}

fn parse() -> Vec<Monkey> {
    let mut monkeys : Vec<Monkey> = Vec::new();
    let input = include_str!("input.txt");
    for group in input.trim().split("\n\n") {
        let lines : Vec<&str> = group.trim().split('\n').map(|l| l.trim().split(':').next_back().unwrap().trim()).collect();
        let op_line : Vec<&str> = lines[2][6..].split(' ').collect(); 
        monkeys.push(Monkey{
            items: lines[1].split(',').map(|i| i.trim().parse().unwrap()).collect(),
            operation: Operation {
                operator: op_line[1].chars().next().unwrap(),
                right: op_line[2].parse().unwrap_or(0),
            },
            divisor: lines[3].split(' ').next_back().unwrap().parse().unwrap(),
            target_true: lines[4].split(' ').next_back().unwrap().parse().unwrap(),
            target_false: lines[5].split(' ').next_back().unwrap().parse().unwrap(),
        });
    }
    return monkeys;
}

fn main() {
    let monkeys = parse(); 
    println!("pt1: {}", solve(monkeys.clone(), false));
    println!("pt2: {}", solve(monkeys.clone(), true));
}
