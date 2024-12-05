<?php

$time_start = microtime(true);
$input = trim(file_get_contents('05.txt'));
$input = explode("\n\n", $input);
$ordering_rules = array_map(function($line) {
	return explode("|", trim($line));
}, explode("\n", trim($input[0])));
$updates = array_map(function($line) {
	return explode(",", trim($line));
}, explode("\n", trim($input[1])));

// for part 1, add up all middle numbers for correct updates
$pt1 = 0;
foreach ($updates as $update) {
	$correct = true;
	$seen = [];
	foreach ($update as $pos => $page_no) {
		$seen[$page_no] = $pos;
	}

	foreach ($ordering_rules as $rule) {
		if (isset($seen[$rule[0]]) && isset($seen[$rule[1]])) {
			if ($seen[$rule[0]] >= $seen[$rule[1]]) {
				$correct = false;
				break;
			}
		}
	}

	if ($correct) {
		$mid = $update[count($update) / 2];
		$pt1 += $mid;
	}
}




$pt2 = 0;
echo "--- Day 5: Print Queue ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;
