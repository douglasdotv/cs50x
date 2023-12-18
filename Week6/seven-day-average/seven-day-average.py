import csv
import requests

def main():
    # Read NYTimes Covid Database
    download = requests.get(
        "https://raw.githubusercontent.com/nytimes/covid-19-data/master/us-states.csv"
    )
    decoded_content = download.content.decode("utf-8")
    file = decoded_content.splitlines()
    reader = csv.DictReader(file)

    # Construct 14 day lists of new cases for each states
    new_cases = calculate(reader)

    # Create a list to store selected states
    states = []
    print("Choose one or more states to view average COVID cases.")
    print("Press enter when done.\n")

    while True:
        state = input("State: ")
        if state in new_cases:
            states.append(state)
        if len(state) == 0:
            break

    print(f"\nSeven-Day Averages")

    # Print out 7-day averages for this week vs last week
    comparative_averages(new_cases, states)


def calculate(reader):
    new_cases = {}
    previous_cases = {}
    recent_days = 14

    for row in reader:
        state = row["state"]
        current_total_cases = int(row["cases"])

        if state not in new_cases:
            new_cases[state] = []

        today_cases = current_total_cases - previous_cases.get(state, 0)
        new_cases[state].append(today_cases)
        previous_cases[state] = current_total_cases

        if len(new_cases[state]) > recent_days:
            new_cases[state].pop(0)

    return new_cases


def comparative_averages(new_cases, states):
    for state in states:
        current_week_total_cases = sum(new_cases[state][-7:])
        previous_week_total_cases = sum(new_cases[state][:7])

        current_week_avg = current_week_total_cases / 7
        previous_week_avg = previous_week_total_cases / 7

        try:
            diff = current_week_avg - previous_week_avg
            percentage_change = round(abs(diff) / previous_week_total_cases * 100)
            change_type = "increase" if diff > 0 else "decrease"
        except ZeroDivisionError:
            print(f"Insufficient data for comparison ({state.capitalize()}).")

        print(f"{state.capitalize()} had a 7-day average of {current_week_total_cases} and a {change_type} of {percentage_change}%.")


if __name__ == "__main__":
    main()
