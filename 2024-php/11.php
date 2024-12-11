<?php

$time_start = microtime(true);
$input = trim(file_get_contents('11.txt'));
$stones = explode(" ", $input);

function blink(string $stone, int $times) : int
{
	if ($times === 0) {
		return 1;
	}

	static $cache = [];
	if (isset($cache["$stone-$times"])) {
		return $cache["$stone-$times"];
	}

	if ($stone === '0') {
		$result = blink('1', $times - 1);
    } else if (strlen($stone) % 2 === 0) {
       	$left = substr($stone, 0, strlen($stone) / 2);
       	$right = (string) ((int) substr($stone, strlen($stone) / 2));
       	$result = blink($left, $times - 1) + blink($right, $times - 1);
    } else {
    	$result = blink($stone * 2024, $times - 1);
    }

	$cache["$stone-$times"] = $result;

    return $result;
}


$pt1 = 0;
$pt2 = 0;
foreach ($stones as $stone) {
	$pt1 += blink($stone, 25);
	$pt2 += blink($stone, 75);
}

echo "--- Day 11 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

assert($pt1 === 209412);
assert($pt2 === 248967696501656);
