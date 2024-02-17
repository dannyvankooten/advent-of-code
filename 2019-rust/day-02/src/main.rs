use std::io;
use std::time::Instant;

const OP_ADD: i64 = 1;
const OP_MUL: i64 = 2;
const OP_HALT: i64 = 99;

fn run(mut ins: Vec<i64>, noun: i64, verb: i64) -> i64 {
    ins[1] = noun;
    ins[2] = verb;

    let mut ip: usize = 0;
    while ip < ins.len() && ins[ip] != OP_HALT {
        let a = ins[ip + 1] as usize;
        let b = ins[ip + 2] as usize;
        let t = ins[ip + 3] as usize;
        ins[t] = match ins[ip] {
            OP_ADD => ins[a] + ins[b],
            OP_MUL => ins[a] * ins[b],
            _ => {
                panic!("Invalid operand");
            }
        };
        ip += 4;
    }

    return ins[0];
}

fn main() {
    let t = Instant::now();
    let ins: Vec<i64> = io::read_to_string(io::stdin())
        .unwrap()
        .trim()
        .split(',')
        .map(|v| v.parse().unwrap())
        .collect();

    let pt1 = run(ins.clone(), 12, 2);
    let mut pt2 = 0;
    'outer: for noun in 0..100 {
        for verb in 0..100 {
            if run(ins.clone(), noun, verb) == 19690720 {
                pt2 = 100 * noun + verb;
                break 'outer;
            }
        }
    }

    println!("Part 1: {}", pt1);
    println!("Part 2: {}", pt2);
    println!("Time: {:.3?} ms", t.elapsed().as_millis());
}
