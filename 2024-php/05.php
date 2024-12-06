<?php

$time_start = microtime(true);
$input = trim(file_get_contents('05.txt'));
$input = explode("\n\n", $input);
$ordering_rules = array_map(function($line) {
	return array_map('intval', explode("|", trim($line)));
}, explode("\n", trim($input[0])));
$updates = array_map(function($line) {
	return array_map('intval', explode(",", trim($line)));
}, explode("\n", trim($input[1])));

// create a hashmap to store our ordering rules so we can check order rules in constant time
$ordering_rules_map = [];
foreach ($ordering_rules as $rule) {
	$ordering_rules_map[($rule[0] << 8) + $rule[1]] = true;
}

function is_sorted_correctly(array $update): bool {
	global $ordering_rules_map;

	for ($i = 1; $i < count($update); $i++) {
		$a = $update[$i-1];
		$b = $update[$i];
		if (isset($ordering_rules_map[($b << 8) + $a])) return false;
	}

	return true;
}

// for part 1, add up all middle numbers for correct updates
$pt1 = 0;
$pt2 = 0;
foreach ($updates as $update) {
	if (is_sorted_correctly($update)) {
		$mid = $update[count($update) / 2];
		$pt1 += $mid;
	} else {
		// for part 2, sort each incorrect update according to ordering rules
		usort($update, function($a, $b) use ($ordering_rules_map) {
			if (isset($ordering_rules_map[($a << 8) + $b])) return -1;
			if (isset($ordering_rules_map[($b << 8) + $a])) return 1;
			return 0;
		});

		$mid = $update[count($update) / 2];
		$pt2 += $mid;
	}
}

echo "--- Day 5: Print Queue ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

assert($pt1 === 5064);
assert($pt2 === 5152);
