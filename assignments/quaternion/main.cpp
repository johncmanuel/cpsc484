//============================================================
// file: main.cpp
//============================================================
#include <cassert>
#include <cstring>
#include <initializer_list>
#include <iostream>

// MATRIX and VECTOR classes assignment
#include "matrix3d.h"
#include "quaternion.h"
#include "vector3d.h"
// #include "quaternion_soln.h"

int main(int argc, const char *argv[]) {
  vector3D::run_tests();
  matrix3D::run_tests();
  quaternionD::run_tests();

  return 0;
}
