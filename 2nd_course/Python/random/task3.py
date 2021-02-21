
def permutation(string):
	if len(string) < 2:
		return [string]

	perm_list = []
	for letter in set(string):
		small_string = string.replace(letter, "", 1)
		other = permutation(small_string)

		for other_letter in other:
			perm_list.append(letter + other_letter)

	return perm_list

print(permutation("aab"))
print(permutation("abc"))
