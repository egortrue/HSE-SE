import csv

print("====================================================")
quakes_ra6 = 0
with open("quake.csv", "r") as file:
	reader = csv.DictReader(file)
	for row in reader:
		if float(row['Richter']) > 6.0:
			quakes_ra6 += 1

print(f"Count of quakes with the Richter above 6: {quakes_ra6}")
print("====================================================")

def getArea(coord: tuple):
	if (-60 <= float(coord[0]) <= 40) and (-30 <= float(coord[1]) <= 60):
		return "Africa"
	if (40 <= float(coord[0]) <= 80) and (-30 <= float(coord[1]) <= 35):
		return "Europe"
	if (-10 <= float(coord[0]) <= 80) and (35 <= float(coord[1]) <= 180):
		return "Asia"
	if (-60 <= float(coord[0]) <= -10) and (100 <= float(coord[1]) <= 180):
		return "Australia"
	if (-60 <= float(coord[0]) <= 15) and (-100 <= float(coord[1]) <= -30):
		return "South America"
	if (15 <= float(coord[0]) <= 80) and (-180 <= float(coord[1]) <= -30):
		return "North America"
	if (-60 <= float(coord[0]) <= 60) and (-180 <= float(coord[1]) <= -100):
		return "Pacific Ocean"

quakes = {}
with open("quake.csv", "r") as file:
	reader = csv.DictReader(file)
	for row in reader:

		k = 0
		key = (row['Latitude'], row['Longitude'])
		while key in quakes.keys():
			key = (row['Latitude'], row['Longitude'], k)
			k += 1

		value = (row['Focal depth'], row['Richter'])
		quakes.update({key: value})

print(f"Count of quakes: {len(quakes.keys())}")
print()

top_depth = list(reversed(sorted(quakes.items(), key=lambda item: int(item[1][0]))))[0:10]
top_area_d = {}
print("Top focal depth:")
counter = 0
for top in top_depth:
	area = getArea(top[0])
	if area not in top_area_d.keys():
		top_area_d.update({area: 0})
	top_area_d[area] += 1

	print(f"\t{counter}) {top[1][0]}: {top[0]} - {area}")
	counter += 1

top_area = list(reversed(sorted(top_area_d.items(), key=lambda item: int(item[1]))))[0]
print(f"\tTop area: {top_area[0]} - {top_area[1]} times ")
print()


top_richter = list(reversed(sorted(quakes.items(), key=lambda item: float(item[1][1]))))[0:10]
top_area_d = {}
print("Top Richter:")
counter = 0
for top in top_richter:
	area = getArea(top[0])
	if area not in top_area_d.keys():
		top_area_d.update({area: 0})
	top_area_d[area] += 1

	print(f"\t{counter}) {top[1][1]}: {top[0]} - {area}")
	counter += 1

top_area = list(reversed(sorted(top_area_d.items(), key=lambda item: int(item[1]))))[0]
print(f"\tTop area: {top_area[0]} - {top_area[1]} times ")
print()


print("====================================================")

quakes_NE = {}
quakes_NW = {}
quakes_SE = {}
quakes_SW = {}
with open("quake.csv", "r") as file:
	reader = csv.DictReader(file)
	for row in reader:

		k = 0
		key = (row['Latitude'], row['Longitude'])
		while ((key in quakes_NE.keys()) or
			  (key in quakes_NW.keys()) or
			  (key in quakes_SE.keys()) or
			  (key in quakes_SW.keys())):
			key = (row['Latitude'], row['Longitude'], k)
			k += 1

		value = (row['Focal depth'], row['Richter'])

		if (0 <= float(key[0]) <= 90) and (0 <= float(key[1]) <= 180):
			quakes_NE.update({key: value})
		if (0 <= float(key[0]) <= 90) and (-180 <= float(key[1]) <= 0):
			quakes_NW.update({key: value})
		if (-90 <= float(key[0]) <= 0) and (0 <= float(key[1]) <= 180):
			quakes_SE.update({key: value})
		if (-90 <= float(key[0]) <= 0) and (-180 <= float(key[1]) <= 0):
			quakes_SW.update({key: value})

print(f"Count of quakes in North-East: {len(quakes_NE.keys())}")
print(f"Count of quakes in North-West: {len(quakes_NW.keys())}")
print(f"Count of quakes in South-East: {len(quakes_SE.keys())}")
print(f"Count of quakes in South-West: {len(quakes_SW.keys())}")
print(f"Count of quakes: {len(quakes_NE.keys()) + len(quakes_NW.keys()) + len(quakes_SE.keys()) + len(quakes_SW.keys())}")
