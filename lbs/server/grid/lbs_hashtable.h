#ifndef H_HASHTABLE_H_
#define H_HASHTABLE_H_

#include<pthread.h>
#include"server/grid/lbs_defs.h"

typedef struct lbs_hashnode_s{
        //���� 
        lbs_queue_t queue;
        //�ڵ� 
        lbs_mov_node_t* mov_node;
        //cell id
        int cell_id;
}       lbs_hashnode_t;

typedef struct lbs_hashtable_s{
        //�� 
        //pthread_mutex_t mutex;
        //��ռ�� 
        int size;
        //���� 
        int capacity;
        //��ϣ������ַ 
        lbs_hashnode_t* hash_nodes;
}       lbs_hashtable_t;

//��ʼ��
int lbs_hashtable_init(lbs_hashtable_t* lbs_hashtable);
//����
int lbs_hashtable_destroy(lbs_hashtable_t* lbs_hashtable);
//���� 
int lbs_hashtable_set(lbs_hashtable_t* lbs_hashtable, uint32_t id, lbs_mov_node_t* lbs_mov_node, int cell_id);
//��ȡ 
lbs_hashnode_t* lbs_hashtable_get(lbs_hashtable_t* lbs_hashtable, uint32_t id);

#endif
