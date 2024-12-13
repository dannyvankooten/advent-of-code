<?php

$time_start = microtime(true);
$input = trim(file_get_contents('13.txt'));

$pt1 = 0;
$pt2 = 0;

function gcd(int $a, int $b): int
{
    while ($b != 0) {
        $temp = $b;
        $b = $a % $b;
        $a = $temp;
    }
    return $a;
}

function lcm(int $a,int $b): int
{
    $gcd = gcd($a, $b);
    $lcm = ($a * $b) / $gcd;
    return $lcm;
}

function solve(int $ax, int $ay, int $bx, int $by, int $px, int $py): int
{
	$ma = lcm($ax, $ay);
	$mx = $ma / $ax;
	$my = $ma / $ay;

	$b = ($mx*$px - $my*$py) / ($mx*$bx - $my*$by);
	$a = ($px - $b * $bx) / $ax;

	if ($a !== (int) $a) {
		return 0;
	}

	return ($a * 3) + $b;
}

// px = a * ax + b * bx
// py = a * ay + b * by
$pt1 = 0;
$pt2 = 0;
foreach (explode("\n\n", $input) as $section) {

	$matches = [];
	preg_match('/Button A: X\+(\d+), Y\+(\d+)\nButton B: X\+(\d+), Y\+(\d+)\nPrize: X=(\d+), Y=(\d+)/', $section, $matches);

	array_shift($matches);
	[$ax, $ay, $bx, $by, $px, $py] = $matches;
	$pt1 += solve($ax, $ay, $bx, $by, $px, $py);
	$pt2 += solve($ax, $ay, $bx, $by, $px+10000000000000, $py+10000000000000);
}


echo "--- Day 13 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

