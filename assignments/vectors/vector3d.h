//============================================================
// FILE: vector3dT.h
//============================================================

#ifndef __vector3d_H__
#define __vector3d_H__

#include <iostream>
#include <iomanip>
#include <cstring>
#include <cmath>
#include <initializer_list>
#include <cassert>

template <typename T> class vector3d;

typedef vector3d<double> vector3D;
typedef vector3d<float>  vector3F;
typedef vector3d<int>    vector3I;
typedef vector3d<long>   vector3L;
typedef vector3D vec3;

template <typename T>
class vector3d {  // class that serves as both vectors and points
public:
  vector3d();
  vector3d(const std::string& name, int dims);
  vector3d(const std::string& name, int dims, const std::initializer_list<T>& li);
//---------------------------------------------------------------------
  T  operator[](int i) const;
  T& operator[](int i);
//---------------------------------------------------------------------
  std::string name() const;
  void name(const std::string& name);

  T x() const;    // read-only make a copy of the element
  T y() const;    //   if you change the copy, the orig is unchanged
  T z() const;    //   hence the const at the end of the fn name

  T& x();        // read-write (have a reference to the data element)
  T& y();        //    cannot say const here, you might change the orig
  T& z();

  vector3d<T>& operator+=(const vector3d<T>& o);  // increment by another vector
  vector3d<T>& operator-=(const vector3d<T>& o);  //    o for other -- short name, will use it a lot!

  vector3d<T>& operator+=(T k);   // increment by a scalar (not a vector)
  vector3d<T>& operator-=(T k);
  vector3d<T>& operator*=(T k);
  vector3d<T>& operator/=(T k);

  vector3d<T> operator-();

  vector3d<T> operator+(const vector3d<T>& o);   // add two vectors
  vector3d<T> operator-(const vector3d<T>& o);

  vector3d<T> operator+(T k) const;     // Note: *** friend templated *** methods must be lined
  friend vector3d<T> operator+(T k, const vector3d<T>& v) { return v.operator+(k); }

  vector3d<T> operator-(T k) const;
  friend vector3d<T> operator-(T k, const vector3d<T>& v) { return v * -1 + k; }

  vector3d<T> operator*(T k) const;
  friend vector3d<T> operator*(T k, const vector3d<T>& v) { return v.operator*(k); }

  vector3d<T> operator/(T k) const;

  bool operator<(const vector3d<T>& o);    // logical operators, C++ will fill in rest
  bool operator==(const vector3d<T>& o);
  bool operator!=(const vector3d<T>& o);
 
  double dot(const vector3d<T>& o) const;  // dot-product  u dot v
  double mag() const;
  double norm() const { return mag(); }  // L2 norm
  double angle(const vector3d<T>& o) const;
  vector3d<T> cross(const vector3d<T>& o) const;
 
  static vector3d<T> zero();
  static double value(double val);

  friend std::ostream& operator<<(std::ostream& os, const vector3d<T>& v) {
    os << "<" << v.name_ << ", ";
    if (v.dims_ == 0) { return os << "empty>"; }
    for (int i = 0; i < v.dims_ + 1; ++i) {
      os << std::setw(3) << std::setprecision(3) << value(v[i]);
      if (i < v.dims_) { os << " "; }
    }
    return os << ">";
  }
  void show();
  void show(const std::string& msg);

  static void run_tests();

private:
  friend void check_equal_dims(const vector3d& u, const vector3d& v) {
    if (u.dims_ != v.dims_) { throw new std::invalid_argument("vector3d dims mismatch"); }
  }
  void check_bounds(int i) const;

private:
  std::string name_;
  int dims_;
  double data_[4];

// use for operator== and division to test if dividing by a number that is very close to 0
  static double epsilon_;
};

template<typename T> double vector3d<T>::epsilon_ = 1e-10;



//==============================================================================
// implementation code
//==============================================================================

template <typename T> vector3d<T>::vector3d() : vector3d("no_name", 3) {}  // 3d default dims
template <typename T> vector3d<T>::vector3d(const std::string& name, int dims)
  : name_(name), dims_(dims) {
    memset(data_, 0, dims_ * sizeof(double));
    data_[3] = 0;  // vectors have 0 at end, pts have 1
  }
template <typename T> vector3d<T>::vector3d(const std::string& name, int dims,
                                            const std::initializer_list<T>& li)
  : vector3d(name, dims) {
    int i = 0;
    for (T value : li) {
      if (i > dims_) { break; }
      data_[i++] = value;
    }
    data_[3] = 0;
  }

//---------------------------------------------------------------------
template <typename T>  /* read only idx */
T  vector3d<T>::operator[](int i) const {  check_bounds(i);  return data_[i]; }

template <typename T> T& vector3d<T>::operator[](int i) {
  if (i > 3 /* 4? */) { throw std::invalid_argument("index overflow"); }
  return data_[i];
}
//---------------------------------------------------------------------
template <typename T> std::string vector3d<T>::name() const { return name_; }
template <typename T> void vector3d<T>::name(const std::string& name) { name_ = name; }

  // read-only
template <typename T> T vector3d<T>::x() const { return data_[0]; }
template <typename T> T vector3d<T>::y() const { return data_[1]; }
template <typename T> T vector3d<T>::z() const { return data_[2]; }
  // read-write
template <typename T> T& vector3d<T>::x() { return data_[0]; }
template <typename T> T& vector3d<T>::y() { return data_[1]; }
template <typename T> T& vector3d<T>::z() { return data_[2]; }

template <typename T>
vector3d<T>& vector3d<T>::operator+=(const vector3d<T>& v) {
  vector3d<T>& u = *this;
  u.name_ += " + " + v.name_;
  u[0] += v[0];  u[1] += v[1];  u[2] += v[2];  u[3] = v[3];
  return *this;
}
template <typename T>
vector3d<T>& vector3d<T>::operator-=(const vector3d<T>& v) { return operator+=(v * -1); }

template <typename T> vector3d<T>& vector3d<T>::operator+=(T k) { 
    vector3d<T>& u = *this;
    u.name_ += "+=" + std::to_string(k);
    u[0] += k;  u[1] += k;  u[2] += k;
    return *this;
}
template <typename T> vector3d<T>& vector3d<T>::operator-=(T k) { 
    vector3d<T>& u = *this;
    u.name_ += "-=" + std::to_string(k);
    u[0] -= k;  u[1] -= k;  u[2] -= k;
    return *this;
 }
template <typename T> vector3d<T>& vector3d<T>::operator*=(T k) { 
    vector3d<T>& u = *this;
    u.name_ += "*=" + std::to_string(k);
    u[0] *= k;  u[1] *= k;  u[2] *= k;
    return *this;
 }
template <typename T> vector3d<T>& vector3d<T>::operator/=(T k) { 
    if (abs(k) < epsilon_) { throw new std::invalid_argument("divide by zero\n"); }
    vector3d<T>& u = *this;
    u.name_ += "/=" + std::to_string(k);
    u[0] /= k;  u[1] /= k;  u[2] /= k;
    return *this;
 }

template <typename T> vector3d<T> vector3d<T>::operator+(const vector3d<T>& o) {
  check_equal_dims(*this, o);
  return vector3d<T>(name_ + "+" + o.name_, dims_,
                  { data_[0] + o[0], data_[1] + o[1], data_[2] + o[2], 0} );
}
template <typename T> vector3d<T> vector3d<T>::operator-(const vector3d<T>& o) {
  check_equal_dims(*this, o);
  return vector3d<T>(name_ + "-" + o.name_, dims_,
                  { data_[0] - o[0], data_[1] - o[1], data_[2] - o[2], 0} );
}

template <typename T> vector3d<T> vector3d<T>::operator+(T k) const { 
  return vector3d<T>(name_ + "+" + std::to_string(k), dims_, { data_[0] + k, data_[1] + k, data_[2] + k, 0 });
}

template <typename T> vector3d<T> vector3d<T>::operator-(T k) const {
  return vector3d<T>(name_ + "-" + std::to_string(k), dims_, { data_[0] - k, data_[1] - k, data_[2] - k, 0 });
}

template <typename T> vector3d<T> vector3d<T>::operator*(T k) const {
  return vector3d<T>(name_ + "*" + std::to_string(k), dims_, { data_[0] * k, data_[1] * k, data_[2] * k, 0 });
};

template <typename T> vector3d<T> vector3d<T>::operator/(T k) const {
  if (k == 0) { throw new std::invalid_argument("divide by zero"); }
  double kinv = 1.0 / k;
  return *this * kinv;
}

template <typename T>
vector3d<T> vector3d<T>::operator-() { return vector3d("-" + name_, dims_, { 
    -data_[0], -data_[1], -data_[2], 0 }); }

template <typename T> bool vector3d<T>::operator<(const vector3d<T>& o) {   
    check_equal_dims(*this, o);  return mag() < o.mag(); }
template <typename T> bool vector3d<T>::operator==(const vector3d<T>& o) {  
    vector3d<T>& u = *this;
    check_equal_dims(u, o);  
    return /* u.name_ == o.name_ && */ u[0] == o[0] && u[1] == o[1] && u[2] == o[2]; 
}
template <typename T> bool vector3d<T>::operator!=(const vector3d<T>& o) { return !(*this == o); }

template <typename T>
double vector3d<T>::dot(const vector3d<T>& o) const { 
    const vector3d<T>& u = *this;
    return u[0] * o[0] + u[1] * o[1] + u[2] * o[2]; 
}

template <typename T>
double vector3d<T>::mag() const {  return sqrt(dot(*this));  }

template <typename T>
double vector3d<T>::angle(const vector3d<T>& o) const { 
    // because... a.dot(b) = a.mag() * b.mag() * cos(theta)
    return acos(dot(o) / (mag() * o.mag()));
}

template <typename T>
vector3d<T> vector3d<T>::cross(const vector3d<T>& o) const {
  check_equal_dims(*this, o);
  if (o.dims_ != 3) {
    throw new std::invalid_argument("cross_product only implemented for vector3d's");
  }
  const vector3d<T>& u = *this;
  return vector3d<T>(name_ + " x " + o.name_, dims_,
                    { u[1]*o[2] - u[2]*o[1], -(u[0]*o[2] - u[2]*o[0]),
                      u[0]*o[1] - u[1]*o[0], 0 });
}

template <typename T> vector3d<T> vector3d<T>::zero() { return vector3d<T>("zero", 3, {0, 0, 0, 0}); }

template <typename T> void vector3d<T>::check_bounds(int i) const {  // 1 extra dimension for pts/vectors
  if (i > dims_) { throw new std::invalid_argument("out of bounds"); }
}

template <typename T> double vector3d<T>::value(double val) { return abs(val) < 1e-5 ? 0 : val; }

template <typename T> void vector3d<T>::show() { std::cout << *this << "\n"; }
template <typename T> void vector3d<T>::show(const std::string& msg) { std::cout << msg << *this << "\n"; }

template <typename T> void vector3d<T>::run_tests() {
  vector3d<double> u("u", 3, {1,  2,  4});
  // std::cout << u.name() << "\n";
  // std::cout << u << "\n";
  std::cout << "\n====================  TESTING VECTORS  ====================================================\n";
  std::cout << "u.name_ is: " << u.name() << "\n";
  u.zero();
  u.show();
  vector3D v("v", 3, {8, 16, 32});
  vector3D i("i", 3, {1, 0, 0}), j("j", 3, {0, 1, 0}), k("k", 3, {0, 0, 1});
  vector3D w(3 * i + 4 * j - 2 * k);

  u.show();  v.show();  
  i.show();  j.show();  k.show();
  std::cout << "j + k is: " << j + k << "\n";
  w.show();

  std::cout << "*** asserting u == u and u != v" << "\n";
  assert(u == u);   assert(v == v);   assert(u != v);

  std::cout << "*** asserting u + v == v + u   and  u - v == -(v - u)    and   -(-u) == u" << "\n";
  assert(u + v == v + u);   assert(u - v == -(v - u));   assert(-(-u) == u);

  std::cout << "*** 3.0 + u == u + 3.0   and   3.0 * u == u * 3.0" << "\n";
  assert(3.0 + u == u + 3.0);    assert(3.0 * u == u * 3.0);

  // vector3d<double> a = u - 3.0;       // example of tracking down a failed assertion
  // vector3d<double> b = -( 3.0 - u);
  // u.show();   a.show();   b.show();
  std::cout << "*** asserting u - 3.- == -(3.0 - u)" << "\n";
  assert((u - 3.0) == -(3.0 - u));
  std::cout << "*** asserting 5.0 * u == u * 5.0" << "\n";
  assert((5.0 * u) / 5.0 == u);

  std::cout << "*** asserting u + vector3D::zero() == u" << "\n";
  assert(u + vector3D::zero() == u);

  std::cout << "*** asserting i.dot(j) == j.dot(k) == k.dot(i) == 0" << "\n";
  assert(i.dot(j) == j.dot(k) == k.dot(i) == 0);

  std::cout << "*** asserting i.cross(j) == k   and  j.cross(k) == i   and   k.cross(i) == j" << "\n";
  assert(i.cross(j) == k);   assert(j.cross(k) == i);   assert(k.cross(i) == j);

  std::cout << "*** asserting u.cross(v) == -v.cross(u)" << "\n";
  assert(u.cross(v) == -v.cross(u));
  assert(u.cross(v + w) == u.cross(v) + u.cross(w));
  assert((u.cross(v)).dot(u) == 0);

  std::cout << "i.angle(j) is: " << i.angle(j) << "\n";
  std::cout << "pi/2 is: " << M_PI/2 << "\n";

  std::cout << "*** asserting i.angle(j) == j.angle(k) == k.angle(i) == M_PI/2" << "\n";
  assert(i.angle(j) == M_PI_2);   assert(j.angle(k) == M_PI_2);    assert(k.angle(i) == M_PI_2);

  vector3D uhat = u / u.mag();
  u.show();   uhat.show();
  std::cout << "length of uhat.mag() is... " << uhat.mag() << "\n";
  std::cout << "*** asserting u.hat.mag() - 1.0 < 1.0e-10" << "\n";
  assert(uhat.mag() - 1.0 < 1.0e-10);

  std::cout << "\n\n...test vectors assertions passed" << "\n\n";
  std::cout << "====================  FINISHED testing vectors  ============================================\n\n";
}

#endif
