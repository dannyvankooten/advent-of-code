#![feature(clamp)]

use std::collections::HashSet;
use std::hash::Hash;
use std::fs;

fn main() {
    let input = fs::read_to_string("input.txt").expect("Error reading input file");
    let map = parse_map(input);

    let (best, count ) = find_best_location(&map);
    println!("Best: {:?} with count: {}", best, count);
}

//fn count_asteroids_in_sight()

fn find_best_location(map : &Map) -> (Option<&Asteroid>, usize) {
    let mut best : Option<&Asteroid> = None;
    let mut best_count = 0;

    for a1 in &map.set {
        let count = map.set.iter().filter(|a2| a1 != *a2 && map.line_of_sight(a1, a2)).count();
        if count > best_count {
            best = Some(a1);
            best_count = count;
        }
    }

    return (best, best_count);
}

#[derive(Eq, PartialEq, Hash, Debug)]
struct Asteroid {
    x: usize,
    y: usize,
}

struct Map {
    set : HashSet<Asteroid>
}


fn reduced_angle(from: (usize, usize), to: (usize, usize)) -> (isize, isize) {
    let from = (from.0 as isize, from.1 as isize);
    let to = (to.0 as isize, to.1 as isize);

    let x_diff = to.0 - from.0;
    let y_diff = to.1 - from.1;

    let gcf = gcf(x_diff, y_diff);

    (x_diff / gcf, y_diff / gcf)
}

fn gcf(a: isize, b: isize) -> isize {
    if b == 0 {
        a.abs()
    } else {
        gcf(b, a % b)
    }
}

impl Map {
    fn line_of_sight(&self, a1 : &Asteroid, a2 : &Asteroid) -> bool {
       
        let angle = reduced_angle((a1.x, a1.y), (a2.x, a2.y));
        let mut x = a1.x as isize + angle.0;
        let mut y = a1.y as isize + angle.1;
        while x != a2.x as isize || y != a2.y as isize {
            if let Some(_) = self.set.get(&Asteroid{x: x as usize, y: y as usize}) {
                return false;
            }

            x = x + angle.0;
            y = y + angle.1;
        }

        println!("{:?} can see {:?}", a1, a2);
        true
    }

    fn get(&self, x : usize, y : usize) -> Option<&Asteroid> {
        return self.set.get(&Asteroid{x, y});
    }
}

fn parse_map(map: String) -> Map {
    let mut set: HashSet<Asteroid> = HashSet::new();

    for (y, line) in map.lines().enumerate() {
        for (x, c) in line.trim().chars().enumerate() {
            // skip non-asteroid chars
            if c != '#' {
                continue;
            }

            set.insert(Asteroid {
                x, y
            });
        }
    }

    Map{set}
}

#[cfg(test)]
mod test {
    use super::*;

    #[test]
    fn test_parse_map() {
        let map = parse_map(
            ".#..#
            .....
            #####
            ....#
            ...##"
                .to_string(),
        );

        assert_eq!(map.set.len(), 10);
        assert_eq!(map.get(0, 0), None);
        assert_eq!(map.get( 1, 0), Some(&Asteroid{x: 1, y: 0}));
        assert_eq!(map.get( 2, 0), None);
        assert_eq!(map.get(4, 3), Some(&Asteroid{x: 4, y: 3}));
        assert_eq!(map.get(3, 4), Some(&Asteroid{x: 3, y: 4}));
        assert_eq!(map.get(4, 4), Some(&Asteroid{x: 4, y: 4}));
    }

    #[test]
    fn test_asteroid_can_see() {
        let map = parse_map(
            ".#..#
            .....
            #####
            ....#
            ...##"
                .to_string(),
        );

        let a1 = map.get(1, 0).unwrap();
        let a2 = map.get(4, 0).unwrap();
        assert!(map.line_of_sight(a1, a2)); 
        assert!(map.line_of_sight(a2, a1)); 

        assert_eq!(map.line_of_sight(map.get(0, 2).unwrap(), map.get(1, 2).unwrap()), true);
        assert_eq!(map.line_of_sight(map.get(0, 2).unwrap(), map.get(2, 2).unwrap()), false);
        assert_eq!(map.line_of_sight(map.get(4, 4).unwrap(), map.get(4, 0).unwrap()), false);
    }

    #[test]
    fn test_find_best_location() {
        let map = parse_map(
            ".#..#
            .....
            #####
            ....#
            ...##"
                .to_string(),
        );

        let (best, count) = find_best_location(&map);
        println!("{:?}: {}", best, count);
        assert_eq!(best, map.get(3, 4));
        assert_eq!(count, 8);
    }
}
