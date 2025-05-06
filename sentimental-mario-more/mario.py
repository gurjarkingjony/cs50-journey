def get_height():
    while True:
        try:
            height = int(input("Height: "))
            if 1 <= height <= 8:
                return height
        except ValueError:
            pass

def print_pyramid(height):
    for row in range(1, height + 1):
        # Left pyramid
        left = "#" * row
        left = left.rjust(height)

        # Right pyramid
        right = "#" * row

        # Combine with gap
        print(f"{left}  {right}")

# Run the program
height = get_height()
print_pyramid(height)
