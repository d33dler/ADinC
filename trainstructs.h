//
// Created by radubereja on 16-03-21.
//

#ifndef WEEK5_TRAINSTRUCTS_H
#define WEEK5_TRAINSTRUCTS_H

#include <stdbool.h>

#define N 15
#define MAXINPUT 100  /* maximal length of the input */


typedef struct TrainNetwork *Graph;
typedef struct Station *Node;
typedef struct Connection *Link;
typedef struct Heaproot *Prior;

typedef struct TrainNetwork {
    Node neighbourList[N];
    int dsrptnr;
} TrainNetwork;

typedef struct Station {
    char *A;
    char *B;
    int nbix;
    int dist;
    Node close;
    Node links[N];
    int linkswgt[N];
    Node next;
    bool visited;
    Prior root;
    Prior last;
    Prior hnod;
    int lastix;
    Prior harr[2 * N + 1];
} Station;

struct Heaproot {
    Prior parent;
    Node holder;
    Prior left;
    Prior right;
} Heaproot;

typedef struct Connection {
    char *A, *B;
    int weight;
} Connection;

Graph createDisrpt();

Graph createGraph(struct Connection points[], Graph disrpt);
void swap(Prior first, Prior secnd);
void freeMap(Graph mao);
Graph readMap(Graph disrpt);
char *readInput();
void freeD(Graph li);

Node newNode(char *a, char *b, int *ptr);

#endif //WEEK5_TRAINSTRUCTS_H
