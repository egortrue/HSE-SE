# Standart imports
import os
from datetime import datetime
# Local imports
import database as db

#######################################################################################
# Common constants

title = """
\t\t\t\t.=================. 
\t\t\t\t| Tiny Phone Book | 
\t\t\t\t'================='
"""

menu = """\
  1) Print all the records in the book
  2) Find current records in the book
  3) Add/Update a record
  4) Delete a record

  0) Exit the program
"""

DB_FIELDS = db.DB_FIELDS
book = db.Book()

#######################################################################################


def clearScreen():
	if os.name == "nt":
		os.system("cls")
	else
		os.system("clear")
	print(title)


def printTable(data: tuple):
	table, count = data
	if count == 0:
		print("Records not found!")
	else:
		print(f"Total count of the records: {count} records\n\n{table}")


# Letters + numbers + spaces
def checkName(name: str):
	if len(name) == 0:
		return None
	if not name[0].isupper():
		return None
	if name.count(" ") == 0:
		if name.isalnum():
			return name
	else:
		for subname in name.split(" "):
			if not subname.isalnum():
				return None
		return name


def checkNumber(number: str):
	if len(number) < 11 or len(number) > 12:
		return None
	if number[0] == "+":
		if not number[1:].isdigit():
			return None
		number = list(number)
		number.pop(0)
		number[0] = str(int(number[0]) + 1) 
		return ("").join(number)
	else:
		if number.isdigit():
			return number
		return None


def checkDate(date: str):
    try:
        datetime.strptime(date, '%d.%m.%Y')
        return date
    except ValueError:
        return None


def addRecord():

	print("Fill in the necessary criteries to add a new record.")
	print("Criteries with * on the end are absolutely necessary.\n")

	criteries = {}
	for i in range(len(DB_FIELDS)):
		key = DB_FIELDS[i]

		# First and Last names
		if (i == 0) or (i == 1):
			input_line = (' ').join(DB_FIELDS[i].split('_'))
			value = checkName(input(f"{input_line + '*'}: "))
			if value == None:
				print(f"It is necessary to fill in the \"{input_line}\" correctly")
				print("Name should contains only letters, numbers and spaces")
				print("It is also necessary that the name begin with upper letter")
				return

		# Mobile phone
		elif (i == 2):
			input_line = (' ').join(DB_FIELDS[i].split('_'))
			value = checkNumber(input(f"{input_line + '*'}: "))
			if value == None:
				print(f"It is necessary to fill in the \"{input_line}\" correctly")
				print("Phone number should contains only 11 numbers and plus sign at begin optionally")
				return

		# Date
		elif (i == 4):
			value = input(f"{DB_FIELDS[i]}: ")
			if len(value) != 0:
				value = checkDate(value)
				if value == None:
					print(f"It is necessary to fill in the \"{DB_FIELDS[i]}\" correctly")
					print("Date should be in this format: day.month.year")
					return
		else:
			value = input(f"{DB_FIELDS[i]}: ")

		if (value != None) and (value != ''):
			criteries.update({key: value})

	unique_name = {DB_FIELDS[0]: criteries[DB_FIELDS[0]],
				   DB_FIELDS[1]: criteries[DB_FIELDS[1]]}

	table, count = book.get(unique_name)
	if count == 0:
		print("\nDo you want to update database with this data:")
		for key, value in criteries.items():
			print(f"{key}: {value}")
		answer = input("\nAnswer(Y/n): ")
		if (answer == "Y"):
			book.append(criteries)
	else:
		print("\nThis record already exist. Do you want to update data:\n")
		print(f"Old record:\n {table}\n\nNew record:")
		for key, value in criteries.items():
			print(f"{key}: {value}")
		answer = input("\nAnswer(Y/n): ")
		if (answer == "Y"):
			book.delete(unique_name)
			book.append(criteries)

def findRecord():

	print("Fill in the necessary criteries to find records.")
	print("You can use several keys in one criteria using commas. (First name: Egor, Ilya, ...)\n")

	criteries = {}
	for i in range(len(DB_FIELDS)):
		key = DB_FIELDS[i]
		value = input(f"  {(' ').join(DB_FIELDS[i].split('_'))}: ")
		if value != '':
			criteries.update({key: value.split(', ')})

	clearScreen()
	print("========================")
	print("Search criteries: ")
	for key, value in criteries.items():
		print(f"  {key}: {(', ').join(value)}")
	print("\n========================")

	printTable(book.get(criteries))

def deleteRecord():

	print("Fill in the first and last of record, which is gonna be deleted.\n")

	criteries = {}
	for i in range(0, 2):
		key = DB_FIELDS[i]
		value = input(f"  {(' ').join(DB_FIELDS[i].split('_'))}: ")
		if value != '':
			criteries.update({key: value.split(', ')})

	unique_name = {DB_FIELDS[0]: criteries[DB_FIELDS[0]],
				   DB_FIELDS[1]: criteries[DB_FIELDS[1]]}

	table, count = book.get(unique_name)
	if (count == 0):
		print("\nThe record not found.")
	else:
		print("\nDo you want to delete this record:")
		print(table)
		answer = input("\nAnswer(Y/n): ")
		if (answer == "Y"):
			book.delete(unique_name)

while True:
	clearScreen()
	print(menu)

	#########################
	# Check incorrect input
	try:
		choice = int(input("Choose one of the actions above: "))
	except ValueError:
		continue
	if choice < 0 or choice > 4:
		continue
	else:
		clearScreen()
	#########################

	if choice == 1:
		printTable(book.get())
	elif choice == 2:
		findRecord()
	elif choice == 3:
		addRecord()
	elif choice == 4:
		deleteRecord()
	elif choice == 0:
		print("\t\t\t\tSee you next time <3")
		break
	input("\n\nPress Enter to continue...")

	# in a case when we updated database manually
	book.update()
