#include "server/grid/lbs_bitmap.h"

#include<stdint.h>
#include<stdlib.h>
#include<math.h>

/*
typedef struct lbs_bitmap_s{
    uint8_t *bits;
    uint32_t bits_num;
} lbs_bitmap_t;
*/


int lbs_bitmap_init(lbs_bitmap_t* lbs_bitmap,uint32_t bits_num)
{

    lbs_bitmap->bits_num=bits_num;
    int temp_num= bits_num/8 + (bits_num % 8 == 0 ? 0 : 1);
    lbs_bitmap->bits=(uint8_t*)malloc(sizeof(uint8_t) * temp_num);
    int i;
    for(i=0;i<temp_num;i++)
    {
        lbs_bitmap->bits[i]=0x00;
    }
    return 0;
}
int lbs_bitmap_destroy(lbs_bitmap_t* lbs_bitmap){return 0;}
int lbs_bitmap_setbit(lbs_bitmap_t* lbs_bitmap,uint32_t pos)
{
    int temp_num=pos/8;
    int temp_offset=pos%8;
    lbs_bitmap->bits[temp_num] |= (0x80 >> temp_offset);
    return 0;
    //return lbs_bitmap->bits[temp_num]|(0x80>>temp_offset);
}
int lbs_bitmap_unsetbit(lbs_bitmap_t* lbs_bitmap,uint32_t pos)
{
    int temp_num=pos/8;
    int temp_offset=pos%8;
    lbs_bitmap->bits[temp_num] &= ~(0x80 >> temp_offset);
    return 0;
    //return lbs_bitmap->bits[temp_num]&(0x7F>>temp_offset);
}
int lbs_bitmap_isset(lbs_bitmap_t* lbs_bitmap,uint32_t pos)
{
    int temp_num=pos/8;
    int temp_offset=pos%8;
    return (lbs_bitmap->bits[temp_num]&(0x80>>temp_offset))>>(7-temp_offset);
}
