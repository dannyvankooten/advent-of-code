<?php

$time_start = microtime(true);
$input = trim(file_get_contents('19.txt'));

[$available, $designs] = explode("\n\n", $input);
$available = explode(", ", $available);
$designs = explode("\n", $designs);

function eat(string $design, array $available): int
{
	static $cache = [];
	if ($design === "") return 1;
	if (isset($cache[$design])) return $cache[$design];

	$p = 0;
	foreach ($available as $a) {
		if (str_starts_with($design, $a)) $p += eat(substr($design, strlen($a)), $available);
	}

	$cache[$design] = $p;
	return $p;
}

$pt1 = 0;
$pt2 = 0;
foreach ($designs as $d) {
	$p = eat($d, $available);
 	if ($p) $pt1++;
 	$pt2 += $p;
}

echo "--- Day 19 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;
