from cs50 import get_int

min_height = 1
max_height = 8

while True:
    height = get_int("Height: ")

    if height in range(min_height, max_height + 1):
        break

for i in range(1, height + 1):
    print(" " * (height - i) + "#" * i)
