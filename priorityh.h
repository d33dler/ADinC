//
// Created by radubereja on 22-03-21.
//

#ifndef WEEK5_PRIORITYH_H
#define WEEK5_PRIORITYH_H
#include "trainstructs.h"

Prior makeHeap(Node st, Prior ancestor, Prior lc, Prior rc);
void insertH(Node st, Prior shell, int *inx);
void upheap(Node st, Prior child);
void downheap(Node st, Prior node);
void freeHeap(Prior tr);
void removeMin(Node begin, Prior hnode);
#endif //WEEK5_PRIORITYH_H
