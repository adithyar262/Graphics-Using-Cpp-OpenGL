/**************************************************************************//**
* @file      vec4.c
* @brief     Implement a four-dimensional vector class to be used in
*            3D graphics transformations. The orientation of the vector
*            is context-dependent, but it is better to generally think of
*            it as a column vector.
* @author    Adi
* @date      2024-1-27

******************************************************************************/

/******************************************************************************
* Includes
******************************************************************************/
#include <math.h>
#include "vec4.h"
/******************************************************************************
* Defines
******************************************************************************/

/******************************************************************************
* Variables
******************************************************************************/

/******************************************************************************
* Function Prototypes
******************************************************************************/

/******************************************************************************
* Function Implementation
******************************************************************************/

///----------------------------------------------------------------------
/// Constructors
///----------------------------------------------------------------------
// Default constructor. Initializes all elements to 0.
vec4::vec4()
    : vec4(0, 0, 0, 0)
{}

vec4::vec4(float x, float y, float z, float w) {
    data[0] = x;
    data[1] = y;
    data[2] = z;
    data[3] = w;
}

vec4::vec4(const vec4 &v2)
    : vec4(v2.data[0], v2.data[1], v2.data[2], v2.data[3])
{}

///----------------------------------------------------------------------
/// Getters/Setters
///----------------------------------------------------------------------
/// Returns the value at index
float vec4::operator[](unsigned int index) const {
    if(index > 3) {
        throw std::out_of_range ("Index Out of Range!");
    }
    return this->data[index];
}

/// Returns a reference to the value at index
float& vec4::operator[](unsigned int index) {
    if(index > 3) {
        throw std::out_of_range ("Index Out of Range!");
    }
    return this->data[index];
}

///----------------------------------------------------------------------
/// Operator Functions
///----------------------------------------------------------------------

/// Sets each element of this->data to v2's corresponding data, then returns this
vec4& vec4::operator=(const vec4 &v2) {
    data[0] = v2.data[0];
    data[1] = v2.data[1];
    data[2] = v2.data[2];
    data[3] = v2.data[3];
    return *this;
}

/// Test for equality
bool vec4::operator==(const vec4 &v2) const {
    return (data[0] == v2.data[0] &&
            data[1] == v2.data[1] &&
            data[2] == v2.data[2] &&
            data[3] == v2.data[3]);
}	   //Component-wise comparison

/// Test for inequality
bool vec4::operator!=(const vec4 &v2) const {
    return (data[0] != v2.data[0] ||
            data[1] != v2.data[1] ||
            data[2] != v2.data[2] ||
            data[3] != v2.data[3]);
}	   //Component-wise comparison

/// Arithmetic:
/// e.g. += adds v2 to this and return this (like regular +=)
///      +  returns a new vector that is sum of this and v2
vec4& vec4::operator+=(const vec4 &v2) {
    data[0] += v2.data[0];
    data[1] += v2.data[1];
    data[2] += v2.data[2];
    data[3] += v2.data[3];
    return *this;
}
vec4& vec4::operator-=(const vec4 &v2) {
    data[0] -= v2.data[0];
    data[1] -= v2.data[1];
    data[2] -= v2.data[2];
    data[3] -= v2.data[3];
    return *this;
}
vec4& vec4::operator*=(float c) {
    data[0] *= c;
    data[1] *= c;
    data[2] *= c;
    data[3] *= c;
    return *this;
}                 // multiplication by a scalar
vec4& vec4::operator/=(float c) {
    data[0] /= c;
    data[1] /= c;
    data[2] /= c;
    data[3] /= c;
    return *this;
}                 // division by a scalar

vec4  vec4::operator+(const vec4 &v2) const {
    return vec4(this->data[0] + v2.data[0],
                this->data[1] + v2.data[1],
                this->data[2] + v2.data[2],
                this->data[3] + v2.data[3]);
}

vec4  vec4::operator-(const vec4 &v2) const {
    return vec4(this->data[0] - v2.data[0],
                this->data[1] - v2.data[1],
                this->data[2] - v2.data[2],
                this->data[3] - v2.data[3]);
}

vec4  vec4::operator*(float c) const {
    return vec4(this->data[0] * c,
                this->data[1] * c,
                this->data[2] * c,
                this->data[3] * c);
}             // multiplication by a scalar
vec4  vec4::operator/(float c) const {
    return vec4(this->data[0] / c,
                this->data[1] / c,
                this->data[2] / c,
                this->data[3] / c);
}             // division by a scalar

/// Dot Product
float dot(const vec4 &v1, const vec4 &v2) {
    return (v1[0] * v2[0] +
            v1[1] * v2[1] +
            v1[2] * v2[2] +
            v1[3] * v2[3]);
}

/// Cross Product
//Compute the result of v1 x v2 using only their X, Y, and Z elements.
//In other words, treat v1 and v2 as 3D vectors, not 4D vectors.
//The fourth element of the resultant vector should be 0.
vec4 cross(const vec4 &v1, const vec4 &v2) {
    vec4 v3;
    v3[0] = (v1[1] * v2[2]) - (v1[2] * v2[1]);
    v3[1] = (v1[2] * v2[0]) - (v1[0] * v2[2]);
    v3[2] = (v1[0] * v2[1]) - (v1[1] * v2[0]);
    v3[3] = 0;
    return v3;
}

/// Returns the geometric length of the input vector
float length(const vec4 &v) {
    return (sqrt((v[0] * v[0]) +
                (v[1] * v[1]) +
                (v[2] * v[2]) +
                (v[3] * v[3])));
}

/// Scalar Multiplication (c * v)
vec4 operator*(float c, const vec4 &v) {
    return v * c;
}

vec4 normalize(const vec4& v) {
    return v / length(v);
}

/// Prints the vector to a stream in a nice format
std::ostream &operator<<(std::ostream &o, const vec4 &v) {
    o<<"\tx = "<<v[0]<<std::endl;
    o<<"\ty = "<<v[1]<<std::endl;
    o<<"\tz = "<<v[2]<<std::endl;
    o<<"\tw = "<<v[3]<<std::endl;
    return o;
}
