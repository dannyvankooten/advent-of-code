use std::fs;
use std::sync::mpsc::{channel, Receiver, Sender};
use std::thread;

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
    let program: Vec<i64> = input
        .trim()
        .split_terminator(",")
        .map(|v| v.parse().unwrap())
        .collect();

    // TODO: Skip duplicate phase sequence parameters

    let mut max = 0;
    for ps1 in 5..10 {
        for ps2 in 5..10 {
            for ps3 in 5..10 {
                for ps4 in 5..10 {
                    for ps5 in 5..10 {
                        let phase_sequence = [ps1, ps2, ps3, ps4, ps5];
                        if has_duplicate_elements(&phase_sequence) {
                            continue;
                        }

                        let signal = run_amplifiers(program.clone(), phase_sequence);
                        if signal > max {
                            max = signal;

                            println!("Max {} from phase sequence {:?}", max, phase_sequence);
                        }
                    }
                }
            }
        }
    }

    println!("Max thruster signal {}", max)
}

fn has_duplicate_elements(slice: &[i64; 5]) -> bool {
    let mut slice = slice.clone();
    slice.sort_unstable();
    slice.windows(2).any(|win| win[0] == win[1])
}

fn run_amplifiers(program: Vec<i64>, phase_sequence: [i64; 5]) -> i64 {
    let (output_main, input_a) = channel();
    let (output_a, input_b) = channel();
    let (output_b, input_c) = channel();
    let (output_c, input_d) = channel();
    let (output_d, input_e) = channel();
    let (output_e, input_main) = channel();

    output_main.send(phase_sequence[0]).expect("Error sending");
    output_main.send(0).expect("Error sending"); // initial second input for Amplifier A
    output_a.send(phase_sequence[1]).expect("Error sending");
    output_b.send(phase_sequence[2]).expect("Error sending");
    output_c.send(phase_sequence[3]).expect("Error sending");
    output_d.send(phase_sequence[4]).expect("Error sending");

    let p = program.clone();
    thread::spawn(move || {
        intcode(p, input_a, output_a);
    });
    let p = program.clone();
    thread::spawn(move || {
        intcode(p, input_b, output_b);
    });
    let p = program.clone();
    thread::spawn(move || {
        intcode(p, input_c, output_c);
    });
    let p = program.clone();
    thread::spawn(move || {
        intcode(p, input_d, output_d);
    });
    let p = program.clone();
    thread::spawn(move || {
        intcode(p, input_e, output_e);
    });

    let mut final_value = -1;

    // send output from amp E to input from A
    while let Ok(received_value) = input_main.recv() {
        let _ = output_main.send(received_value);
        final_value = received_value;
    }

    // return last received output from amp E
    final_value
}

fn intcode(mut data: Vec<i64>, input: Receiver<i64>, output: Sender<i64>) {
    let mut i: usize = 0;

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
        let mut param_modes = [MODE_POSITION, MODE_POSITION];
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
            OP_INPUT => {
                data[params[0] as usize] = input.recv().unwrap();
            }
            OP_OUTPUT => {
                output
                    .send(data[params[0] as usize])
                    .expect("Output channel has gone away");
            }
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
            _ => panic!("invalid opcode"),
        }

        // increment pointer by number of values in operation
        i = i + params.len() + 1;
    }
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
    fn test_has_duplicate_elements() {
        assert!(has_duplicate_elements(&[0, 0, 1, 2, 3]));
        assert!(has_duplicate_elements(&[1, 2, 3, 4, 1]));
        assert_eq!(false, has_duplicate_elements(&[0, 5, 4, 3, 2]));
    }

    #[test]
    fn test_intcode() {
        assert_eq!(
            run_amplifiers(
                vec![3, 15, 3, 16, 1002, 16, 10, 16, 1, 16, 15, 15, 4, 15, 99, 0, 0],
                [4, 3, 2, 1, 0]
            ),
            43210
        );
        assert_eq!(
            run_amplifiers(
                vec![
                    3, 23, 3, 24, 1002, 24, 10, 24, 1002, 23, -1, 23, 101, 5, 23, 23, 1, 24, 23,
                    23, 4, 23, 99, 0, 0
                ],
                [0, 1, 2, 3, 4]
            ),
            54321
        );
        assert_eq!(
            run_amplifiers(
                vec![
                    3, 31, 3, 32, 1002, 32, 10, 32, 1001, 31, -2, 31, 1007, 31, 0, 33, 1002, 33, 7,
                    33, 1, 33, 31, 31, 1, 32, 31, 31, 4, 31, 99, 0, 0, 0
                ],
                [1, 0, 4, 3, 2]
            ),
            65210
        );

        assert_eq!(
            run_amplifiers(
                vec![
                    3, 26, 1001, 26, -4, 26, 3, 27, 1002, 27, 2, 27, 1, 27, 26, 27, 4, 27, 1001,
                    28, -1, 28, 1005, 28, 6, 99, 0, 0, 5
                ],
                [9, 8, 7, 6, 5]
            ),
            139629729
        );
    }
}
