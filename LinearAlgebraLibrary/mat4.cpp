/**************************************************************************//**
* @file      mat4.c
* @brief     Implement a 4x4 matrix class to be used in
*            3D graphics transformations.
* @author    Adi
* @date      2024-1-28

******************************************************************************/

/******************************************************************************
* Includes
******************************************************************************/
#include <math.h>
#include "mat4.h"
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
/// Default Constructor.  Initialize to identity matrix.
mat4::mat4()
    : mat4(1.0)
{}

/// Initializes the diagonal values of the matrix to diag. All other values are 0.
mat4::mat4(float diag)
    : mat4(vec4(diag, 0, 0, 0),
           vec4(0, diag, 0, 0),
           vec4(0, 0, diag, 0),
           vec4(0, 0, 0, diag))
{}

/// Initializes matrix with each vector representing a column in the matrix
mat4::mat4(const vec4 &col0, const vec4 &col1, const vec4 &col2, const vec4& col3) {
    this->data[0] = col0;
    this->data[1] = col1;
    this->data[2] = col2;
    this->data[3] = col3;
}

mat4::mat4(const mat4 &m2)
    : data(m2.data)
{}    // copy constructor

///----------------------------------------------------------------------
/// Getters
///----------------------------------------------------------------------
/// Returns the values of the column at the index
vec4  mat4::operator[](unsigned int index) const {
    if(index > 3) {
        throw std::out_of_range ("Index Out of Range!");
    }
    return this->data[index];
}

/// Returns a reference to the column at the index
vec4& mat4::operator[](unsigned int index){
    if(index > 3) {
        throw std::out_of_range ("Index Out of Range!");
    }
    return this->data[index];
}

///----------------------------------------------------------------------
/// Static Initializers
///----------------------------------------------------------------------
/// Creates a 3-D rotation matrix.
/// Takes an angle in degrees and an axis represented by its xyz components, and outputs a 4x4 rotation matrix
/// Use Rodrigues' formula to implement this method
mat4 mat4::rotate(float angle, float x, float y, float z) {
    vec4 v1(x, y, z, 1);
    // mat4 rot();
    float normal = length(v1);
    // Normalize vector
    v1 /= normal;

    float radians = ((angle / 180) * M_PI);

    vec4 c0((cos(radians) + ((v1[0] * v1[0]) * (1 - cos(radians)))), // cosθ + (wx^2 * (1 - cosθ))
            ((v1[2] * sin(radians)) + ((v1[0] * v1[1]) * (1 - cos(radians)))), // (wz * sinθ) + (wx * wy * (1 - cosθ))
            ((-1 * v1[1] * sin(radians)) + ((v1[0] * v1[2]) * (1 - cos(radians)))), // (-wy * sinθ) + (wx * wz * (1 - cosθ))
            0);
    vec4 c1(((-1 * v1[2] * sin(radians)) + ((v1[0] * v1[1]) * (1 - cos(radians)))), // (-wz * sinθ) + (wx * wy * (1 - cosθ))
            (cos(radians) + (v1[1] * v1[1]) * (1 - cos(radians))), // cosθ + wy^2 * (1 - cosθ)
            ((v1[0] * sin(radians)) + ((v1[1] * v1[2]) * (1 - cos(radians)))), // (wx * sinθ) + (wy * wz * (1 - cosθ))
            0);
    vec4 c2(((v1[1] * sin(radians)) + ((v1[0] * v1[2]) * (1 - cos(radians)))),
            ((-1 * v1[0] * sin(radians)) + ((v1[1] * v1[2]) * (1 - cos(radians)))),
            (cos(radians) + (v1[2] * v1[2]) * (1 - cos(radians))),
            0);
    vec4 c3(0,
            0,
            0,
            1);

    return mat4(c0, c1, c2, c3);
}

/// Takes an xyz displacement and outputs a 4x4 translation matrix
mat4 translate(float x, float y, float z) {
    vec4 c0(1,
            0,
            0,
            0);
    vec4 c1(0,
            1,
            0,
            0);
    vec4 c2(0,
            0,
            1,
            0);
    vec4 c3(x,
            y,
            z,
            1);

    return mat4(c0, c1, c2, c3);
}

/// Takes an xyz scale and outputs a 4x4 scale matrix
mat4 scale(float x, float y, float z) {
    vec4 c0(x,
            0,
            0,
            0);
    vec4 c1(0,
            y,
            0,
            0);
    vec4 c2(0,
            0,
            z,
            0);
    vec4 c3(0,
            0,
            0,
            1);

    return mat4(c0, c1, c2, c3);
}

/// Generates a 4x4 identity matrix
mat4 identity() {
    return mat4();
}

///----------------------------------------------------------------------
/// Operator Functions
///----------------------------------------------------------------------

/// Assign m2's data to this's data and return this
mat4& mat4::operator=(const mat4 &m2) {
    this->data[0] = m2.data[0];
    this->data[1] = m2.data[1];
    this->data[2] = m2.data[2];
    this->data[3] = m2.data[3];
    return *this;
}

/// Test for equality
bool mat4::operator==(const mat4 &m2) const {
    return (data[0] == m2.data[0] &&
            data[1] == m2.data[1] &&
            data[2] == m2.data[2] &&
            data[3] == m2.data[3]);
}

/// Test for inequality
bool mat4::operator!=(const mat4 &m2) const {
    return (data[0] != m2.data[0] ||
            data[1] != m2.data[1] ||
            data[2] != m2.data[2] ||
            data[3] != m2.data[3]);
}

/// Element-wise arithmetic
/// e.g. += adds the elements of m2 to this and returns this (like regular +=)
///      +  returns a new matrix whose elements are the sums of this and v2
mat4& mat4::operator+=(const mat4 &m2) {
    data[0] += m2.data[0];
    data[1] += m2.data[1];
    data[2] += m2.data[2];
    data[3] += m2.data[3];
    return *this;
}
mat4& mat4::operator-=(const mat4 &m2) {
    data[0] -= m2.data[0];
    data[1] -= m2.data[1];
    data[2] -= m2.data[2];
    data[3] -= m2.data[3];
    return *this;
}
mat4& mat4::operator*=(float c) {
    data[0] *= c;
    data[1] *= c;
    data[2] *= c;
    data[3] *= c;
    return *this;
}                // multiplication by a scalar
mat4& mat4::operator/=(float c) {
    data[0] /= c;
    data[1] /= c;
    data[2] /= c;
    data[3] /= c;
    return *this;
}                 // division by a scalar
mat4  mat4::operator+(const mat4 &m2) const {
    return mat4(this->data[0] + m2.data[0],
                this->data[1] + m2.data[1],
                this->data[2] + m2.data[2],
                this->data[3] + m2.data[3]);
}
mat4  mat4::operator-(const mat4 &m2) const {
    return mat4(this->data[0] - m2.data[0],
                this->data[1] - m2.data[1],
                this->data[2] - m2.data[2],
                this->data[3] - m2.data[3]);
}
mat4  mat4::operator*(float c) const {
    return mat4(this->data[0] * c,
                this->data[1] * c,
                this->data[2] * c,
                this->data[3] * c);
}             // multiplication by a scalar
mat4  mat4::operator/(float c) const {
    return mat4(this->data[0] / c,
                this->data[1] / c,
                this->data[2] / c,
                this->data[3] / c);
}             // division by a scalar

/// Matrix multiplication (m1 * m2)
mat4 mat4::operator*(const mat4 &m2) const {
    mat4 m(transpose(*this));
    return mat4(vec4(dot(m[0], m2[0]),
                     dot(m[1], m2[0]),
                     dot(m[2], m2[0]),
                     dot(m[3], m2[0])),
                vec4(dot(m[0], m2[1]),
                     dot(m[1], m2[1]),
                     dot(m[2], m2[1]),
                     dot(m[3], m2[1])),
                vec4(dot(m[0], m2[2]),
                     dot(m[1], m2[2]),
                     dot(m[2], m2[2]),
                     dot(m[3], m2[2])),
                vec4(dot(m[0], m2[3]),
                     dot(m[1], m2[3]),
                     dot(m[2], m2[3]),
                     dot(m[3], m2[3])));
}

/// Matrix/vector multiplication (m * v)
/// Assume v is a column vector (ie. a 4x1 matrix)
vec4 mat4::operator*(const vec4 &v) const {
    mat4 m(transpose(*this));
    return vec4(dot(m[0], v),
                dot(m[1], v),
                dot(m[2], v),
                dot(m[3], v));
}

///----------------------------------------------------------------------
/// Matrix Operations
///----------------------------------------------------------------------
/// Returns the transpose of the input matrix (v_ij == v_ji)
mat4 transpose(const mat4 &m) {
    return mat4(vec4(m[0][0], m[1][0], m[2][0], m[3][0]),
                vec4(m[0][1], m[1][1], m[2][1], m[3][1]),
                vec4(m[0][2], m[1][2], m[2][2], m[3][2]),
                vec4(m[0][3], m[1][3], m[2][3], m[3][3]));
}

/// Returns a row of the input matrix
vec4 row(const mat4 &m, unsigned int index) {
    if(index > 3) {
        throw std::out_of_range ("Index Out of Range!");
    }
    return vec4(m[0][index], m[1][index], m[2][index], m[3][index]);
}

/// Scalar multiplication (c * m)
mat4 operator*(float c, const mat4 &m) {
    return m * c;
}

/// Vector/matrix multiplication (v * m)
/// Assume v is a row vector (ie. a 1x4 matrix)
vec4 operator*(const vec4 &v, const mat4 &m) {
    return vec4(dot(v, m[0]),
                dot(v, m[1]),
                dot(v, m[2]),
                dot(v, m[3]));
}

/// Prints the matrix to a stream in a nice format
std::ostream &operator<<(std::ostream &o, const mat4 &m) {
    o<<"Row1 = \t"<<m[0][0]<<" "<<m[1][0]<<" "<<m[2][0]<<" "<<m[3][0]<<std::endl;
    o<<"Row2 = \t"<<m[0][1]<<" "<<m[1][1]<<" "<<m[2][1]<<" "<<m[3][1]<<std::endl;
    o<<"Row3 = \t"<<m[0][2]<<" "<<m[1][2]<<" "<<m[2][2]<<" "<<m[3][2]<<std::endl;
    o<<"Row4 = \t"<<m[0][3]<<" "<<m[1][3]<<" "<<m[2][3]<<" "<<m[3][3]<<std::endl;
    return o;
}
