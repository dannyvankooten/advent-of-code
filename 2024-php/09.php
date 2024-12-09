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

function checksum(array $disk): int {
	$checksum = 0;
	for ($i = 0; $i < count($disk); $i++) {
		if ($disk[$i] === -1) continue;
		$checksum += $i * $disk[$i];
	}
	return $checksum;
}

// part 1: move individual blocks
$disk_copy = $disk;
$pt1 = 0;
for ($i = count($disk)-1, $f = 0; $i >= 0; $i--) {
	while ($disk[$f] !== -1) $f++;
	while ($disk[$i] === -1) $i--;
	if ($f >= $i) break;

	$disk[$f] = $disk[$i];
	$disk[$i] = -1;
}
$pt1 = checksum($disk);

// part 2: move entire file blocks to leftmost free space region that fits
$disk = $disk_copy;
for ($f = count($disk) - 1; $f >= 0; $f--) {
	while ($disk[$f] === -1) $f--;
	$id = $disk[$f];
	$end = $f;
	while ($f > 0 && $disk[$f-1] === $id) $f--;
	if ($f === 0) break;
	$length = $end - $f +1;
	$seen[$id] = true;

	for ($i = 0; $i <= $f; $i++) {
		while ($disk[$i] !== -1) $i++;
		if ($i > $f) break;
		$start = $i;
		while ($disk[$i] === -1) $i++;
		$free_length = $i - $start;

		if ($free_length >= $length) {
			for ($j = $start; $j < $start+$length; $j++) {
				$disk[$j] = $id;
			}
			for($j = $f; $j <= $end; $j++) {
				$disk[$j] = -1;
			}
			break;
		}
	}
}

$pt2 = checksum($disk);


echo "--- Day 9: Disk Fragmenter ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

assert($pt1 === 6398608069280);
assert($pt2 === 6427437134372);
