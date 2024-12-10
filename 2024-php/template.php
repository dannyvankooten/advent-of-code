<?php

$time_start = microtime(true);
$input = trim(file_get_contents('%%DAYNAME%%.txt'));

$pt1 = 0;
$pt2 = 0;

echo "--- Day %%DAYNAME%% ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

