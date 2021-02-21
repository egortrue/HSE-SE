from random import randint
N = int(input("Max number which can be guessed: "))
mystery_number = randint(1, N)

user_answer = 0
counter = 0

print("== The game begin ==")
while user_answer != mystery_number:
    counter += 1
    user_answer = int(input("Input the number: "))
    if user_answer > mystery_number:
        print("Your number is higher")
    elif user_answer < mystery_number:
        print("Your number is lower")
    else:
        print(f"You win!! Answer: {mystery_number}\nAttempts: {counter}")
        break
