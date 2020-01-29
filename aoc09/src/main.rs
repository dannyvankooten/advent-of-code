use std::collections::HashMap;
use std::fs;

const PARAM_MODE_POSITION: i64 = 0;
const PARAM_MODE_IMMEDIATE: i64 = 1;
const PARAM_MODE_RELATIVE: i64 = 2;
const OP_ADD: i64 = 1;
const OP_MULTIPLY: i64 = 2;
const OP_INPUT: i64 = 3;
const OP_OUTPUT: i64 = 4;
const OP_JUMP_IF_TRUE: i64 = 5;
const OP_JUMP_IF_FALSE: i64 = 6;
const OP_LESS_THAN: i64 = 7;
const OP_EQUALS: i64 = 8;
const OP_OFFSET_RELATIVE_BASE: i64 = 9;

fn main() {
    let input = fs::read_to_string("input.txt").expect("Error reading input file");
    let program: Vec<i64> = input
        .trim()
        .split_terminator(",")
        .map(|v| v.parse().unwrap())
        .collect();

    let mut computer = Computer::new(program);
    let output = computer.run(vec![1]);
    println!("{:?}", output);
}

struct Computer {
    memory: HashMap<usize, i64>,
    address: usize,
    relative_base: usize,
    program_length: usize,
}

impl Computer {
    fn new(program: Vec<i64>) -> Computer {
        println!("Program: {:?}", program);

        // copy program into memory
        let mut memory: HashMap<usize, i64> = HashMap::with_capacity(program.len());
        for (i, d) in program.iter().enumerate() {
            memory.insert(i, *d);
        }

        Computer {
            memory: memory,
            address: 0,
            relative_base: 0,
            program_length: program.len(),
        }
    }

    fn run(&mut self, input: Vec<i64>) -> Vec<i64> {
        let mut input_n: usize = 0;
        let mut output = Vec::new();

        while self.address < self.program_length {
            let opcode = *self.memory.get(&self.address).unwrap();
            if opcode == 99 {
                break;
            }

            let instruction = opcode.to_string();
            let mut chars = instruction.chars().rev();

            // parse opcode
            let opcode = chars.next().unwrap().to_digit(10).unwrap() as i64;
            chars.next(); // ignore 2nd opcode char, we already checked for opcode 99 above

            // parse param modes
            let mut param_modes = [PARAM_MODE_POSITION, PARAM_MODE_POSITION];
            let mut j: usize = 0;
            while let Some(c) = chars.next() {
                param_modes[j] = c.to_digit(10).unwrap() as i64;
                j = j + 1;
            }

            // parse params
            let mut params: Vec<i64> = vec![];
            match opcode {
                OP_ADD | OP_MULTIPLY | OP_LESS_THAN | OP_EQUALS => {
                    params.push(self.get_param_value(self.address + 1, param_modes[0]));
                    params.push(self.get_param_value(self.address + 2, param_modes[1]));
                    // result parameter is NEVER IN IMMEDIATE MODE
                    params.push(self.get_param_value(self.address + 3, PARAM_MODE_IMMEDIATE));
                }
                OP_INPUT | OP_OUTPUT | OP_OFFSET_RELATIVE_BASE => {
                    params.push(self.get_param_value(self.address + 1, param_modes[0]));
                    // result pos is NEVER IN IMMEDIATE MODE
                }
                OP_JUMP_IF_FALSE | OP_JUMP_IF_TRUE => {
                    params.push(self.get_param_value(self.address + 1, param_modes[0]));
                    params.push(self.get_param_value(self.address + 2, param_modes[1]));
                }
                _ => panic!("invalid opcode"),
            }

            println!(
                "opcode: {:?}, params: {:?}, param_modes: {:?}",
                opcode, params, param_modes
            );

            // run operation
            match opcode {
                OP_ADD => {
                    self.memory
                        .insert(params[2] as usize, params[0] + params[1]);
                }
                OP_MULTIPLY => {
                    self.memory
                        .insert(params[2] as usize, params[0] * params[1]);
                }
                OP_INPUT => {
                    self.memory.insert(params[0] as usize, input[input_n]);
                    input_n = input_n + 1;
                }
                OP_OUTPUT => {
                    output.push(params[0]);
                }
                OP_JUMP_IF_TRUE => {
                    if params[0] != 0 {
                        self.address = params[1] as usize;
                        continue;
                    }
                }
                OP_JUMP_IF_FALSE => {
                    if params[0] == 0 {
                        self.address = params[1] as usize;
                        continue;
                    }
                }
                OP_LESS_THAN => {
                    self.memory.insert(
                        params[2] as usize,
                        if params[0] < params[1] { 1 } else { 0 },
                    );
                }
                OP_EQUALS => {
                    self.memory.insert(
                        params[2] as usize,
                        if params[0] == params[1] { 1 } else { 0 },
                    );
                }
                OP_OFFSET_RELATIVE_BASE => {
                    self.relative_base = self.relative_base + params[0] as usize;
                }
                _ => panic!("invalid opcode"),
            }

            // increment pointer by number of values in operation
            self.address = self.address + params.len() + 1;
        }


        output
    }

    fn get_param_value(&self, pos: usize, mode: i64) -> i64 {
        match mode {
            PARAM_MODE_IMMEDIATE => *self.memory.get(&pos).unwrap(),
            PARAM_MODE_POSITION => {
                let value = *self.memory.get(&pos).unwrap() as usize;
                *self.memory.get(&value).unwrap_or(&0)
            }
            PARAM_MODE_RELATIVE => {
                //let value = std::cmp::max(0, pos as i64 + self.relative_base as i64) as usize;
                let value = pos + self.relative_base;

                *self.memory.get(&value).unwrap_or(&0)
            }
            _ => panic!("invalid mode"),
        }
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_intcode() {
        // 104,1125899906842624,99 should output the large number in the middle.
        assert_eq!(
            Computer::new(vec![104, 1125899906842624, 99]).run(vec![]),
            vec![1125899906842624]
        );

        // 1102,34915192,34915192,7,4,7,99,0 should output a 16-digit number.
        let output = Computer::new(vec![1102, 34915192, 34915192, 7, 4, 7, 99, 0]).run(vec![]);
        assert_eq!(output.len(), 1);
        assert_eq!(
            output[0].to_string().chars().collect::<Vec<char>>().len(),
            16
        );

        // 109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99 takes no input and produces a copy of itself as output.
        assert_eq!(
            Computer::new(vec![
                109, 1, 204, -1, 1001, 100, 1, 100, 1008, 100, 16, 101, 1006, 101, 0, 99
            ])
            .run(vec![]),
            vec![109, 1, 204, -1, 1001, 100, 1, 100, 1008, 100, 16, 101, 1006, 101, 0, 99]
        );

        assert_eq!(Computer::new(vec![109, -1, 4, 1, 99]).run(vec![]), vec![-1]);

        assert_eq!(
            Computer::new(vec![109, -1, 104, 1, 99]).run(vec![]),
            vec![1]
        );

        assert_eq!(
            Computer::new(vec![109, -1, 204, 1, 99]).run(vec![]),
            vec![109]
        );

        assert_eq!(
            Computer::new(vec![109, -1, 204, 1, 99]).run(vec![]),
            vec![109]
        );

        assert_eq!(
            Computer::new(vec![109, 1, 9, 2, 204, -6, 99]).run(vec![]),
            vec![204]
        );

        assert_eq!(
            Computer::new(vec![109, 1, 109, 9, 204, -6, 99]).run(vec![]),
            vec![204]
        );

        assert_eq!(
            Computer::new(vec![109, 1, 209, -1, 204, -106, 99]).run(vec![]),
            vec![204]
        );

        assert_eq!(
            Computer::new(vec![109, 1, 3, 3, 204, 2, 99]).run(vec![100]),
            vec![100]
        );

        assert_eq!(
            Computer::new(vec![109, 1, 203, 2, 204, 2, 99]).run(vec![200]),
            vec![200]
        );
    }
}
