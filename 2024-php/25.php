<?php

$time_start = microtime(true);
$input = trim(file_get_contents('25.txt'));
$locks = [];
$keys = [];
$space = 0;
foreach (explode("\n\n", $input) as $section) {
	$section = explode("\n", $section);
	$type = $section[0][0] === '#' ? 'lock' : 'key';
	$schema = [];

	for ($x = 0; $x < strlen($section[0]); $x++) {
		$height = -1;
		for ($y = 0; $y < count($section); $y++) {
			if ($section[$y][$x] === '#') $height++;
		}
		$schema[] = $height;
		$space = count($section) - 2;
	}

	if ($type === 'lock') {
		$locks[] = $schema;
	} else {
		$keys[] = $schema;
	}
}

$pt1 = 0;
foreach ($locks as $lock) {
	foreach ($keys as $key) {
		for ($x = 0; $x < count($lock); $x++) {
			if ($lock[$x] + $key[$x] > $space) {
				continue 2;
			}
		}

		// key fits in lock
		$pt1++;
	}
}

$pt2 = "🎄";

echo "--- Day 25 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

