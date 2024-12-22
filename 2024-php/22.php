<?php

$time_start = microtime(true);
$input = explode("\n", trim(file_get_contents('22.txt')));

function f(int $secret): int
{
	$secret ^= ($secret * 64);
	$secret %= 16777216;
	$secret ^= (int) ($secret / 32);
	$secret %= 16777216;
	$secret ^= ($secret * 2048);
	$secret %= 16777216;
	return $secret;
}

$pt1 = 0;
$pt2 = 0;



foreach ($input as $line) {
	$secret = (int) $line;
	for ($i = 0; $i < 2000; $i++) {
		$secret = f($secret);
	}

	$pt1 += $secret;
}

echo "--- Day 22 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

