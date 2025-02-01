//============================================================
// file: main.cpp
//============================================================
#include <iostream>
#include <cstring>
#include <initializer_list>
#include <cassert>

//MATRIX and VECTOR classes assignment
#include "vector3d.h"
#include "matrix3d.h"

int main(int argc, const char * argv[]) {
  vector3D::run_tests();
  matrix3D::run_tests(); 
    
  return 0;
}


