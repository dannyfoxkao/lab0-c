#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    /* TODO: What if malloc returned NULL? */
    if (!q)
        return NULL;

    q->head = NULL;
    q->tail = NULL;
    q->size = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* Free queue structure */
    list_ele_t *tmp = NULL;

    if (!q)
        return;

    tmp = q->head;

    while (q->head) {
        q->head = q->head->next;
        memset(tmp->value, 0, strlen(tmp->value) + 1);
        free(tmp->value);
        free(tmp);
        tmp = q->head;
    }

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;

    /* TODO: What should you do if the q is NULL? */
    if (!q)
        goto err0;

    newh = malloc(sizeof(list_ele_t));
    if (!newh)
        goto err0;

    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    newh->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newh->value)
        goto err1;

    memset(newh->value, '\0', strlen(s) + 1);
    memcpy(newh->value, s, strlen(s));

    /*	if(q->size == 2)
        q->tail = q->head->next;
*/
    newh->next = q->head;
    q->head = newh;
    q->size++;

    if (!q->tail) {
        q->tail = q->head;
        q->tail->next = NULL;
    } else {
        while (q->tail->next) {
            q->tail = q->tail->next;
        }
    }

    return true;

err1:
    free(newh);
err0:
    return false;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newt;

    if (!q)
        goto err0;

    newt = malloc(sizeof(list_ele_t));
    if (!newt)
        goto err0;

    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    newt->value = malloc(sizeof(char) * (strlen(s) + 1));
    if (!newt->value)
        goto err1;

    newt->next = NULL;

    memset(newt->value, '\0', strlen(s) + 1);
    memcpy(newt->value, s, strlen(s));

    q->size++;
    if (q->tail)
        q->tail->next = newt;

    q->tail = newt;

    if (!q->head) {
        q->head = q->tail;
    }

    return true;

err1:
    free(newt);
err0:
    return false;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    list_ele_t *rmvh;
    size_t val_size;

    if (!q || !q->head)
        goto err0;

    if (bufsize > (strlen(q->head->value) + 1))
        val_size = strlen(q->head->value) + 1;
    else
        val_size = bufsize;

    memcpy(sp, q->head->value, val_size);
    sp[val_size - 1] = '\0';
    memset(q->head->value, 0, strlen(q->head->value) + 1);

    rmvh = q->head;
    q->head = q->head->next;
    free(rmvh->value);
    free(rmvh);
    q->size--;

    return true;

err0:
    return false;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    return q->size;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    list_ele_t *tmp = NULL;
    list_ele_t *new = NULL;

    if (!q || !q->head)
        return;

    if (!q->head->next)
        return;

    q->tail = q->head;

    while (q->head) {
        tmp = q->head;
        q->head = q->head->next;
        tmp->next = new;
        new = tmp;
    }
    q->head = new;

    q->tail->next = NULL;

    return;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void merge_sort(list_ele_t **head)
{
    list_ele_t *left = NULL;
    list_ele_t *right = NULL;

    if (!(*head) || !(*head)->next) {
        return;
    }

    left = *head;
    right = (*head)->next;

    /*split list into two part*/
    while (right && right->next) {
        left = left->next;
        right = right->next->next;
    }

    right = left->next;
    left->next = NULL;
    left = *head;

    /*break down list to only one element*/
    merge_sort(&left);
    merge_sort(&right);

    /*start to merge*/
    *head = NULL;
    list_ele_t **tmp = head;

    while (right && left) {
        if (strcmp(left->value, right->value) < 0) {
            *tmp = left;
            left = left->next;
        } else {
            *tmp = right;
            right = right->next;
        }
        tmp = &((*tmp)->next);
    }
    *tmp = right ? right : left;
}


void q_sort(queue_t *q)
{
    /* TODO: You need to write the code for this function */
    /* TODO: Remove the above comment when you are about to implement. */
    if (q->size < 1) {
        printf("q size <1\n");
        return;
    }
    merge_sort(&q->head);

    while (q->tail->next) {
        q->tail = q->tail->next;
    }
}
