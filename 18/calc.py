import re

with open("input.txt") as f:
    formulae = list(map(str.rstrip, f.readlines()))

def resultsimple(formula):
    if formula.find("(") > -1:
        [before, middle, after] = re.match("^(.*?)\(([^()]+)\)(.*)$", formula).groups()
        if re.match("^\s*$", before): before = ""
        if re.match("^\s*$", after):  after = ""
        if (before==None or before=="") and (after== None or after==""):
            return resultsimple(middle)
        else:
            return resultsimple(before + str(resultsimple(middle)) + after) 
    elif re.match("^([0-9]+)$", formula):
        return int(formula)
    else:
        [num1, op, num2, rest] = re.match("^([0-9-]+) ([+*]) ([0-9-]+)([^0-9].*)?$", formula).groups()
        res = int(num1) + int(num2) if op=="+" else int(num1) * int(num2)
        if rest==None or re.match("^\s*$", rest):
            return res
        else:
            return resultsimple(str(res) + rest)

def resultadv(formula):
    if re.match("^\(([^()]*)\)$", formula):
        return resultadv(re.match("^\((.*)\)$", formula).group()[0])
    elif str.isnumeric(formula):
        return int(formula)
    elif re.match("^(-?[0-9]+) \+ (-?[0-9]+)$", formula):
        [num1, num2] = re.match("^(-?[0-9]+) \+ (-?[0-9]+)$", formula).groups()
        return int(num1) + int(num2)
    elif re.match("^(-?[0-9]+) \* (-?[0-9]+)$", formula):
        [num1, num2] = re.match("^(-?[0-9]+) \* (-?[0-9]+)$", formula).groups()
        return int(num1) * int(num2)
    elif re.match("^(.*)\(([^)]*)\)(.*)$", formula):
        [before, paren, after] = re.match("^(.*)\(([^)]*)\)(.*)$", formula).groups()
        return resultadv(before + str(resultadv(paren)) + after)
    elif re.match("^(.+?) \* (.+)$", formula):
        [part1, part2] = re.match("^(.*) \* (.*)$", formula).groups()
        return resultadv(part1) * resultadv(part2)
    elif re.match("^(.*?) \+ (.*)$", formula):
        [part1, part2] = re.match("^(.*) \+ (.*)$", formula).groups()
        return resultadv(part1) + resultadv(part2)
    else:
        print("unknown syntax")
        return ()

# Part 1
sum = 0
for f in formulae:
    res = resultsimple(f)
    #print(f, " = ", res)
    sum += res
print(sum)

# Part 2
sum = 0
for f in formulae:
    res = resultadv(f)
    print(f, " = ", res)
    sum += res
print(sum)
