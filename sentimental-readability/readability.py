def count_letters(text):
    return sum(1 for char in text if char.isalpha())

def count_words(text):
    return len(text.strip().split())

def count_sentences(text):
    return sum(1 for char in text if char in ['.', '!', '?'])

def compute_coleman_liau_index(letters, words, sentences):
    L = (letters / words) * 100
    S = (sentences / words) * 100
    index = 0.0588 * L - 0.296 * S - 15.8
    return round(index)

def get_grade(index):
    if index < 1:
        return "Before Grade 1"
    elif index >= 16:
        return "Grade 16+"
    else:
        return f"Grade {index}"

def main():
    text = input("Text: ")
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    index = compute_coleman_liau_index(letters, words, sentences)
    grade = get_grade(index)

    print(grade)

main()
