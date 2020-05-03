# Visualization code for the FCM demo.
# Code by Zachary Hutchinson

# Plots the data points and cluster centers.
# Data points are circles. Centers are triangles.
# Colors are randomly selected. If you get a bad combination, just run again.
# Expects 3 dimensional data points.

import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
import math
import random

points = []
clusters = {}
cfinal = {}

def Dist(A,B):
    return math.sqrt(
        (B[0]-A[0])**2 +
        (B[1]-A[1])**2 +
        (B[2]-A[2])**2
    )

with open('pts_data','r') as f:
    for line in f:
        point = line.strip().split(',')
        for v in range(len(point)):
            point[v]=float(point[v])
        points.append(point)

with open('clu_data','r') as f:
    for line in f:
        l = line.strip().split()
        ind = int(l[1])
        if(ind not in clusters):
            clusters[ind] = [[],[],[]]
        cdata = l[2].split(',')
        for i in range(len(cdata)):
            cdata[i]=float(cdata[i])
        for i in range(len(cdata)):
            clusters[ind][i].append(cdata[i])
        cfinal[ind]=cdata


colors = []
for i in range(len(clusters.keys())):
    colors.append((
        random.uniform(0.2,1.0),
        random.uniform(0.2,1.0),
        random.uniform(0.2,1.0)
    ))

fig = plt.figure()
ax = fig.add_subplot(111,projection='3d')

for k,v in clusters.items():
    ax.plot3D(v[0],v[1],v[2],color=colors[k])
    ax.scatter3D(v[0][-1],v[1][-1],v[2][-1],color=colors[k],marker='^')
for p in points:
    mink = 0
    mind = 100000000
    for k,v in cfinal.items():
        d = Dist(p,v)
        if d < mind:
            mink = k
            mind = d


    ax.scatter3D(p[0],p[1],p[2],color=colors[mink])

plt.show()

