#pragma once
#include <iostream>

namespace Saitama
{

  class Vector3
  {
  public:
    Vector3() : x(0), y(0), z(0) {}
    Vector3(double x, double y, double z) : x(x), y(y), z(z) {}
    ~Vector3() {};

    Vector3 operator+(const Vector3& v) const
    {
      return Vector3(x + v.x, y + v.y, z + v.z);
    }
    void operator+=(const Vector3& v)
    {
      x += v.x;
      y += v.y;
      z += v.z;
    }
    Vector3 operator-(const Vector3& v) const
    {
      return Vector3(x - v.x, y - v.y, z - v.z);
    }
    void operator-=(const Vector3& v)
    {
      x -= v.x;
      y -= v.y;
      z -= v.z;
    }
    Vector3 operator*(const double value) const
    {
      return Vector3(x * value, y * value, z * value);
    }
    void operator*=(const double value)
    {
      x *= value;
      y *= value;
      z *= value;
    }
    double DotProduct(const Vector3& v) const
    {
      return x * v.x + y * v.y + z * v.z;
    }
    Vector3 CrossProduct(const Vector3& v) const
    {
      return Vector3(y*v.z - z*v.y,
        z*v.x - x*v.z,
        x*v.y - y*v.x);
    }
    double Magnitude() const
    {
      return sqrt(x*x + y*y + z*z);
    }

    double SquareMagnitude() const
    {
      return x*x + y*y + z*z;
    }

    Vector3 Normalized() const
    {
      double mag = Magnitude();
      Vector3 result = *this;
      if (mag > 0)
      {
        result *= (double)1 / mag;
      }
      return result;
    }

    void Print() { std::cout << x << ", " << y << ", " << z << std::endl; }

    double x;
    double y;
    double z;
  };

  class Matrix3x3
  {
  public:
    double data[3][3];

    Matrix3x3()
    {
      data[0][0] = data[0][1] = data[0][2] = 
      data[1][0] = data[1][1] = data[1][2] = 
      data[2][0] = data[2][1] = data[2][2] = 1;
    }

    Matrix3x3(Vector3 v1, Vector3 v2, Vector3 v3)
    {
      data[0][0] = v1.x; data[0][1] = v1.y; data[0][2] = v1.z;
      data[1][0] = v2.x; data[1][1] = v2.y; data[1][2] = v2.z;
      data[2][0] = v3.x; data[2][1] = v3.y; data[2][2] = v3.z;
    }

    Matrix3x3 operator*(const Matrix3x3& m) const
    {
      Matrix3x3 matrix;
      matrix.data[0][0] = matrix.data[0][0] * m.data[0][0] + matrix.data[0][1] * m.data[1][0] + matrix.data[0][2] * m.data[2][0];
      matrix.data[0][1] = matrix.data[0][0] * m.data[0][1] + matrix.data[0][1] * m.data[1][1] + matrix.data[0][2] * m.data[2][1];
      matrix.data[0][2] = matrix.data[0][0] * m.data[0][2] + matrix.data[0][1] * m.data[1][2] + matrix.data[0][2] * m.data[2][2];
      matrix.data[1][0] = matrix.data[1][0] * m.data[0][0] + matrix.data[1][1] * m.data[1][0] + matrix.data[1][2] * m.data[2][0];
      matrix.data[1][1] = matrix.data[1][0] * m.data[0][1] + matrix.data[1][1] * m.data[1][1] + matrix.data[1][2] * m.data[2][1];
      matrix.data[1][2] = matrix.data[1][0] * m.data[0][2] + matrix.data[1][1] * m.data[1][2] + matrix.data[1][2] * m.data[2][2];
      matrix.data[2][0] = matrix.data[2][0] * m.data[0][0] + matrix.data[2][1] * m.data[1][0] + matrix.data[2][2] * m.data[2][0];
      matrix.data[2][1] = matrix.data[2][0] * m.data[0][1] + matrix.data[2][1] * m.data[1][1] + matrix.data[2][2] * m.data[2][1];
      matrix.data[2][2] = matrix.data[2][0] * m.data[0][2] + matrix.data[2][1] * m.data[1][2] + matrix.data[2][2] * m.data[2][2];
      return matrix;
    }

    Matrix3x3 transpose() const
    {
      
    }
  };

}