use std::fs;

fn main() {
    let input = fs::read_to_string("input.txt").expect("Error reading input file");
    let masses : Vec<i64> = input.split_terminator("\n").map(|v| v.trim().parse().unwrap()).collect();

    let fuel : i64 = masses.into_iter().map(fuel_required).map(|mut f| {
        let mut extra = fuel_required(f);
        while extra > 0 {
            f = f + extra;
            extra = fuel_required(extra);
        }

        f
    }).sum();

    
    println!("{:?}", fuel);

}

fn fuel_required(mass : i64) -> i64 {
    std::cmp::max(0, (mass as f64 / 3.0).floor() as i64 - 2)
}


#[cfg(test)]
mod test {
    use crate::fuel_requirement;

    #[test]
    fn test_fuel_requirement() {
        assert_eq!(fuel_requirement(12), 2);
        assert_eq!(fuel_requirement(14), 2);
        assert_eq!(fuel_requirement(1969), 654);
        assert_eq!(fuel_requirement(100756), 33583);
    }
}