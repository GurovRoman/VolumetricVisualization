//
// Created by memes on 13.12.2020.
//

#ifndef VOLUMETRICRAYMARCHING_MATH_H
#define VOLUMETRICRAYMARCHING_MATH_H

////////////////////////////////////////////////////////////////////////////////
///                      <|---- Vector ----|>                        ///
////////////////////////////////////////////////////////////////////////////////

struct Vector {
public:
    double x {};
    double y {};
    double z {};

public:
    Vector() = default;
    Vector(double x, double y, double z);

    Vector operator+() const;
    Vector operator-() const;

    Vector& operator+=(const Vector& other);
    Vector& operator-=(const Vector& other);
    Vector& operator*=(double);
    Vector& operator/=(double);

    double length() const;

    double dot(const Vector& rhs) const;
    double cross(const Vector& rhs) const;
    bool collinear(const Vector& rhs) const;
};

bool operator==(const Vector&, const Vector&);
bool operator!=(const Vector&, const Vector&);

Vector operator+(const Vector& lhs, const Vector& rhs);
Vector operator-(const Vector& lhs, const Vector& rhs);
Vector operator*(const Vector&, double);
Vector operator*(double, const Vector&);
Vector operator/(const Vector&, double);


////////////////////////////////////////////////////////////////////////////////
///                      <|---- Angle ----|>                        ///
////////////////////////////////////////////////////////////////////////////////

struct Angle {
    double p {};
    double y {};
    double r {};


};

////////////////////////////////////////////////////////////////////////////////
///                      <|---- Misc ----|>                        ///
////////////////////////////////////////////////////////////////////////////////

bool equal(const double first, const double second, const double eps = 1e-6);


#endif // VOLUMETRICRAYMARCHING_MATH_H
