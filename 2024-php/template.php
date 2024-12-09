<?php

$time_start = microtime(true);
$input = trim(file_get_contents('DAY_HERE.txt'));

$pt1 = 0;
$pt2 = 0;

echo "--- DAY_NAME_HERE ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

