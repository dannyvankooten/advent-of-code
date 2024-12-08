<?php

$time_start = microtime(true);
$input = trim(file_get_contents('08.txt'));
$grid = explode("\n", $input);

function count_unique_antinodes(array $grid, bool $pt2 = false)
{
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


					$ra = $r2;
					$ca = $c2;
					if ($pt2) {
						$antinodes["$ra-$ca"] = true;
					}

					while (true) {
						// place antinode
						$ra += ($r2 - $r);
						$ca += ($c2 - $c);
						if ($ra < 0 || $ca < 0 || $ra >= count($grid) || $ca >= strlen($grid[$ra])) {
							break;
						}
						$antinodes["$ra-$ca"] = true;

						if (!$pt2) {
							break;
						}
					}
				}
			}
		}
	}

	return count($antinodes);
}


$pt1 = count_unique_antinodes($grid);
$pt2 = count_unique_antinodes($grid, true);

echo "--- Day 8: Resonant Collinearity ---", PHP_EOL;
echo "Part 1: ", $pt1, PHP_EOL;
echo "Part 2: ", $pt2, PHP_EOL;
echo "Took ", (microtime(true) - $time_start) * 1000, " ms", PHP_EOL;
echo PHP_EOL;

assert($pt1 === 252);
assert($pt2 === 839);
