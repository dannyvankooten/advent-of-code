input = open('input.txt').read().split('\n')

max_width = 1500
max_height = 1000
width = 0
height = 0

paper = [' ' for n in range(0, max_width * max_height)]

def print_paper():

    dot_count = 0
    print("")
    for y in range(0, height+1):
        for x in range(0, width+1):
            print(paper[y*max_width + x], end='')

            if paper[y*max_width + x] == '#':
                dot_count += 1
        print("")

    print("Size: {}x{}".format(width, height))
    print("Dot count: {:d}".format(dot_count))

for l in input:
    if l.find(',') > -1:
        (x, y) = [int (v) for v in l.split(',')]
        paper[y*max_width+ x]='#'
        width = max(width, x)
        height = max(height, y)
    elif l == "":
        print_paper()
    elif l.startswith("fold along "):

        # determine axes to fold over
        # for x axes, every '#' larger than this x should move to new position
        # for y axes, every '#' larger than this y should move to new position
        # new position is = width-x or height - y
        (ax, value) = l[len("fold along "):].split("=")
        value = int(value)
        match ax:
            case 'y':
                for y in range(value, height+1):
                    for x in range(0, width+1):
                        if paper[y * max_width + x] == '#':
                            paper[(y - (y - value ) * 2) * max_width + x] = '#'
                
                height = value - 1
            case 'x':
                for y in range(0, height+1):
                    for x in range(value, width+1):
                        if paper[y * max_width + x] == '#':
                            paper[y* max_width + (x - (x - value ) * 2)] = '#'

                width = value - 1

    

print_paper()


