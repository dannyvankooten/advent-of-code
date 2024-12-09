<?php

$time_start = microtime(true);
$input = trim(file_get_contents('09.txt'));

$disk = [];
for ($i = 0, $f = 0; $i < strlen($input); ) {
	for ($j = 0; $j < $input[$i]; $j++) {
		$disk[] = $f;
	}
	$i++;
 	$f++;

	if ($i < strlen($input)) {
		for ($j = 0; $j < $input[$i]; $j++) {
			$disk[] = -1;
		}
		$i++;
	}
}



$pt1 = 0;
for ($i = count($disk)-1, $f = 0; $i >= 0; $i--) {
	while ($disk[$f] !== -1) $f++;
	while ($disk[$i] === -1) $i--;
	if ($f >= $i) break;

	$disk[$f] = $disk[$i];
	$disk[$i] = -1;
}

for ($i = 0; $i < count($disk) && $disk[$i] !== -1; $i++) {
	$pt1 += $i * $disk[$i];
}


$pt2 = 0;

echo "--- Day 9: Disk Fragmenter ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

assert($pt1 === 6398608069280);
