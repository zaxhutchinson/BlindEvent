import matplotlib.pyplot as plt
import numpy as np
import random

data = {}
f = open('data','r')
d = []
i = None
for line in f:
    if line[0]=='#':
        if i != None:
            #print(i)
            data[i]=list(d)
        i = int(line.strip().split()[1])
        d = []
    else:
        l = line.strip().split(',')
        d.append([float(l[0]),float(l[1])])
#print(i)
data[i]=list(d)
f.close()

colors=[]
for i in range(len(data.keys())):
    colors.append(np.array([random.random(),random.random(), random.random()],))


for k,v in data.items():
    for d in v:
        plt.plot(d[0],d[1],c=colors[k],marker='.',linestyle='')

plt.show()