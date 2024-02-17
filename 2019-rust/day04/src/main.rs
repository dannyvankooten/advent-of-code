use std::time::Instant;

fn main() {
    let t = Instant::now();

    let mut c = 0;
    for n in 165432..707913 {
        if matches_1(n) {
            c = c + 1;
        }
    }

    println!("Answer: {}", c);
    println!("Time: {:.3?}", t.elapsed());
}

// It is a six-digit number.
// The value is within the range given in your puzzle input.
// Two adjacent digits are the same (like 22 in 122345).
// Going from left to right, the digits never decrease; they only ever increase or stay the same (like 111123 or 135679).

// 111111 meets these criteria (double 11, never decreases).
// 223450 does not meet these criteria (decreasing pair of digits 50).
// 123789 does not meet these criteria (no double).

// An Elf just remembered one more important detail: the two adjacent matching digits are not part of a larger group of matching digits.

// Given this additional criterion, but still ignoring the range rule, the following are now true:

//     112233 meets these criteria because the digits never decrease and all repeated digits are exactly two digits long.
//     123444 no longer meets the criteria (the repeated 44 is part of a larger group of 444).
//     111122 meets the criteria (even though 1 is repeated more than twice, it still contains a double 22).

fn matches_1(n : i64) -> bool {
    if n < 165432 || n > 707912 {
        return false;
    }
    let mut consecutive = false;
    let n = n.to_string();
    let bytes : &[u8] = n.as_bytes();

    for i in 0..bytes.len()-1 {
        if bytes[i] == bytes[i+1] && !(i < (bytes.len() - 2) && bytes[i] == bytes[i+2]) && !(i > 0 && bytes[i-1] == bytes[i]){
            consecutive = true;
        }

        if (bytes[i] as char).to_digit(10).unwrap() > (bytes[i+1] as char).to_digit(10).unwrap() {
            return false;
        }
    }

    return consecutive;
}


#[cfg(test)]
mod test {
    use super::matches_1;

    #[test]
    fn test_matches() {
        assert_eq!(matches_1(165431), false);
        assert_eq!(matches_1(123789), false);
        assert_eq!(matches_1(223450), false);
        assert_eq!(matches_1(222456), false);
        assert!(matches_1(234556));
        assert!(matches_1(223344));
        assert!(matches_1(222233));

    }
}
