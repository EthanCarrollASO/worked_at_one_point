#ifndef FIND_PIXEL_TRACE_H
#define FIND_PIXEL_TRACE_H

struct return_vals{

  int * struct_xpix;
  int * struct_ypix;
  double * struct_s;
  int size;

};


struct return_vals *find_pixel_trace(float xs, float ys, float xe, float ye);

#endif  