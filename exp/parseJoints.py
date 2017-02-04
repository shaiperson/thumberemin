from sys import argv as args

# file lines are "<freq> <freq> <numsamples>"

f = open(args[1], 'r')
lines = f.readlines()

accum = -1

res = ''

for line in lines:
    comps = line.split(' ')
    accum += int(comps[2])
    res += str(accum) + ','

print res[:len(res)-1] # print without trailiing ','
