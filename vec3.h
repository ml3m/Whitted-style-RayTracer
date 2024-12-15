#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

using std::sqrt;

class vec3 {
    public:
        // Default constructor init the vector to (0, 0, 0)
        vec3() : e{0,0,0} {}

        // Constructor init the vector with specified 
        // x, y, z components
        vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

        double x() const { return e[0]; } // Get the x-component
        double y() const { return e[1]; } // Get the y-component
        double z() const { return e[2]; } // Get the z-component

        // Negation operator to create 
        // a vector pointing in the opposite direction
        vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }

        // Access components using the subscript operator (read-only) and (modifiable)
        double operator[](int i) const { return e[i]; }
        double& operator[](int i) { return e[i]; }

        // Vector addition in-place: adds another vector to this one
        vec3& operator+=(const vec3 &v) {
            e[0] += v.e[0]; // add the x-components
            e[1] += v.e[1]; // add the y-components
            e[2] += v.e[2]; // add the z-components
            return *this;
        }

        // Scalar multiplication in-place: scales the vector by a factor t
        vec3& operator*=(const double t) {
            e[0] *= t; // scale the x-component
            e[1] *= t; // scale the y-component
            e[2] *= t; // scale the z-component
            return *this;
        }

        // Scalar division in-place: scales the vector by 1/t
        vec3& operator/=(const double t) {
            return *this *= 1/t; // division is implemented as scaling by the reciprocal
        }

        // Computes the Euclidean length (magnitude) of the vector
        double length() const {
            // √(x² + y² + z²) - Standard formula for vector magnitude
            return sqrt(length_squared());
        }

        double length_squared() const {
            // x² + y² + z²
            return e[0] * e[0] +
                   e[1] * e[1] +
                   e[2] * e[2];
        }

    public:
        double e[3]; // components of the vector: x, y, z
};

#endif
