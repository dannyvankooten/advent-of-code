use std::io::prelude::*;
use std::fs::File;

fn main() {
    let mut f = File::open("input.txt").unwrap();
    let mut input = String::new();
    f.read_to_string(&mut input).unwrap();

    let program_input : Vec<i64> = input.split_terminator(",").map(|v| v.parse().unwrap()).collect();
    println!("part 1: {}", run_intcode(&program_input, 12, 2));

    'outer : for noun in 0..100 {
        for verb in 0..100 {
            if run_intcode(&program_input, noun, verb) == 19690720 {
                println!("part 2: {}", 100 * noun + verb);
                break 'outer;
            }
        }
    }

    println!("Done");
}

fn run_intcode(input : &Vec<i64>, noun : i64, verb: i64) -> i64 {
    let mut input = input.clone();
    input[1] = noun;
    input[2] = verb;
    intcode(input)[0]
}


fn intcode(mut data : Vec<i64>) -> Vec<i64> {
    let mut i : usize = 0;
    while i < data.len() {
        let opcode = data[i];

        if opcode == 99 {
            break;
        }
       
        let p1_pos = data[i+1] as usize;
        let p2_pos = data[i+2] as usize;
        let result_pos = data[i+3] as usize;

        match opcode {
            1 => {
                data[result_pos] = data[p1_pos] + data[p2_pos];
            },
            2 => {
                data[result_pos] = data[p1_pos] * data[p2_pos];
            },
            _ => {
                panic!("Invalid opcode");
            },
        };

        i = i + 4;
    }

    data
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_intcode() {
        assert_eq!(intcode(vec![1,0,0,0,99]), vec![2,0,0,0,99]);
        assert_eq!(intcode(vec![2,3,0,3,99]), vec![2,3,0,6,99]);
        assert_eq!(intcode(vec![2,4,4,5,99,0]), vec![2,4,4,5,99,9801]);
        assert_eq!(intcode(vec![1,1,1,4,99,5,6,0,99]), vec![30,1,1,4,2,5,6,0,99]);
    }
}