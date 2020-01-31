use std::hash::{Hash, Hasher};
use std::fs;

#[derive(Debug)]
struct Point(i64, i64, i64);

#[derive(Debug)]
struct Turn {
    dir : char,
    amount: i64,
}

impl Hash for Point {
    fn hash<H: Hasher>(&self, state: &mut H) {
        self.0.hash(state);
        self.1.hash(state);
    }
}
impl PartialEq for Point {
    fn eq(&self, other: &Self) -> bool {
        self.0 == other.0 && self.1 == other.1
    }
}
impl Eq for Point {}

fn main() {
    let input = fs::read_to_string("input.txt").expect("Error reading input file");
    let lines : Vec<&str> = input.lines().collect();
    let closest_intersection_distance = calculate_intersection_distances(lines);
    println!("Answer: {}", closest_intersection_distance);
}

fn calculate_intersection_distances(lines : Vec<&str>) -> i64 {
    let mut wires : [HashSet<Point>; 2] = [HashSet::new(), HashSet::new()];
    use std::collections::HashSet;


    // Calculate wire paths
    for i in 0..2 {
        let turns : Vec<Turn> = lines[i].split_terminator(",").map(|v| {
            let (dir, amount) = v.split_at(1);
            let amount : i64 = amount.parse().unwrap();
            
            Turn {
                dir: dir.chars().nth(0).unwrap(),
                amount
            }
        }).collect();

        let mut x = 0;
        let mut y = 0;
        let mut steps = 0;

        for t in turns {
            for _ in 0..t.amount {
                match t.dir {
                    'R' => x = x + 1,
                    'L' => x = x - 1,
                    'D' => y = y - 1,
                    'U' => y = y + 1,
                    _ => panic!("Invalid char"),
                };
                steps = steps + 1;
                wires[i].insert(Point(x, y, steps));
            }
        }
    }

    let mut closest : i64 = 0;
    for p1 in wires[0].intersection(&wires[1]) {
        if let Some(p2) = wires[0].get(p1) {
            if closest == 0 || (p1.2 + p2.2) < closest {
                closest = p1.2 + p2.2;
            }
        }
    }
    
    return closest;
}

fn distance(p1 : &Point, p2 : &Point) -> i64 {
    (p2.0 - p1.0).abs() + (p2.1 - p1.1).abs()
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_something() {
        assert_eq!(calculate_intersection_distances(vec!["R75,D30,R83,U83,L12,D49,R71,U7,L72", "U62,R66,U55,R34,D71,R55,D58,R83"]), 159);
        assert_eq!(calculate_intersection_distances(vec!["R98,U47,R26,D63,R33,U87,L62,D20,R33,U53,R51", "U98,R91,D20,R16,D67,R40,U7,R15,U6,R7"]), 135);
    }
}