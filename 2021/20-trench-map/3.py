GRID = [(x - 1, y - 1) for y in range(3) for x in range(3)]
TRANSLATION = str.maketrans('.#','01')

with open("input.txt") as f:
    data = f.read()
filter, _, *image = data.splitlines()
width, height = len(image), len(image[0])
filter = filter.translate(TRANSLATION)
do_toggle_exterior_on_when_off = (filter[0] == '1')
do_toggle_exterior_off_when_on = (filter[-1] == '0')
lit_pixels = {(x, y) for y, line in enumerate(image) for x, c in enumerate(line) if c == '#'}

def get_pixel(x, y, lit_pixels, step, is_exterior_lit):
    if ((-step < x < width + step - 1) and (-step < y < height + step - 1)):
        return (x, y) in lit_pixels
    else:
        return is_exterior_lit

def enhance(lit_pixels, step, max_step, is_exterior_lit):
    new_lit_pixels = set()
    x_axis, y_axis = range(-step, width + step), range(-step, height + step)
    for x, y in [(x, y) for x in x_axis for y in y_axis]:
        pixels = [str(int(get_pixel(x + i, y + j, lit_pixels, step, is_exterior_lit))) for i, j in GRID]
        if filter[int("".join(pixels), 2)] == '1':
            new_lit_pixels.add((x, y))
    is_exterior_lit =(True if is_exterior_lit == False and do_toggle_exterior_on_when_off else
              (False if is_exterior_lit == True and do_toggle_exterior_off_when_on else is_exterior_lit))
    return new_lit_pixels if step == max_step else enhance(new_lit_pixels, step + 1, max_step, is_exterior_lit)

def count_lit_pixels_after_n_steps(lit_pixels, steps):
    return len(enhance(lit_pixels, 1, steps, False))

print('Part 1:', count_lit_pixels_after_n_steps(lit_pixels, 2))
print('Part 2:', count_lit_pixels_after_n_steps(lit_pixels, 50))