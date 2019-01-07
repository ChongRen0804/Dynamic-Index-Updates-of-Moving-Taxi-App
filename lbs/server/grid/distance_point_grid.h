//
//  main.c
//  distance_point_grid.h
//
//  Created by Jiawei Zhan on 2017/7/24.
//  Copyright © 2017年 Jiawei Zhan. All rights reserved.
//
#include "server/grid/lbs_index.h"
#include <stdlib.h>
#include "server/grid/lbs_distance.h"
#include "server/grid/lbs_grid.h"
#include <stdio.h>
#define LBS_LON_MIN 116
#define LBS_LON_MAX 117
#define LBS_LAT_MIN 39
#define LBS_LAT_MAX 41

#define LBS_ROW_NUM 200
#define LBS_COL_NUM 100

void lbs_cell_range(lbs_grid_t* lbs_grid, int cell_id, double* lon1, double* lon2, double* lat1, double* lat2) {
  int cell_row, cell_col;
  lbs_grid_cell_row_col(lbs_grid, cell_id, &cell_row, &cell_col);
  *lon1 = lbs_grid->lon_min + cell_col * lbs_grid->cell_width;
  *lon2 = *lon1 + lbs_grid->cell_width;

  *lat1 = lbs_grid->lat_min + cell_row * lbs_grid->cell_height;
  *lat2 = *lat1 + lbs_grid->cell_height;
}

double lbs_point_rect_distance(double lon, double lat, double lon1, double lon2, double lat1, double lat2) {
  if (lon < lon1) {
    if (lat < lat1) {
      return lbs_distance(lon, lat, lon1, lat1);
    } else if (lat > lat2) {
      return lbs_distance(lon, lat, lon1, lat2);
    } else {
      return lbs_distance(lon, lat, lon1, lat);
    }
  } else if (lon > lon2) {
    if (lat < lat1) {
      return lbs_distance(lon, lat, lon2, lat1);
    } else if (lat > lat2) {
      return lbs_distance(lon, lat, lon2, lat2);
    } else {
      return lbs_distance(lon, lat, lon2, lat);
    }
  } else {
    if (lat < lat1) {
      return lbs_distance(lon, lat, lon, lat1);
    } else if (lat > lat2) {
      return lbs_distance(lon, lat, lon, lat2);
    } else {
      return 0;
    }
  }
}

double lbs_cell_distance(lbs_grid_t* lbs_grid, double lon, double lat, int cell_id) {
  double lon1, lon2, lat1, lat2;
  /// Get range of cell
  lbs_cell_range(lbs_grid, cell_id, &lon1, &lon2, &lat1, &lat2);
  /// compute lon,lat -> Range distance
  return lbs_point_rect_distance(lon, lat, lon1, lon2, lat1, lat2);
}

#if 0
double distance_point_grid(double lon,double lat,int cell_id){//要注意经纬度和距离的关系
    int cell_row=lbs_grid_cell_row(&lbs_grid,lat);
    int cell_col=lbs_grid_cell_col(&lbs_grid, lon);
    double cell_length=(LBS_LON_MAX-LBS_LON_MIN)/LBS_COL_NUM;
    double cell_height=(LBS_LAT_MAX-LBS_LAT_MIN)/LBS_ROW_NUM;
    int cell_id1=lbs_grid_cell_id(&lbs_grid, cell_row, cell_col);

    e=((cell_id%LBS_COL_NUM)-1)*cell_length;
    f=((int)(cell_id/LBS_COL_NUM))*cell_height;
    if((cell_id%LBS_COL_NUM==cell_id1%LBS_COL_NUM)&&(cell_id>cell_id1)){
        return lbs_distance(lon,lat,lon,f);
    }
    if((cell_id%LBS_COL_NUM==cell_id1%LBS_COL_NUM)&&(cell_id<cell_id1)){
        return lbs_distance(lon,lat,lon,f+cell_height);
    }
    if((cell_id/LBS_COL_NUM==cell_id1/LBS_COL_NUM)&&(cell_id<cell_id1)){
        return lbs_distance(lon,lat,e+cell_length,lat);
        
    }
    if((cell_id/LBS_COL_NUM==cell_id1/LBS_COL_NUM)&&(cell_id>cell_id1)){
        return lbs_distance(lon,lat,e,lat);
    }
    if(cell_id==cell_id1){
        return 0;
    }
    else{
        double a,b,c,d,e,f,min;    //(cell_id/LBS_COL_NUM*cell_height,(cell_id%LBS_COL_NUM-1)*lenght)
        a=lbs_distance(lon,lat,e,f);
        b=lbs_distance(lon,lat,e+cell_length,f);
        c=lbs_distance(lon,lat,e,f+cell_height);
        d=lbs_distance(lon,lat,e+cell_length,f+cell_height);
        min=(a<b)? a:b;
        min=(min<c)? min:c;
        min=(min<d)? min:d;
        return min;
    }
}
#endif
