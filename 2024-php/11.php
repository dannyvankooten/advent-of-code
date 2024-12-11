<?php

$time_start = microtime(true);
$input = trim(file_get_contents('11.txt'));
// $input = "125 17";

$stones = explode(" ", $input);

function blink(array $stones) : array
{
	$new = [];
	for ($i = 0; $i < count($stones); $i++) {
		if ($stones[$i] === '0') {
			$new[] = '1';
			continue;
		}

		if (strlen($stones[$i]) % 2 === 0) {
			$left = substr($stones[$i], 0, strlen($stones[$i]) / 2);
			$right = (string) ((int) substr($stones[$i], strlen($stones[$i]) / 2));
			$new[] = $left;
			$new[] = $right;
			continue;
		}

		$new[] = (string) ((int) $stones[$i] * 2024);

	}
	return $new;
}

for ($i = 0; $i < 25; $i++) {
	$stones = blink($stones);
}

$pt1 = count($stones);
$pt2 = 0;

echo "--- Day 11 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

