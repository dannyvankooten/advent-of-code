from collections import defaultdict

connect = defaultdict(list)
for line in open("input.txt").readlines():
    pair = line.strip().split('-')
    for p1, p2 in zip(pair, reversed(pair)):
        if p2 != 'start':
            connect[p1].append(p2)
    
del connect['end']

def pt1(data, path):
    final = 0
    for point in data[path[-1]]:
        if point.isupper() or not point in path:
            final += 1 if point == 'end' else pt1(data, path + [point])
    return final


count = pt1(connect, ['start'])
print(count)