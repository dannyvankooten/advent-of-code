<?php

$time_start = microtime(true);
$input = trim(file_get_contents('24.txt'));

$sections = explode("\n\n", $input);

$wires = [];
foreach (explode("\n", $sections[0]) as $line) {
	[$wire, $value] = explode(": ", $line);
	$wires[$wire] = (int) $value;
}


$gates = [];

foreach (explode("\n", $sections[1]) as $line) {
	[$expr, $out] = explode(" -> ", $line);
	[$a, $op, $b] = explode(" ", $expr);
	$gates[] = [$a, $op, $b, $out];
}

while (!empty($gates)) {
	foreach ($gates as $i => [$a, $op, $b, $out]) {
		if (!isset($wires[$a], $wires[$b])) continue;

		switch ($op) {
			case "AND":
			$wires[$out] = (int) ($wires[$a] && $wires[$b]);
			break;

			case "OR":
			$wires[$out] = (int) ($wires[$a] || $wires[$b]);
			break;

			case "XOR":
			$wires[$out] = (int) ($wires[$a] != $wires[$b]);
			break;
		}

		unset($gates[$i]);
	}
}

ksort($wires);

$pt1 = 0;
foreach (array_reverse($wires) as $name => $value) {
	if ($name[0] !== 'z') continue;

	$pt1 = ($pt1 << 1) | $value;
}

$pt2 = 0;

echo "--- Day 24 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

assert($pt1 === 58367545758258);
