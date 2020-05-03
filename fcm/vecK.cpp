#include"vecK.hpp"

VecK::VecK()
{}

VecK::VecK(int k) {
    for(int i = 0; i < k; i++) {
        v.push_back(0.0);
    }
}

VecK::VecK(std::vector<double> & _v)
{
    for(std::size_t i = 0; i < _v.size(); i++) {
        v.push_back(_v[i]);
    }
}

VecK::VecK(const VecK &other) 
{
    v = other.v;
}

VecK::VecK(VecK && other) {
    *this = std::move(other);
}

VecK& VecK::operator=(VecK&&other) {
    if(this != &other) {
        v = other.v;
    }
    return *this;
}

VecK& VecK::operator+=(const VecK &other) {
    if(v.size() == other.v.size()) {
        for(std::size_t i = 0; i < v.size(); i++) {
            v[i] += other.v[i];
        }
    }
    return *this;
}
VecK& VecK::operator-=(const VecK &other) {
    if(v.size() == other.v.size()) {
        for(std::size_t i = 0; i < v.size(); i++) {
            v[i] -= other.v[i];
        }
    }
    return *this;
}
const VecK VecK::operator+(const VecK &other) const {
    return VecK(*this) += other;
}
const VecK VecK::operator-(const VecK &other) const {
    return VecK(*this) -= other;
}
const VecK VecK::operator*(double o) const {
    return VecK(*this) *= o;
}
const VecK VecK::operator/(double o) const {
    return VecK(*this) /= o;
}
VecK& VecK::operator/=(double div) {
    for(std::size_t i = 0; i < v.size(); i++) {
        v[i] /= div;
    }
    return *this;
}
VecK& VecK::operator*=(double mul) {
    for(std::size_t i = 0; i < v.size(); i++) {
        v[i] *= mul;
    }
    return *this;
}
std::ostream &operator<<(std::ostream & output, const VecK &vec) {
    for(std::size_t i = 0; i < vec.v.size(); i++) {
        output << vec.v[i];
        if(i < vec.v.size()-1) {
            output << ",";
        }
    }
    return output;
}
bool VecK::operator==(const VecK & other) {
    if(v.size()!=other.v.size()) return false;
    else {
        bool rtn = true;
        for(std::size_t i = 0; i < v.size(); i++) {
            rtn = rtn && v[i]==other.v[i];
        }
        return rtn;
    }
}
bool VecK::operator!=(const VecK & other) {
    return !(*this==other);
}

int VecK::K() {
    return v.size();
}
double VecK::Data(int index) {
    return v[index];
}
void VecK::Data(int index, double data) {
    v[index]=data;
}


double VecK::Distance(const VecK & other) {
    double sum = 0.0;
    for(std::size_t i = 0; i < v.size(); i++) {
        sum += std::pow(other.v[i]-v[i],2.0);
    }
    return std::sqrt(sum);
}