use std::io::prelude::*;
use std::fs::File;

fn main() {
    let mut f = File::open("../input.txt").unwrap();
    let mut input = String::new();
    f.read_to_string(&mut input).unwrap();
    println!("input: {}", input);

    let input : Vec<i64> = input.split_terminator(",").map(|v| v.parse().unwrap()).collect();
    println!("input: {:?}", input);

    let out = intcode(input);
    println!("output: {:?}", out);
}


fn intcode(input : Vec<i64>) -> Vec<i64> {
    let out = input.clone();
    let mut iter = input.into_iter();

    
    while let value = iter.next() {
        println!("{}", value.unwrap());
    }

    out
}