local time_start = os.clock()

for program in io.popen("ls -pa . | grep '[0-9].lua'"):lines() do
	input = string.sub(program, 0, 2) .. ".txt"
	out = io.popen("lua " .. program .. " < " .. input):read("*all")
	print(out)
end

print("Total time: " .. (os.clock() - time_start) .. "ms")
