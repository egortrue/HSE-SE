import os


class TextIterator():
    def __init__(self, path):
        self.path = path
        self.list_files = [element for element in os.listdir(self.path) if os.path.isfile(f"{self.path}/{element}")]
        self.list_lines = self.get_lines_from_all_files()
        self.index = 0

    def __iter__(self):
        self.index = 0
        return self

    def __next__(self):
        if self.index == len(self.list_lines):
            raise StopIteration
        line = self.list_lines[self.index]
        self.index += 1
        return line

    def get_lines_from_all_files(self):
        lines = []
        for file in self.list_files:
            with open(f"{self.path}/{file}", "r") as f:
                if len(f.read()) > 140:
                    f.seek(0)
                    lines.extend([line.strip() for line in f.readlines()])
        return lines


path = "./test"
it = TextIterator(path)
for line in it:
    print(line)
