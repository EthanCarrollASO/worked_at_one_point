#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <stdint.h>

#include "find_pixel_trace.h"

double xs,ys,xe,ye;
double dx,dy,ds;
double m,b,sgn_x,sgn_y;
double ixs,ixe,iys,iye;
int n_vert_cross,n_hori_cross,n_cross;
int i,j;
double min_ixs_ixe,min_iys_iye;
double *vert_cross_x,*vert_cross_y,*vert_s;
double *hori_cross_x,*hori_cross_y,*hori_s;
int *xpix,*ypix,*cross_type;
double *cross_dist;
double *s;
int *vert_cross_cum,*hori_cross_cum;

typedef struct _str{
    double value;
    int index;
} t_str;


int cmpfunc (const void *a, const void *b) {
  t_str *struct_a=(t_str *)a;
  t_str *struct_b=(t_str *)b;
  if (struct_a->value > struct_b->value) return 1;
    else if (struct_a->value == struct_b->value) return 0;
    else return -1;
}

struct return_vals *find_pixel_trace(float xs, float ys, float xe, float ye){
  //
  // intital comments
  //

  //printf("xs=%f\n",xs);
  //printf("ys=%f\n",ys);
  //printf("xe=%f\n",xe);
  //printf("xe=%f\n",ye);
  //
  //
  // figure the basics of the line segment
  //
  dx=xe-xs;
  dy=ye-ys;
  ds=sqrt(dx*dx+dy*dy);
  sgn_x=1.;
  if(dx < 0){sgn_x=-1.;}
  sgn_y=1.;
  if(dy < 0){sgn_y=-1.;}
  //printf("dx=%f\n",dx);
  //printf("dy=%f\n",dy);
  //printf("sgn_x=%f\n",sgn_x);
  //printf("sgn_y=%f\n",sgn_y);
  if(dx != 0.){
    m=dy/dx;
  }else{
    m=(sgn_y/sgn_x)*1e30;
  }
  b=ye-m*xe;
  //printf("m=%f\n",m);
  //printf("b=%f\n",b);
  //
  // find the number of crossing of vertical boundaries (changes sample #)
  // and allocate the associated arrays
  //
  //printf("xs=%f\n",xs);
  ixs=floor(xs);
  ixe=floor(xe);
  n_vert_cross=abs(ixe-ixs);
  if(n_vert_cross > 0){
    vert_cross_x=(double *)malloc(n_vert_cross*sizeof(double));
    vert_cross_y=(double *)malloc(n_vert_cross*sizeof(double));
    vert_s=(double *)malloc(n_vert_cross*sizeof(double));
  }
  //printf("ixs=%f\n",ixs);
  //printf("ixe=%f\n",ixe);
  //printf("n_vert_cross=%d\n",n_vert_cross);
  //
  // find the number of crossing of horizontal boundaries (changes line #)
  // and allocate the associated arrays
  //
  iys=floor(ys);
  iye=floor(ye);
  n_hori_cross=abs(iye-iys);
  if(n_hori_cross > 0){
    hori_cross_x=(double *)malloc(n_hori_cross*sizeof(double));
    hori_cross_y=(double *)malloc(n_hori_cross*sizeof(double));
    hori_s=(double *)malloc(n_hori_cross*sizeof(double));
  }
  //printf("iys=%f\n",iys);
  //printf("iye=%f\n",iye);
  //printf("n_hori_cross=%d\n",n_hori_cross);
  //
  // if not all within once column, figure the vertical crossing x,y coords
  // and distances from the start
  //
  if(ixs < ixe){
    min_ixs_ixe=ixs;
  }else{
    min_ixs_ixe=ixe;
  }
  //
  if(n_vert_cross > 0){
    for(i=0;i<n_vert_cross;i++){
      vert_cross_x[i]=(double) i+min_ixs_ixe+1;
      vert_cross_y[i]=vert_cross_x[i]*m+b;
      vert_s[i]=sqrt(pow(vert_cross_x[i]-xs,2)+pow(vert_cross_y[i]-ys,2));
    }
  }
  //
  // if not all within once row, figure the horizontal crossing x,y coords
  // and distances from the start, handle special case of vertical line
  //
  if(iys < iye){
    min_iys_iye=iys;
  }else{
    min_iys_iye=iye;
  }
  //
  if(n_hori_cross > 0){
    for(i=0;i<n_hori_cross;i++){
      hori_cross_y[i]=(double)i+min_iys_iye+1;
      if(dx != 0.){
        hori_cross_x[i]=(hori_cross_y[i]-b)/m;
      }else{
        hori_cross_x[i]=xs;
      }
      
      hori_s[i]=sqrt(pow(hori_cross_x[i]-xs,2)+pow(hori_cross_y[i]-ys,2));
    }
  }
  // 
  // build the combined list of all crossings (both tyeps) then sort them
  // and the matching type-flag vector on distance from start point
  //
  n_cross=n_vert_cross+n_hori_cross;
  xpix=(int *)malloc((n_cross+1)*sizeof(int));
  ypix=(int *)malloc((n_cross+1)*sizeof(int));
  s=(double *)malloc((n_cross+1)*sizeof(double));
  cross_type=(int *)malloc((n_cross)*sizeof(int));
  cross_dist=(double *)malloc((n_cross)*sizeof(double));
  vert_cross_cum=(int *)malloc((n_cross)*sizeof(int));
  hori_cross_cum=(int *)malloc((n_cross)*sizeof(int));
  //
  if(n_cross > 1){
    //
    // concatenate vert and hori crossings
    //
    if(n_vert_cross > 0){
      for(i=0;i<n_vert_cross;i++){
        cross_type[i]=1;
        cross_dist[i]=vert_s[i];
      }
      if(n_hori_cross >0){
        for(i=n_vert_cross;i<n_cross;i++){
            cross_type[i]=2;
            cross_dist[i]=hori_s[i-n_vert_cross];
        }
      }
    }else{
      for(i=0;i<n_cross;i++){
        cross_type[i]=2;
        cross_dist[i]=hori_s[i];
      }
    }
    //
    // sort type-flag and dist vectors based on dist
    //
    t_str *str;
    str=(t_str *)malloc(sizeof(t_str)*n_cross);
    for(i=0;i<n_cross;i++){
      str[i].value=cross_dist[i];
      str[i].index=cross_type[i];
    }
    qsort(str,n_cross,sizeof(str[0]),cmpfunc);
    //
    // accumulate vert and hori crossing counts
    //
    vert_cross_cum[0]=(int)cross_type[0]==1;
    hori_cross_cum[0]=(int)cross_type[0]==2;
    for(i=1;i<n_cross;i++){
      vert_cross_cum[i]=vert_cross_cum[i-1]+((int)str[i].index==1);
      hori_cross_cum[i]=hori_cross_cum[i-1]+((int)str[i].index==2);
    }
    //
    // figure xpix, ypix and s for output
    //
    xpix[0]=(int)ixs;
    for(i=1;i<n_cross+1;i++){
        xpix[i]=vert_cross_cum[i-1]*(int)sgn_x+(int)ixs;
    }
    ypix[0]=(int)iys;
    for(i=1;i<n_cross+1;i++){
        ypix[i]=hori_cross_cum[i-1]*(int)sgn_y+(int)iys;
    }
    s[0]=0.;
    for(i=1;i<n_cross;i++){
      s[i]=(str[i].value+str[i-1].value)/2.;
    }
    s[n_cross]=ds;
  }else{
    //
    // just neighboring pixels
    //
    xpix[0]=(int) ixs;
    xpix[1]=(int) ixe;
    ypix[0]=(int) iys;
    ypix[1]=(int) iye;
  }

  // Does creating a struct take too much time?
  struct return_vals temp;
  struct return_vals *my_struct_p = &temp;
  my_struct_p->struct_xpix = xpix; 
  my_struct_p->struct_ypix = ypix;
  my_struct_p->struct_s = s;
  my_struct_p->size = n_cross + 1;

  //printf("xpix: ");
  //printf("%d\n", xpix);
  //printf("ypix: ");
  //printf("%d\n", ypix);
  //printf("s: ");
  //printf("%d\n", s);
  //for(i=0;i<n_cross+1;i++){
  //    printf("%d %d %15.6f\n",my_struct_p->struct_xpix[i],my_struct_p->struct_ypix[i],my_struct_p->struct_s[i]);
  //}
  //QueryPerformanceCounter(&end);
  //interval = (double) (end.QuadPart - start.QuadPart) / frequency.QuadPart;
  //printf("%f\n", interval);

  printf("FROM find_pixel_trace()\n========================================\n");
  printf("%p: %f\n", &(my_struct_p->struct_s[0]), my_struct_p->struct_s[0]);
  printf("%p: %f\n", &(my_struct_p->struct_s[1]), my_struct_p->struct_s[1]);
  printf("%p: %f\n", &(my_struct_p->struct_s[2]), my_struct_p->struct_s[2]);
  return my_struct_p;
}