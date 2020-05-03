// VecK
// Code by: Zachary Hutchinson
//
// A struct facilitating the use of a K dimensional vector.

#pragma once

#include<utility>
#include<cmath>
#include<iostream>
#include<vector>

struct VecK {
    std::vector<double> v;

    VecK();
    VecK(int k);
    VecK(std::vector<double> & _v);
    VecK(const VecK & other);
    VecK(VecK && other);
    VecK& operator=(VecK && other);
    VecK& operator+=(const VecK &other);
    VecK& operator-=(const VecK &other);
    const VecK operator+(const VecK &other) const;
    const VecK operator-(const VecK &other) const;
    const VecK operator*(double o) const;
    const VecK operator/(double o) const;
    VecK& operator/=(double div);
    VecK& operator*=(double mul);
    friend std::ostream &operator<<(std::ostream & output, const VecK &vec);
    bool operator==(const VecK & other);
    bool operator!=(const VecK & other);

    int K();
    double Data(int index);
    void Data(int index, double data);
    double Distance(const VecK & other);
};