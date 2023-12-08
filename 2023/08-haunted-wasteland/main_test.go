package main

import "testing"

func Test_primeFactors(t *testing.T) {
	tests := []struct {
		input   int
		expects []int
	}{
		{2, []int{2}},
		{5, []int{5}},
		{5 * 5 * 3, []int{3, 5, 5}},
		{2 * 4 * 7, []int{2, 2, 2, 7}},
	}

	for _, tc := range tests {
		got := primeFactors(tc.input)

		if len(got) != len(tc.expects) {
			t.Errorf("expected %#v, got %#v\n", tc.expects, got)
		}
		for i := 0; i < len(tc.expects); i++ {
			if got[i] != tc.expects[i] {
				t.Errorf("expected %#v, got %#v\n", tc.expects, got)
			}
		}

	}
}
