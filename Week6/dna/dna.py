import csv
import sys


def main():
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    dna_profiles_csv = sys.argv[1]

    dna_profiles = {}

    with open(dna_profiles_csv, "r") as file:
        reader = csv.DictReader(file)

        str_names = [field for field in reader.fieldnames if field != "name"]

        for row in reader:
            person_name = row["name"]
            str_counts = {str_name: int(row[str_name]) for str_name in str_names}
            dna_profiles[person_name] = str_counts

    dna_sequence_txt = sys.argv[2]

    with open(dna_sequence_txt, "r") as file:
        dna_sequence = file.readline().strip()

    individual_longest_runs = {}

    for str in str_names:
        longest_run = longest_match(dna_sequence, str)
        individual_longest_runs[str] = longest_run

    for name in dna_profiles:
        is_match = True

        for str in individual_longest_runs:
            if dna_profiles[name][str] != individual_longest_runs[str]:
                is_match = False
                break

        if is_match:
            matching_individual = name
            break

    result = matching_individual if is_match else "No match"
    print(result)


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):
        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:
            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


if __name__ == "__main__":
    main()
