use std::collections::HashSet;
use std::hash::Hash;
use std::fs;

fn main() {
    let input = fs::read_to_string("input.txt").expect("Error reading input file");
    let map = parse_map(input);
    
    // TODO: Loop over all asteroids. For each asteroid, iterate over all other asteroids and count amount that are in line of sight.
}

#[derive(Eq, PartialEq, Hash, Debug)]
struct Asteroid {
    x: u32,
    y: u32,
}

struct Map {
    set : HashSet<Asteroid>
}

impl Map {
    fn line_of_sight(&self, a1 : &Asteroid, a2 : &Asteroid) -> bool {
        let mut xs = 0;
        let mut ys = 0;

        if a2.x == a1.x {
            ys = 1;
        } else if a2.y == a1.y {
            xs = 1;
        } else {
            let dx = a2.x - a1.x;
            let dy = a2.y - a1.y;
            xs = dx / dy;
            ys = dy / dy;
        }

        let mut x = a1.x + xs;
        let mut y = a1.y + ys;
        while x < a2.x || y < a2.y {
            if let Some(_) = self.set.get(&Asteroid{x, y}) {
                return false;
            }

            x = x + xs;
            y = y + ys;
        }


        true
    }

    fn get(&self, x : u32, y : u32) -> Option<&Asteroid> {
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
                x: x as u32,
                y: y as u32,
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
    }
}
