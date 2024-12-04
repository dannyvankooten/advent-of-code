<?php
$time_start = microtime(true);
$input = trim(file_get_contents('04.txt'));
$grid = explode("\n", $input);
$grid = array_map('str_split', $grid);
$dirs = [
	[0, 1], 	// down
	[0, -1], 	// up
	[1, 0], 	// right
	[-1, 0], 	// left
	[1, 1], 	// south-east
	[-1, -1], 	// north-west
	[1, -1], 	// north-east
	[-1, 1] 	// south-west
];

function walk(array $grid, int $row, int $col, array $d, int $char_index): bool {
	static $word = "XMAS";
	$row += $d[1];
	$col += $d[0];

	if ($char_index >= strlen($word)) {
		return true;
	}

	if (
		$row < 0
		|| $col < 0
		|| $row >= count($grid)
		|| $col >= count($grid[$row])
		|| $grid[$row][$col] !== $word[$char_index]
	) {
		return false;
	}

	return walk($grid, $row, $col, $d, $char_index+1);
}

$pt1 = 0;
for ($row = 0; $row < count($grid); $row++) {
	for ($col = 0; $col < count($grid[$row]); $col++) {
		if ($grid[$row][$col] !== 'X') continue;

		// current grid pos == 'X', search all directions for XMAS
		foreach ($dirs as $d) {
			if (walk($grid, $row, $col, $d, 1)) {
				$pt1++;
			};
		}
	}
}

$pt2 = 0;
for ($row = 1; $row < count($grid) - 1; $row++) {
	for ($col = 1; $col < count($grid[$row]) -1; $col++) {
		if ($grid[$row][$col] !== 'A') continue;

		$a = $grid[$row-1][$col-1] . $grid[$row][$col] . $grid[$row+1][$col+1];
		$b = $grid[$row-1][$col+1] . $grid[$row][$col] . $grid[$row+1][$col-1];

		if (
			($a === "MAS" && $b === "MAS")
			|| ($a === "SAM" && $b === "SAM")
			|| ($a === "MAS" && $b === "SAM")
			|| ($a === "SAM" && $b === "MAS")
		) {
			$pt2 += 1;
		}
	}
}

echo "--- Day 4: Ceres Search ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;
