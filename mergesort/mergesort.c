#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "common.h"
#include "list.h"
void Spiltlist(struct list_head *head,
               struct list_head *left,
               struct list_head *right)
{
    struct list_head *slow;
    struct list_head *fast;

    if (list_empty(head) || list_is_singular(head))
        return;

    fast = head->next;
    slow = head->next;
    while ((fast->next->next != head) && (fast->next != head)) {
        fast = fast->next->next;
        slow = slow->next;
    }
    list_cut_position(left, slow, head->prev);
    list_cut_position(right, head, slow);
}
void SortedMerge(struct list_head *head,
                 struct list_head *left,
                 struct list_head *right)
{
    struct listitem *listitem_l = NULL, *listitem_r = NULL;
    while (1) {
        if (list_empty(left)) {
            list_splice_tail(right, head);
            break;
        } else if (list_empty(right)) {
            list_splice_tail(left, head);
            break;
        }
        listitem_l = list_first_entry(left, struct listitem, list);
        listitem_r = list_first_entry(right, struct listitem, list);
        if (listitem_l->i < listitem_r->i)
            list_move_tail(left->next, head);
        else
            list_move_tail(right->next, head);
    }
}
void Mergesort(struct list_head *head)
{
    struct list_head left, right;
    INIT_LIST_HEAD(&left);
    INIT_LIST_HEAD(&right);
    if (list_empty(head) || list_is_singular(head))
        return;
    Spiltlist(head, &left, &right);
    Mergesort(&left);
    Mergesort(&right);
    SortedMerge(head, &left, &right);
}

int main()
{
    uint16_t test[64];
    random_shuffle_array(test, ARRAY_SIZE(test));
    struct list_head head;
    struct listitem *item, *is = NULL;
    INIT_LIST_HEAD(&head);
    assert(list_empty(&head));
    size_t i = 0;
    printf("Before sorted\n");
    for (i = 0; i < ARRAY_SIZE(test); i++) {
        item = (struct listitem *) malloc(sizeof(struct listitem));
        item->i = test[i];
        printf("%2d ", item->i);
        if (i % 16 == 15)
            printf("\n");
        list_add_tail(&item->list, &head);
    }

    assert(!list_empty(&head));

    qsort(test, ARRAY_SIZE(test), sizeof(test[0]), cmpint);
    Mergesort(&head);

    i = 0;

    printf("After sorted\n");
    list_for_each_entry_safe (item, is, &head, list) {
        assert(item->i == test[i]);
        printf("%2d ", item->i);
        if (i % 16 == 15)
            printf("\n");
        list_del(&item->list);
        free(item);
        i++;
    }

    assert(i == ARRAY_SIZE(test));
    assert(list_empty(&head));
    return 0;
}