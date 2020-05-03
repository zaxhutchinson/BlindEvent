// FCM: Fuzzy C Means Clustering.
// Code by: Zachary hutchinson
// v1: 5/2020 - First version

// An implementation of the Fuzzy C Means clustering algorithm as described in:

// @article{bezdek1984fcm,
//   title={FCM: The fuzzy c-means clustering algorithm},
//   author={Bezdek, James C and Ehrlich, Robert and Full, William},
//   journal={Computers \& Geosciences},
//   volume={10},
//   number={2-3},
//   pages={191--203},
//   year={1984},
//   publisher={Pergamon}
// }

// Things to note:
//     Stores the resulting centers for each cluster.
//     Stores the membership matrix which is CLUSTERxPOINTS.
//     Does not store the data points so as to facilitate using
//         it on larger data sets.
//     Calculates the minimum and maximum of each dimension for all data data points
//         which is used to create the initial random centers.
//     Membership function uses the K-dimensional distance to determine grades.
//         Override at will.
//     Supports simple simulated annealing. sa is the starting percent chance
//         that a center will be moved to a random location. Each iteration
//         the chance for random movement diminishes based on (sa / sa_rounds) until
//         it reaches zero. Defaults to no simulated annealing.

// Uses the VecK structure.

#pragma once

#include<vector>
#include<string>
#include<utility>
#include<cmath>
#include<random>
#include<unordered_map>
#include<random>

#include"vecK.hpp"

struct FCM {
    FCM();
    FCM(
        long rand_seed,
        int _num_clusters, 
        double _fuzzifier,
        double _epsilon,
        double _sa=0.0,
        int _sa_rounds=0
    );
    
    void Run(int _K, std::vector<VecK> & pts);
    void Membership(int point_index, int center_index, std::vector<VecK> & pts);

    // Membership matrix is [cluster][point]
    //   CLUSTER x POINT in size.
    std::vector<std::vector<double>> & GetMembershipMatrix();
    std::vector<VecK> & GetCenters();

private:

    void Init(int _K, std::vector<VecK> & pts);
    VecK RandomPoint();

    std::vector<VecK> centers;
    std::vector<std::vector<double>> memmatrix;
    int K;
    int num_clusters;
    int num_points;
    double fuzzifier;
    double epsilon;
    double sa;
    int sa_rounds;
    std::mt19937_64 rng;
    std::vector<double> mins;
    std::vector<double> maxs;
};