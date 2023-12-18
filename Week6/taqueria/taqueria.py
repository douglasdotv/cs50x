prices = {
    "baja taco": 4.25,
    "burrito": 7.50,
    "bowl": 8.50,
    "nachos": 11.00,
    "quesadilla": 8.50,
    "super burrito": 8.50,
    "super quesadilla": 9.50,
    "taco": 3.00,
    "tortilla salad": 8.00
}

total_price = 0

try:
    while True:
        item = input("Item: ").lower()

        if item in prices:
            total_price += prices[item]
        else:
            continue

        print(f"Total: ${total_price:.2f}")
except EOFError:
    print(f"\nTotal: ${total_price:.2f}")
