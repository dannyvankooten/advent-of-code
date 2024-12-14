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

	public function quadrant(): int
	{
		$y_mid = (int) (HEIGHT / 2);
		$x_mid = (int) (WIDTH / 2);
		if ($this->x === $x_mid || $this->y === $y_mid) return -1;

		if ($this->x < $x_mid) {
			return $this->y < $y_mid ? 0 : 2;
		}

		return $this->y < $y_mid ? 1 : 3;
	}

	public function move(int $seconds): void {
		$this->x = ($this->x + $seconds * $this->vx) % WIDTH;
		$this->y = ($this->y + $seconds * $this->vy) % HEIGHT;
		if ($this->y < 0) $this->y += HEIGHT;
		if ($this->x < 0) $this->x += WIDTH;
	}
}

$pt1 = 0;
$pt2 = 0;

// count robots in each quadrant
$quadrants = [0, 0, 0, 0];
foreach ($robots as $r) {
	$r->move(100);
	$q = $r->quadrant();
	if ($q > -1) {
		$quadrants[$q]++;
	}
}

$pt1 = $quadrants[0] * $quadrants[1] * $quadrants[2] * $quadrants[3];

echo "--- Day 14 ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

