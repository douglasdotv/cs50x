from cs50 import get_string


def main():
    text = get_string("Text: ")

    total_letters = count_letters(text)
    total_words = count_words(text)
    total_sentences = count_sentences(text)

    coleman_liau_index = get_coleman_liau_index(
        total_letters, total_words, total_sentences
    )

    print_result(coleman_liau_index)


def count_letters(text):
    return sum(1 for char in text if char.isalpha())


def count_words(text):
    return len(text.split())


def count_sentences(text):
    return sum(1 for char in text if char in ".?!")


def get_coleman_liau_index(letters, words, sentences):
    l = letters / words * 100
    s = sentences / words * 100
    return 0.0588 * l - 0.296 * s - 15.8


def print_result(index):
    index = round(index)

    if index < 1:
        print("Before Grade 1")
    elif index > 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


if __name__ == "__main__":
    main()
