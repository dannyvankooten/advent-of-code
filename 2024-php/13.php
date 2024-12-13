<?php

$time_start = microtime(true);
$input = trim(file_get_contents('13.txt'));

$pt1 = 0;
$pt2 = 0;

function solve(int $ax, int $ay, int $bx, int $by, int $px, int $py): int
{
	$a = ($px * $by - $py * $bx) / ($ax * $by - $ay * $bx);
    $b = ($ax * $py - $ay * $px) / ($ax * $by - $ay * $bx);
    return is_int($a) && is_int($b) ? $a * 3 + $b : 0;
}

// px = a * ax + b * bx
// py = a * ay + b * by

$pt1 = 0;
$pt2 = 0;
foreach (explode("\n\n", $input) as $section) {
	preg_match_all('/(\d+)/', $section, $matches);
	[$ax, $ay, $bx, $by, $px, $py] = array_map('intval', $matches[0]);
	$pt1 += solve($ax, $ay, $bx, $by, $px, $py);
	$pt2 += solve($ax, $ay, $bx, $by, $px+10000000000000, $py+10000000000000);
}

echo "--- Day 13 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

assert($pt1 === 34787);
assert($pt2 === 85644161121698);
