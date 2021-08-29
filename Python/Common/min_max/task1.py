def max_number(a: int or float,  b: int or float, comp=None) -> int or float:
    if comp is None:
        return a if a > b else b
    else:
        return a if comp(a, b) == a else b


def min_number(a: int or float,  b: int or float, comp=None) -> int or float:
    if comp is None:
        return a if a < b else b
    else:
        return a if comp(a, b) == a else b


def max_string(s1: str, s2: str, comp=None) -> str:
    if comp is None:
        return s1 if s1 > s2 else s2
    else:
        return s1 if comp(s1, s2) == s1 else s2


def min_string(s1: str, s2: str, comp=None) -> str:
    if comp is None:
        return s1 if s1 < s2 else s2
    else:
        return s1 if comp(s1, s2) == s1 else s2


def max_list(arr1: list, arr2: list, comp=None) -> list:
    if comp is None:
        if len(arr1) != len(arr2):
            return arr1 if len(arr1) > len(arr2) else arr2
        else:
            for i in range(len(arr1)):
                elem1, elem2 = arr1[i], arr2[i]
                if isinstance(elem1, (int, float)) and isinstance(elem2, (int, float)):
                    if elem1 == elem2:
                        continue
                    else:
                        if max_number(elem1, elem2) == elem1:
                            return arr1
                        else:
                            return arr2

                elif isinstance(elem1, str) and isinstance(elem2, str):
                    if elem1 == elem2:
                        continue
                    else:
                        if max_string(elem1, elem2) == elem1:
                            return arr1
                        else:
                            return arr2
    else:
        return arr1 if comp(arr1, arr2) == arr1 else arr2


def min_list(arr1: list, arr2: list, comp=None) -> list:
    if comp is None:
        if len(arr1) != len(arr2):
            return arr1 if len(arr1) < len(arr2) else arr2
        else:
            for i in range(len(arr1)):
                elem1, elem2 = arr1[i], arr2[i]
                if isinstance(elem1, (int, float)) and isinstance(elem2, (int, float)):
                    if elem1 == elem2:
                        continue
                    else:
                        if min_number(elem1, elem2) == elem1:
                            return arr1
                        else:
                            return arr2
                elif isinstance(elem1, str) and isinstance(elem2, str):
                    if elem1 == elem2:
                        continue
                    else:
                        if min_string(elem1, elem2) == elem1:
                            return arr1
                        else:
                            return arr2
    else:
        return arr1 if comp(arr1, arr2) == arr1 else arr2


def max_of_list_numbers(arr: list, comp=None) -> int or float:
    result = arr[0]
    for elem in arr[1::]:
        result = max_number(result, elem, comp=comp)
    return result


def min_of_list_numbers(arr: list, comp=None) -> int or float:
    result = arr[0]
    for elem in arr[1::]:
        result = min_number(result, elem, comp=comp)
    return result


def max_of_list_strings(arr: list, comp=None) -> str:
    result = arr[0]
    for elem in arr[1::]:
        result = max_string(result, elem, comp=comp)
    return result


def min_of_list_strings(arr: list, comp=None) -> str:
    result = arr[0]
    for elem in arr[1::]:
        result = min_string(result, elem, comp=comp)
    return result


def max_of_list_lists(arr: list, comp=None) -> list:
    result = arr[0]
    for elem in arr[1::]:
        result = max_list(result, elem, comp=comp)
    return result


def min_of_list_lists(arr: list, comp=None) -> list:
    result = arr[0]
    for elem in arr[1::]:
        result = min_list(result, elem, comp=comp)
    return result


print(max_string("x", "abc"))
print(max_of_list_numbers([5, 3, 1, 4, 6, 7]))
