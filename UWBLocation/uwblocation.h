#ifndef UWBLOCATION_H
#define UWBLOCATION_H


#include "stdlib.h"
#include "math.h"
#include "string.h"
#include "stdint.h"

//__declspec(dllexport)
extern "C"{

 bool get_position(float *an_x, float *an_y, float *an_z, float *dis, int len, float *x, float *y, float *z);

}

#endif // UWBLOCATION_H
