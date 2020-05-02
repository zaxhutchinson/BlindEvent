///////////////////////////////////////////////////////////////////////////////
// Clustering using Lloyd's algorithm
// Code by zax
// c++ implementation is a translation (with modifications) of the python code 
//      found here: https://datasciencelab.wordpress.com/tag/lloyds-algorithm/
//
// Requires the vec2.hpp file.
//
// Compile with g++ vec2.cpp main.cpp -std=c++17
//  - Produces a data file which can be read by the python3 script: display.py
//  - display.py will create a visualization of the clustering.
//
///////////////////////////////////////////////////////////////////////////////

#include<iostream>
#include<random>
#include<fstream>
#include<algorithm>
#include<limits>
#include<utility>
#include<list>

#include"vec2.hpp"

template<typename T>
using vec = std::vector<T>;
template<typename T,typename U>
using umap = std::unordered_map<T,U>;
template<typename T, typename U>
using pair = std::pair<T,U>;
template<typename T>
using list = std::list<T>;

// Clusters the points based on distance to the nearest centroid in centers.
void ClusterPoints(
    umap<unsigned, vec<Vec2>> & clusters, 
    vec<Vec2> & points, 
    vec<Vec2> & centers
);
// Moves the old centroids to the center of each's new cluster
void ShiftCenters(
    vec<Vec2> & newC,
    umap<unsigned, vec<Vec2>> & clusters
);
// Determines if the previous and current centroids have moved.
bool HasConverged(
    vec<Vec2> & oldC,
    vec<Vec2> & newC,
    double epsilon
);
// Driver for the clustering.
void KMeansCluster(
    vec<Vec2> & points,
    unsigned K,
    double epsilon,
    vec<Vec2> & centers,
    umap<unsigned, vec<Vec2>> & clusters
);
// Generates initial points based on local densities.
// An unfinished attempt to generate non-random looking random points.
void GeneratePoints(
    vec<Vec2> & points,
    vec<pair<Vec2,double>> & densities,
    unsigned n_nearest,
    double mapwidth,
    double mapheight,
    unsigned cellswide,
    unsigned cellshigh
);


int main(int argc, char** argv) {

    std::random_device rd;
    std::mt19937_64 rng(rd());
    std::uniform_real_distribution<double> xDist(0,1000.0);
    std::uniform_real_distribution<double> yDist(0,1000.0);
    std::uniform_real_distribution<double> gDist(0.8,0.9);
    std::uniform_real_distribution<double> dDist1(0.0,3.0);
    std::uniform_real_distribution<double> dDist2(0.0,40.0);

    vec<Vec2> points;
    vec<double> gravities;

    unsigned num_densities1 = 10;
    unsigned num_densities2 = 50;
    vec<pair<Vec2,double>> densities1;
    vec<pair<Vec2,double>> densities2;

    for(unsigned i = 0; i < num_densities1; i++) {
        Vec2 p(xDist(rng),yDist(rng));
        double d = dDist1(rng);
        std::uniform_real_distribution<double> dDistA(0.0,dDist2(rng));
        std::uniform_real_distribution<double> dDistB(0.0,dDistA(rng));
        densities1.push_back(std::make_pair(p,dDistB(rng)));
    }
    for(unsigned i = 0; i < num_densities2; i++) {
        Vec2 p(xDist(rng),yDist(rng));
        std::uniform_real_distribution<double> dDistA(0.0,dDist2(rng));
        std::uniform_real_distribution<double> dDistB(0.0,dDistA(rng));
        std::uniform_real_distribution<double> dDistC(0.0,dDistB(rng));
        densities2.push_back(std::make_pair(p,dDistC(rng)));
    }
    
    GeneratePoints(points,densities1,2,1000.0,1000.0,5,5);
    GeneratePoints(points,densities2,1,1000.0,1000.0,15,15);

    vec<Vec2> centers;
    umap<unsigned, vec<Vec2>> clusters;
    KMeansCluster(points,40,0.000000001,centers,clusters);

    std::ofstream ofs("data");
    for(umap<unsigned,vec<Vec2>>::iterator it = clusters.begin(); it != clusters.end(); it++) {
        ofs << "# " << it->first << std::endl;
        for(unsigned i = 0; i < it->second.size(); i++) {
            ofs << it->second[i] << std::endl;
        }
    }
}

void ClusterPoints(umap<unsigned, vec<Vec2>> & clusters, vec<Vec2> & points, vec<Vec2> & centers) {
    for(vec<Vec2>::iterator it = points.begin(); it != points.end(); it++) {
        double min_norm = std::numeric_limits<double>::max();
        unsigned min_index = 0;
        for(unsigned i = 0; i < centers.size(); i++) {
            double x_diff_sq = std::pow(it->X()-centers[i].X(),2.0);
            double y_diff_sq = std::pow(it->Y()-centers[i].Y(),2.0);
            double sumsqrt = std::sqrt(x_diff_sq+y_diff_sq);
            if(sumsqrt < min_norm) {
                min_norm = sumsqrt;
                min_index = i;
            }
        }
        try {
            clusters.at(min_index).push_back(*it);
        } catch(std::out_of_range e) {
            vec<Vec2> v;
            v.push_back(*it);
            clusters.emplace(min_index,v);
        }
    }
}

void ShiftCenters(
    vec<Vec2> & newC,
    umap<unsigned, vec<Vec2>> & clusters
) {
    vec<unsigned> cluster_keys;
    cluster_keys.reserve(clusters.size());
    for(umap<unsigned,vec<Vec2>>::iterator it = clusters.begin(); it != clusters.end(); it++) {
        cluster_keys.push_back(it->first);
    }
    std::sort(cluster_keys.begin(),cluster_keys.end());
    for(vec<unsigned>::iterator it = cluster_keys.begin(); it != cluster_keys.end(); it++) {
        Vec2 new_cluster;
        for(vec<Vec2>::iterator pit = clusters.at(*it).begin(); pit != clusters.at(*it).end(); pit++) {
            new_cluster+=(*pit);
        }
        new_cluster /= static_cast<double>(clusters.at(*it).size());
        newC.push_back(new_cluster);
    }
}
bool HasConverged(
    vec<Vec2> & oldC,
    vec<Vec2> & newC,
    double epsilon
) {
    bool has_converged = true;
    if(oldC.size()==newC.size()) {
        for(unsigned i = 0; i < oldC.size(); i++) {
            if(oldC[i] != newC[i]) {
                has_converged=false;
            }
        }
    } else {
        std::cout << "HasConverged: old and new center vectors are of different sizes.\n";
        has_converged=true;
    }
    return has_converged;
}

void KMeansCluster(
    vec<Vec2> & points,
    unsigned K,
    double epsilon,
    vec<Vec2> & centers,
    umap<unsigned, vec<Vec2>> & clusters

) {

    // Select K points from the vector of points equally positioned within
    // the list. Not a random selection, but should be just as good.
    unsigned jump = points.size() / K;
    vec<Vec2> old_centers;
    vec<Vec2> new_centers;
    for(unsigned i = 0, j=0; i < K; i++,j+=jump) {
        old_centers.push_back(points[j]);
        new_centers.push_back(points[j+1]);
    }
    int count = 0;
    while(!HasConverged(old_centers,new_centers,epsilon)) {

        old_centers = std::move(new_centers);
        new_centers.clear();

        clusters.clear();

        ClusterPoints(clusters,points,old_centers);

        ShiftCenters(new_centers,clusters);

    }
    centers = std::move(new_centers);
}


void GeneratePoints(
    vec<Vec2> & points,
    vec<pair<Vec2,double>> & densities,
    unsigned n_nearest,
    double mapwidth,
    double mapheight,
    unsigned cellswide,
    unsigned cellshigh
) {

    std::random_device rd;
    std::mt19937_64 rng(rd());
    
    
    std::uniform_real_distribution<double> xDist(0.0,mapwidth);
    std::uniform_real_distribution<double> yDist(0.0,mapheight);

    

    double cell_w = mapwidth / cellswide;
    double cell_h = mapheight / cellshigh;

    for(unsigned X = 0; X < cellswide; X++) {
        for(unsigned Y = 0; Y < cellshigh; Y++) {
            double r = X * cell_w;
            double t = Y * cell_h;

            Vec2 cell_center(r + cell_w/2,t + cell_h/2);

            list<double> mins;
            list<unsigned> min_indices;
            for(int i = 0; i < n_nearest; i++) {
                mins.push_back(std::numeric_limits<double>::max());
                min_indices.push_back(0);
            }
            for(unsigned i = 0; i < densities.size(); i++) {
                double cur = densities[i].first.Distance(cell_center);
                list<double>::iterator it = mins.begin();
                list<unsigned>::iterator iit = min_indices.begin();
                for(; it != mins.end(); it++,iit++) {
                    if(cur < *it) {
                        mins.insert(it,cur); mins.pop_back();
                        min_indices.insert(iit,i); min_indices.pop_back();
                        break;
                    }
                }
            }

            double den = 0.0;
            for(list<unsigned>::iterator it = min_indices.begin(); it != min_indices.end(); it++) {
                den += densities[*it].second;
            }
            //den /= n_nearest;

            std::poisson_distribution<int> pointsDist(den);
            int num_points = pointsDist(rng);

            std::uniform_real_distribution<double> cellXDist(r,r+cell_w);
            std::uniform_real_distribution<double> cellYDist(t,t+cell_h);
            for(int i = 0; i < num_points; i++) {
                points.push_back(Vec2(cellXDist(rng),cellYDist(rng)));
            }
            
        }
    }


}