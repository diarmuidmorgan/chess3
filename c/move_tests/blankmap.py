import sys
filename = sys.argv[1];
f = open(filename,'a')
for i in range(8):
    f.write("_ _ _ _ _ _ _ _\n")
f.close()

