<?php

$time_start = microtime(true);
$input = trim(file_get_contents('23.txt'));

$pt1 = 0;
$pt2 = 0;

$connections = [];
$computers = [];
foreach (explode("\n", $input) as $line) {
	[$a, $b] = explode("-", $line);
	if (!isset($connections[$a])) {
		$connections[$a] = [];
	}
	$connections[$a][$b] = true;

	if (!isset($connections[$b])) {
		$connections[$b] = [];
	}
	$connections[$b][$a] = true;
}

$computers = array_keys($connections);

for ($i  = 0; $i < count($computers) - 2; $i++) {
	$a = $computers[$i];

	for ($j = $i+1; $j < count($computers) - 1; $j++) {
		$b = $computers[$j];

		// a-b
		if (!isset($connections[$a][$b])) continue;

		for ($k = $j+1; $k < count($computers); $k++) {
			$c = $computers[$k];

			// a-c & b-c
			$interconnected = isset($connections[$a][$c]) && isset($connections[$b][$c]);
			$name_matches = $a[0] === 't' || $b[0] === 't' || $c[0] === 't';
			if ($interconnected && $name_matches) {
				$pt1++;
			}
		}
	}
}


function f(array $connections, array $set): array {
	foreach ($set as $b => $_) {
		foreach ($set as $c => $_) {
			if ($b === $c) continue;

			if (!isset($connections[$b][$c])) {
				$set_a = $set;
				unset($set_a[$b]);
				$left = f($connections, $set_a);

				$set_b = $set;
				unset($set_b[$c]);
				$right = f($connections, $set_b);

				return count($left) > count($right) ? $left : $right;
			}
		}
	}

	return $set;

}

$largest_graph = [];
foreach ($computers as $a) {
	$set = f($connections, $connections[$a]);
	$graph = array_keys($set);
	$graph[] = $a;

	if (count($graph) > count($largest_graph)) {
		$largest_graph = $graph;
	}
}

sort($largest_graph);
$pt2 = join(',', $largest_graph);

echo "--- Day 23 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;
