def range_float(start: float, finish: float, step=1.0):
    i = start
    while i < finish:
        yield i
        i += step


def convert_from_second2full_format(second: int) -> str:
    hour = second // 3600
    minute = (second - hour * 3600) // 60
    sec = second % 60
    return "%02d:%02d:%02d" % (hour % 24, minute % 60, sec)


def convert_from_full_format2second(time: str) -> int:
    hour, minute, sec = map(int, time.split(":"))
    return hour * 3600 + minute * 60 + sec


def range_time(start: str, finish: str, step="00:01:00"):
    start_in_second = convert_from_full_format2second(start)
    step_in_second = convert_from_full_format2second(step)
    finish_in_second = convert_from_full_format2second(finish)

    if start_in_second > finish_in_second:
        finish_in_second += 24 * 3600

    while start_in_second < finish_in_second:
        yield convert_from_second2full_format(start_in_second)
        start_in_second += step_in_second


for i in range_float(1.2, 5.0):
    print(i)

for time in range_time("22:58:00", "23:00:00", "00:00:30"):
    print(time)
