<?php

$time_start = microtime(true);
$input = trim(file_get_contents('08.txt'));
$grid = explode("\n", $input);

class Antenna {
	public function __construct(
		public int $row,
		public int $col,
	) {}
}

function count_unique_antinodes(array $frequency_antennas, int $width, int $height, bool $pt2 = false)
{
	$antinodes = [];

	foreach ($frequency_antennas as $frequency => $antennas) {
		foreach ($antennas as $a) {
			foreach ($antennas as $b) {
				if ($a === $b) continue;
				$rd = ($b->row - $a->row);
				$cd = ($b->col - $a->col);
				$r = $pt2 ? $a->row : $b->row;
				$c = $pt2 ? $a->col : $b->col;

				do {
					$r += $rd;
					$c += $cd;
					if ($r < 0 || $c < 0 || $r >= $height || $c >= $width) {
						break;
					}
					$antinodes["$r-$c"] = true;
				} while ($pt2);
			}
		}
	}

	return count($antinodes);
}


$height = count($grid);
$width = strlen($grid[0]);
$antennas = [];
for ($r = 0; $r < $height; $r++) {
	for ($c = 0; $c < $width; $c++) {
		if ($grid[$r][$c] === '.') continue;
		if (!isset($antennas[$grid[$r][$c]])) {
			$antennas[$grid[$r][$c]] = [];
		}
		$antennas[$grid[$r][$c]][] = new Antenna($r, $c);
	}
}
$pt1 = count_unique_antinodes($antennas, $width, $height);
$pt2 = count_unique_antinodes($antennas, $width, $height, true);

echo "--- Day 8: Resonant Collinearity ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

assert($pt1 === 252);
assert($pt2 === 839);
