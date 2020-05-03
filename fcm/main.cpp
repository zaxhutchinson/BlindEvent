// A demo of the FCM algorithm.
// Code by Zachary Hutchinson
//
// Creates a collection of random data points and uses the FCM struct to
// identify clusters. Works in tandem with the viz.py Python3 file for
// a visualization.
//
// Compile: g++ -std=c++17 *.cpp
// Run: ./a.out
//
// Outputs the random points and the final centers of each cluster into two
// separate data files (pts_data and clu_data)
//
// Although the C++ code below and the FCM class can handle data points
// of many dimensions, the python visualization code expects points of
// 3 dimensions.

#include<iostream>
#include<vector>
#include<string>
#include<utility>
#include<cmath>
#include<random>
#include<unordered_map>
#include<fstream>

#include"fcm.hpp"

template<typename T>
using vec = std::vector<T>;
template<typename T,typename U>
using umap = std::unordered_map<T,U>;

int main(int argc, char** argv) {

    // Necessary evils
    std::ofstream ofs_syns("pts_data");
    std::ofstream ofs_clus("clu_data");
    std::random_device rd;
    std::mt19937_64 rng(rd());
    std::uniform_real_distribution<double> lDist(-100.0,100.0);
    std::uniform_real_distribution<double> dDist(0.0,1.0);

    // Various parameters
    int num_clusters = 3;   // Number of clusters
    int num_syns = 100;     // Number of data points
    double fuz = 2.0;       // Fuzzifier
    double E = 0.1;         // Stopping condition
    double sa = 0.1;        // Simulated annealing percent
    int sa_rounds = 10;      // Number of iterations to run SA.
    int K = 3;              // Data point dimension

    // Create FCM object
    FCM fcm(rng(),num_clusters,fuz,E,sa,sa_rounds);

    // Create data points
    vec<VecK> syns;
    for(int i = 0; i < num_syns; i++) {
        vec<double> d;
        for(int k = 0; k < K; k++) {
            d.push_back(lDist(rng));
        }
        syns.emplace_back(VecK(d));
    }

    // Save data points to file.
    for(int i = 0; i < num_syns; i++) {
        ofs_syns << syns[i] << std::endl;
    }

    // Run the clustering algorithm
    fcm.Run(K,syns);

    // Get results. (not using the membership matrix here)
    vec<vec<double>> & results = fcm.GetMembershipMatrix();
    vec<VecK> & centers_after = fcm.GetCenters();

    // Output centers to a file.
    for(std::size_t i = 0; i < centers_after.size(); i++) {
        ofs_clus << 0 << " " << i << " " << centers_after[i] << std::endl; 
    }

    return 0;
}