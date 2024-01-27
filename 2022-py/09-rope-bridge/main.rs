use std::collections::HashSet;

#[derive(Debug, Copy, Clone, PartialEq, Eq, Hash)]
struct Point {
    x: i64,
    y: i64,
}

impl Point {
    fn follow(&mut self, other: &Point) {
        let dx = other.x - self.x;
        let dy = other.y - self.y;

        if dx.abs() <= 1 && dy.abs() <= 1 {
            return;
        }

        if dx == 0 {
            self.y += if dy == 2 { 1 } else { -1 };
        } else if dy == 0 {
            self.x += if dx == 2 { 1 } else { -1 };
        } else {
            self.y += if dy > 0 { 1 } else { -1 };
            self.x += if dx > 0 { 1 } else { -1 };
        }

    }

    fn step(&mut self, dir: &str) {
        match dir {
            "R" => self.x += 1,
            "L" => self.x -= 1,
            "U" => self.y += 1,
            "D" => self.y -= 1,
            _ => panic!("Invalid direction"),
        }
    }
}

fn solve(input: &str, n: u8) -> usize {
    let mut knots : Vec<Point> = (0..n).map(|_| Point{x: 0, y: 0}).collect();
    let mut seen : HashSet<Point> = HashSet::new();

    for line in input.trim().lines() {
        let mut parts = line.trim().split(' ');
        let dir = parts.next().unwrap();
        let steps : u8 = parts.next().unwrap().parse().unwrap();
        for _ in 0..steps {
            // update head position 
            knots[0].step(dir);

            // update all elements that follow (tails)
            for i in 1..n {
                let prev = knots[i as usize - 1];
                let cur = &mut knots[i as usize];
                cur.follow(&prev);
            }

            seen.insert(*knots.last().unwrap());
        }
    }

    return seen.len();
}

fn main() {
    let input = include_str!("input.txt");
    println!("pt1: {}", solve(&input, 2));
    println!("pt2: {}", solve(&input, 10));
}
