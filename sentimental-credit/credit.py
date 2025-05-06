def get_card_number():
    while True:
        try:
            number = input("Number: ").strip()
            if number.isdigit():
                return number
        except ValueError:
            pass

def luhn_checksum(card_number):
    total = 0
    reversed_digits = card_number[::-1]

    for i, digit in enumerate(reversed_digits):
        n = int(digit)
        if i % 2 == 1:
            n *= 2
            if n > 9:
                n -= 9
        total += n

    return total % 10 == 0

def get_card_type(card_number):
    if not luhn_checksum(card_number):
        return "INVALID"

    length = len(card_number)
    start_digits = int(card_number[:2])
    first_digit = int(card_number[0])

    if first_digit == 4 and length in [13, 16]:
        return "VISA"
    elif start_digits in [34, 37] and length == 15:
        return "AMEX"
    elif 51 <= start_digits <= 55 and length == 16:
        return "MASTERCARD"
    else:
        return "INVALID"

# Run the program
card_number = get_card_number()
card_type = get_card_type(card_number)
print(card_type)
