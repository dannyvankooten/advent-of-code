use std::io;
use std::time::Instant;
use std::collections::HashSet;

#[derive(Clone)]
struct Moon {
    x: i32,
    y: i32,
    z: i32,
    vx: i32,
    vy: i32,
    vz: i32,
}

fn solve_pt1(mut moons: Vec<Moon>) -> usize {
    for _s in 0..1000 {
        // first, update velocity for each moon
        for i in 0..moons.len() {
            for j in i+1..moons.len() {
                let dx = if moons[i].x < moons[j].x { 1 } else if moons[i].x > moons[j].x { -1 } else { 0 };
                let dy = if moons[i].y < moons[j].y { 1 } else if moons[i].y > moons[j].y { -1 } else { 0 };
                let dz = if moons[i].z < moons[j].z { 1 } else if moons[i].z > moons[j].z { -1 } else { 0 };

                moons[i].vx += dx;
                moons[j].vx -= dx;
                moons[i].vy += dy;
                moons[j].vy -= dy;
                moons[i].vz += dz;
                moons[j].vz -= dz;
            }
        }

        // update position for all moons
        for m in &mut moons {
            m.x += m.vx;
            m.y += m.vy;
            m.z += m.vz;
        }
    }

    let mut energy : usize = 0;
    for m in moons {
        energy += (m.x.abs() + m.y.abs() + m.z.abs()) as usize * (m.vx.abs() + m.vy.abs() + m.vz.abs()) as usize
    }

    return energy;
}

fn solve_pt2(mut moons: Vec<Moon>) -> u64 {
    let mut seenx : HashSet<u64> = HashSet::new();
    let mut seeny : HashSet<u64> = HashSet::new();
    let mut seenz : HashSet<u64> = HashSet::new();
    let mut mx = 0;
    let mut my = 0;
    let mut mz = 0;
    for _s in 0..std::u64::MAX {
        // first, update velocity for each moon
        for i in 0..moons.len() {
            for j in i+1..moons.len() {
                let dx = if moons[i].x < moons[j].x { 1 } else if moons[i].x > moons[j].x { -1 } else { 0 };
                let dy = if moons[i].y < moons[j].y { 1 } else if moons[i].y > moons[j].y { -1 } else { 0 };
                let dz = if moons[i].z < moons[j].z { 1 } else if moons[i].z > moons[j].z { -1 } else { 0 };

                moons[i].vx += dx;
                moons[j].vx -= dx;
                moons[i].vy += dy;
                moons[j].vy -= dy;
                moons[i].vz += dz;
                moons[j].vz -= dz;
            }
        }

        // update position for all moons
        for m in &mut moons {
            m.x += m.vx;
            m.y += m.vy;
            m.z += m.vz;
        }

        if mx == 0 {
           let mut h : u64 = 0;
            for m in &moons {
                h <<= 16;
                h += (m.x as u64) << 8;
                h += m.vx as u64;
            }
            if seenx.contains(&h) {
                mx = _s;
            } else {
                seenx.insert(h);
            }
        }

        if my == 0 {
            let mut h : u64 = 0;
            for m in &moons {
                h <<= 16;
                h += (m.y as u64) << 8;
                h += m.vy as u64;
            }
            if seeny.contains(&h) {
                my = _s;
            } else {
                seeny.insert(h);
            }
        }

        if mz == 0 {
            let mut h : u64 = 0;
            for m in &moons {
                h <<= 16;
                h += (m.z as u64) << 8;
                h += m.vz as u64;
            }
            if seenz.contains(&h) {
                mz = _s;
            } else {
                seenz.insert(h);
            }
        }

        if mx > 0 && my > 0 && mz > 0 {
            break;
        }
    }

    let a = lcm(mx, my);
    return lcm(a, mz);
}

fn lcm(a: u64, b: u64) -> u64 {
    return a * b / gcd(a, b);
}

fn gcd(mut a: u64, mut b: u64) -> u64 {
    while a != b {
        if a > b {
            a -= b;
        } else {
            b -= a;
        }
    }

    return a;
}

fn main() {
    let t = Instant::now();
    let moons: Vec<Moon> = io::stdin()
        .lines()
        .map(|l| {
            let values: Vec<i32> = l
                .unwrap()
                .strip_suffix(">")
                .unwrap()
                .split(',')
                .map(|p| p[3..].parse().unwrap())
                .collect();
            Moon {
                x: values[0],
                y: values[1],
                z: values[2],
                vx: 0,
                vy: 0,
                vz: 0,
            }
        })
        .collect();

    let pt1 = solve_pt1(moons.clone());
    let pt2 = solve_pt2(moons);
    println!("Part 1: {}", pt1);
    println!("Part 2: {}", pt2);
    println!("Time: {:.3?} ms", t.elapsed().as_millis());
}
