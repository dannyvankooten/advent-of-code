use std::io;
use std::time::Instant;

fn main() {
    let now = Instant::now();
    let input: Vec<i32> = std::io::read_to_string(io::stdin())
        .unwrap()
        .trim()
        .split('-')
        .map(|v| v.parse::<i32>().unwrap())
        .collect();

    let mut pt1 = 0;
    let mut pt2 = 0;
    for mut n in input[0]..=input[1] {
        let mut digits : [u8; 6] = [0; 6];
        for i in 0..6 {
            digits[6 - 1 - i] = (n % 10) as u8;
            n /= 10;
        }

        let mut has_double = false;
        let mut has_double_pt2 = false;
        let mut decreases = false;
        for i in 0..6 - 1 {
            if digits[i] == digits[i + 1] {
                has_double = true;

                if (i == 0 || digits[i] != digits[i - 1]) && (i >= 4 || digits[i] != digits[i + 2])
                {
                    has_double_pt2 = true;
                }
            }

            if digits[i] > digits[i + 1] {
                decreases = true;
                break;
            }
        }

        if has_double && !decreases {
            pt1 += 1;

            if has_double_pt2 {
                pt2 += 1;
            }
        }
    }

    println!("Part 1: {}", pt1);
    println!("Part 2: {}", pt2);
    println!("Time: {:.3?} ms", now.elapsed().as_millis());
}
