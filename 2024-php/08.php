<?php

$time_start = microtime(true);
$input = trim(file_get_contents('08.txt'));
$grid = explode("\n", $input);

$antinodes = [];

for ($r = 0; $r < count($grid); $r++) {
	for ($c = 0; $c < strlen($grid[$r]); $c++) {
		// if antenna
		if ($grid[$r][$c] === '.') continue;

		// find other antenna's of same frequency
		for ($r2 = 0; $r2 < count($grid); $r2++) {
			for ($c2 = 0; $c2 < strlen($grid[$r2]); $c2++) {
				if ($grid[$r2][$c2] !== $grid[$r][$c]) continue;
				if ($r2 === $r && $c2 === $c) continue;

				// place antinode
				$ra = $r2 + ($r2 - $r);
				$ca = $c2 + ($c2 - $c);
				if ($ra < 0 || $ca < 0 || $ra >= count($grid) || $ca >= strlen($grid[$ra])) continue;
				$antinodes["$ra-$ca"] = true;
			}
		}
	}
}

$pt1 = count($antinodes);


echo "--- Day 8: Resonant Collinearity ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

// 250 too low
