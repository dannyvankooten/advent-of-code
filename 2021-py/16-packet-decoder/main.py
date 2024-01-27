from pathlib import Path
import sys 

def decode(input):
    bits = []
    for c in input:
        bit = bin(int(c, 16))[2:].zfill(4)
        bits.append(bit)
    return "".join(bits)
    
assert(decode("D2FE28") == "110100101111111000101000")

def parse_literal_value(bits):
    i = 0
    literal_bits = []
    while True:
        literal_bits.append(bits[i+1:i+5])

        if bits[i] == '0':
            break
        else:
            i += 5


    size = len(literal_bits) * 5
    bits = "".join(literal_bits)
    
    return (int(bits, 2), size)

assert(parse_literal_value(decode("D2FE28")[6:]) == (2021, 15))
    
class Transmission:
    def __init__(self, hexa):
        self.hexa = hexa
        self.bits = decode(hexa)
    
    def take(self, n):
        value = int(self.bits[:n], 2)
        self.bits = self.bits[n:]
        return value 

def handle_operator_packet(tx):
    length_type_id = tx.take(1)
    values = []

    if length_type_id == 0:
        subpacket_length = tx.take(15)
        end_at_length = len(tx.bits) - subpacket_length
        while len(tx.bits) > end_at_length:
            value = run(tx)
            values.append(value)
    else:
        number_of_subpackets = tx.take(11)
        for n in range(number_of_subpackets):
            value = run(tx)
            values.append(value)

    return values

version_sum = 0

def run(tx):
    global version_sum 
    packet_version = tx.take(3)
    version_sum += packet_version

    packet_type_id = tx.take(3)

    match packet_type_id:
        case 0:
            packet_value = sum(handle_operator_packet(tx))
        case 1:
            packet_value = 1
            for v in handle_operator_packet(tx):
                packet_value *= v
        case 2:
            packet_value = min(handle_operator_packet(tx))
        case 3:
            packet_value = max(handle_operator_packet(tx))
        case 4:
            (packet_value, value_bits) = parse_literal_value(tx.bits) 
            tx.bits = tx.bits[value_bits:]  
        case 5:
            values = handle_operator_packet(tx)
            packet_value = int(values[0] > values[1])
        case 6:
            values = handle_operator_packet(tx)
            packet_value = int(values[0] < values[1])
        case 7:
            values = handle_operator_packet(tx)
            packet_value = int(values[0] == values[1])

    return packet_value

if __name__ == '__main__':
    input_file = "input_test.txt" if len(sys.argv) > 1 and sys.argv[1] == "--test" else "input.txt"
    lines = Path(input_file).read_text().split("\n")
    tx = Transmission(lines[0])
    result = run(tx)
    print("--- Day 16: Packet Decoder ---")
    print("Part 1: ", version_sum, version_sum == 873)
    print("Part 2: ", result, result == 402817863665)