f=open('cfriendly','r').read().split('\n')[:-1]
string=""
for i in range(0, len(f), 2):

    line1 = f[i]
    line2 = f[i+1].split('_')[1]
    if line2 == 'white':
        score = '1'
    elif line2 == 'black':
        score = '-1'
    elif line2 == 'draw':
        score = '0'
    else:
        print(line2)
    string += line1 +'\n' + score + '\n'

f2=open('cfriendly2','w')
f2.write(string)
f2.close()
