# Standart imports
import os
import csv

# External imports
import pandas as pd

##############################################
# Common constants

DB_NAME = "database.csv"
DB_DELIMITER = ';'

DB_FIELDS = ["First_name",
			 "Last_name",
			 "Mobile_phone",
			 # Additional information:
			 "Extra_phone",
			 "Birthday",
			 "Address"]

##############################################

class Book:

	dataframe = None # Pandas table
	datacount = 0    # Count of rows in the table
	def update(self):
		self.dataframe = pd.read_csv(DB_NAME, delimiter=DB_DELIMITER, na_filter=False)
		self.datacount = self.dataframe.shape[0]


	# Init a database if file is empty or doesn't exist
	def __init__(self):	

		# Is file empty?
		try:
			file_info = os.stat(DB_NAME)
			size = file_info.st_size
		# File doesn't exist
		except:
			size = 0

		# Create a new one
		if size == 0:
			with open(DB_NAME, "w", newline='') as db:
				handler = csv.DictWriter(db, fieldnames=DB_FIELDS, delimiter=DB_DELIMITER)
				handler.writeheader()

		# First create of pandas table
		self.update()

	# Rewrite the database without current record
	def delete(self, data: dict):
		with open(DB_NAME + ".temp", "w", newline='') as tmp:
			db = open(DB_NAME, "r")
			writer = csv.DictWriter(tmp, fieldnames=DB_FIELDS, delimiter=DB_DELIMITER)
			for row in csv.DictReader(db,  fieldnames=DB_FIELDS, delimiter=DB_DELIMITER):
				if (row[DB_FIELDS[0]] == data[DB_FIELDS[0]]) and (row[DB_FIELDS[1]] == data[DB_FIELDS[1]]):
					continue
				writer.writerow(row)
			db.close()
		os.remove(DB_NAME)
		os.rename(DB_NAME + ".temp", DB_NAME)
		self.update()

	# Insert a new record in the database
	def append(self, data: dict):
		with open(DB_NAME, "a", newline='') as db:
			handler = csv.DictWriter(db, fieldnames=DB_FIELDS, delimiter=DB_DELIMITER)
			handler.writerow(data)
		self.update()


	# Get the rows with current values
	def get(self, criteries=None) -> tuple:
		self.update()

		# Return all the records
		if (criteries == None) or (len(criteries.keys()) == 0):
			return (self.dataframe, self.datacount)

		# Intersection of tables
		result = self.dataframe
		for key, value in criteries.items():
			result = pd.merge(result, self.dataframe[self.dataframe[key].isin(value)])
		return (result, result.shape[0])
