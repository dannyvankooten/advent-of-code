<?php

$time_start = microtime(true);
$input = trim(file_get_contents('15.txt'));
[$grid, $moves] = explode("\n\n", $input);
$grid = explode("\n", $grid);
$moves = str_split($moves);

// find robot position in grid
foreach ($grid as $y => $line) {
	$x = strpos($line, '@');
	if ($x !== false) {
		$ry = $y;
		$rx = $x;
		break;
	}
}

// attempt to perform all the moves
$directions = [
	'^' => [-1, 0],
	'>' => [0, 1],
	'v' => [1, 0],
	'<' => [0, -1],
];

foreach ($moves as $i => $m) {
	if ($m === "\n") continue;
	// echo "Move $m\n";

	// step to first free spot in direction
	$y2 = $ry;
	$x2 = $rx;
	do {
		$y2 += $directions[$m][0];
		$x2 += $directions[$m][1];
	} while ($y2 >= 0 && $x2 >= 0 && $y2 < count($grid) && $x2 < strlen($grid[$y2]) && $grid[$y2][$x2] === 'O');

	// if we're still inside grid and at a free spot, we can perform the move
	if ($y2 >= 0 && $x2 >= 0 && $y2 < count($grid) && $x2 < strlen($grid[$y2]) && $grid[$y2][$x2] === '.') {
		// shift everything up to robot in direction

		do {
			$y3 = $y2 + $directions[$m][0] * -1;
			$x3 = $x2 + $directions[$m][1] * -1;
			$grid[$y2][$x2] = $grid[$y3][$x3];
			$y2 = $y3;
			$x2 = $x3;
		} while ($grid[$y2][$x2] !== '@');


		// robot leaves behind a free spot
		$grid[$ry][$rx] = '.';
		$ry += $directions[$m][0];
		$rx += $directions[$m][1];

		assert($grid[$ry][$rx] === '@');
	}

}


$pt1 = 0;
foreach ($grid as $r => $row) {
	for ($c = 0; $c < strlen($row); $c++) {
		if ($row[$c] === 'O') $pt1 += ($r * 100) + $c;
	}
}

$pt2 = 0;

echo "--- Day 15 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

