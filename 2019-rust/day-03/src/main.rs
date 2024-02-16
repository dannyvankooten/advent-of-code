use std::collections::HashMap;
use std::io;
use std::time::Instant;

fn main() {
    let now = Instant::now();
    let wires: Vec<HashMap<(i32, i32), i32>> = io::stdin()
        .lines()
        .map(|line| {
            let mut wire = HashMap::new();
            let mut x = 0;
            let mut y = 0;
            let mut s = 0;
            let raw = line.unwrap();
            for i in raw.trim().split(',') {
                let mut value: i32 = i32::from_str_radix(&i[1..], 10).unwrap();
                let ch = i.chars().nth(0).unwrap();
                while value > 0 {
                    match ch {
                        'D' => y += 1,
                        'U' => y -= 1,
                        'L' => x -= 1,
                        _ => x += 1,
                    }

                    value -= 1;
                    s += 1;
                    wire.insert((x, y), s);
                }
            }

            wire
        })
        .collect();

    let mut closest = 1 << 30;
    let mut min_steps = 1 << 30;
    for (coords, s1) in &wires[0] {
        if let Some(s2) = wires[1].get(coords) {
            closest = i32::min(closest, coords.0.abs() + coords.1.abs());
            min_steps = i32::min(min_steps, s1 + s2);
        }
    }

    let pt1 = closest;
    let pt2 = min_steps;
    println!("Part 1: {}", pt1);
    println!("Part 2: {}", pt2);
    println!("Time: {:.3?}", now.elapsed());
}
