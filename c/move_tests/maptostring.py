import sys
fname = sys.argv[1]
text = [s.split(' ') for s in open(fname,'r').read().split('\n')][:-1]
outfile = open('positions/' + fname + '.chess','w')
string = ""
for t in text:
    for i in t:
        string += i
outfile.write(string)
outfile.close()


