#include <stdio.h>
#include <stdio.h>  /* printf */
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <string.h>
#include <limits.h>
#include "trainstructs.h"
#include "dijkstra.h"
#include "priorityh.h"



char *readInput() {
    int strLen = MAXINPUT;
    int c = getchar();
    int i = 0;
    char *s = malloc((strLen + 1) * sizeof(char));
    assert(s != NULL);
    while (c != '\n') {
        s[i] = c;
        i++;
        if (i >= strLen) { /* s is not large enough, its length is doubled */
            strLen = 2 * strLen;
            s = realloc(s, (strLen + 1) * sizeof(char));
            assert(s != NULL);
        }
        c = getchar();
    }
    s[i] = '\0';
    return s;
}


//Function to create new nodes for the disruption graph or main map graph.
Node newNode(char *a, char *b, int *ptr) {
    Node new = malloc(sizeof(struct Station));
    assert(new != NULL);
    new->A = a;
    new->B = b;
    for (int i = 0; i < N; i++) {
        new->links[i] = NULL;
    }
    for (int i = 0; i < 2*N+1; i++) {
        new->harr[i] = NULL;
    }
    new->harr[N] = NULL;
    new->root = NULL;
    new->nbix = 1;
    new->visited = false;
    new->dist = INT_MAX ;
    new->close = NULL;
    new->next = (Node) ptr;
    return new;
}


/*
 * Function that stores all the required edges that are part of the default map
 * It transport the edges to the main createGraph function and initiates it, afterwards
 * returning the final version of the graph accounting for the disruptions as well.
*/

Graph readMap(Graph disrpt) {
    struct Connection points[] = {
            {"Amsterdam",  "Den Haag",   46},
            {"Amsterdam",  "Den Helder", 77},
            {"Amsterdam",  "Utrecht",    26},
            {"Den Haag",   "Eindhoven",  89},
            {"Eindhoven",  "Maastricht", 63},
            {"Eindhoven",  "Nijmegen",   55},
            {"Eindhoven",  "Utrecht",    47},
            {"Enschede",   "Zwolle",     50},
            {"Groningen",  "Leeuwarden", 34},
            {"Groningen",  "Meppel",     49},
            {"Leeuwarden", "Meppel",     40},
            {"Maastricht", "Nijmegen",   111},
            {"Meppel",     "Zwolle",     15},
            {"Nijmegen",   "Zwolle",     77},
            {"Utrecht",    "Zwolle",     51},
            {"Neverland", "Groningen", 999}
    };
    Graph map = createGraph(points, disrpt);
    return map;
}

/*Function to check whether a station is already present in the Graph
*in order to avoid duplicates and unnecessary memory allocation.
*/


int checkStationA(Link points, Graph map, int *ret) {
    for (int i = 0; i < N && map->neighbourList[i] != NULL; i++) {
        if (strcmp(points->A, map->neighbourList[i]->A) == 0) {
            *ret = i;
            return 1;
        }
    }
    return 0;
}

/*Function to check whether a station is already present in the Graph
*in order to avoid duplicates and unnecessary memory allocation.
*/

int checkStationB(Link points, Graph map, int *ret) {
    for (int i = 0; i < N && map->neighbourList[i] != NULL; i++) {
        if (strcmp(points->B, map->neighbourList[i]->A) == 0) {
            *ret = i;
            return 2;
        }
    }
    return 0;
}

//Creates the map graph of disruptions.

Graph createDisrpt() {
    int d, i, x;
    i = x = d = 0;
    Graph disrpt = malloc(sizeof(struct TrainNetwork));
    while (i != N) {
        disrpt->neighbourList[i] = NULL;
        i++;
    }
    scanf("%d\n", &d);
    disrpt->dsrptnr = d;
    while (d > 0) {
        disrpt->neighbourList[x] = newNode(readInput(), readInput(), NULL);
        d--;
        x++;
    }
    return disrpt;
}


/*
 * Function createGraph allocates memory to the Graph struct for the main map
 * It initiates all locations in the array with null and starts selecting each
 * edge and compare it to edges in the disruptions map Graph. This edges and the
 * adjacent nodes are not going to participate in the final version of the created
 * graph. Also , neighbor list for particular nodes are not going to be updated
 * as well.
 */
Graph createGraph(struct Connection points[], Graph disrpt) {
    Graph map = malloc(sizeof(struct TrainNetwork));
    assert(map != NULL);
    int i = 0;
    bool check = false;
    while (i != N) {
        map->neighbourList[i] = NULL;
        i++;
    }
    int sts = 0;
    int ret = 0;
    Node stationA = NULL;
    Node stationB = NULL;
    i = 0;
    while (1) {
        for (int q = 0; q < disrpt->dsrptnr; q++) {

            if ((strcmp(disrpt->neighbourList[q]->A, points[i].A) == 0
                 && strcmp(disrpt->neighbourList[q]->B, points[i].B) == 0)
                ||
                (strcmp((*disrpt->neighbourList[q]).A, points[i].B) == 0
                 && strcmp((*disrpt->neighbourList[q]).B, points[i].A) == 0)) { //comparing edges & finding matches
                check = true;
                i++;
                break;
            }
            check = false;
        }
        if(check == false ) {
            if (checkStationA(&points[i], map, &ret) != 1) { //if node is not found - create new node
                map->neighbourList[sts] = newNode(points[i].A, points[i].B, NULL); //assign address
                stationA = map->neighbourList[sts];
                sts++;
                check = true;
            } else {
                stationA = map->neighbourList[ret];
                stationA->nbix++;
            }
            if (checkStationB(&points[i], map, &ret) != 2) {   //if node is not found - create new node
                map->neighbourList[sts] = newNode(points[i].B, points[i].A, NULL); //assign address
                stationB = map->neighbourList[sts];
                sts++;
                check = true;
            } else {
                stationB = map->neighbourList[ret];
                stationB->nbix++;
            }
            stationA->links[stationA->nbix] = stationB; //update neighbor list for node A
            stationB->links[stationB->nbix] = stationA; //update neighbor list for node B
            stationA->linkswgt[stationA->nbix] = stationB->linkswgt[stationB->nbix] = points[i].weight; //update weight
            i++;
            check = false; //reset bool
        }
        if (i == 15) {
            return map;
        }
    }
}


void freeMap(Graph map) {
    for (int i = 0; map->neighbourList[i] != NULL; i++) {
        free(map->neighbourList[i]->root);
        map->neighbourList[i]->root = NULL;
        free(map->neighbourList[i]);
    }
    free(map);
}

void freeD(Graph map) {
    for (int i = 0; map->neighbourList[i] != NULL; i++) {
        free(map->neighbourList[i]->A);
        free(map->neighbourList[i]->B);
        free(map->neighbourList[i]);
    }
    free(map);
}

int main() {
    Graph disrupt = createDisrpt();
    Graph map = readMap(disrupt);
    nodepass(map);
    freeMap(map);
    freeD(disrupt);
    return 0;
}
