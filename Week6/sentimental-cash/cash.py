from cs50 import get_float


def main():
    dollars = get_change()
    cents = round(dollars * 100)

    quarters = calculate_quarters(cents)
    cents -= quarters * 25

    dimes = calculate_dimes(cents)
    cents -= dimes * 10

    nickels = calculate_nickels(cents)
    cents -= nickels * 5

    pennies = calculate_pennies(cents)
    cents -= pennies

    coins = quarters + dimes + nickels + pennies
    print(f"{coins:.0f}")


def get_change():
    while True:
        change = get_float("Change owed: ")

        if change >= 0:
            break

    return change


def calculate_quarters(cents):
    return cents // 25


def calculate_dimes(cents):
    return cents // 10


def calculate_nickels(cents):
    return cents // 5


def calculate_pennies(cents):
    return cents


if __name__ == "__main__":
    main()
