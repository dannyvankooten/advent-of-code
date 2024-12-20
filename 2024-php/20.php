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

function dijkstra (array $grid, int $sx, int $sy, int $ex, int $ey): int {
	global $w, $h;
	$directions = [[1, 0], [0, 1], [-1, 0], [0, -1]];
	$queue = new \Ds\Queue();
	$queue->push([$sx, $sy, 0]);
	$seen = [];
	foreach ($queue as [$x, $y, $dist]) {
		if ($x === $ex && $y === $ey) {
			return $dist - 2;
		}

		// take steps in all possible directions
		foreach ($directions as [$dx, $dy]) {
			$x2 = $x + $dx;
			$y2 = $y + $dy;

			if ($grid[$y2][$x2] !== '#' && !isset($seen["$x2-$y2"])) {
				$queue->push([$x2, $y2, $dist+1]);
				$seen["$x-$y"] = true;
			}
		}
	}

	// no path from left to right?
	return -1;
}

$cutoff = 100;
$pt1 = 0;
$pt2 = 0;
for ($y = 1; $y < $h-1; $y++) {
	for ($x = 1; $x < $w-1; $x++) {
	 	if ($grid[$y][$x] !== '#') continue;

	 	foreach ([
	 		[1, 0],
	 		[0, 1],
	 	] as [$dx, $dy]) {
	 		if ($grid[$y-$dy][$x-$dx] !== '#' && $grid[$y+$dy][$x+$dx] !== '#') {
	 			$pt1 += dijkstra($grid, $x - $dx, $y - $dy, $x + $dx, $y + $dy) >= $cutoff ? 1 : 0;
	 		}
	 	}
	}
}


echo "--- Day 20 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

assert($pt1 === 1293);
