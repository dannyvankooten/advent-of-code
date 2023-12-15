
def parse():
    with open(0) as f:
        [replacements, medicine] = f.read().strip().split("\n\n")
        replacements = replacements.splitlines()
        replacements = [r.split(" => ") for r in replacements]
    return replacements, medicine

start = "e"
replacements, medicine = parse()
print(medicine, replacements)

steps = 0
while medicine != "e":
    for r in replacements:
        if r[1] in medicine:
            medicine = medicine.replace(r[1], r[0], 1)
            steps += 1

print(steps)