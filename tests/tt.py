import time

ms = time.time_ns()
print(ms)
time.sleep(1)
me = time.time_ns()
print(me)
print((me - ms) / (1000 * 1000 * 1000))
