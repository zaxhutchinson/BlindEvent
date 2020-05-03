#include"fcm.hpp"

FCM::FCM() 
    : num_clusters(0), fuzzifier(2.0), epsilon(1.0), sa(0.0), sa_rounds(0)
{}

FCM::FCM(
    long rand_seed,
    int _num_clusters, 
    double _fuzzifier,
    double _epsilon,
    double _sa,
    int _sa_rounds
) : num_clusters(_num_clusters), fuzzifier(_fuzzifier), epsilon(_epsilon),
    sa(_sa), sa_rounds(_sa_rounds) {

    // Init the random number generator
    rng = std::mt19937_64(rand_seed);
}

void FCM::Init(int _K, std::vector<VecK> & pts) {
    K = _K;
    num_points = pts.size();

    // Build the min/max with dummy values.
    for(int k = 0; k < K; k++) {
        mins.push_back(std::numeric_limits<double>::max());
        maxs.push_back(std::numeric_limits<double>::min());
    }

    // Find the actual min/maxes
    for(std::size_t i = 0; i < pts.size(); i++) {
        for(int k = 0; k < K; k++) {
            if(pts[i].Data(k) < mins[k]) mins[k]=pts[i].Data(k);
            else if(pts[i].Data(k) > maxs[k]) maxs[k]=pts[i].Data(k);
        }
    }

    // Create initial centers
    for(int i = 0; i < num_clusters; i++) {
        VecK veck = RandomPoint();
        centers.push_back(std::move(veck));
    }

    // Create the membership matrix with random values.
    std::uniform_real_distribution<double> dist(0.0,1.0);
    for(int i = 0; i < num_clusters; i++) {
        std::vector<double> vec;
        for(int m = 0; m < num_points; m++) {
            vec.push_back(dist(rng));
        }
        memmatrix.push_back(std::move(vec));
    }
}

VecK FCM::RandomPoint() {
    VecK veck(K);
    for(int k = 0; k < K; k++) {
        std::uniform_real_distribution<double> dist(mins[k],maxs[k]);
        veck.Data(k,dist(rng));
    }
    return veck;
}

void FCM::Run(int _K, std::vector<VecK> & pts) {

    // Initialize
    Init(_K,pts);

    // Simulated Annealing
    double sa_delta = 0.0;
    if(sa_rounds > 0) {
        sa_delta = sa / sa_rounds;
    }
    double cur_sa = sa;
    std::uniform_real_distribution<double> saDist(0.0,1.0);

    // The FCM.
    double E = epsilon;
    while(E >= epsilon) {
        
        // Reset E
        E = 0.0;

        // Calculate new centers
        for(int i = 0; i < num_clusters; i++) {

            // If simulated annealing happens this step....
            if(saDist(rng) < cur_sa) {
                VecK veck = RandomPoint();
                double ep = veck.Distance(centers[i]);
                if(ep > E) E = ep;
                centers[i]=std::move(veck);
            }
            // Else just shift as regular. 
            else {
                double bot = 0.0;
                VecK top(K);
                for(int m = 0; m < num_points; m++) {
                    double mem = std::pow(memmatrix[i][m], fuzzifier);
                    bot += mem;
                    top += pts[m]*mem;
                }
                top/=bot;

                double ep = top.Distance(centers[i]);
                if(ep > E) E = ep;
                centers[i]=std::move(top);
            }
        }

        // Update membership matrix
        for(int i = 0; i < num_points; i++) {
            for(int k = 0; k < num_clusters; k++) {
                Membership(i,k,pts);
            }
        }

        // Move sa toward zero if it isn't already there.
        if(cur_sa > 0.0) cur_sa-=sa_delta;
    }
}

void FCM::Membership(int point_index, int center_index, std::vector<VecK> & pts) {
    double sum = 0.0;
    for(int i =0; i < num_clusters; i++) {
        sum += pts[point_index].Distance(centers[center_index]) /
                pts[point_index].Distance(centers[i]);
    }
    memmatrix[center_index][point_index] = 1.0 / std::pow(sum,2/(fuzzifier-1));
}

std::vector<std::vector<double>> & FCM::GetMembershipMatrix() {
    return memmatrix;
}

std::vector<VecK> & FCM::GetCenters() {
    return centers;
}