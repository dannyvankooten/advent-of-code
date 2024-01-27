
import z3

with open(0) as f:
    input = f.read().strip().replace(' @' , ', ').splitlines()
    data = [line.split(',') for line in input]
    data = [[int(n.strip()) for n in line] for line in data]

px, py, pz, vx, vy, vz = z3.Ints("px py pz vx vy vz")
times = [z3.Int("t" + str(i)) for i in range(len(data))]

s = z3.Solver()
for i, (px2, py2, pz2, vx2, vy2, vz2) in enumerate(data):
    s.add(px + vx * times[i] == px2 + vx2 * times[i])
    s.add(py + vy * times[i] == py2 + vy2 * times[i])
    s.add(pz + vz * times[i] == pz2 + vz2 * times[i])
s.check()
ans = s.model().evaluate(px + py + pz)

print(ans.as_long())
