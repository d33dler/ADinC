


#include <stdio.h>
#include <stdio.h>  /* printf */
#include <stdlib.h> /* malloc, free */
#include <string.h>
#include <limits.h>
#include "trainstructs.h"
#include "priorityh.h"
#include "dijkstra.h"

Travel newList() {
    Travel new = malloc(1*sizeof(struct List));
    for (int i = 0; i < N; i++) {
        new->fast[i] = NULL;
    }
    new->iix = 0;
    return new;
}


/*
 * Function to find the relevant node from the quarry
 */

Node findBE(Graph map, char *start) {
    for (int i = 0; map->neighbourList[i]!=0; i++ ) {
        if (strcmp(start, map->neighbourList[i]->A)==0) {
            return map->neighbourList[i];
        }
    }
    return NULL;
}

/*
 * Function to initiate the search algorithm and print the path if it is available
 * Also it initiates the cleaning (freeing) of nodes, if we require the same instance of the map
 * for further quarries.
 */

void nodepass(Graph map) {
    char *start;
    char *end;
    start =  readInput();
    while(start[0] != '!') {
        end  = readInput();
        Node begin  = findBE(map, start);
        Node finish = findBE(map, end);
        if (begin == NULL || finish == NULL) {
            printf("UNREACHABLE\n");
        } else if (strcmp(begin->A, finish->A) == 0 ) {
            printf("%s\n%s\n%d\n", begin->A, begin->A, 0);
        } else {
            formSourceheap(map,begin);
            dijkstraSearch(map, begin, begin, finish);
            printpath(begin, finish);
            cleanNodes(map,begin);
        }
        free(start);
        free(end);
        start =  readInput();
    }
    free(start);
}


/*
 * Reset values and deallocate memory for the heap structs and nodes.
 */

void cleanNodes(Graph map,Node begin){
    for (int i =0; map->neighbourList[i]!=NULL;i++) {
        Node ptr = map->neighbourList[i];
        ptr->dist = INT_MAX;
        ptr->visited= false;
        ptr->close=NULL;
        ptr->root = NULL;
    }
    for (int z = 0; z < 2*N+1; z++) {
        free(begin->harr[z]);
        begin->harr[z] = NULL;
    }
}


/*
 * Use the valuees in the list struct to print the path
 */
void printpath(Node begin , Node finish) {
    if (finish->close==NULL) {
        printf("UNREACHABLE\n");
    } else {
        Travel path = newList();
        while (strcmp(finish->A, begin->A ) != 0 ) {
            path->fast[path->iix]=finish;

            finish = finish->close;
            if(finish==NULL) {
                free(path);
                printf("UNREACHABLE\n");
                return;
            }
            path->iix++;
        }
        path->fast[path->iix] = finish;
        while(path->iix>-1){
            printf("%s\n", path->fast[path->iix]->A);
            path->iix--;
        }
        path->iix++;
        printf("%d\n", path->fast[path->iix]->dist);
        free(path);
    }
}

/*
 * Create the binary heap tree and array.
 */

void formSourceheap(Graph map, Node begin) {
    int inx = 1;
    begin->dist = 0;
    begin->root = makeHeap(begin, NULL, NULL, NULL);
    begin->harr[1] = begin->root;
    begin->harr[2] = begin->root->left;
    begin->harr[3] = begin->root->right;
    for (int i = 0;  map->neighbourList[i]!= NULL; i++) {
        if(strcmp(begin->A,map->neighbourList[i]->A)!=0) {
            inx++;
            Prior new = makeHeap(map->neighbourList[i], NULL, NULL, NULL);
            insertH(begin, new, &inx);
            begin->last = begin->harr[inx];
            begin->lastix = inx;
        }
    }
}


/*
 * The removal and update of the root node of the binary heap tree.
 */

void removeMin(Node begin, Prior hnode) {
    Prior temp = begin->last->parent;
    swap(hnode,begin->last);
    if(temp->parent!= NULL && strcmp(temp->left->holder->A, begin->last->holder->A) == 0){
        temp->left = NULL;
    } else {
        temp->right = NULL;
    }
    begin->lastix--;
    begin->last = begin->harr[begin->lastix];
    downheap(begin,begin->harr[1]);
}


/*
 * The Dijkstra algorithm itself.
 */
void dijkstraSearch(Graph map, Node begin, Node current, Node finish) {
    begin->dist = 0;
    while (begin->lastix > 1) {
        if ( begin->harr[1]->holder->visited == false) {
            begin->harr[1]->holder->visited  = true;
            current =  begin->harr[1]->holder;
            if(strcmp(current->A, finish->A) == 0) {
                return;
            }
            removeMin( begin, begin->harr[1]);
            upheap(begin, begin->last);
            for (int x = 1; x<=current->nbix;x++) {
                if ( current->links[x]->visited ==false) {
                    Node neighb = current->links[x];
                    int diffw = current->dist + current->linkswgt[x];
                    if ( diffw < neighb->dist) {
                        neighb->dist = diffw;
                        neighb->close = current;
                        upheap(begin, neighb->hnod);
                    }
                }
            }
        }
    }
}
