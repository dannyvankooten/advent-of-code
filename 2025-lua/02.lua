local time_start = os.clock()
local pt1 = 0
local pt2 = 0

local pat = "(%d+)-(%d+),?"
for s, e in string.gmatch(io.read("*all"), pat) do

end

print("--- Day %%DAYNAME%% ---")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took ", os.clock() - time_start, "ms")
