import os
files = os.listdir()
count = 0
for f in files:
    if '.' in f and f[-4:] != '.out':
        text = open(f,'r')
        lines = len(text.read().split('\n'))
        count += lines
        text.close()
print(count)
