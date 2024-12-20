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

	assert($x === $ex && $y === $ey);

	return $path;
}

function get_neighbors (array $grid, array $start, int $n): array {
	global $w, $h;
	$directions = [[1, 0], [0, 1], [-1, 0], [0, -1]];
	$neighbors = [];
	$seen = [];

	$queue = new \Ds\Queue();
	$queue->push([$start[0], $start[1], 0]);
	foreach ($queue as [$x, $y, $dist]) {
		if ($grid[$y][$x] !== '#' && $dist > 0) {
			$neighbors[] = [$x, $y, $dist];
		}

		if ($dist < $n) {
			// take steps in all possible directions
			foreach ($directions as [$dx, $dy]) {
				$x2 = $x + $dx;
				$y2 = $y + $dy;

				// bounds check
				if ($x2 < 0 || $y2 < 0 || $x2 >= $w || $y2 >= $h) continue;

				if (!isset($seen["$x2-$y2"])) {
					$seen["$x2-$y2"] = true;
					$queue->push([$x2, $y2, $dist + 1]);
				}
			}
		}
	}

	return $neighbors;
}

$pt1 = 0;
$pt2 = 0;

// first, create path from start to finish
$path = get_path($grid, $start[0], $start[1], $end[0], $end[1]);
$dist = count($path);
$distance_to_end = [];
foreach ($path as $dist => [$x, $y]) {
	$distance_to_end["$x-$y"] = count($path) - $dist;
}
$distance_to_end["{$end[0]}-{$end[1]}"] = 0;

$savings = [];
foreach ($path as $start_dist => $v) {
	foreach (get_neighbors($grid, $v, 2) as [$xn, $yn, $cheat_dist]) {
		$to_end = $cheat_dist + $distance_to_end["$xn-$yn"];
		$diff = $dist - $start_dist - $to_end + 1;
		if ($diff >= 100) $pt1++;
	}

	foreach (get_neighbors($grid, $v, 20) as [$xn, $yn, $cheat_dist]) {
		$to_end = $cheat_dist + $distance_to_end["$xn-$yn"];
		$diff = $dist - $start_dist - $to_end + 1;
		if ($diff >= 100) $pt2++;
	}

}

echo "--- Day 20 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

assert($pt1 === 1293);
