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

function walk(array $grid, array $pos): int {
	$dir = NORTH; // guard starts facing up
	$dirs = [
		NORTH => [-1, 0],
		EAST => [0, 1],
		SOUTH => [1, 0],
		WEST => [0, -1],
	];
	$seen = [];
	$seen_exact = [];
	while (true) {
		$r2 = $pos[0] + $dirs[$dir][0];
		$c2 = $pos[1] + $dirs[$dir][1];
		if ($r2 < 0 || $c2 < 0 || $r2 >= count($grid) || $c2 >= strlen($grid[$r2])) {
			break;
		}

		// check for obstacle
		if ($grid[$r2][$c2] === '#') {
			$dir = ($dir + 1) % 4;
			continue;
		}



		// take a step
		$pos[0] = $r2;
		$pos[1] = $c2;

		// check if we've been there in exact same orientation
		if (isset($seen_exact[($pos[0] << 24) + ($pos[1] << 8) + $dir])) {
			return 0;
		}

		// mark as seen
		$seen[($pos[0] << 16) + $pos[1]] = true;
		$seen_exact[($pos[0] << 24) + ($pos[1] << 8) + $dir] = true;
	}


	return count($seen);
}

$pos = find_pos($grid);
$grid[$pos[0]][$pos[1]] = '.';
$pt1 = walk($grid, $pos);
$pt2 = 0;
for ($r = 0; $r < count($grid); $r++) {
	for ($c = 0; $c < strlen($grid[$r]); $c++) {
		if ($grid[$r][$c] === '#') continue;
		if ($r === $pos[0] && $c === $pos[1]) continue;

		// try placing an obstacle and then walking grid
		$grid_copy = $grid;
		$grid_copy[$r][$c] = '#';
		if (walk($grid_copy, $pos) === 0) {
			$pt2++;
		}
	}
}


echo "--- Day 6: Guard Gallivant ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;


assert($pt1 === 5409);
