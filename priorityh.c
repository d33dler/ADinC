
#include <stdio.h>
#include <stdio.h>  /* printf */
#include <stdlib.h> /* malloc, free */
#include <assert.h> /* assert */
#include <string.h>
#include "limits.h"
#include "priorityh.h"
#include "trainstructs.h"


Prior makeHeap(Node st, Prior ancestor, Prior lc, Prior rc) {
    Prior h = malloc(1 * sizeof(struct Heaproot));
    assert (h != NULL);
    st->hnod = h;
    h->holder = st;
    h->parent = ancestor;
    h->left = NULL;
    h->right = NULL;
    return h;
}


/*
 * Function to insert new nodes in the binary heap structure and link parents
 * children nodes and also store the address in the heap array.
 */
void insertH(Node st, Prior shell, int *inx) {
    Prior parent = st->harr[*inx / 2];
    if (parent == NULL) {
        printf("%d\n", *inx);
    }
    shell->parent = st->harr[*inx/2]; //binary heap parent can by identified by (child index) div 2
    if (parent->left == NULL) {
        parent->left = shell;
    } else {
        parent->right = shell;
    }
    st->harr[*inx] = shell;
}

/*
 * Generic upheap function.
 */

void upheap(Node st, Prior child) {
    if (child->parent == NULL)
        return;
    int cweight;
    cweight = child->holder->dist;
    Prior cdp = child->parent;

    if (cweight < cdp->holder->dist) {
        swap(child, child->parent);
    }
    upheap(st, child->parent);
}

/*
 * Generic downheap function. We focus on the dist value of the holder(Station struct node )
 */

void downheap(Node st, Prior node) {
    if (node->left == NULL) {
        return;
    }
    Prior childL = node->left;
    Prior childR = node->right == NULL ? node->left : node->right;
    if (childL->holder->dist < node->holder->dist && childL->holder->dist < childR->holder->dist) {
        swap(childL,node);
    } else if (childR->holder->dist < node->holder->dist) {
        swap(childR,node);
        downheap(st, childR);
    }
}

/*
 * Swap function implemented to swap the chains pointers between two heap nodes and the
 * map struct Node relating to each of them.
 */

void swap(Prior first, Prior secnd) {
    Node temp = first->holder;
    Node temp2 = secnd->holder;
    temp->hnod = secnd;
    temp2->hnod = first;
    first->holder = temp2;
    secnd->holder = temp;
}

void freeHeap(Prior tr) {
    if (tr == NULL) {
        return;
    }
    freeHeap(tr->left);
    freeHeap(tr->right);
    free(tr);
}