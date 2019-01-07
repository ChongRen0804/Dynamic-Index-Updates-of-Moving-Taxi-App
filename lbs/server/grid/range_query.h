//
//  main.c
//  range_query
//
//  Created by Jiawei Zhan on 2017/7/23.
//  Copyright © 2017年 Jiawei Zhan. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "lbs_grid.c"
#include "grid.h"
#include "lbs_hashtable.h"
#include "lbs/server/grid/lbs_defs.h"
#include <math.h>
#include "lbs/server/grid/lbs_index.h"
extern int lbs_grid_index_range_query(double lon1, double lon2, double lat1, double lat2, lbs_res_node_t* out){
    lbs_res_node_t lbs_res_node_s;
    lbs_grid_t* lbs_grid;
    int cell_row1,cell_col1,cell_row2,cell_col2;
    cell_row1=lbs_grid_cell_row(lbs_grid,lat1);
    cell_col1=lbs_grid_cell_col(lbs_grid, lon1);
    cell_row2=lbs_grid_cell_row(lbs_grid,lat2);
    cell_col2=lbs_grid_cell_col(lbs_grid, lon2);
        if(((cell_col1<=lbs_grid_cell_col(lbs_grid, lbs_res_node_s.lon)&&lbs_grid_cell_col(lbs_grid, lbs_res_node_s.lon)<=cell_col2))&&((cell_row1<=lbs_grid_cell_row(lbs_grid, lbs_res_node_s.lat))&&(lbs_grid_cell_row(lbs_grid, lbs_res_node_s.lat)<=cell_row2))){
            out->lat=lbs_res_node_s.lat;
            out->lon=lbs_res_node_s.lon;
            out->id=lbs_res_node_s.id;
            out->queue=lbs_res_node_s.queue;
            out->timestamp=lbs_res_node_s.timestamp;
        }
}

