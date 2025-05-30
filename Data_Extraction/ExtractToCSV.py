import sys
import csv

input_dir = "arduinoOutput/"
output_dir = "extractedOutput/"
data_prefix = "DataEntry:"

def main():
	if len(sys.argv) != 3:
		print("Correct usage: python ExtractToCSV.py input_file output_file")
		return

	input_file_name = sys.argv[1]
	dataset = []
	with open(input_dir + input_file_name, "r") as file_to_process:
		for line in file_to_process:
			values = line.split()
			if values[0] != data_prefix:
				continue
			values = values[1:]
			dataset.append(values)

	output_file_name = sys.argv[2]
	header = ["Flex 1", "Flex 2", "Flex 3", "Flex 4", "Flex 5", "Label"]
	with open(output_dir + output_file_name, "w") as output_csv:
		writer = csv.writer(output_csv)
		writer.writerow(header)
		writer.writerows(dataset)

	print("Success!")


if __name__ == "__main__":
	main()
