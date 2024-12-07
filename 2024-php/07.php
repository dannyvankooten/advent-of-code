<?php

$time_start = microtime(true);
$input = trim(file_get_contents('07.txt'));

function solve(int $expected_result, int $left, array $numbers, bool $concat = false) {
	if (count($numbers) === 0) {
		return $left === $expected_result ? $expected_result : 0;
	}

	$right = array_pop($numbers);
	$result = solve($expected_result, $left * $right, $numbers, $concat);
	if ($result) return $result;

	$result = solve($expected_result, $left + $right, $numbers, $concat);
	if ($result) return $result;

	if ($concat) {
		$result = solve($expected_result, intval("$left$right"), $numbers, $concat);
		if ($result) return $result;
	}

	return 0;
}

$input = array_map(function($line) {
	[$expected, $numbers] = explode(": ", $line);
	$numbers = array_map(function($n) {
		return intval($n);
	}, explode(" ", $numbers));

	return [intval($expected), array_reverse($numbers)];
}, explode("\n", $input));


$pt1 = 0;
$pt2 = 0;
foreach ($input as [$expected, $numbers]) {
	$left = array_pop($numbers);
	$pt1 += solve($expected, $left, $numbers, false);
	$pt2 += solve($expected, $left, $numbers, true);
}

echo "--- Day 7: Bridge Repair ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

assert($pt1 === 303766880536);
assert($pt2 === 337041851384440);
// 333903416016 too low
// 337041851395637 too high
// 337041851395637 too high
