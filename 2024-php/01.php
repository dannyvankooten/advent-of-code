<?php

$time_start = microtime(true);

// parse input into two sorted arrays
$input = trim(file_get_contents('01.txt'));
$a = [];
$b = [];

foreach (explode("\n", $input) as $line) {
	$parts = explode('   ', $line);
	$a[] = (int) $parts[0];
	$b[] = (int) $parts[1];
}

sort($a);
sort($b);

// add up distances between two corresponding locations
$pt1 = 0;
for ($i = 0; $i < count($a); $i++) {
	$pt1 += abs($b[$i] - $a[$i]);
}

// find number of occurences that item in $a occurs in $b
$pt2 = 0;
for ($i = 0, $j = 0; $i < count($a); $i++) {
	$occurs = 0;
	for (; $j < count($b) && $b[$j] <= $a[$i]; $j++) {
		if ($a[$i] == $b[$j]) $occurs++;
	}

	$pt2 += $a[$i] * $occurs;
}

echo "--- Day 1: Historian Hysteria ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;
