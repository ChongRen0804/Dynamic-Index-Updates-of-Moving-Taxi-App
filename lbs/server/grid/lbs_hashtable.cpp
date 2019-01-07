#include"server/grid/lbs_hashtable.h"
#include"server/grid/lbs_queue.h"
#include <stdlib.h>
#include <malloc.h>
#define MAXSIZE 500


int hash(int nKey)//哈希函数
{
    return nKey%MAXSIZE;//返回数组的索引
}

//初始化
int lbs_hashtable_init(lbs_hashtable_t* lbs_hashtable)
{

        lbs_hashtable->hash_nodes = (lbs_hashnode_t*)malloc(sizeof(lbs_hashnode_t)*MAXSIZE);
        //lbs_hashtable.size = 0;
        for (int i=0;i<MAXSIZE;i++)
        {
            lbs_queue_init(&lbs_hashtable->hash_nodes[i].queue);
            //lbs_hashtable.hash_nodes[i].size = 0;
        }
        return 0;
}

//销毁
//int lbs_hashtable_destroy(lbs_hashtable_t* lbs_hashtable)



//设置
int lbs_hashtable_set(lbs_hashtable_t* lbs_hashtable, uint32_t id, lbs_mov_node_t* lbs_mov_node, int cell_id)
{
    lbs_hashnode_t* temp_hashnode=(lbs_hashnode_t*)malloc(sizeof(lbs_hashnode_t));
    temp_hashnode->cell_id=cell_id;
    temp_hashnode->mov_node=lbs_mov_node;
    lbs_queue_insert_head(&lbs_hashtable->hash_nodes[hash(id)].queue,&temp_hashnode->queue);
        /*
         nBuf = hash(id);
         pBuf = new lbs_hashnode_s;
         pBuf.cell_id = cell_id;
         pBuf.mov_node = mov_node
         lbs_queue_insert_head(lbs_hashtable.hash_nodes[nBuf].queue, pBuf);
         */
    return 0;
}
//提取
lbs_hashnode_t* lbs_hashtable_get(lbs_hashtable_t* lbs_hashtable, uint32_t id)
{
    lbs_hashnode_t* temp_hash_nodes=lbs_hashtable->hash_nodes+hash(id);
    temp_hash_nodes=(lbs_hashnode_t*)temp_hash_nodes->queue.next;
    while(temp_hash_nodes!=lbs_hashtable->hash_nodes+hash(id))
    {
        if(temp_hash_nodes->mov_node->id==id)
            return  temp_hash_nodes;
        temp_hash_nodes=(lbs_hashnode_t*)temp_hash_nodes->queue.next;
    }
    return NULL;
}
//lbs_hashnode_s.queue.prev
