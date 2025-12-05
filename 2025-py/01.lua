local time_start = os.clock()
local pt1 = 0
local pt2 = 0
local p = 50

for line in io.lines() do
    local dir = string.sub(line, 0, 1)
    local amount = string.sub(line, 2)

    if dir == "L" then
        amount = -amount
    end

    p = p + amount
    pt2 = pt2 + math.abs(p // 100)
    p = p % 100

    if p == 0 then
        pt1 = pt1 + 1
    end
end

print("--- Day 1: Secret Entrance ---")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took ", os.clock() - time_start, "ms")

assert(pt1 == 999, "pt1 answer is incorrect")
assert(pt2 == 6099, "pt2 answer is incorrect")
