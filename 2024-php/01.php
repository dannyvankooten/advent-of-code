<?php

$time_start = microtime(true);
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
$diff_sum = 0;
for ($i = 0; $i < count($a); $i++) {
	$diff_sum += abs($b[$i] - $a[$i]);
}

// find number of occurences that item in $a occurs in $b
$pt2 = 0;
foreach ($a as $item) {
	$occurs = 0;
	foreach ($b as $other) {
		if ($item == $other) {
			$occurs++;
		}

		// array is sorted, so we can stop comparing once $b is larger
		if ($other > $item) {
			break;
		}
	}

	$pt2  += $item * $occurs;
}



echo "Day 1: Historian Hysteria", PHP_EOL;
echo "Part 1: ", $diff_sum, PHP_EOL;
echo "Part 1: ", $pt2, PHP_EOL;
echo "Took ", microtime(true) - $time_start, " seconds", PHP_EOL;
echo PHP_EOL;
