<?php

$input = trim(file_get_contents('03.txt'));

$matches = [];
preg_match_all('/mul\((\d{1,3}),(\d{1,3})\)/', $input, $matches);

$pt1 = 0;
for ($i = 0; $i < count($matches[0]); $i++) {
	$pt1 += $matches[1][$i] * $matches[2][$i];
}

echo "Part 1: " , $pt1, PHP_EOL;
