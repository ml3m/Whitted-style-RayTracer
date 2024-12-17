#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <filesystem>
#include <iostream>
#include <ostream>

#include "mlem.h"

using std::sqrt;

class vec3 {
    public:
        double e[3]; // components of the vector: x, y, z
                     
    public:
        // Default constructor init the vector to (0, 0, 0)
        vec3() { e[0] = 0; e[1] = 0; e[2] = 0; } // Default constructor
        vec3(double e0, double e1, double e2) { // Parameterized constructor
            e[0] = e0; 
            e[1] = e1; 
            e[2] = e2; 
        }
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

        static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }

        static vec3 random(double min, double max) {
            return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
        }

        bool near_zero() const {
            // return true if the vector is close to zero in all dimensions.
            auto s = 1e-8;
            return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
        }
};

std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    // Outputs the vector in "x y z" format to an output stream.
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

vec3 operator+(const vec3 &u, const vec3 &v) {
    // Adds two vectors component-wise: (u.x + v.x, u.y + v.y, u.z + v.z)
    return vec3(u.e[0] + v.e[0], 
                u.e[1] + v.e[1],
                u.e[2] + v.e[2]);
}

vec3 operator-(const vec3 &u, const vec3 &v) {
    // Subtracts two vectors component-wise: (u.x - v.x, u.y - v.y, u.z - v.z)
    return vec3(u.e[0] - v.e[0], 
                u.e[1] - v.e[1],
                u.e[2] - v.e[2]);
}

vec3 operator*(const vec3 &u, const vec3 &v) {
    // (Hadamard product)
    // Multiplies two vectors component-wise : (u.x * v.x, u.y * v.y, u.z * v.z)
    return vec3(u.e[0] * v.e[0], 
                u.e[1] * v.e[1],
                u.e[2] * v.e[2]);
}

vec3 operator*(double t, const vec3 &v) {
    // Scales a vector by a scalar t: (t * v.x, t * v.y, t * v.z)
    return vec3(t * v.e[0], 
                t * v.e[1],
                t * v.e[2]);
}

vec3 operator/(vec3 v, double t) {
    // Divides a vector by a scalar t, equivalent to multiplying by 1/t
    return (1/t) * v;
}

double dot(const vec3 &u, const vec3 &v) {
    // Computes the dot product of two vectors:
    // dot(u, v) = u.x * v.x + u.y * v.y + u.z * v.z
    return u.e[0] * v.e[0] +
           u.e[1] * v.e[1] +
           u.e[2] * v.e[2];
}

vec3 cross(const vec3 &u, const vec3 &v) {
    // Computes the cross product of two vectors:
    // cross(u, v) = (u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x)
    // The result is a vector perpendicular to both u and v.
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1], 
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

vec3 unit_vector(vec3 v) {
    // Normalizes the vector to have a magnitude of 1:
    // unit_vector(v) = v / ||v||, where ||v|| is the vector's length
    return v / v.length();
}

vec3 random_in_unit_sphere() {
    while (true) {
        auto p = vec3::random(-1,1);
        if (p.length_squared() < 1) {
            return p;
        }
    }
}

vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

vec3 sample_square() {
    // Returns the vector to a random point in the [-.5,-.5]-[+.5,+.5] unit square.
    return vec3(random_double() - 0.5, random_double() - 0.5, 0);
}

vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) { // in the same hemisphere as the normal.
        return on_unit_sphere; 
    } else {
        return -on_unit_sphere; 
    }
}

vec3 reflect(const vec3& v, const vec3& n){
    return v - 2 * dot(v,n) * n;
}

inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat) {
    auto cos_theta = fmin(dot(-uv, n), 1.0);
    vec3 r_out_perp = etai_over_etat * (uv + cos_theta*n);
    vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

// Type aliases
using point3 = vec3; // Represents a 3D point in space
using color = vec3;  // Represents an RGB color value

#endif
