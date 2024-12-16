<?php

$time_start = microtime(true);
$input = trim(file_get_contents('16.txt'));
$grid = explode("\n", $input);

$end = [false, false];
$start = [false, false];
foreach ($grid as $r => $row) {
	$end = $end[1] ? $end : [$r, strpos($row, 'E')];
	$start = $start[1] ? $start : [$r, strpos($row, 'S')];
}

const EAST = 0;
const SOUTH = 1;
const WEST = 2;
const NORTH = 3;
$directions = [
	EAST => [0, 1],
	SOUTH => [1, 0],
	WEST => [0, -1],
	NORTH => [-1, 0],
];

$queue = new \SplPriorityQueue();
$queue->insert([$start, EAST, 0, []], 0);
$visited = [];
$pt1 = PHP_INT_MAX;
foreach ($queue as [$pos, $d, $cost, $path]) {
	if ($pos === $end) {
		if ($cost > $pt1) {
			break;
		}

		$pt1 = $cost;
		foreach ($path as $p) {
			$grid[$p[0]][$p[1]] = 'O';
		}
		$grid[$pos[0]][$pos[1]] = 'O';
		continue;
	}

	// add position to path
	$path[] = $pos;
	$visited["{$pos[0]}-{$pos[1]}-$d"] = $cost;

	// we can take a step
	$left = $d === 0 ? 3 : $d - 1;
	$right = $d === 3 ? 0 : $d + 1;
	foreach ([$left, $d, $right] as $dd) {
		$dest = [$pos[0] + $directions[$dd][0], $pos[1] + $directions[$dd][1]];
		if ($grid[$dest[0]][$dest[1]] !== '#') {
			$new_cost = $cost + 1;
			if ($dd != $d) {
				$new_cost += 1000;
			}

			$prev_cost = $visited["{$dest[0]}-{$dest[1]}-$dd"] ?? PHP_INT_MAX;
			if ($new_cost <= $prev_cost) {
				$queue->insert([$dest, $dd, $new_cost, $path], -$new_cost);
			}
		}
	}
}

$pt2 = count_chars(join("", $grid), 1)[ord('O')];

echo "--- Day 16 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

