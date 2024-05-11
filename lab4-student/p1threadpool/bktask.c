// Group members:
//   Nguyễn Quốc Lộc     - 2252458
//   Trần Tường Khang    - 2252313
//   Diệp Vũ Minh        - 2252472
//   Nguyễn Lê Minh Huân - 2252242


#include "bktpool.h"

struct bktask_t * bktask_get_byid(unsigned int bktaskid) {
  struct bktask_t * ptask = bktask;

  if (bktaskid < 0 || bktaskid > bktask_sz)
    return NULL;

  if (ptask == NULL)
    return NULL;

  while (ptask != NULL) {
    if (ptask -> bktaskid == bktaskid)
      return ptask;

    ptask = ptask -> tnext;
  }

  return NULL;
}

int bktask_init(unsigned int * bktaskid, void * func, void * arg) {
  struct bktask_t * new_task = malloc(sizeof(struct bktask_t));

  * bktaskid = taskid_seed++;
  bktask_sz++;

  new_task -> func = func;
  new_task -> arg = arg;
  new_task -> bktaskid = * bktaskid;

  new_task -> tnext = bktask;

  bktask = new_task;

  return 0;
}