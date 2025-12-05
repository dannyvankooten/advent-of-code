
import fs from 'fs';
import { strict as assert } from 'node:assert';

let time_start = performance.now()
let input =  String(fs.readFileSync("02.txt")).trim().split(',').map(v => v.split('-').map(v => parseInt(v)));
let pt1 = 0;
let pt2 = 0;

for (let i = 0; i < input.length; i++) {
	let [start, end] = input[i];
	for (;start <= end; start++) {
		let v = String(start);
		let expect = v.substring(0, v.length/2).repeat(2);
		if (v === expect) {
			pt1 += start;
			pt2 += start;
			continue;
		}

		for (let repeats = 3; repeats <= v.length; repeats++) {
			if (v.length % repeats !== 0) continue;
			let expect = v.substring(0, v.length/repeats).repeat(repeats);
			if (v === expect) {
				pt2 += start;
				break;
			}
		}
	}
}

console.log("--- Day 2: Gift Shop ---")
console.log("Part 1", pt1);
console.log("Part 2", pt2);
console.log("Took", performance.now() - time_start, "ms")

assert.equal(pt1, 24747430309)
assert.equal(pt2, 30962646823)
