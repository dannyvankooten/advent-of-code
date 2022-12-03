#[repr(u8)]
#[derive(Debug, Copy, Clone)]
enum Op {
    Nothing,
    Add,
    Multiply,
    Input,
    Output,
    JumpIfTrue,
    JumpIfFalse,
    LessThan,
    Equals,
    OffsetRelativeBase,
}

#[repr(u8)]
#[derive(Debug, Copy, Clone, PartialEq, Eq)]
enum ParameterMode {
    Position,
    Immediate,
    Relative,
}

fn main() {
    let program: Vec<i64> = include_str!("../input.txt")
        .trim()
        .split_terminator(',')
        .map(|v| v.parse().unwrap())
        .collect();

    let output = eval(program.clone(), vec![1]);
    assert_eq!(output, 2932210790);
    println!("Part 1: {:?}", output);

    let output = eval(program.clone(), vec![2]);
    assert_eq!(output, 73144);
    println!("Part 2: {:?}", output);
}

#[inline(always)]
fn get_address(
    program: &[i64],
    relative_base: usize,
    address: usize,
    mode: ParameterMode,
) -> usize {
    match mode {
        ParameterMode::Position => program[address] as usize,
        ParameterMode::Immediate => address,
        ParameterMode::Relative => (program[address] + relative_base as i64) as usize,
    }
}

#[inline(always)]
fn get_value(program: &[i64], relative_base: usize, address: usize, mode: ParameterMode) -> i64 {
    let address = get_address(program, relative_base, address, mode);
    program[address]
}

fn eval(mut program: Vec<i64>, mut input: Vec<i64>) -> i64 {
    assert_eq!(input.len(), 1);
    let input = input.pop().unwrap();
    let mut result = 0;
    let mut address = 0;
    let mut relative_base = 0;

    // create some additional space for new variables (sometimes set outside of current length, but never very far)
    program.extend(&[0; 256]);

    loop {
        let opcode = program[address];
        if opcode == 99 {
            break;
        }

        let mut instruction = opcode;
        // let opcode : Op = unsafe {
        //     std::mem::transmute(((instruction & 10)) as u8)
        // };
        let opcode = match instruction % 10 {
            1 => Op::Add,
            2 => Op::Multiply,
            3 => Op::Input,
            4 => Op::Output,
            5 => Op::JumpIfTrue,
            6 => Op::JumpIfFalse,
            7 => Op::LessThan,
            8 => Op::Equals,
            9 => Op::OffsetRelativeBase,
            _ => unreachable!("Invalid opcode"),
        };
        instruction /= 100;

        let mut param_modes = [
            ParameterMode::Position,
            ParameterMode::Position,
            ParameterMode::Position,
        ];
        let mut digit: u8 = 0;
        while instruction != 0 {
            param_modes[digit as usize] = match instruction % 10 {
                0 => ParameterMode::Position,
                1 => ParameterMode::Immediate,
                2 => ParameterMode::Relative,
                _ => panic!("Invalid parameter mode"),
            };
            instruction /= 10;
            digit += 1;
        }
        // dbg!(&param_modes);

        // println!(
        //     "address: {:?}\ninstruction: {:?}\nopcode: {:?}\nparams: {:?}\nparam_modes: {:?}\nrelative base: {:?}\noutput: {:?}\nmemory before op: {:?}",
        //     self.address, instruction, opcode, params, param_modes, self.relative_base, output, self.memory
        // );

        // run operation
        match opcode {
            Op::Nothing => unreachable!("no no"),
            Op::Add => {
                let lhs = get_value(&program, relative_base, address + 1, param_modes[0]);
                let rhs = get_value(&program, relative_base, address + 2, param_modes[1]);
                let idx = get_address(&program, relative_base, address + 3, param_modes[2]);
                program[idx] = lhs + rhs;
                address += 4;
            }
            Op::Multiply => {
                let lhs = get_value(&program, relative_base, address + 1, param_modes[0]);
                let rhs = get_value(&program, relative_base, address + 2, param_modes[1]);
                let idx = get_address(&program, relative_base, address + 3, param_modes[2]);
                program[idx] = lhs * rhs;
                address += 4;
            }
            Op::Input => {
                let idx = get_address(&program, relative_base, address + 1, param_modes[0]);
                program[idx] = input;
                address += 2;
            }
            Op::Output => {
                result = get_value(&program, relative_base, address + 1, param_modes[0]);
                address += 2;
            }
            Op::JumpIfTrue => {
                let condition = get_value(&program, relative_base, address + 1, param_modes[0]);
                if condition != 0 {
                    address =
                        get_value(&program, relative_base, address + 2, param_modes[1]) as usize;
                    continue;
                }
                address += 3;
            }
            Op::JumpIfFalse => {
                let condition = get_value(&program, relative_base, address + 1, param_modes[0]);
                if condition == 0 {
                    address =
                        get_value(&program, relative_base, address + 2, param_modes[1]) as usize;
                    continue;
                }
                address += 3;
            }
            Op::LessThan => {
                let lhs = get_value(&program, relative_base, address + 1, param_modes[0]);
                let rhs = get_value(&program, relative_base, address + 2, param_modes[1]);
                let idx = get_address(&program, relative_base, address + 3, param_modes[2]);
                program[idx] = if lhs < rhs { 1 } else { 0 };
                address += 4;
            }
            Op::Equals => {
                let lhs = get_value(&program, relative_base, address + 1, param_modes[0]);
                let rhs = get_value(&program, relative_base, address + 2, param_modes[1]);
                let idx = get_address(&program, relative_base, address + 3, param_modes[2]);
                program[idx] = if lhs == rhs { 1 } else { 0 };
                address += 4;
            }
            Op::OffsetRelativeBase => {
                let value = get_value(&program, relative_base, address + 1, param_modes[0]);
                relative_base = relative_base.wrapping_add(value as usize);
                address += 2;
            }
        }
    }

    result
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_intcode() {
        // 104,1125899906842624,99 should output the large number in the middle.
        assert_eq!(
            eval(vec![104, 1125899906842624, 99], vec![]),
            1125899906842624
        );

        // 1102,34915192,34915192,7,4,7,99,0 should output a 16-digit number.
        let output = eval(vec![1102, 34915192, 34915192, 7, 4, 7, 99, 0], vec![]);
        assert_eq!(output.to_string().len(), 16);

        assert_eq!(
            eval(
                vec![3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, -1, 0, 1, 9],
                vec![0]
            ),
            0
        );
        assert_eq!(
            eval(
                vec![3, 12, 6, 12, 15, 1, 13, 14, 13, 4, 13, 99, -1, 0, 1, 9],
                vec![1]
            ),
            1
        );
        assert_eq!(
            eval(
                vec![3, 3, 1105, -1, 9, 1101, 0, 0, 12, 4, 12, 99, 1],
                vec![0]
            ),
            0
        );
        assert_eq!(
            eval(
                vec![3, 3, 1105, -1, 9, 1101, 0, 0, 12, 4, 12, 99, 1],
                vec![1]
            ),
            1
        );
        // assert_eq!(
        //     eval(vec![204, 1125899906842624, 99], vec![]),
        //     0
        // );
        assert_eq!(eval(vec![109, 1, 204, 0, 99], vec![]), 1);

        // // 109,1,204,-1,1001,100,1,100,1008,100,16,101,1006,101,0,99 takes no input and produces a copy of itself as output.
        // assert_eq!(
        //     Computer::new(vec![
        //         109, 1, 204, -1, 1001, 100, 1, 100, 1008, 100, 16, 101, 1006, 101, 0, 99
        //     ])
        //     .run(vec![]),
        //     vec![109, 1, 204, -1, 1001, 100, 1, 100, 1008, 100, 16, 101, 1006, 101, 0, 99]
        // );

        assert_eq!(eval(vec![109, -1, 4, 1, 99], vec![]), -1);

        assert_eq!(eval(vec![109, -1, 104, 1, 99], vec![]), 1);

        assert_eq!(eval(vec![109, -1, 204, 1, 99], vec![]), 109);

        assert_eq!(eval(vec![109, -1, 204, 1, 99], vec![]), 109);

        assert_eq!(eval(vec![109, 1, 9, 2, 204, -6, 99], vec![]), 204);

        assert_eq!(eval(vec![109, 1, 109, 9, 204, -6, 99], vec![]), 204);

        assert_eq!(eval(vec![109, 1, 209, -1, 204, -106, 99], vec![]), 204);

        assert_eq!(eval(vec![109, 1, 3, 3, 204, 2, 99], vec![100]), 100);

        assert_eq!(eval(vec![109, 1, 203, 2, 204, 2, 99], vec![200]), 200);
    }
}
