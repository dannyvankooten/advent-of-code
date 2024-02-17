use std::io;
use std::time::Instant;

fn main() {
    let t = Instant::now();
    let lines = io::stdin().lines();
    let pt1 = 0;
    let pt2 = 0;
    println!("Part 1: {}", pt1);
    println!("Part 2: {}", pt2);
    println!("Time: {:.3?} ms", t.elapsed().as_millis());
}
