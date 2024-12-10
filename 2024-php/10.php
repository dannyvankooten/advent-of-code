<?php

$time_start = microtime(true);
$input = trim(file_get_contents('10.txt'));
$map = array_map(function ($line) {
	return array_map('intval', str_split($line));
}, explode("\n", $input));


function hike(array $map, int $r, int $c) {
	if ($map[$r][$c] === 9) {
		return ["$r-$c"];
	}

	$directions = [
		[1, 0],
		[-1, 0],
		[0, 1],
		[0, -1],
	];
	$trailheads = [];
	foreach ($directions as $dir) {
		$r2 = $r + $dir[0];
		$c2 = $c + $dir[1];
		if ($r2 < 0 || $c2 < 0 || $r2 >= count($map) || $c2 >= count($map[$r2])) {
			continue;
		}

		if ($map[$r2][$c2] - $map[$r][$c] !== 1) {
			continue;
		}

		$trailheads = array_merge($trailheads, hike($map, $r + $dir[0], $c + $dir[1]));
	}

	return array_unique($trailheads);
}

$pt1 = 0;
for ($r = 0; $r < count($map); $r++) {
	for ($c = 0; $c < count($map[$r]); $c++) {
		if ($map[$r][$c] === 0) {
			$pt1 += count(hike($map, $r, $c));
		}
	}
}


$pt2 = 0;

echo "--- Day 10 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

