import os


# format path: d:\python_project\IAD_minor
def gen_for_files(path: str):
    list_elements = os.listdir(path)
    for elem in list_elements:
        elem_path = f"{path}\{elem}"
        if os.path.isfile(elem_path):
            yield elem_path
        else:
            yield from gen_for_files(elem_path)


for element in gen_for_files(path="./test"):
    print(element)
