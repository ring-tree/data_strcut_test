#ifndef __OTHER_H__
#define __OTHER_H__

#include "data_struct.h"

#define RAND_INT(min, max) (rand() % (max - min + 1) + min) 

void SL_randLink(
  SL_link* const link,
  const uint32 len,
  const int32 min,
  const int32 max
);

#endif // !__OTHER_H__