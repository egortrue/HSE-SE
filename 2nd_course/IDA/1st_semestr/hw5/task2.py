import random

def get_new_str(string: str, lenght: int, repeat=False):

	if repeat == False and len(string) < lenght:
		return "ERROR: not enough symbols in source string"

	if repeat:
		string = list(set(string))
		return ("").join( [string[random.randint(0, len(string)-1)] for i in range(lenght)] )
	else:
		string = {letter: string.count(letter) for letter in sorted(set(string))}
		new_str, i = [], 0
		while i < lenght:
			random_key = list(string.keys())[random.randint(0, len(string)-1)]
			new_str.append(random_key)
			string[random_key] -= 1
			if string[random_key] == 0:
				del string[random_key]
			i += 1
		return ("").join(new_str)

print(get_new_str("abc", 10, True))
print(get_new_str("aabbcc", 3, False))
