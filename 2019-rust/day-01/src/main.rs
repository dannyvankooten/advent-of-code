use std::io;

fn fuel_required(mass: i32) -> i32 {
    return (mass / 3) - 2;
}

fn main() {
    let lines = io::stdin().lines();
    let mut pt1 : i32 = 0;
    let mut pt2 : i32 = 0;

    for line in lines {
        let mass : i32 = match line.unwrap().parse() {
            Ok(n) => n,
            Err(_e) => break,
        };

        let mut fuel = fuel_required(mass);
        pt1 += fuel;

        while fuel > 0 {
            pt2 += fuel;
            fuel = fuel_required(fuel);
        }
    }
    println!("Part 1: {}", pt1);
    println!("Part 2: {}", pt2);
}
