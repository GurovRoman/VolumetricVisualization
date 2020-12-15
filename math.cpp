//
// Created by memes on 13.12.2020.
//

#include "math.h"
#include <cmath>

Vector::Vector(const double x, const double y, double z) : x(x), y(y), z(z) {}

Vector Vector::operator+() const { return Vector(*this); }

Vector Vector::operator-() const { return Vector() -= Vector(*this); }

Vector& Vector::operator+=(const Vector& other) { return x += other.x, y += other.y, z += other.z, *this; }

Vector& Vector::operator-=(const Vector& other) { return x -= other.x, y -= other.y, z -= other.z, *this; }

Vector& Vector::operator*=(const double mul) { return x *= mul, y *= mul, z *= mul, *this; }

Vector& Vector::operator/=(const double mul) { return x /= mul, y /= mul, z /= mul, *this; }

double Vector::length() const { return std::sqrt(dot(*this)); }

Vector operator+(const Vector& lhs, const Vector& rhs) { return Vector(lhs) += rhs; }

Vector operator-(const Vector& lhs, const Vector& rhs) { return Vector(lhs) -= rhs; }

Vector operator*(const Vector& lhs, const double rhs) { return Vector(lhs) *= rhs; }

Vector operator*(const double lhs, const Vector& rhs) { return Vector(rhs) *= lhs; }

Vector operator/(const Vector& lhs, const double rhs) { return Vector(lhs) /= rhs; }

bool operator==(const Vector& lhs, const Vector& rhs) {
    return equal(lhs.x, rhs.x) && equal(lhs.y, rhs.y) && equal(lhs.z, rhs.z);
}

bool operator!=(const Vector& lhs, const Vector& rhs) {
    return !(lhs == rhs);
}

double Vector::dot(const Vector& rhs) const {
    return x * rhs.x + y * rhs.y + z * rhs.z;
}

double Vector::cross(const Vector& rhs) const {
    std::terminate();
    return x * rhs.y - y * rhs.x;  // TODO
}

bool Vector::collinear(const Vector& rhs) const {
    return equal(cross(rhs), 0.0) && dot(rhs) >= 0.0;
}


bool equal(const double first, const double second, const double eps) {
    return std::fabs(first - second) < eps;
}

