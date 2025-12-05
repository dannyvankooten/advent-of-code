local time_start = os.clock()
local pt1 = 0
local pt2 = 0

local pat = "(%d+)-(%d+),?"
for s, e in string.gmatch(io.read("*all"), pat) do
	e = math.tointeger(e)
	v = math.tointeger(s)
	while v <= e do
		full_id = tostring(v)

		for repeats = 2,string.len(full_id) do
			if (string.len(full_id) % repeats) == 0 then
				half_id = string.sub(full_id, 0, string.len(full_id) // repeats)
				if string.rep(half_id, repeats) == full_id then
					if repeats == 2 then
						pt1 = pt1 + v
					end

					pt2 = pt2 + v
					break
				end
			end
		end
		v = v + 1
	end

end

print("--- Day 2 ---")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took ", (os.clock() - time_start) * 1000, "ms")

assert(pt1 == 24747430309)
assert(pt2 == 30962646823)
