const OP_ADD : i64 = 1;
const OP_MUL : i64 = 2;
const OP_HALT : i64 = 99;

fn main() {
    let input = include_str!("../input.txt");
    let input : Vec<i64> = input.split_terminator(",").map(|v| v.parse().unwrap()).collect();

    println!("part 1: {}", eval_modified(&input, 12, 2));

    'outer : for noun in 0..100 {
        for verb in 0..100 {
            if eval_modified(&input, noun, verb) == 19690720 {
                println!("part 2: {}", 100 * noun + verb);
                break 'outer;
            }
        }
    }
}

fn eval_modified(input : &Vec<i64>, noun : i64, verb: i64) -> i64 {
    let mut input = input.clone();
    input[1] = noun;
    input[2] = verb;
    eval(input)[0]
}

fn eval(mut data : Vec<i64>) -> Vec<i64> {
    let mut i : usize = 0;
    while i < data.len() {
        match data[i] {
            OP_ADD => {
                let p1 = data[i+1] as usize;
                let p2 = data[i+2] as usize;
                let idx = data[i+3] as usize;
                data[idx] = data[p1] + data[p2];
            },
            OP_MUL => {
                let p1 = data[i+1] as usize;
                let p2 = data[i+2] as usize;
                let idx = data[i+3] as usize;
                data[idx] = data[p1] * data[p2];
            },
            OP_HALT => {
                break;
            },
            _ => panic!("Invalid opcode {:?} at pos {:?}", data[i], i)
        }
       
        i = i + 4;
    }

    data
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_intcode() {
        assert_eq!(eval(vec![1,0,0,0,99]), vec![2,0,0,0,99]);
        assert_eq!(eval(vec![2,3,0,3,99]), vec![2,3,0,6,99]);
        assert_eq!(eval(vec![2,4,4,5,99,0]), vec![2,4,4,5,99,9801]);
        assert_eq!(eval(vec![1,1,1,4,99,5,6,0,99]), vec![30,1,1,4,2,5,6,0,99]);
    }
}