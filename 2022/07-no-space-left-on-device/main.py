from pathlib import Path
from dataclasses import dataclass 

@dataclass 
class Dir:
    name: str 
    parent: ...
    children: list
    size: int

def parse_into_tree(input) -> Dir:
    cur = root = Dir('/', None, [], 0)

    for commant in filter(None, input.split('$ ')[2:]):
        lines = commant.strip().split('\n')
        args = lines[0].split(' ')
        match args[0]:
            case 'cd':
                if args[1] == '..':
                    cur = cur.parent 
                else:
                    child = Dir(args[1], cur, [], 0)
                    cur.children.append(child)
                    cur = child
            case 'ls':
                for line in lines[1:]:
                    size, _ = line.split()
                    if size != 'dir':
                        cur.size += int(size)

                        parent = cur.parent
                        while parent != None:
                            parent.size += int(size)
                            parent = parent.parent

    return root

def flatten(root) -> list[Dir]:
    all = [root.size]
    for c in root.children:
        all += flatten(c)
    return all 

def pt1(dirs) -> int:
    return sum(filter(lambda s: s < 100000, dirs))

def pt2(dirs) -> int:
    needed = 30000000 - (70000000 - max(dirs))
    return min(filter(lambda s: s > needed, dirs))


if __name__ == '__main__':
    input = Path("input.txt").read_text()
    tree = parse_into_tree(input)
    dirs = flatten(tree)
    print("pt1: ", pt1(dirs))
    print("pt2: ", pt2(dirs))

