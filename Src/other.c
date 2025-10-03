#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "other.h"

/**
 * @brief 随机生成链表
 *
 * @param link 链表
 * @param len 链表长度
 * @param min 最小值
 * @param max 最大值
 */
void SL_randLink(SL_link *const link, const uint32 len, const int32 min,
                 const int32 max) {
  srand(time(NULL));
  uint32 randNum = 0;
  for (size_t i = 0; i < len; i++) {
    randNum = RAND_INT(min, max);
    SL_add(link, randNum);
  }
}


/*
n % (max- min + 1)

n % max  ->  0 ~ max-1
20 ~ 10
n % (max- min + 1) + min
*/