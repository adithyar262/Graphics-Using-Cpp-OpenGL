// Base code written by Jan Allbeck, Chris Czyzewicz, Cory Boatright, Tiantian Liu, Benedict Brown, and Adam Mally
// University of Pennsylvania

// At least some C++ compilers do funny things
// to C's math.h header if you don't define this
#define _USE_MATH_DEFINES

#include "vec4.h"
#include "mat4.h"
#include <iostream>
using namespace std;

void vec4Tests() {
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "              Vec4 Tests               " << std::endl;
    std::cout << "--------------------------------------" << std::endl << std::endl;

    std::cout << "---------------" << std::endl;
    std::cout << "1. Constructors" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::cout << "Default constructor - vec4()"<< std::endl ;
    vec4 v1;
    cout<<v1;

    std::cout << "Parameterized constructor - vec4(1.2, 2.3, 3.4, 4.5)"<< std::endl ;
    vec4 v2(1.2, 2.3, 3.4, 4.5);
    cout<<v2;

    std::cout << "Copy constructor - vec4(const vec4 &v2)"<< std::endl ;
    vec4 v3(v2);
    cout<<v3;

    std::cout << "---------------" << std::endl;
    std::cout << "2. Operator<<" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::cout << "Printing all 3 vectors using <<"<< std::endl ;
    cout<<"Vector v1 - \n"<<v1<<std::endl;
    cout<<"Vector v2 - \n"<<v2<<std::endl;
    cout<<"Vector v3 - \n"<<v3<<std::endl;

    std::cout << "---------------" << std::endl;
    std::cout << "3. Operator=" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::cout << "Assigning v1 to v3"<< std::endl ;
    v3 = v1;
    cout<<"Updated Vector v3 - \n"<<v3<<std::endl;

    std::cout << "---------------" << std::endl;
    std::cout << "4. Operator==" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::cout << "Comparing v1 and v2"<< std::endl ;
    if(v1 == v2)
        std::cout << "Identical Vectors!" <<std::endl;
    else
        std::cout << "Unidentical Vectors!" <<std::endl;

    std::cout <<std::endl<< "Comparing v2 and v3"<< std::endl ;
    if(v2 == v3)
        std::cout << "Identical Vectors!" <<std::endl;
    else
        std::cout << "Unidentical Vectors!" <<std::endl;

    std::cout <<std::endl<< "Comparing v1 and v3"<< std::endl ;
    if(v1 == v3)
        std::cout << "Identical Vectors!" <<std::endl;
    else
        std::cout << "Unidentical Vectors!" <<std::endl;


    std::cout << "---------------" << std::endl;
    std::cout << "5. Operator!=" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::cout << "Comparing v1 and v2"<< std::endl ;
    if(v1 != v2)
        std::cout << "Unidentical Vectors!" <<std::endl;
    else
        std::cout << "Identical Vectors!" <<std::endl;

    std::cout <<std::endl<< "Comparing v2 and v3"<< std::endl ;
    if(v2 != v3)
        std::cout << "Unidentical Vectors!" <<std::endl;
    else
        std::cout << "Identical Vectors!" <<std::endl;

    std::cout <<std::endl<< "Comparing v1 and v3"<< std::endl ;
    if(v1 != v3)
        std::cout << "Unidentical Vectors!" <<std::endl;
    else
        std::cout << "Identical Vectors!" <<std::endl;

    std::cout << "---------------" << std::endl;
    std::cout << "6. Dot Product" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::cout <<std::endl<< "Reassigning vectors v1, v2, v3"<< std::endl ;
    for(int i = 0; i < 4; ++i) {
        v1[i] = i + 1;
        v2[i] = i + 2;
        v3[i] = i + 3;
    }
    cout<<"Vector v1 - \n"<<v1<<std::endl;
    cout<<"Vector v2 - \n"<<v2<<std::endl;
    cout<<"Vector v3 - \n"<<v3<<std::endl;

    std::cout<<std::endl<<"Dot product of v1 and v2 - "<<dot(v1, v2)<<std::endl;
    std::cout<<std::endl<<"Dot product of v2 and v3 - "<<dot(v2, v3)<<std::endl;
    std::cout<<std::endl<<"Dot product of v3 and v1 - "<<dot(v3, v1)<<std::endl;

    std::cout << "---------------" << std::endl;
    std::cout << "7. Cross Product" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::cout<<std::endl<<"Cross product of v1 and v2 -\n"<<cross(v1, v2)<<std::endl;
    std::cout<<std::endl<<"Cross product of v2 and v3 -\n"<<cross(v2, v3)<<std::endl;
    std::cout<<std::endl<<"Cross product of v3 and v1 -\n"<<cross(v3, v1)<<std::endl;

    std::cout << "---------------" << std::endl;
    std::cout << "8. Normalize" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::cout<<std::endl<<"Length v1 - "<<length(v1)<<", Normalized vector v1 -\n"<<normalize(v1)<<std::endl;
    std::cout<<std::endl<<"Length v1 - "<<length(v2)<<", Normalized vector v2 -\n"<<normalize(v2)<<std::endl;
    std::cout<<std::endl<<"Length v1 - "<<length(v3)<<", Normalized vector v3 -\n"<<normalize(v3)<<std::endl;

    return;
}

void mat4Tests() {
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "              Mat4 Tests              " << std::endl;
    std::cout << "--------------------------------------" << std::endl;

    vec4 v1(1, 2, 3, 4);
    vec4 v2(5, 6, 7, 8);
    vec4 v3(9, 10, 11, 12);
    vec4 v4(13, 14, 15, 16);

    std::cout << "---------------" << std::endl;
    std::cout << "1. Constructors" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;
    // /// Default Constructor.  Initialize to identity matrix.
    // mat4();

    // /// Initializes the diagonal values of the matrix to diag. All other values are 0.
    // mat4(float diag);

    // /// Initializes matrix with each vector representing a column in the matrix
    // mat4(const vec4 &col0, const vec4 &col1, const vec4 &col2, const vec4& col3);

    // mat4(const mat4 &m2); // copy constructor
    std::cout << "Default constructor - mat4()"<< std::endl ;
    mat4 m1;
    cout<<m1;

    std::cout << "Parameterized constructor - mat4(float diag)"<< std::endl ;
    mat4 m2(3.14);
    cout<<m2;

    std::cout << "Parameterized constructor - mat4(const vec4 &col0, const vec4 &col1, const vec4 &col2, const vec4& col3)"<< std::endl ;
    mat4 m3(v1, v2, v3, v4);
    cout<<m3;

    std::cout << "Copy constructor - mat4(const mat4 &m2)"<< std::endl ;
    mat4 m4(m2);
    cout<<m4;

    std::cout << "---------------" << std::endl;
    std::cout << "2. Operator<<" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::cout << "Printing all 3 vectors using <<"<< std::endl ;
    cout<<"Matrix m1 - \n"<<m1<<std::endl;
    cout<<"Matrix m2 - \n"<<m2<<std::endl;
    cout<<"Matrix m3 - \n"<<m3<<std::endl;
    cout<<"Matrix m4 - \n"<<m4<<std::endl;

    std::cout << "---------------" << std::endl;
    std::cout << "3. Operator=" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::cout << "Assigning m1 to m4"<< std::endl ;
    m4 = m1;
    cout<<"Updated Matrix m4 - \n"<<m4<<std::endl;

    std::cout << "---------------" << std::endl;
    std::cout << "4. Operator==" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::cout << "Comparing m1 and m2"<< std::endl ;
    if(m1 == m2)
        std::cout << "Identical Vectors!" <<std::endl;
    else
        std::cout << "Unidentical Vectors!" <<std::endl;

    std::cout <<std::endl<< "Comparing m2 and m3"<< std::endl ;
    if(m2 == m3)
        std::cout << "Identical Vectors!" <<std::endl;
    else
        std::cout << "Unidentical Vectors!" <<std::endl;

    std::cout <<std::endl<< "Comparing m3 and m4"<< std::endl ;
    if(m3 == m4)
        std::cout << "Identical Vectors!" <<std::endl;
    else
        std::cout << "Unidentical Vectors!" <<std::endl;

    std::cout <<std::endl<< "Comparing m1 and m3"<< std::endl ;
    if(m1 == m4)
        std::cout << "Identical Vectors!" <<std::endl;
    else
        std::cout << "Unidentical Vectors!" <<std::endl;


    std::cout << "---------------" << std::endl;
    std::cout << "5. Operator!=" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::cout << "Comparing m1 and m2"<< std::endl ;
    if(m1 != m2)
        std::cout << "Unidentical Vectors!" <<std::endl;
    else
        std::cout << "Identical Vectors!" <<std::endl;

    std::cout <<std::endl<< "Comparing m2 and m3"<< std::endl ;
    if(m2 != m3)
        std::cout << "Unidentical Vectors!" <<std::endl;
    else
        std::cout << "Identical Vectors!" <<std::endl;

    std::cout <<std::endl<< "Comparing m3 and m4"<< std::endl ;
    if(m3 != m4)
        std::cout << "Unidentical Vectors!" <<std::endl;
    else
        std::cout << "Identical Vectors!" <<std::endl;

    std::cout <<std::endl<< "Comparing m1 and m4"<< std::endl ;
    if(m1 != m4)
        std::cout << "Unidentical Vectors!" <<std::endl;
    else
        std::cout << "Identical Vectors!" <<std::endl;


    std::cout << "---------------" << std::endl;
    std::cout << "6. Rotate" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::cout<<std::endl<<"Rotate m1 by 90° Anticlockwise at (0,0,0,0) - \n"<<m1 * mat4::rotate(90, 0, 0, 0)<<std::endl;

    std::cout << "---------------" << std::endl;
    std::cout << "7. Matrix-matrix multiplication" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::cout<<std::endl<<"m1 * m2 -\n"<<m1 * m2<<std::endl;
    std::cout<<std::endl<<"m2 * m3 -\n"<<m2 * m3<<std::endl;
    std::cout<<std::endl<<"m3 * m4 -\n"<<m3 * m4<<std::endl;
    std::cout<<std::endl<<"m4 * m1 -\n"<<m4 * m1<<std::endl;

    std::cout << "---------------" << std::endl;
    std::cout << "8. Matrix-vector multiplication" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::cout<<std::endl<<"m1 * v1 -\n"<<m1 * v1<<std::endl;
    std::cout<<std::endl<<"m2 * v1 -\n"<<m2 * v1<<std::endl;
    std::cout<<std::endl<<"m3 * v1 -\n"<<m3 * v1<<std::endl;
    std::cout<<std::endl<<"m4 * v1 -\n"<<m4 * v1<<std::endl;

    std::cout << "---------------" << std::endl;
    std::cout << "9. Vector-matrix multiplication" << std::endl;
    std::cout << "---------------" << std::endl << std::endl;

    std::cout<<std::endl<<"v1 * m1 -\n"<<v1 * m1<<std::endl;
    std::cout<<std::endl<<"v1 * m2 -\n"<<v1 * m2<<std::endl;
    std::cout<<std::endl<<"v1 * m3 -\n"<<v1 * m3<<std::endl;
    std::cout<<std::endl<<"v1 * m4 -\n"<<v1 * m4<<std::endl;

    mat4 r0 = mat4::rotate(0, 0, 0, 0);
    std::cout<<std::endl<<"Rotation matrix 0 -\n"<<r0<<std::endl;

    mat4 r1 = mat4::rotate(90, 0, 0, 0);
    std::cout<<std::endl<<"Rotation matrix 1 -\n"<<r1<<std::endl;

    mat4 r2 = mat4::rotate(45, 0, 0, 0);
    std::cout<<std::endl<<"Rotation matrix 2 -\n"<<r2<<std::endl;

    mat4 r3 = mat4::rotate(90, 10, 20, 30);
    std::cout<<std::endl<<"Rotation matrix 3 -\n"<<r3<<std::endl;

    mat4 r4 = mat4::rotate(90, 30, 40, 50);
    std::cout<<std::endl<<"Rotation matrix 4 -\n"<<r4<<std::endl;

    mat4 r5 = mat4::rotate(90, 30, 40, 50);
    std::cout<<std::endl<<"Rotation matrix 5 -\n"<<r5<<std::endl;

    return;
}

int main() {

    vec4Tests();

    mat4Tests();

    return 0;
}
