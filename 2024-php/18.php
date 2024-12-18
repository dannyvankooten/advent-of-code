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

dump($grid);

$start = [0, 0];
$end = [WIDTH - 1, HEIGHT - 1];
$queue = new SplPriorityQueue();
$queue->setExtractFlags(SplPriorityQueue::EXTR_BOTH);
$queue->insert($start, 0);

$directions = [
	[0, -1],
	[1, 0],
	[0, 1],
	[-1, 0],
];

$pt1 = PHP_INT_MAX;
$visited = [];
foreach ($queue as ['data' => $pos, 'priority' => $priority]) {
	if ($pos === $end) {
		$pt1 = -$priority;
		break;
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

		// echo "Stepping to {$p[0]}, {$p[1]}\n";
		$visited["{$x2}-{$y2}"] = true;
		$queue->insert([$x2, $y2], $priority-1);
	}
}

$pt2 = 0;

echo "--- Day 18 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

