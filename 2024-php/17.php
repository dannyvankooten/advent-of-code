<?php

$time_start = microtime(true);
$input = trim(file_get_contents('17.txt'));

preg_match_all("/(\d+)/", $input, $matches);
$a = (int) array_shift($matches[0]);
$b = (int) array_shift($matches[0]);
$c = (int) array_shift($matches[0]);
$program = array_map('intval', $matches[0]);

function opval($operand): int {
	global $a, $b, $c;
	if ($operand <= 3) return $operand;
	switch ($operand) {
		case 1:
		case 2:
		case 3:
		return $operand;
		break;

		case 4: return $a; break;
		case 5: return $b; break;
		case 6: return $c; break;
	}

	throw new \Exception("invalid operand");

}
$out = [];

for ($ip = 0; $ip >= 0 && $ip < count($program); ) {
	$opcode = $program[$ip];
	$operand = $program[$ip+1];

	switch ($opcode) {
		// adv
		case 0:
		$a = (int) ($a / pow(2, opval($operand)));
		break;

		// bxl
		case 1:
		$b ^= $operand;
		break;

		// bst
		case 2:
		$b = opval($operand) % 8;
		break;

		// jnz
		case 3:
		if ($a != 0) {
			$ip = $operand;
			continue 2;
		}
		break;

		// bxc
		case 4:
		$b ^= $c;
		break;

		// out
		case 5:
		$out[] = opval($operand) % 8;
		break;

		// bdv
		case 6:
		$b = (int) ($a / pow(2, opval($operand)));
		break;

		// cdv
		case 7:
		$c = (int) ($a / pow(2, opval($operand)));
		break;
	}

	$ip += 2;
}


$pt1 = join(",", $out);
$pt2 = 0;

echo "--- Day 17 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

