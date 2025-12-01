local time_start = os.clock()
local p = 50
local pt1 = 0
local pt2 = 0

for line in io.lines() do
    local dir = string.sub(line, 0, 1)
    local amount = string.sub(line, 2)

    if dir  == "L" then
        p = p - amount

        while p < 0 do
            p = 100 + p
            pt2 = pt2 + 1
        end
    else
        p = p + amount
        while p > 99 do
            p = 0 + (p - 100)
            pt2 = pt2 + 1
        end
    end

    if p == 0 then
        pt1 = pt1 + 1
    end

end

print("--- Day 1: Secret Entrance ---")
print("Part 1", pt1)
print("Part 2", pt2)
print("Took ", os.clock() - time_start, "ms")
