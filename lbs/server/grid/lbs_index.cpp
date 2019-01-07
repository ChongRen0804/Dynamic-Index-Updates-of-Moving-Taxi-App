#include "server/grid/lbs_index.h"

#include <stdio.h>
#include <stdlib.h>

#include "common/log.h"

#include "server/grid/lbs_distance.h"
#include "server/grid/lbs_grid.h"
#include "server/grid/lbs_heap.h"
#include "server/grid/lbs_bitmap.h"
#include "server/grid/distance_point_grid.h"

#define LBS_LON_MIN 116
#define LBS_LON_MAX 117
#define LBS_LAT_MIN 39
#define LBS_LAT_MAX 41

#define LBS_ROW_NUM 200
#define LBS_COL_NUM 100

static lbs_grid_t lbs_grid;

// 初始化网格索引
int lbs_grid_index_init() {
  // TODO: by student
    lbs_grid_init(&lbs_grid, LBS_LON_MIN, LBS_LON_MAX, LBS_LAT_MIN, LBS_LAT_MAX, LBS_ROW_NUM, LBS_COL_NUM);
  return 0;
}

// 更新接口[出租车位置更新]
int lbs_grid_index_update(double lon,
                           double lat,
                           uint64_t timestamp,
                           uint32_t id) {
  // TODO: by student
    lbs_grid_update(&lbs_grid, lon, lat, timestamp, id);
  return 0;
}

// 范围查询接口[查询某一范围内的所有出租车信息]
int lbs_grid_index_range_query(double lon1,
                               double lon2,
                               double lat1,
                               double lat2,
                               lbs_res_node_t* out) {
  // TODO: by student
    int cell_row1,cell_col1,cell_row2,cell_col2;
    cell_row1=lbs_grid_cell_row(&lbs_grid,lat1);
    cell_col1=lbs_grid_cell_col(&lbs_grid, lon1);
    cell_row2=lbs_grid_cell_row(&lbs_grid,lat2);
    cell_col2=lbs_grid_cell_col(&lbs_grid, lon2);

    printf("row=%d %d col=%d %d\n", cell_row1, cell_row2, cell_col1, cell_col2);

    for (int row = cell_row1; row <= cell_row2; ++row) {
      for (int col = cell_col1; col <= cell_col2; ++col) {
        int cell_id = lbs_grid_cell_id(&lbs_grid, row, col);
        printf("cell_id = %d\n", cell_id);
        lbs_cell_t* cell = lbs_grid_cell(&lbs_grid, cell_id);
        lbs_mov_node_t* head = &cell->dammy_node;
        lbs_mov_node_t* next = (lbs_mov_node_t*)head->queue.next;
        while (head != next) {
          if (next->lon < lon1 || next->lon > lon2 || next->lat < lat1 || next->lat > lat2) {
            next = (lbs_mov_node_t*)next->queue.next;
            continue;
          }
          lbs_res_node_t* new_node = (lbs_res_node_t*)malloc(sizeof(lbs_res_node_t));
          new_node->lon = next->lon;
          new_node->lat = next->lat;
          lbs_queue_insert_head(&(out->queue), &(new_node->queue));
          next = (lbs_mov_node_t*)next->queue.next;
        }
      }
    }
    /*
    if(((cell_col1<=lbs_grid_cell_col(lbs_grid, lbs_res_node_s.lon)&&lbs_grid_cell_col(lbs_grid, lbs_res_node_s.lon)<=cell_col2))&&((cell_row1<=lbs_grid_cell_row(lbs_grid, lbs_res_node_s.lat))&&(lbs_grid_cell_row(lbs_grid, lbs_res_node_s.lat)<=cell_row2))){
            out->lat=lbs_res_node_s.lat;
            out->lon=lbs_res_node_s.lon;
            out->id=lbs_res_node_s.id;
            out->queue=lbs_res_node_s.queue;
            out->timestamp=lbs_res_node_s.timestamp;
        }
*/
  return 0;
}

// NN查询接口[查询离lon,lat最近的出租车]
int lbs_grid_index_nn_query(double lon, double lat, lbs_res_node_t* out) {
    lbs_nnheap_t lbs_heap;
    lbs_bitmap_t lbs_bitmap;
    
    // TODO: by student
    lbs_nnheap_init(&lbs_heap);
    lbs_bitmap_init(&lbs_bitmap, LBS_ROW_NUM*LBS_COL_NUM);

    int cell_row=lbs_grid_cell_row(&lbs_grid,lat);
    int cell_col=lbs_grid_cell_col(&lbs_grid,lon);
    int cell_id=lbs_grid_cell_id(&lbs_grid,cell_row,cell_col);
    //distance=distance_point_grid(lon,lat,cell_id);
    double distance = lbs_cell_distance(&lbs_grid, lon, lat, cell_id);

    lbs_heapnode_t* temp_heapnode;
    /*temp_heapnode->cell_id=cell_id;
    temp_heapnode->distance=distance;
    temp_heapnode->is_grid=1; //是网格
    temp_heapnode->node=NULL;*/


    lbs_nnheap_insert(&lbs_heap,NULL,cell_id,1,distance);
    lbs_bitmap_setbit(&lbs_bitmap, cell_id);

    while(1)
    {

        temp_heapnode=lbs_nnheap_top(&lbs_heap);
        if(temp_heapnode->is_grid==0)
            break;

        lbs_cell_t* cell = lbs_grid_cell(&lbs_grid, temp_heapnode->cell_id);
        lbs_mov_node_t* head = &cell->dammy_node;
        lbs_mov_node_t* next = (lbs_mov_node_t*)head->queue.next;
        while (head != next)
        {
              /*lbs_res_node_t* temp_node = (lbs_res_node_t*)malloc(sizeof(lbs_res_node_t));
              temp_node->lon = next->lon;
              temp_node->lat = next->lat;
              temp_node->id =next->id;
              temp_node->queue=next->queue;
              temp_node->timestamp=next->timestamp;*/
              lbs_nnheap_insert(&lbs_heap,next,-1,0,lbs_distance(lon,lat,next->lon,next->lat));
              next = (lbs_mov_node_t*)next->queue.next;
         }
        int a,b;

        for(a=-1;a<=1;a++)
        {
            for(b=-1;b<=1;b++)
            {
              int  temp_cell_id=temp_heapnode->cell_id+a*lbs_grid.col_num+b;
                if(lbs_bitmap_isset(&lbs_bitmap, temp_cell_id)==0/* && (temp_cell_id!=temp_heapnode->cell_id)*/) {
                    lbs_nnheap_insert(&lbs_heap,NULL, temp_cell_id,1,
                                      lbs_cell_distance(&lbs_grid, lon,lat,temp_cell_id)); //插入八个格
                    lbs_bitmap_setbit(&lbs_bitmap, temp_cell_id);
                }
            }

        }
        lbs_nnheap_pop(&lbs_heap);

    }

    lbs_res_node_t* new_node = (lbs_res_node_t*)malloc(sizeof(lbs_res_node_t));
    new_node->id=temp_heapnode->node->id;
    new_node->lat=temp_heapnode->node->lat;
    new_node->lon=temp_heapnode->node->lon;
    new_node->timestamp=temp_heapnode->node->timestamp;
    lbs_queue_insert_head(&(out->queue), &(new_node->queue));
    return 0;
}
