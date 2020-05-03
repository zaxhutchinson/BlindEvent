## Fuzzy C Means Clustering

fcm.hpp/cpp contains an implementation of the FCM algorithm described in
Bezdek 1984: FCM: The fuzzy c-means clustering algorithm.

main.cpp contains a demo which creates 100 random data points and places them
in 3 clusters.

### Instructions

g++ -std=c++17 *.cpp

./a.out (creates two files [data points and final cluster centers])

python3 viz.py (for visualization)