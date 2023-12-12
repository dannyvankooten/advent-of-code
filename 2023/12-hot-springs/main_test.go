package main

import "testing"

func TestIsValid(t *testing.T) {
	tests := []struct {
		in        string
		blueprint []int
		expected  bool
	}{
		{"#.#.###", []int{1, 1, 3}, true},
		{"#.#.###", []int{1, 1, 2}, false},
		{"#.#.###", []int{2, 1, 2}, false},
		{"####.#...#...", []int{4, 1, 1}, true},
		{"#....######..#####.", []int{1, 6, 5}, true},
		{"#....#####..#####.", []int{1, 6, 5}, false},
		{".###.##....#", []int{1, 6, 5}, false},
		{".###.##....#", []int{3, 2, 1}, true},
		{".#.###.#.######", []int{3, 2, 1}, false},
		{".#.###.#.######", []int{1, 3, 1, 6}, true},
	}

	for _, tc := range tests {
		got := isValid2([]rune(tc.in), len(tc.in), tc.blueprint)
		if got != tc.expected {
			t.Errorf("%#v = %#v failed. expected %#v; got %#v", tc.in, tc.blueprint, tc.expected, got)
		}
	}
}

func TestIsValidPartial(t *testing.T) {
	tests := []struct {
		in        string
		length    int
		blueprint []int
		expected  bool
	}{
		{"#.#.###", 3, []int{1, 1, 3}, true},
		{"#.#.###", 4, []int{1, 1, 3}, true},
		{"#.#.###", 5, []int{1, 1, 3}, true},
		{"###.###", 1, []int{1, 1, 3}, true},
		{".#...#....###.", 4, []int{1, 1, 1}, true},
		{"##.#.", 2, []int{1, 1, 3}, false},
	}

	for _, tc := range tests {
		got := isValid2([]rune(tc.in), tc.length, tc.blueprint)
		if got != tc.expected {
			t.Errorf("%#v = %#v failed. expected %#v; got %#v", tc.in, tc.blueprint, tc.expected, got)
		}
	}
}
