use std::fs;

const MODE_POSITION: i64 = 0;
const MODE_IMMEDIATE: i64 = 1;
const OP_ADD: i64 = 1;
const OP_MULTIPLY: i64 = 2;
const OP_INPUT: i64 = 3;
const OP_OUTPUT: i64 = 4;
const OP_JUMP_IF_TRUE: i64 = 5;
const OP_JUMP_IF_FALSE: i64 = 6;
const OP_LESS_THAN: i64 = 7;
const OP_EQUALS: i64 = 8;

fn main() {
    let input = fs::read_to_string("input.txt").expect("Error reading input file");
    let program_input: Vec<i64> = input
        .split_terminator(",")
        .map(|v| v.parse().unwrap())
        .collect();
    let user_input = 5;

    let output = intcode(program_input, user_input);
    println!("Output: {:?}", output);
}

fn intcode(mut data: Vec<i64>, user_input: i64) -> Vec<i64> {
    let mut i: usize = 0;
    let mut output: Vec<i64> = vec![];

    while i < data.len() {
        let opcode = data[i];
        if opcode == 99 {
            break;
        }

        let instruction = opcode.to_string();
        let mut chars = instruction.chars().rev();

        // parse opcode
        let opcode = chars.next().unwrap().to_digit(10).unwrap() as i64;
        chars.next(); // ignore 2nd opcode char, we already checked for opcode 99 above

        // parse param modes
        let mut param_modes: Vec<i64> = vec![MODE_POSITION, MODE_POSITION];
        let mut j: usize = 0;
        while let Some(c) = chars.next() {
            param_modes[j] = c.to_digit(10).unwrap() as i64;
            j = j + 1;
        }

        // parse params
        let mut params: Vec<i64> = vec![];
        match opcode {
            OP_ADD | OP_MULTIPLY | OP_LESS_THAN | OP_EQUALS => {
                params.push(get_parameter_value(&data, i + 1, param_modes[0]));
                params.push(get_parameter_value(&data, i + 2, param_modes[1]));
                params.push(data[i + 3]); // result pos is always in position mode, but we don't need its value
            }
            OP_INPUT | OP_OUTPUT => params.push(data[i + 1]),
            OP_JUMP_IF_FALSE | OP_JUMP_IF_TRUE => {
                params.push(get_parameter_value(&data, i + 1, param_modes[0]));
                params.push(get_parameter_value(&data, i + 2, param_modes[1]));
            }
            _ => panic!("invalid opcode"),
        }

        // run operation
        match opcode {
            OP_ADD => data[params[2] as usize] = params[0] + params[1],
            OP_MULTIPLY => data[params[2] as usize] = params[0] * params[1],
            OP_INPUT => data[params[0] as usize] = user_input,
            OP_OUTPUT => output.push(data[params[0] as usize]),
            OP_JUMP_IF_TRUE => {
                if params[0] != 0 {
                    i = params[1] as usize;
                    continue;
                }
            }
            OP_JUMP_IF_FALSE => {
                if params[0] == 0 {
                    i = params[1] as usize;
                    continue;
                }
            }
            OP_LESS_THAN => data[params[2] as usize] = if params[0] < params[1] { 1 } else { 0 },
            OP_EQUALS => data[params[2] as usize] = if params[0] == params[1] { 1 } else { 0 },
            _ => panic!("Invalid opcode"),
        }

        // increment pointer by number of values in operation
        i = i + params.len() + 1;
    }

    output
}

fn get_parameter_value(data: &Vec<i64>, pos: usize, mode: i64) -> i64 {
    match mode {
        MODE_POSITION => data[data[pos] as usize],
        MODE_IMMEDIATE => data[pos],
        _ => panic!("invalid mode"),
    }
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_intcode() {
        assert_eq!(intcode(vec![1002, 4, 3, 4, 33], 0), vec![]);
        assert_eq!(intcode(vec![3, 0, 4, 0, 99], 0), vec![0]);
        assert_eq!(
            intcode(vec![3, 9, 8, 9, 10, 9, 4, 9, 99, -1, 8], 8),
            vec![1]
        );
        assert_eq!(
            intcode(vec![3, 9, 8, 9, 10, 9, 4, 9, 99, -1, 8], 7),
            vec![0]
        );
        assert_eq!(
            intcode(vec![3, 9, 8, 9, 10, 9, 4, 9, 99, -1, 8], 9),
            vec![0]
        );
    }
}
