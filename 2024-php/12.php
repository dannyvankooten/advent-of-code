<?php

$time_start = microtime(true);
$input = trim(file_get_contents('12.txt'));



function walk(array $map, int $r, int $c, array &$seen, array &$fences): array
{
	// mark location as seen
	$seen["$r-$c"] = true;

	static $directions = [
		[-1, 0],		// NORTH
		[0, 1],			// EAST
		[1, 0],			// SOUTH
		[0, -1],		// WEST
	];

	$perimeter = 0;
	$area = 1;
	if(!isset($fences["$r-$c"])) {
		$fences["$r-$c"] = [false, false, false, false];
	}

	foreach ($directions as $o => $d) {
		$r2 = $r + $d[0];
		$c2 = $c + $d[1];

		if ($r2 < 0 || $c2 < 0 || $r2 >= count($map) || $c2 >= strlen($map[$r2])) {
			$perimeter += 1;
			$fences["$r-$c"][$o] = true;
		} else if ($map[$r2][$c2] === $map[$r][$c]) {
			if (isset($seen["$r2-$c2"])) continue;

			// descend into new plant spot
			[$a, $b] = walk($map, $r2, $c2, $seen, $fences);
			$area += ($a );
			$perimeter += $b;
		} else {
			$fences["$r-$c"][$o] = true;
			$perimeter += 1;
		}
	}

	return [$area, $perimeter];
}

$map = explode("\n", $input);
$seen = [];
$pt1 = 0;
$pt2 = 0;

for ($r = 0; $r < count($map); $r++) {
	for ($c = 0; $c < strlen($map[$r]); $c++) {
		if (isset($seen["$r-$c"])) continue;
		$fences = [];
		[$area, $perimeter] = walk($map, $r, $c, $seen, $fences);
		$pt1 += ($area * $perimeter);


		// count horizontal fence sections
		$sides = 0;
		for ($i = 0; $i < count($map); $i++) {
			// count NORTH side
			for ($j = 0; $j < strlen($map[0]); $j++) {
				if (isset($fences["$i-$j"]) && $fences["$i-$j"][0]) {
					$sides += 1;
					while (isset($fences["$i-" . ($j+1)]) && $fences["$i-" . ($j+1)][0]) $j++;
				}
			}

			for ($j = 0; $j < strlen($map[0]); $j++) {
				if (isset($fences["$i-$j"]) && $fences["$i-$j"][2]) {
					$sides += 1;
					while (isset($fences["$i-" . ($j+1)]) && $fences["$i-" . ($j+1)][2]) $j++;
				}
			}
		}

		for ($i = 0; $i < strlen($map[0]); $i++) {
			// count EAST side
			for ($j = 0; $j < count($map); $j++) {
				if (isset($fences["$j-$i"]) && $fences["$j-$i"][1]) {
					$sides += 1;
					while (isset($fences["$j-$i"]) && $fences["$j-$i"][1]) $j++;
				}
			}

			// count WEST side
			for ($j = 0; $j < count($map); $j++) {
				if (isset($fences["$j-$i"]) && $fences["$j-$i"][3]) {
					$sides += 1;
					while (isset($fences["$j-$i"]) && $fences["$j-$i"][3]) $j++;
				}
			}
		}

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
