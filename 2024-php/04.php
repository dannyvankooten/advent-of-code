<?php
$time_start = microtime(true);
$input = trim(file_get_contents('04.txt'));
$grid = array_map(function($row) {
	return '.' . $row . '.';
}, explode("\n", $input));
$placeholder = str_repeat('.', strlen($grid[0]));
$grid = array_merge([$placeholder], $grid, [$placeholder]);

$dirs = [
	[0, 1], 	// down
	[0, -1], 	// up
	[1, 0], 	// right
	[-1, 0], 	// left
	[1, 1], 	// south-east
	[-1, -1], 	// north-west
	[1, -1], 	// north-east
	[-1, 1], 	// south-west
];

function walk(array $grid, int $row, int $col, array $d, int $char_index): int
{
	static $word = "XMAS";
	$row += $d[1];
	$col += $d[0];

	if ($char_index >= strlen($word)) {
		return 1;
	}

	if (
		$grid[$row][$col] !== $word[$char_index]
	) {
		return 0;
	}

	return walk($grid, $row, $col, $d, $char_index + 1);
}

$pt1 = 0;
for ($row = 0; $row < count($grid); $row++) {
	for ($col = 0; $col < strlen($grid[$row]); $col++) {
		if ($grid[$row][$col] !== 'X') continue;

		// current grid pos == 'X', search all directions for XMAS
		foreach ($dirs as $d) {
			$pt1 += walk($grid, $row, $col, $d, 1);
		}
	}
}

$pt2 = 0;
for ($row = 1; $row < count($grid) - 1; $row++) {
	for ($col = 1; $col < strlen($grid[$row]) - 1; $col++) {
		if ($grid[$row][$col] !== 'A') continue;

		// current grid position === 'A', search diagonals for M S combo
		$nw = $grid[$row - 1][$col - 1];
		$ne = $grid[$row - 1][$col + 1];
		$se = $grid[$row + 1][$col + 1];
		$sw = $grid[$row + 1][$col - 1];
		if (
			(($nw === 'M' && $se === 'S') || ($nw === 'S' && $se === 'M'))
			&& (($ne === 'M' && $sw === 'S') || ($ne === 'S' && $sw === 'M'))
		) {
			$pt2++;
		}
	}
}

echo "--- Day 4: Ceres Search ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;
