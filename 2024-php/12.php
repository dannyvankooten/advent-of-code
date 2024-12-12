<?php

$time_start = microtime(true);
$input = trim(file_get_contents('12.txt'));

$input = "AAAA
BBCD
BBCC
EEEC";

function walk(array $map, int $r, int $c, array &$seen, array &$region): int
{
	// mark location as seen
	$seen["$r-$c"] = true;
	$region[] = [$r, $c];

	static $directions = [
		[-1, 0],		// NORTH
		[0, 1],			// EAST
		[1, 0],			// SOUTH
		[0, -1],		// WEST
	];

	$perimeter = 0;
	foreach ($directions as $o => $d) {
		$r2 = $r + $d[0];
		$c2 = $c + $d[1];

		if ($r2 < 0 || $c2 < 0 || $r2 >= count($map) || $c2 >= strlen($map[$r2])) {
			$perimeter++;
			continue;
		} else if ($map[$r2][$c2] === $map[$r][$c]) {
			if (isset($seen["$r2-$c2"])) continue;

			// descend into new plant spot
			$perimeter += walk($map, $r2, $c2, $seen, $region);
		} else {
			$perimeter++;
		}
	}

	return $perimeter;
}

function corners(array $region) {

	$corners = 0;
	foreach ($region as [$r, $c]) {
		$left = [$r, $c-1];
		$right = [$r, $c+1];
		$up = [$r-1, $c];
		$down = [$r+1, $c];

		foreach ([
			[-1, -1],
			[-1, 1],
			[1, 1],
			[1, -1],
		] as $d) {

		}

		if (!array_search($left, $region) && !array_search($up, $region)) {
			$corners++;
		}

		if (!array_search($left, $region) && !array_search($down, $region)) {
			$corners++;
		}

		if (!array_search($right, $region) && !array_search($up, $region)) {
			$corners++;
		}

		if (!array_search($right, $region) && !array_search($down, $region)) {
			$corners++;
		}
	}

	return $corners;
}

$map = explode("\n", $input);
$seen = [];
$pt1 = 0;
$pt2 = 0;

for ($r = 0; $r < count($map); $r++) {
	for ($c = 0; $c < strlen($map[$r]); $c++) {
		if (isset($seen["$r-$c"])) continue;
		$region = [];
		$perimeter = walk($map, $r, $c, $seen, $region);

		$area = count($region);
		$pt1 += ($area * $perimeter);

		$sides = corners($region);

		echo "{$map[$r][$c]}: $sides\n";
		$pt2 += $sides * $area;
	}
}


echo "--- Day 12 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

assert($pt1 === 1433460);
assert($pt2 === 855082);
