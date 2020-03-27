import sys
import random

result = sys.argv[1]
data = sys.argv[2]

with open(result, 'r') as f:
    lines = f.readlines()
    left = [x.strip() for x in lines]

with open(data, 'r') as f:
    lines = f.readlines()
    right = [x.strip() for x in lines]

arr = []
for lx in left:
    for rx in right:
        arr.append(lx + '|' + rx)

random.shuffle(arr)

with open('double.csv', 'w') as f:
    f.writelines('left|right\n')
    for x in arr:
        f.writelines(x + '\n')
