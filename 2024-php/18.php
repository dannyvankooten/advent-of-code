<?php

$time_start = microtime(true);
$input = explode("\n", trim(file_get_contents('18.txt')));

define('WIDTH', 71);
define('HEIGHT', 71);

$grid = array_fill(0, HEIGHT, str_repeat('.', WIDTH));
for ($i = 0; $i < 1024; $i++) {
	[$x, $y] = array_map('intval', explode(',', $input[$i]));
	$grid[$y][$x] = '#';
}

function dijkstra(array $grid): int
{
	$start = [0, 0];
	$end = [WIDTH - 1, HEIGHT - 1];
	$queue = new Ds\Queue();
	$queue->push([$start, 0]);

	$directions = [
		[0, -1],
		[1, 0],
		[0, 1],
		[-1, 0],
	];
	$pt1 = PHP_INT_MAX;
	$visited = [];

	foreach ($queue as [$pos, $steps]) {
		if ($pos === $end) {
			return $steps;
		}

		foreach ($directions as $d) {
			$x2 = $pos[0] + $d[0];
			$y2 = $pos[1] + $d[1];

			// bounds check
			if ($x2 < 0 || $y2 < 0 || $x2 >= WIDTH || $y2 >= HEIGHT) continue;

			// obstacle check
			if ($grid[$y2][$x2] === '#') continue;

			// visited?
			if ($visited["$x2-$y2"] ?? false) continue;

			// mark location as visited and insert in queuephp q
			$visited["{$x2}-{$y2}"] = true;
			$queue->push([[$x2, $y2], $steps+1]);
		}
	}

	return PHP_INT_MAX;
}

$pt1 = dijkstra($grid);

for ($i = 1024; $i < count($input); $i++) {
	[$x, $y] = array_map('intval', explode(',', $input[$i]));
	$grid[$y][$x] = '#';
	$pt2 = $input[$i];

	if (dijkstra($grid) === PHP_INT_MAX) {
		break;
	}
}


echo "--- Day 18 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

