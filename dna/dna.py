import csv
import sys

def load_database(filename):
    with open(filename, newline='') as file:
        reader = csv.DictReader(file)
        return list(reader), reader.fieldnames[1:]

def load_dna_sequence(filename):
    with open(filename, 'r') as file:
        return file.read()

def longest_match(sequence, subsequence):
    longest = 0
    sub_len = len(subsequence)

    for i in range(len(sequence)):
        count = 0
        while sequence[i + count * sub_len : i + (count + 1) * sub_len] == subsequence:
            count += 1
        longest = max(longest, count)

    return longest

def find_matching_profile(database, str_counts, str_names):
    for person in database:
        if all(int(person[str_name]) == str_counts[str_name] for str_name in str_names):
            return person['name']
    return "No match"

def main():
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        return

    database_filename = sys.argv[1]
    sequence_filename = sys.argv[2]

    database, str_names = load_database(database_filename)

    sequence = load_dna_sequence(sequence_filename)

    str_counts = {str_name: longest_match(sequence, str_name) for str_name in str_names}

    match = find_matching_profile(database, str_counts, str_names)
    print(match)

if __name__ == "__main__":
    main()
