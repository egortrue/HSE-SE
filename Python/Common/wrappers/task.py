import time
import datetime

def benchmark(func):
	def wrapper():
		start = time.time()
		func()
		end = time.time()
		print(f"Work time {func.__name__}: {end - start}")
	return wrapper

def sleep(before, after):
	def sleep_func(func):
		def wrapper(*args):
			time.sleep(before)
			func(*args)
			time.sleep(after)
		return wrapper
	return sleep_func

def log(func):
	def wrapper(*args):
		with open("log.txt", "a") as file:
			date = datetime.datetime.today().strftime("%Y.%m.%d(%H:%M:%S)")
			arguments = (", ").join([str(el) for el in args])
			return_data = func(*args)
			file.write(f"{date} | {func.__name__}({arguments}) -> {return_data}\n")
	return wrapper

@sleep(1, 1)
@log
def hello(*args):
	print("hi")
	return "ok"

hello("good day to see ya!", "hello")
hello(100)
hello(5, 6, 1)
hello()
