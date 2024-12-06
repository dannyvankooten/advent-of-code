<?php

$time_start = microtime(true);
$input = trim(file_get_contents('06.txt'));
$grid = explode("\n", $input);

const NORTH = 0;
const EAST = 1;
const SOUTH = 2;
const WEST = 3;

function find_pos(array $grid): array {
	for ($row = 0; $row < count($grid); $row++) {
		for ($col = 0; $col < strlen($grid[$row]); $col++) {
			if ($grid[$row][$col] === '^')  {
				return [$row, $col];
			}
		}
	}

	throw new Exception('No guard in grid?!');
}

function walk(array $grid, int $r, int $c): array {
	static $dirs = [
		NORTH => [-1, 0],
		EAST => [0, 1],
		SOUTH => [1, 0],
		WEST => [0, -1],
	];
	$dir = NORTH; // guard starts facing up
	$seen = [];
	while (true) {
		$r2 = $r + $dirs[$dir][0];
		$c2 = $c + $dirs[$dir][1];

		// check for grid bounds
		if ($r2 < 0 || $c2 < 0 || $r2 >= count($grid) || $c2 >= strlen($grid[$r2])) {
			break;
		}

		// check for obstacle
		if ($grid[$r2][$c2] === '#') {
			$dir = ($dir + 1) % 4;
			continue;
		}

		// take a step
		$r = $r2;
		$c = $c2;

		// mark (row, col) location as seen
		$h = ($r << 8) + $c;

		if (!isset($seen[$h])) $seen[$h] = 0;
		if ($seen[$h] & (1 << $dir)) {
			return [];
		}
		$seen[$h] |= (1 << $dir);
	}

	return $seen;
}

[$rs, $cs] = find_pos($grid);
$grid[$rs][$cs] = '.';

// for part 1, simply walk grid and count seen positions
$positions = walk($grid, $rs, $cs);
$pt1 = count($positions);

// for part 2, brute-force place an obstacle at each visited location from part 1
$pt2 = 0;
foreach ($positions as $p => $_) {
	$r = $p >> 8; 	// last 8-16 bytes hold the row
	$c = $p & 0xFF;	// last 8 bytes hold the column

	// skip if already obstacle or guard starting pos
	if ($grid[$r][$c] === '#' || ($r === $rs && $c === $cs)) continue;

	$grid[$r][$c] = '#';
	if (!walk($grid, $rs, $cs)) {
		$pt2++;
	}
	$grid[$r][$c] = '.';
}

echo "--- Day 6: Guard Gallivant ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

assert($pt1 === 5409);
assert($pt2 === 2022);
