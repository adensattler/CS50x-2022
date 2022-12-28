import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        exit()

    #Read database file into a variable
    database = []
    with open(sys.argv[1], newline='') as csvfile:
        reader = csv.DictReader(csvfile)
        for row in reader:
            # for sub in row:
            #     for key in sub:
            #         sub[key] = int(sub[key])

            database.append(row)
    # [{'name': 'Albus', 'AGATC': '15', 'TTTTTTCT': '49', 'AATG': '38', 'TCTAG': '5', 'GATA': '14', 'TATC': '44', 'GAAA': '14', 'TCTG': '12'},
    #print(database)

    # Read DNA sequence file into a variable
    dna_sequence = ""
    with open(sys.argv[2], 'r') as file:
        for row in file:
            dna_sequence+=row
    #print(dna_sequence)
    
    # Find longest match of each STR in DNA sequence
    str_key_list = list(database[0].keys())
    str_key_list.pop(0)

    highest_strs = {}
    #for each possible str subsequence (like AGATC or TTTTTTCT) find the longest sequence of that sub and add it to the highest_strs dict
    for i in range(len(str_key_list)):
        tmp = longest_match(dna_sequence, str_key_list[i])
        highest_strs[str_key_list[i]] = str(tmp)

    # Check database for matching profiles

    # Now look at each person in the data base
    # then look at each key value pair in the highest_strs dict and see if all values are the same as that person's values
    comparison_strs = list(highest_strs.keys())
    match = ''

    for person in database:
        count = 0
        for substring in comparison_strs:
            if person[substring] == highest_strs[substring]:
                count+=1
        if count == len(highest_strs):
            match = person['name']

    if match == '':
        print("No Match")
    else:
        print(match)
    return


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


main()
