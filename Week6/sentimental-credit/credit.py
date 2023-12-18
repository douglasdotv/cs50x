import re
from cs50 import get_string


def main():
    card_number = get_string("Number: ")

    if not re.match(r"^\d+$", card_number):
        print("INVALID")
        return

    if is_valid(card_number):
        print_card_type(card_number)
    else:
        print("INVALID")


def is_valid(number):
    total_sum = 0
    number_str = str(number)
    length = len(number_str)

    for i in range(length - 2, -1, -2):
        digit = int(number_str[i]) * 2
        total_sum += digit // 10
        total_sum += digit % 10

    for i in range(length - 1, -1, -2):
        total_sum += int(number_str[i])

    return total_sum % 10 == 0


def print_card_type(number):
    if re.match(r"^3[47][0-9]{13}$", number):
        print("AMEX")
    elif re.match(r"^5[1-5][0-9]{14}$", number):
        print("MASTERCARD")
    elif re.match(r"^4[0-9]{12}([0-9]{3})?$", number):
        print("VISA")
    else:
        print("INVALID")


if __name__ == "__main__":
    main()
