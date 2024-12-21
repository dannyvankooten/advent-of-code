<?php

$time_start = microtime(true);
$input = trim(file_get_contents('20.txt'));
$grid = explode("\n", $input);
$h = count($grid);
$w = strlen($grid[0]);

$start = [false, false];
$end = [false, false];
foreach ($grid as $r => $row) {
	$start = $start[0] === false ? [strpos($row, 'S'), $r] : $start;
	$end = $end[0] === false ? [strpos($row, 'E'), $r] : $end;
	if ($start[0] !== false && $end[0] !== false) break;
}

function get_path(array $grid, int $x, int $y, int $ex, int $ey): array {
	global $w, $h;
	$directions = [[1, 0], [0, 1], [-1, 0], [0, -1]];
	$seen = [];
	$path = [];

	while ($x !== $ex || $y !== $ey) {
		$seen["$x-$y"] = true;
		$path[] = [$x, $y];

		// take steps in all possible directions
		foreach ($directions as [$dx, $dy]) {
			$x2 = $x + $dx;
			$y2 = $y + $dy;

			if (isset($seen["$x2-$y2"]) || $grid[$y2][$x2] === '#') continue;

			$x = $x2;
			$y = $y2;
			break;
		}
	}

	$path[] = [$ex, $ey];
	return $path;
}

function get_neighbors (array $grid, array $start, int $n): array {
	global $w, $h;
	[$sx, $sy] = $start;
	$neighbors = [];

	for ($y = max(0, $sy - $n); $y <= min($h-1, $sy + $n); $y++) {
		for ($x = max(0, $sx - $n); $x <= min($w-1, $sx + $n); $x++) {
			if ($grid[$y][$x] === '#' || ($dist = abs($sy - $y) + abs($sx - $x)) > $n) continue;
			$neighbors["$x-$y"] = [$x, $y, $dist];
		}
	}

	return $neighbors;
}


// first, create path from start to finish
$path = get_path($grid, $start[0], $start[1], $end[0], $end[1]);
$dist = count($path);
$distance_to_end = [];
foreach ($path as $d => [$x, $y]) {
	$distance_to_end["$x-$y"] = $dist - $d;
}

$cheat_scores = [
	2 => 0,
	20 => 0,
];

foreach ($path as $start_dist => $v) {

	foreach (array_keys($cheat_scores) as $allowed_cheats) {
		foreach (get_neighbors($grid, $v, $allowed_cheats) as [$xn, $yn, $cheat_dist]) {
			$to_end = $cheat_dist + $distance_to_end["$xn-$yn"];
			$diff = $dist - $start_dist - $to_end;
			if ($diff >= 100) {
				$cheat_scores[$allowed_cheats]++;
			}
		}
	}
}

$pt1 = $cheat_scores[2];
$pt2 = $cheat_scores[20];
echo "--- Day 20 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

assert($pt1 === 1293);
assert($pt2 === 977747);
