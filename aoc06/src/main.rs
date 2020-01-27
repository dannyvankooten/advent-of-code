use std::collections::{HashMap};
use std::fs;

fn main() {
    let input = fs::read_to_string("input.txt").expect("Error reading input file");
    let graph = Graph::new(input);
    println!("Total orbits: {}", graph.total_orbits());
    println!("Distance between YOU and SAN: {}", graph.count_distance("YOU", "SAN"));
}

struct Graph {
   objects: HashMap<String, String>,
}

impl Graph {
    fn new(map : String) -> Graph {
        let path : Vec<(String, String)> = map.lines().map(|l| {
            let mut iter = l.split_terminator(')');
            return (iter.next().unwrap().to_string(), iter.next().unwrap().to_string());
        }).collect();
    
        let mut graph = Graph{
            objects: HashMap::with_capacity(path.len())
        };
        for (cur, next) in path {
           graph.objects.insert(next, cur);
        }

        graph   
    }

    fn count_orbits(&self, p : &str) -> u32 {
        let mut orbits = 0;
        let mut name = p.to_string();
        while let Some(p) = self.objects.get(&name) {
            orbits = orbits + 1;
            name = p.to_string();
        }
        orbits
    }

    fn total_orbits(&self) -> u32 {
        self.objects.keys().map(|k| self.count_orbits(k)).sum()
    }

    fn count_distance(&self, from : &str, to : &str) -> u32 {
        let path1 = self.path(from);

        if let Some(d2) = path1.get(to) {
            println!("Target is directly in path to COM");
            return *d2;
        }

        let path2 = self.path(to);

        // TODO: Only look at closest intersection here.
        let mut lowest = 0;
        for (planet, d1) in path1 {
            if let Some(d2) = path2.get(&planet) {
                let distance = d1 + d2;
                if lowest == 0 || distance < lowest {
                    lowest = distance;
                }
            }
        }
       
        lowest
    }

    fn path(&self, from : &str) -> HashMap<String, u32> {
        let mut set = HashMap::new();
        let mut name = from.to_string();
        let mut distance = 1;
        while let Some(p) = self.objects.get(&name) {
            set.insert(p.to_string(), distance);
            name = p.to_string();
            distance = distance + 1;
        }
        set
    }
}

#[cfg(test)]
mod test {
    use super::Graph;

    #[test]
    fn test_orbits() {
        let graph = Graph::new("COM)B\nB)C\nC)D\nD)E\nE)F\nB)G\nG)H\nD)I\nE)J\nJ)K\nK)L".to_string());
        assert_eq!(graph.count_orbits("D"), 3);
        assert_eq!(graph.count_orbits("L"), 7);
        assert_eq!(graph.count_orbits("COM"), 0);
        assert_eq!(graph.total_orbits(), 42);

        let graph = Graph::new("COM)A\nA)B\nA)C\nA)D".to_string());
        assert_eq!(graph.count_orbits("COM"), 0);
        assert_eq!(graph.count_orbits("A"), 1);
        assert_eq!(graph.count_orbits("B"), 2);
        assert_eq!(graph.count_orbits("C"), 2);
        assert_eq!(graph.count_orbits("D"), 2);
        assert_eq!(graph.total_orbits(), 7);
        assert_eq!(graph.count_distance("D", "A"), 1);
        assert_eq!(graph.count_distance("D", "COM"), 2);

        let graph = Graph::new("COM)A\nA)B\nA)C\nB)FROM\nC)TO".to_string());
        assert_eq!(graph.count_distance("FROM", "TO"), 4);

        let graph = Graph::new("COM)A\nA)B\nA)C\nB)FROM\nC)D\nD)TO".to_string());
        assert_eq!(graph.count_distance("FROM", "TO"), 5);

    }
}