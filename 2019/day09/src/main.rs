use std::collections::HashMap;
use std::cmp::max;

#[derive(Debug)]
enum Op {
    Add,
    Multiply,
    Input,
    Output,
    JumpIfTrue,
    JumpIfFalse,
    LessThan,
    Equals,
    OffsetRelativeBase
}

#[derive(Debug, Copy, Clone, PartialEq, Eq)]
enum ParameterMode {
    Position,
    Immediate,
    Relative
}

fn main() {
    let program: Vec<i64> = include_str!("../input.txt")
        .trim()
        .split_terminator(",")
        .map(|v| v.parse().unwrap())
        .collect();

    let mut computer = Computer::new(&program);
    let output = computer.run(vec![1]);
    println!("Part 1: {:?}", output);

    let mut computer = Computer::new(&program);
    let output = computer.run(vec![2]);
    println!("Part 2: {:?}", output);
}

struct Computer {
    memory: HashMap<usize, i64>,
    address: usize,
    relative_base: usize,
    program_length: usize,
}

impl Computer {
    fn new(program: &Vec<i64>) -> Computer {
        // dbg!(&program);

        // copy program into memoryrust 
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

    fn parse_opcode(c : char) -> Op {
        let opcode = c.to_digit(10).unwrap() as i64;
        match opcode {
            1 => Op::Add,
            2 => Op::Multiply,
            3 => Op::Input,
            4 => Op::Output,
            5 => Op::JumpIfTrue,
            6 => Op::JumpIfFalse,
            7 => Op::LessThan,
            8 => Op::Equals,
            9 => Op::OffsetRelativeBase,
            _ => panic!("Invalid opcode")
        }
    }

    fn parse_parameter_modes(mut chars : std::iter::Rev<std::str::Chars>) -> [ParameterMode; 3] {
        let mut param_modes = [ParameterMode::Position, ParameterMode::Position, ParameterMode::Position];
        let mut j: usize = 0;
        while let Some(c) = chars.next() {
            param_modes[j] = match c.to_digit(10).unwrap() as i64 {
                0 => ParameterMode::Position,
                1 => ParameterMode::Immediate,
                2 => ParameterMode::Relative,
                _ => panic!("Invalid paramteer mode")
            };
            j = j + 1;
        }

        param_modes
    }

    fn run(&mut self, input: Vec<i64>) -> Vec<i64> {
        let mut input_n: usize = 0;
        let mut output = Vec::new();
        let mut params = Vec::new();
        
        // TODO: Memory is not reset between runs

        while self.address < self.program_length {
            let opcode = *self.memory.get(&self.address).unwrap();
            if opcode == 99 {
                break;
            }

            // TODO: Iterate over digits without going through string
            let instruction = opcode.to_string();
            let mut chars = instruction.chars().rev();

            // parse opcode
            let opcode = Computer::parse_opcode(chars.next().unwrap());
            chars.next(); // ignore 2nd opcode char, we already checked for opcode 99 above

            // parse param modes
            let param_modes = Computer::parse_parameter_modes(chars);            

            // parse params
            params.clear();
            match opcode {
                Op::Add | Op::Multiply | Op::LessThan | Op::Equals => {
                    params.push(self.get_param_value(self.address + 1, param_modes[0]));
                    params.push(self.get_param_value(self.address + 2, param_modes[1]));
                    // result parameter is NEVER IN IMMEDIATE MODE
                    params.push(self.get_param_position(self.address + 3, param_modes[2]) as i64);
                },
                Op::Input => {
                    // result parameter is NEVER IN IMMEDIATE MODE
                    params.push(self.get_param_position(self.address + 1, param_modes[0]) as i64);
                },
                Op::Output | Op::OffsetRelativeBase => {
                    params.push(self.get_param_value(self.address + 1, param_modes[0]));
                 },
                Op::JumpIfFalse | Op::JumpIfTrue => {
                    params.push(self.get_param_value(self.address + 1, param_modes[0]));
                    params.push(self.get_param_value(self.address + 2, param_modes[1]));
                }
            }

            // println!(
            //     "adress: {:?}\ninstruction: {:?}\nopcode: {:?}\nparams: {:?}\nparam_modes: {:?}\nrelative base: {:?}\noutput: {:?}\nmemory before op: {:?}",
            //     self.address, instruction, opcode, params, param_modes, self.relative_base, output, self.memory
            // );

            // run operation
            match opcode {
                Op::Add => {
                    self.memory
                        .insert(params[2] as usize, params[0] + params[1]);
                }
                Op::Multiply => {
                    self.memory
                        .insert(params[2] as usize, params[0] * params[1]);
                }
                Op::Input => {
                    self.memory.insert(params[0] as usize, input[input_n]);
                    input_n = input_n + 1;
                }
                Op::Output => {
                    output.push(params[0]);
                }
                Op::JumpIfTrue => {
                    if params[0] != 0 {
                        self.address = params[1] as usize;
                        continue;
                    }
                }
                Op::JumpIfFalse => {
                    if params[0] == 0 {
                        self.address = params[1] as usize;
                        continue;
                    }
                }
                Op::LessThan => {
                    self.memory.insert(
                        params[2] as usize,
                        if params[0] < params[1] { 1 } else { 0 },
                    );
                }
                Op::Equals => {
                    self.memory.insert(
                        params[2] as usize,
                        if params[0] == params[1] { 1 } else { 0 },
                    );
                }
                Op::OffsetRelativeBase => {
                    self.relative_base = max(0, self.relative_base as i64 + params[0]) as usize;
                }
            }

            // increment pointer by number of values in operation
            self.address = self.address + params.len() + 1;
        }


        output
    }

    fn get_param_position(&self, pos : usize, mode : ParameterMode) -> usize {
        match mode {
            ParameterMode::Immediate => pos,
            ParameterMode::Position => {
                *self.memory.get(&pos).unwrap() as usize
            },
            ParameterMode::Relative => {
                let param_value = *self.memory.get(&pos).unwrap() as usize;
                let pos = std::cmp::max(0, param_value as i64 + self.relative_base as i64) as usize;
                pos
            }
        }
    }

    fn get_param_value(&self, pos: usize, mode: ParameterMode) -> i64 {
        let pos = self.get_param_position(pos, mode); 
        *self.memory.get(&pos).unwrap_or(&0)
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_intcode() {
        // 104,1125899906842624,99 should output the large number in the middle.
        assert_eq!(
            Computer::new(&vec![104, 1125899906842624, 99]).run(vec![]),
            vec![1125899906842624]
        );

        // 1102,34915192,34915192,7,4,7,99,0 should output a 16-digit number.
        let output = Computer::new(&vec![1102, 34915192, 34915192, 7, 4, 7, 99, 0]).run(vec![]);
        assert_eq!(output.len(), 1);
        assert_eq!(
            output[0].to_string().chars().collect::<Vec<char>>().len(),
            16
        );

        assert_eq!(Computer::new(&vec![3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9]).run(vec![0]), vec![0]);
        assert_eq!(Computer::new(&vec![3,12,6,12,15,1,13,14,13,4,13,99,-1,0,1,9]).run(vec![1]), vec![1]);
        assert_eq!(Computer::new(&vec![3,3,1105,-1,9,1101,0,0,12,4,12,99,1]).run(vec![0]), vec![0]);
        assert_eq!(Computer::new(&vec![3,3,1105,-1,9,1101,0,0,12,4,12,99,1]).run(vec![1]), vec![1]);
        assert_eq!(
            Computer::new(&vec![204, 1125899906842624, 99]).run(vec![]),
            vec![0]
        );
        assert_eq!(
            Computer::new(&vec![109, 1, 204, 0, 99]).run(vec![]),
            vec![1]
        );


        // // 109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99 takes no input and produces a copy of itself as output.
        // assert_eq!(
        //     Computer::new(vec![
        //         109, 1, 204, -1, 1001, 100, 1, 100, 1008, 100, 16, 101, 1006, 101, 0, 99
        //     ])
        //     .run(vec![]),
        //     vec![109, 1, 204, -1, 1001, 100, 1, 100, 1008, 100, 16, 101, 1006, 101, 0, 99]
        // );

        assert_eq!(Computer::new(&vec![109, -1, 4, 1, 99]).run(vec![]), vec![-1]);

        assert_eq!(
            Computer::new(&vec![109, -1, 104, 1, 99]).run(vec![]),
            vec![1]
        );

        assert_eq!(
            Computer::new(&vec![109, -1, 204, 1, 99]).run(vec![]),
            vec![109]
        );

        assert_eq!(
            Computer::new(&vec![109, -1, 204, 1, 99]).run(vec![]),
            vec![109]
        );

        assert_eq!(
            Computer::new(&vec![109, 1, 9, 2, 204, -6, 99]).run(vec![]),
            vec![204]
        );

        assert_eq!(
            Computer::new(&vec![109, 1, 109, 9, 204, -6, 99]).run(vec![]),
            vec![204]
        );

        assert_eq!(
            Computer::new(&vec![109, 1, 209, -1, 204, -106, 99]).run(vec![]),
            vec![204]
        );

        assert_eq!(
            Computer::new(&vec![109, 1, 3, 3, 204, 2, 99]).run(vec![100]),
            vec![100]
        );

        assert_eq!(
            Computer::new(&vec![109, 1, 203, 2, 204, 2, 99]).run(vec![200]),
            vec![200]
        );
    }
}
