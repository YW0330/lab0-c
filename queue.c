#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if (!head)
        return NULL;
    INIT_LIST_HEAD(head);
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *l)
{
    while (!list_empty(l))
        q_release_element(list_first_entry(l, element_t, list));
    free(l);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;
    int len = strlen(s);
    element_t *element = malloc(sizeof(element_t));
    if (!element)
        return false;
    INIT_LIST_HEAD(&element->list);
    element->value = malloc((len + 1) * sizeof(char));
    if (!element->value) {
        free(element);
        return false;
    }
    strncpy(element->value, s, len);
    *(element->value + len) = '\0';
    list_add(&element->list, head);
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    int len = strlen(s);
    element_t *element = malloc(sizeof(element_t));
    if (!element)
        return false;
    INIT_LIST_HEAD(&element->list);
    element->value = malloc((len + 1) * sizeof(char));
    if (!element->value) {
        free(element);
        return false;
    }
    strncpy(element->value, s, len);
    *(element->value + len) = '\0';
    list_add_tail(&element->list, head);
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return target element.
 * Return NULL if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 *
 * NOTE: "remove" is different from "delete"
 * The space used by the list element and the string should not be freed.
 * The only thing "remove" need to do is unlink it.
 *
 * REF:
 * https://english.stackexchange.com/questions/52508/difference-between-delete-and-remove
 */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *element = list_first_entry(head, element_t, list);
    if (bufsize && sp) {
        strncpy(sp, element->value, bufsize - 1);
        *(sp + bufsize - 1) = '\0';
    }
    list_del_init(&element->list);
    return element;
}

/*
 * Attempt to remove element from tail of queue.
 * Other attribute is as same as q_remove_head.
 */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *element = list_last_entry(head, element_t, list);
    if (bufsize && sp) {
        strncpy(sp, element->value, bufsize - 1);
        *(sp + bufsize - 1) = '\0';
    }
    list_del_init(&element->list);
    return element;
}

/*
 * WARN: This is for external usage, don't modify it
 * Attempt to release element.
 */
void q_release_element(element_t *e)
{
    free(e->value);
    free(e);
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(struct list_head *head)
{
    if (!head)
        return 0;
    int size = 0;
    struct list_head *node = NULL;
    list_for_each (node, head)
        size++;
    return size;
}

/*
 * Delete the middle node in list.
 * The middle node of a linked list of size n is the
 * ⌊n / 2⌋th node from the start using 0-based indexing.
 * If there're six element, the third member should be return.
 * Return true if successful.
 * Return false if list is NULL or empty.
 */
bool q_delete_mid(struct list_head *head)
{
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
    if (!head || list_empty(head))
        return false;
    struct list_head **mid = &head;
    struct list_head *fast = head;
    do {
        mid = &(*mid)->next;
        fast = fast->next->next;
    } while (head != fast->next && head != fast);
    q_release_element(q_remove_head((*mid)->prev, NULL, 0));
    return true;
}

/*
 * Delete all nodes that have duplicate string,
 * leaving only distinct strings from the original list.
 * Return true if successful.
 * Return false if list is NULL.
 *
 * Note: this function always be called after sorting, in other words,
 * list is guaranteed to be sorted in ascending order.
 */
bool q_delete_dup(struct list_head *head)
{
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
    if (!head || list_empty(head) || list_is_singular(head))
        return false;
    bool diff = false;
    struct list_head *curr = head->next, *next = curr->next;
    while (curr != head && next != head) {
        element_t *curr_entry = list_entry(curr, element_t, list);
        element_t *next_entry = list_entry(next, element_t, list);
        if (!strcmp(curr_entry->value, next_entry->value)) {
            diff = true;
            list_del(next);
            q_release_element(next_entry);
            next = curr->next;
        } else {
            if (diff) {
                list_del(curr);
                q_release_element(curr_entry);
                diff = false;
            }
            curr = next;
            next = next->next;
        }
    }

    return true;
}

/*
 * Attempt to swap every two adjacent nodes.
 */
void q_swap(struct list_head *head)
{
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(struct list_head *head)
{
    if (!head || list_empty(head))
        return;
    struct list_head *front = head->prev, *next = NULL, *now = head;
    while (next != head) {
        next = now->next;
        now->next = front;
        now->prev = next;
        front = now;
        now = next;
    }
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(struct list_head *head)
{
    if (!head || list_empty(head) || list_is_singular(head))
        return;
}
