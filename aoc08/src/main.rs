use std::fs;



// The image you received is 25 pixels wide and 6 pixels tall
fn main() {
    let input = fs::read_to_string("input.txt").expect("Error reading input file");
    let image = generate_image(input);

    // Part 1: find the layer that contains the fewest 0 digits. On that layer, what is the number of 1 digits multiplied by the number of 2 digits?
    let mut best_count = [-1, -1, -1];
    for layer in &image {
        let mut count = [0, 0, 0];
        for row in layer.iter() {
            for col in row.iter() {
                count[*col] = count[*col] + 1;
            }
        }

        if best_count[0] == -1 || count[0] < best_count[0] {
            best_count = count;
        }
    }
    println!("Answer: {}", best_count[1] * best_count[2]);


    // Part 2: determine colors
    const COLOR_BLACK : usize = 0;
    const COLOR_WHITE : usize = 1;
    const COLOR_TRANSPARENT : usize = 2;
    let mut image_colors = [[COLOR_TRANSPARENT; 25]; 6];
    for (layer_idx, layer) in image.iter().enumerate() {
        for (row_idx, row) in layer.iter().enumerate() {
            for (col_idx, _) in row.iter().enumerate() {
               if image_colors[row_idx][col_idx] == COLOR_TRANSPARENT {
                   image_colors[row_idx][col_idx] = image[layer_idx][row_idx][col_idx];
               }                           
            }
        }
    }

    // output image
    for row in image_colors.iter() {
        for color in row.iter() {
            match *color {
                COLOR_BLACK => print!("■"),
                COLOR_WHITE => print!("□"),
                _ => (),
            }
        }
        print!("\n");
    }

}

fn generate_image(data : String) -> Vec<[[usize; 25]; 6]> {
    const IMAGE_WIDTH : usize = 25 - 1;
    const IMAGE_HEIGHT : usize = 6 - 1;

    let mut layers : Vec<[[usize; 25]; 6]> = Vec::new();
    let mut row = 0;
    let mut col = 0;
    let mut layer = 0;
    layers.push([[0; 25]; 6]);

    for d in data.chars() {
        layers[layer][row][col] = d.to_digit(10).unwrap() as usize;
        col = col + 1;

        if col > IMAGE_WIDTH {
            row = row + 1;
            col = 0;
        }

        if row > IMAGE_HEIGHT {
            layer = layer + 1;
            row = 0;

            // add new layer to image
            layers.push([[0; 25]; 6])
        }
    }

    layers
}

#[cfg(test)]
mod test {
    use super::generate_image;

    #[test]
    fn test_generate_image() {
        let image = generate_image("123456789012".to_string());

        println!("{:?}", image);
       // [[[1, 2, 3], [4, 5, 6]], [[7, 8, 9], [0, 1, 2]]]
        assert_eq!(image[0][0][0], 1);
    }
}