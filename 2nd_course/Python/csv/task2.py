import csv

def find_data(chemic: str, greater=0):
	with open("AirQualityUCI.csv", "r") as file:
		reader = csv.DictReader(file, delimiter=';')
		data = {}
		for row in reader:
			key = (row['Date'], row['Time'])
			value = float(row[chemic])
			data.update({key: value})
	
	data = list(sorted(data.items(), key=lambda item: int(item[1])))
	if (greater):
		return data[-1]
	else:
		i = 0
		while data[i][1] <= -200:
			i += 1
		return data[i] 

chemic = 'CO(GT)'
data_low = find_data(chemic, 0)
data_gr = find_data(chemic, 1)
print(f"The lowest {chemic} was {data_low[0][0]} in {data_low[0][1]}")
print(f"The greatest {chemic} was {data_gr[0][0]} in {data_gr[0][1]}")
