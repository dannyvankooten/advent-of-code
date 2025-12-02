import fs from 'fs';

let time_start = performance.now()
let input =  String(fs.readFileSync("02.txt")).trim().split(',');

let pt1 = input.reduce((accumulator, range) => {
	let [start, end] = range.split('-').map(v => parseInt(v));
	let invalid_count = 0;
	for (;start <= end; start++) {
		let v = String(start);
		let expect = v.substring(0, v.length/2).repeat(2);
		if (v === expect) {
			invalid_count += start;
		}
	}
	return accumulator + invalid_count;
}, 0);


let pt2 = input.reduce((accumulator, range) => {
	let [start, end] = range.split('-').map(v => parseInt(v));
	let invalid_count = 0;
	for (;start <= end; start++) {
		let v = String(start);

		for (let repeats = 2; repeats <= v.length; repeats++) {
			let expect = v.substring(0, v.length/repeats).repeat(repeats);
			if (v === expect) {
				invalid_count += start;
				break;
			}
		}
	}
	return accumulator + invalid_count;
}, 0);


console.log("--- Day 2: Gift Shop ---")
console.log("Part 1", pt1);
console.log("Part 2", pt2);
console.log("Took", performance.now() - time_start, "ms")

