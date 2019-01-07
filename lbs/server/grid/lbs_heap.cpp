#include "server/grid/lbs_heap.h"
#include "server/grid/lbs_queue.h"
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
//#define MAXSIZE 50

//****************��ʼ������*****************
int lbs_nnheap_init(lbs_nnheap_t* lbs_nnheap)
{
    //int size = lbs_nnheap->size;
    //for (int i=0;i<MAXSIZE;i++)
    lbs_nnheap->capacity =50;
    lbs_nnheap->size = 0;
    lbs_nnheap->heap_nodes = (lbs_heapnode_t*)malloc(sizeof(lbs_heapnode_t)*lbs_nnheap->capacity);
    return 0;

    }

//���ϵ���
void MinHeapFixup(lbs_nnheap_t* lbs_nnheap, int i)
{
    //int father = (i-1)/2;
    while (i > 0) {
      int father = (i - 1) / 2;
      if (lbs_nnheap->heap_nodes[father].distance <= lbs_nnheap->heap_nodes[i].distance) {
        break;
      }
      lbs_heapnode_t temp = lbs_nnheap->heap_nodes[father];
      lbs_nnheap->heap_nodes[father] = lbs_nnheap->heap_nodes[i];
      lbs_nnheap->heap_nodes[i] = temp;
      i = father;
    }
    #if 0
    //lbs_heapnode_t* temp_i = (lbs_heapnode_t*)malloc(sizeof(lbs_heapnode_t);
    lbs_heapnode_t temp_i = lbs_nnheap->heap_nodes[i];
    //lbs_heapnode_t* temp_father = (lbs_heapnode_t*)malloc(sizeof(lbs_heapnode_t));
    //temp_father = lbs_nnheap->heap_nodes + father;
    while(father>=0 && i!=0)
    {
        if(lbs_nnheap->heap_nodes[father].distance <= temp_i.distance)
            break;
        temp_i = lbs_nnheap->heap_nodes[father];
        lbs_nnheap->heap_nodes[father] = lbs_nnheap->heap_nodes[i];
        lbs_nnheap->heap_nodes[i] = temp_i;

        i = father;
        father = (i-1)/2;
    }
    lbs_nnheap->heap_nodes[i] = temp_i;
    #endif
}


//  ���µ���
void MinHeapFixdown(lbs_nnheap_t* lbs_nnheap, int i, int n)
{
#if 0
    int son = 2*i +1;
    //lbs_heapnode_t* temp_i = (lbs_heapnode_t*)malloc(sizeof(lbs_heapnode_t);
    lbs_heapnode_t temp_i = lbs_nnheap->heap_nodes[i];
    while(son < n)
    {
        if(son+1 <n && lbs_nnheap->heap_node[son+1] < lbs_nnheap->heap_node[son] )
            son++;
        if(lbs_nnheap->heap_node[son]>=temp_i)
            break;
        lbs_nnheap->heap_node[i] = lbs_nnheap->heap_node[son];
        i = son;
        son = 2*i +1;
    }
    lbs_nnheap->heap_node[i] = temp_i;
#endif
   
  int lchild = 2 *i + 1;
   while (lchild < n) {
     lchild = 2 * i + 1;
     int rchild = lchild + 1;
     int min_son = lchild;
    
     if (rchild < n && lbs_nnheap->heap_nodes[rchild].distance < lbs_nnheap->heap_nodes[min_son].distance) {
       min_son = rchild;
     }
     lbs_heapnode_t temp = lbs_nnheap->heap_nodes[min_son];
     lbs_nnheap->heap_nodes[min_son] = lbs_nnheap->heap_nodes[i];
     lbs_nnheap->heap_nodes[i] = temp;
     i = min_son;
   }

}



//**************�������*************
int lbs_nnheap_insert(lbs_nnheap_t* lbs_nnheap, lbs_mov_node_t* lbs_mov_node, int cell_id, uint8_t is_grid, double distance)
{
    //��������ռ�ߴ����·���
    if(lbs_nnheap->capacity == lbs_nnheap->size)
    {
       lbs_nnheap->capacity *= 2;
       lbs_nnheap->heap_nodes = (lbs_heapnode_t*)realloc(
            lbs_nnheap->heap_nodes, sizeof(lbs_heapnode_t)*lbs_nnheap->capacity);
    }
    //��ȡ�ڵ���
    int n = lbs_nnheap->size;
    lbs_nnheap->size++;

    //ĩβ�����½ڵ�
    //lbs_heapnode_t* temp_heapnode[n]=(lbs_heapnode_t*)malloc(sizeof(lbs_heapnode_t));
    lbs_nnheap->heap_nodes[n].cell_id = cell_id;
    lbs_nnheap->heap_nodes[n].distance = distance;
    lbs_nnheap->heap_nodes[n].is_grid = is_grid;
    lbs_nnheap->heap_nodes[n].node = lbs_mov_node;

    MinHeapFixup(lbs_nnheap, n);

    return 0;
}


//**************��ȡdistance��С��lbs_heapnode_t**************
lbs_heapnode_t* lbs_nnheap_top(lbs_nnheap_t* lbs_nnheap)
{
    lbs_heapnode_t* temp_node = lbs_nnheap->heap_nodes;
    return temp_node;
}


//ɾ���Ѷ�Ԫ��
void lbs_nnheap_pop(lbs_nnheap_t* lbs_nnheap)
{
#if 0
    i=0, j=lbs_nnheap->size -1;
    //lbs_heapnode_t* temp_i = (lbs_heapnode_t*)malloc(sizeof(lbs_heapnode_t);
    lbs_heapnode_t temp_i = lbs_nnheap->heap_nodes[j];
    lbs_nnheap->heap_nodes[j] = lbs_nnheap->heap_nodes[i];
    lbs_nnheap->heap_nodes[i] = temp_i;
#endif
    lbs_nnheap->heap_nodes[0] = lbs_nnheap->heap_nodes[--lbs_nnheap->size];
    MinHeapFixdown(lbs_nnheap,0,lbs_nnheap->size);
}

