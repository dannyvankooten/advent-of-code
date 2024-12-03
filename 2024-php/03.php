<?php
$time_start = microtime(true);
$input = trim(file_get_contents('03.txt'));

function op($in): int
{
	$matches = [];
	preg_match_all('/mul\((\d{1,3}),(\d{1,3})\)/', $in, $matches);
	$out = 0;
	for ($i = 0; $i < count($matches[0]); $i++) {
		$out += (int) $matches[1][$i] * (int) $matches[2][$i];
	}
	return $out;
}

$pt1 = op($input);

// for part 2, we use the same regex but first do some string manipulation
while (true) {
	$pos = strpos($input, "don't()");
	if ($pos === false) {
		break;
	}

	$new = substr($input, 0, $pos);

	$next = strpos($input, "do()", $pos+1);
	if ($next !== false) {
		$new .= substr($input, $next);
	}
	$input = $new;
}

$pt2 = op($input);

echo "--- Day 3: Mull It Over ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;
