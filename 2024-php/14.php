<?php

$time_start = microtime(true);
$input = trim(file_get_contents('14.txt'));

$robots = array_map(function ($line) {
	preg_match_all('/(-?\d+)/', $line, $matches);
	return new Robot((int) $matches[1][0], (int) $matches[1][1], (int) $matches[1][2], (int) $matches[1][3]);
}, explode("\n", $input));


define('WIDTH', 101);
define('HEIGHT', 103);

class Robot {
	public function __construct(
		public int $x,
		public int $y,
		public int $vx,
		public int $vy,
	) {}

	public function quadrant_after_seconds(int $seconds): int
	{
		$x = ($this->x + $seconds * $this->vx) % WIDTH;
		if ($x < 0) $x += WIDTH;

		$y = ($this->y + $seconds * $this->vy) % HEIGHT;
		if ($y < 0) $y += HEIGHT;

		$y_mid = (int) (HEIGHT / 2);
		$x_mid = (int) (WIDTH / 2);

		// ignore robots in exact middle
		if ($x === $x_mid || $y === $y_mid) return 0;
		if ($x < $x_mid) {
			return $y < $y_mid ? 1 : 3;
		}

		return $y < $y_mid ? 2 : 4;
	}

	public function move(int $seconds): void {
		$this->x = ($this->x + $seconds * $this->vx) % WIDTH;
		if ($this->x < 0) $this->x += WIDTH;

		$this->y = ($this->y + $seconds * $this->vy) % HEIGHT;
		if ($this->y < 0) $this->y += HEIGHT;
	}
}


// count robots in each quadrant after 100 seconds
$quadrants = [0, 0, 0, 0, 0];
foreach ($robots as $r) {
	$q = $r->quadrant_after_seconds(100);
	$quadrants[$q]++;
}
$pt1 = $quadrants[1] * $quadrants[2] * $quadrants[3] * $quadrants[4];


$empty_grid = [];
for ($y = 0; $y < HEIGHT; $y++) {
	$empty_grid[] = str_repeat('.', WIDTH);
}
$needle = str_repeat('O', 16);

$pt2 = 1;
for (; true; $pt2++) {
	$quadrants = [0, 0, 0, 0, 0];
	$grid = $empty_grid;
	foreach ($robots as $r) {
		$r->move(1);
		$grid[$r->y][$r->x] = 'O';
	}

	for ($y = 0; $y < HEIGHT; $y++) {
		if (strpos($grid[$y], $needle) !== false) {
			break 2;
		}
	}
}

echo "--- Day 14 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

assert($pt1 === 225648864);
