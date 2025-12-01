local lines = {}
for line in io.lines() do
  table.insert(lines, line)
end

p = 50
pt1 = 0
pt2 = 0
for i, l in ipairs(lines) do
    dir = string.char(string.byte(l, 1))
    amount = tonumber(string.sub(l, 2))

    if dir  == "L" then
        modifier = -1
    end
    if dir == "R" then
        modifier = 1
    end

    p = p + (modifier * amount)
    while p < 0 do
        p = 100 + p
        pt2 = pt2 + 1
    end
    while p > 99 do
        p = 0 + (p - 100)
        pt2 = pt2 + 1
    end
    if p == 0 then
        pt1 = pt1 + 1
    end

end

print("Part 1", pt1)
print("Part 2", pt2)
