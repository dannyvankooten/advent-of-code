<?php
$time_start = microtime(true);

function is_safe($numbers): bool
{
	$diffs = [];
	for ($i = 1; $i < count($numbers); $i++) {
		$diff = $numbers[$i] - $numbers[$i - 1];

		// diff must be between 1 and 3
		if (abs($diff) < 1 || abs($diff) > 3) {
			return false;
		}

		$diffs[] = $diff;
	}

	// sign of each diff must be the same as all other elements
	for ($i = 1; $i < count($diffs); $i++) {
		if ($diffs[$i] > 0 && $diffs[$i-1] < 0) {
			return false;
		}

		if ($diffs[$i] < 0 && $diffs[$i-1] > 0) {
			return false;
		}
	}

	return true;
}

$input = trim(file_get_contents('02.txt'));
$pt1 = 0;
$pt2 = 0;
foreach (explode("\n", $input) as $line) {
	$numbers = array_map(function($v) { return (int) $v; }, explode(' ', $line));

	if (is_safe($numbers)) {
		$pt1++;
		$pt2++;
	} else {
		for ($i = 0; $i < count($numbers); $i++) {
			$copy = $numbers;
			unset($copy[$i]);
			$copy = array_values($copy);
			if (is_safe($copy)) {
				$pt2++;
				break;
			}
		}
	}
}


echo "--- Day 2: Red-Nosed Reports ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", microtime(true) - $time_start, " seconds", PHP_EOL;
echo PHP_EOL;
