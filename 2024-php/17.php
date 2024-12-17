<?php

$time_start = microtime(true);
$input = trim(file_get_contents('17.txt'));

preg_match_all("/(\d+)/", $input, $matches);
$matches = array_map('intval', $matches[0]);
$registers = array_slice($matches, 0, 3);
$program = array_slice($matches, 3);

function opval(int $operand, array $registers): int {
	switch ($operand) {
		case 1:
		case 2:
		case 3:
		return $operand;
		break;

		case 4:
		case 5:
		case 6:
		return $registers[$operand-4];
		break;
	}

	throw new \Exception("invalid operand");
}

function run(array $program, array $registers): array {
	$out = [];

	for ($ip = 0; $ip >= 0 && $ip < count($program); ) {
		$opcode = $program[$ip];
		$operand = $program[$ip+1];

		switch ($opcode) {
			// adv
			case 0:
			$registers[0] = (int) ($registers[0] / pow(2, opval($operand, $registers)));
			break;

			// bxl
			case 1:
			$registers[1] ^= $operand;
			break;

			// bst
			case 2:
			$registers[1] = opval($operand, $registers) % 8;
			break;

			// jnz
			case 3:
			if ($registers[0] != 0) {
				$ip = $operand;
				continue 2;
			}
			break;

			// bxc
			case 4:
			$registers[1] ^= $registers[2];
			break;

			// out
			case 5:
			$out[] = opval($operand, $registers) % 8;
			break;

			// bdv
			case 6:
			$registers[1] = (int) ($registers[0] / pow(2, opval($operand, $registers)));
			break;

			// cdv
			case 7:
			$registers[2] = (int) ($registers[0] / pow(2, opval($operand, $registers)));
			break;
		}

		$ip += 2;
	}

	return $out;
}

$pt1 = join(',', run($program, $registers));

function solve(int $a, int $n, array $program): bool|int {
	if ($n > count($program)) {
		return $a;
	}

	for ($i = 0; $i < 8; $i++) {
		$_a = $a << 3 | $i;
		$out = run($program, [$_a, 0, 0]);
		if ($out === array_slice($program, -$n)) {
			$result = solve($_a, $n+1, $program);
			if ($result !== false) {
				return $result;
			}
		}
	}

	return false;
}
$pt2 = solve(0, 1, $program);

echo "--- Day 17 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

assert($pt1 === "1,0,2,0,5,7,2,1,3");
assert($pt2 === 265652340990875);
