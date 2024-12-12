<?php

$time_start = microtime(true);
$input = trim(file_get_contents('12.txt'));

function walk(array $map, int $r, int $c, array &$seen) {
	// mark location as seen
	$seen["$r-$c"] = true;

	$directions = [
		[1, 0],
		[-1, 0],
		[0, 1],
		[0, -1],
	];

	$perimeter = 0;
	$area = 1;
	foreach ($directions as $d) {
		$r2 = $r + $d[0];
		$c2 = $c + $d[1];

		if ($r2 < 0 || $c2 < 0 || $r2 >= count($map) || $c2 >= strlen($map[$r2])) {
			$perimeter += 1;
		} else if ($map[$r2][$c2] === $map[$r][$c]) {
			if (isset($seen["$r2-$c2"])) continue;

			// descend into new plant spot
			[$a, $b] = walk($map, $r2, $c2, $seen);
			$area += ($a );
			$perimeter += $b;
		} else {
			$perimeter += 1;
		}
	}

	return [$area, $perimeter];
}

$map = explode("\n", $input);
$seen = [];
$pt1 = 0;
for ($r = 0; $r < count($map); $r++) {
	for ($c = 0; $c < strlen($map[$r]); $c++) {
		if (isset($seen["$r-$c"])) continue;
		[$area, $perimeter] = walk($map, $r, $c, $seen);
		$pt1 += ($area * $perimeter);
	}
}

$pt2 = 0;

echo "--- Day 12 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

