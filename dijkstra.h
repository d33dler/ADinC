//
// Created by radubereja on 22-03-21.
//

#ifndef WEEK5_DIJKSTRA_H
#define WEEK5_DIJKSTRA_H
#include "trainstructs.h"

typedef struct List *Travel;


typedef struct List {
    Node fast[N];
    int iix;
}List;

void cleanNodes(Graph map,Node begin);
void formSourceheap(Graph map, Node begin);
void dijkstraSearch(Graph map, Node begin, Node temp, Node finish);
void nodepass(Graph map);
Node findBE(Graph map, char *start);
void printpath(Node begin , Node finish);
Travel newList();
#endif //WEEK5_DIJKSTRA_H
