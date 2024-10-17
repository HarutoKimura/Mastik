import csv

def parse_out_file(input_file, output_file):
    with open(input_file, 'r') as infile:
        lines = infile.readlines()

    # Find the start of the data section
    data_start = False
    data_lines = []
    for line in lines:
        if data_start:
            if line.strip():  # Ignore empty lines
                data_lines.append(line.strip())
        if line.startswith("# datapoints"):
            data_start = True

    # Parse the data lines
    parsed_data = []
    for i, line in enumerate(data_lines):
        parts = line.split()
        if len(parts) == 3:
            square, multiply, reduce = map(int, parts)
            parsed_data.append([i, square, multiply, reduce])

    # Write to CSV
    with open(output_file, 'w', newline='') as csvfile:
        csv_writer = csv.writer(csvfile)
        csv_writer.writerow(["Slot", "Square", "Multiply", "Reduce"])
        csv_writer.writerows(parsed_data)

# Usage
input_file = 'FR.out'  # Replace with your .out file path
output_file = 'FR.csv'  # Replace with your desired output CSV file path
parse_out_file(input_file, output_file)