import sys
import random
from pyfiglet import Figlet

def main():
    num_args = len(sys.argv) - 1

    if num_args not in [0, 2]:
        print("Invalid usage")
        sys.exit(1)

    figlet = Figlet()
    fonts = figlet.getFonts()
    font = get_font(fonts, num_args)
    figlet.setFont(font=font)

    text = input("Input: ")

    print(figlet.renderText(text))


def get_font(fonts, num_args):
    if num_args == 0:
        return random.choice(fonts)
    elif num_args == 2:
        flag = sys.argv[1]
        font = sys.argv[2]

        if flag not in ["-f", "--font"] or font not in fonts:
            print("Invalid usage")
            sys.exit(1)

        return font


if __name__ == "__main__":
    main()
