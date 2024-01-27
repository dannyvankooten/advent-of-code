use std::collections::HashSet;

#[derive(Debug, Copy, Clone, Eq, PartialEq, Hash)]
struct Point {
    x: i64,
    y: i64,
}

#[derive(Debug)]
struct Sensor {
    pos: Point,
    size: i64,
}

impl Point {
    fn new(x: i64, y: i64) -> Self {
        return Point{ x, y }
    }
}

impl Sensor {
    fn new(sx: i64, sy: i64, beacon_pos: &Point) -> Self {
        let mut top = Point::new(sx, sy + 1);
        let pos = Point{ x: sx, y: sy };
        let treshold = manhattan_distance(&pos, beacon_pos); 
        while manhattan_distance(&pos, &top) < treshold {
            top.y += 1;
        }

        Sensor {
            pos,
            size: top.y - pos.y,
        }
    }
    
    #[inline]
    fn get_width_on_pos(&self, pos: &Point) -> i64 {
        let dy = (pos.y - self.pos.y).abs();
        let size = self.size - dy;
        if size <= 0 {
           return 0;
        }

        let x1 = self.pos.x - size;
        if x1 > pos.x {
            return 0;
        }

        let x2 = self.pos.x + size;
        if x2 < pos.x {
            return 0;
        }

        return size;
    }
}

#[inline(always)]
fn manhattan_distance(a: &Point, b: &Point) -> i64 {
    (a.x - b.x).abs() + (a.y - b.y).abs()
}

fn parse() -> (Vec<Sensor>, HashSet<Point>) {
    let input = include_str!("input.txt");
    let mut sensors = Vec::new(); 
    let mut beacons = HashSet::new();
    let input = input.replace("Sensor at ", "").replace(": closest beacon is at", "").replace(",", "").replace("x=", "").replace("y=", "");
    for line in input.trim().lines() {
        let parts : Vec<i64> = line.split(" ").map(|p| p.parse().unwrap()).collect();
        let [sx, sy, bx, by] = parts[..] else { todo!(); };
        let beacon_pos = Point::new(bx, by);
        sensors.push(Sensor::new(sx, sy, &beacon_pos));
        beacons.insert(beacon_pos);
    }
    
    (sensors, beacons)
}

fn pt1(sensors: &Vec<Sensor>, beacons: &HashSet<Point>) -> i64 {
    let mut ranges = Vec::new();

    for s in sensors {
        let size = s.get_width_on_pos(&Point::new(s.pos.x, 2000000));
        if size == 0 {
            continue;
        }
        ranges.push((s.pos.x - size, s.pos.x + size));
    }
    
    ranges.sort();
    let mut count = 0;
    let mut prev = ranges[0].0 - 1;
    for (mut start, end) in ranges {
        if start <= prev {
            start = prev + 1;
        }
        let diff = end - start + 1;
        if diff > 0 {
            count += end - start + 1;
        }

        if end > prev {
            prev = end;
        }
    }
    
    for b in beacons {
        if b.y == 2000000 {
            count -= 1;
        }
    }

    return count;
}

fn pt2(sensors: &Vec<Sensor>) -> i64 {
    let mut pos = Point::new(0, 4000000);
    let mut found = false;
    while found == false {
        found = true;

        for s in sensors {
            let size = s.get_width_on_pos(&pos);
            if size == 0 {
                continue;
            }

            found = false;
            pos.x = s.pos.x + size + 1;
            if pos.x >= 4000000 {
                pos.y -= 1;
                pos.x = 0;
            }
            break;
        }
    }

    return pos.x * 4000000 + pos.y;
}

fn main() {
    let (sensors, beacons) = parse();
    let pt1 = pt1(&sensors, &beacons);
    let pt2 = pt2(&sensors);
    println!("pt1: {}\npt2: {}", pt1, pt2);
}
