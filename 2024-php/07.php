<?php

$time_start = microtime(true);
$input = trim(file_get_contents('07.txt'));

function solve(int $expected_result, int $left, array $numbers, bool $concat = false) {
	if ($left > $expected_result) {
		return 0;
	}

	if (count($numbers) === 0) {
		return $left === $expected_result;
	}

	$right = array_pop($numbers);
	return solve($expected_result, $left * $right, $numbers, $concat) || solve($expected_result, $left + $right, $numbers, $concat) || ($concat && solve($expected_result, intval("$left$right"), $numbers, $concat));
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
	$pt1 += solve($expected, $left, $numbers, false) ? $expected : 0;
	$pt2 += solve($expected, $left, $numbers, true) ? $expected : 0;
}

echo "--- Day 7: Bridge Repair ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

assert($pt1 === 303766880536);
assert($pt2 === 337041851384440);
