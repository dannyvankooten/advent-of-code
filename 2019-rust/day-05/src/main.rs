use std::io;
use std::time::Instant;

const OP_ADD: i32 = 1;
const OP_MUL: i32 = 2;
const OP_SET: i32 = 3;
const OP_OUTPUT: i32 = 4;
const OP_JUMP_IF_TRUE: i32 = 5;
const OP_JUMP_IF_FALSE: i32 = 6;
const OP_LT: i32 = 7;
const OP_EQ: i32 = 8;
const OP_HALT: i32 = 99;

#[inline]
fn read_operand(ins: &Vec<i32>, ip: &mut usize, mode: &mut i32) -> i32 {
    let value = ins[*ip];
    let immediate = *mode % 10 == 1;

    *ip += 1;
    *mode /= 10;

    if immediate {
        return value;
    }

    return ins[value as usize];
}

fn run(mut ins: Vec<i32>, input: i32) -> i32 {
    let mut output = 0;
    let mut ip: usize = 0;
    while ip < ins.len() {
        let mut i = ins[ip];
        ip += 1;
        let op = i % 100;
        i /= 100;
        match op {
            OP_ADD => {
                let left = read_operand(&ins, &mut ip, &mut i);
                let right = read_operand(&ins, &mut ip, &mut i);
                let t = read_operand(&ins, &mut ip, &mut 1) as usize;
                ins[t] = left + right;
            }
            OP_MUL => {
                let left = read_operand(&ins, &mut ip, &mut i);
                let right = read_operand(&ins, &mut ip, &mut i);
                let t = read_operand(&ins, &mut ip, &mut 1) as usize;
                ins[t] = left * right;
            }
            OP_SET => {
                let t = read_operand(&ins, &mut ip, &mut 1) as usize;
                ins[t] = input;
            }
            OP_OUTPUT => {
                let t = read_operand(&ins, &mut ip, &mut 1) as usize;
                output = ins[t];
            }
            OP_JUMP_IF_TRUE => {
                let a = read_operand(&ins, &mut ip, &mut i);
                let b = read_operand(&ins, &mut ip, &mut i);
                if a != 0 {
                    ip = b as usize;
                }
            }
            OP_JUMP_IF_FALSE => {
                let a = read_operand(&ins, &mut ip, &mut i);
                let b = read_operand(&ins, &mut ip, &mut i);
                if a == 0 {
                    ip = b as usize;
                }
            }
            OP_LT => {
                let left = read_operand(&ins, &mut ip, &mut i);
                let right = read_operand(&ins, &mut ip, &mut i);
                let t = read_operand(&ins, &mut ip, &mut 1) as usize;
                ins[t] = if left < right { 1 } else { 0 };
            }
            OP_EQ => {
                let left = read_operand(&ins, &mut ip, &mut i);
                let right = read_operand(&ins, &mut ip, &mut i);
                let t = read_operand(&ins, &mut ip, &mut 1) as usize;
                ins[t] = if left == right { 1 } else { 0 };
            }
            OP_HALT => {
                return output;
            }
            _ => {
                panic!("Invalid opcode: {}", op);
            }
        };
    }

    return ins[0];
}

fn main() {
    let t = Instant::now();
    let ins: Vec<i32> = io::read_to_string(io::stdin())
        .unwrap()
        .trim()
        .split(',')
        .map(|v| v.parse().unwrap())
        .collect();

    let pt1 = run(ins.clone(), 1);
    let pt2 = run(ins, 5);
    println!("Part 1: {}", pt1);
    println!("Part 2: {}", pt2);
    println!("Time: {:.3?} ms", t.elapsed().as_millis());
}
