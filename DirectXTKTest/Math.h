#pragma once
#include <iostream>

namespace Saitama
{

  class Vector3
  {
  public:
    Vector3() : m_x(0), m_y(0), m_z(0) {}
    Vector3(double x, double y, double z) : m_x(x), m_y(y), m_z(z) {}
    ~Vector3() {};

    Vector3 operator+(const Vector3& v) const
    {
      return Vector3(m_x + v.GetX(), m_y + v.GetY(), m_z + v.GetZ());
    }
    void operator+=(const Vector3& v)
    {
      m_x += v.GetX();
      m_y += v.GetY();
      m_z += v.GetZ();
    }
    Vector3 operator-(const Vector3& v) const
    {
      return Vector3(m_x - v.GetX(), m_y - v.GetY(), m_z - v.GetZ());
    }
    void operator-=(const Vector3& v)
    {
      m_x -= v.GetX();
      m_y -= v.GetY();
      m_z -= v.GetZ();
    }
    Vector3 operator*(const double value) const
    {
      return Vector3(m_x * value, m_y * value, m_z * value);
    }
    void operator*=(const double value)
    {
      m_x *= value;
      m_y *= value;
      m_z *= value;
    }
    double DotProduct(const Vector3& v) const
    {
      return m_x * v.GetX() + m_y * v.GetY() + m_z * v.GetZ();
    }
    Vector3 CrossProduct(const Vector3& v) const
    {
      return Vector3(m_y*v.GetZ() - m_z*v.GetY(),
        m_z*v.GetX() - m_x*v.GetZ(),
        m_x*v.GetY() - m_y*v.GetX());
    }
    double Magnitude() const
    {
      return sqrt(m_x*m_x + m_y*m_y + m_z*m_z);
    }

    double SquareMagnitude() const
    {
      return m_x*m_x + m_y*m_y + m_z*m_z;
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

    // Getter and setter function
    double GetX() const { return m_x; }
    double GetY() const { return m_y; }
    double GetZ() const { return m_z; }
    void SetX(const double x) { m_x = x; }
    void SetY(const double y) { m_y = y; }
    void SetZ(const double z) { m_z = z; }

    void Print() { std::cout << m_x << ", " << m_y << ", " << m_z << std::endl; }

  private:
    double m_x;
    double m_y;
    double m_z;
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
      return 
    }
  }

}