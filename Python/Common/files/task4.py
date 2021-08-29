def my_enumerate(iter_obj, start_value=0):
    index = start_value
    for element in iter_obj:
        yield index, element
        index += 1


for ind, elem in my_enumerate([1, 2, 3, 4], 100):
    print(ind, elem)
