#ifndef SORTED_LIST_H
#define SORTED_LIST_H

#include <stdlib.h>
#include <assert.h>

/*
    IMPORTANT!

    As we stick to pure C, we cannot use templates. We will just asume
    some type T was previously defined.
*/

// -----------------------------------------------------------------------------

typedef struct node {
    int value;
    struct node* next;
} TNode, *TSortedList;

typedef struct list {
    TNode* first;
    TNode* last;
} List;

// TODO : Cerința 1 & Cerința 2

TSortedList create(int value) {
    // TODO (se va completa la Cerința 2)
    TSortedList node;
    node = (TSortedList)malloc(sizeof(TNode));
    node->value=value;
    node->next = NULL;
    return node;
}

int isEmpty(TSortedList head)
{
    if(head==NULL)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int contains(TSortedList head,int element)
{
    if(isEmpty(head))
    {
        return 0;
    }
    while(head!=NULL)
    {
        if(head->value==element)
            return 1;
        head=head->next;
    }
    return 0;
}

void insert(List *list,int value){
    TSortedList node;
    node = create(value);
    if(list->first == NULL){
        list->first = node;
        list->last = node;
    }
    else{
        list->last->next = node;
        list->last = list->last->next;
    }
}

TSortedList deleteOnce(TSortedList head,int element)
{
    TSortedList kontor,pt_ant;
    kontor=head;
    if(head==NULL)
    {
        return head;
    }
    if(head->value==element)
    {
        head=head->next;
        free(kontor);
        return head;
    }
    while(kontor!=NULL)
    {
        if(kontor->value==element)
        {
            pt_ant->next=kontor->next;
            free(kontor);
            return head;
        }
        pt_ant=kontor;
        kontor=kontor->next;
    }
    if(kontor->value==element)
    {
        pt_ant->next=NULL;
        free(kontor);
        return head;
    }
    return head;
}

long length(TSortedList head)
{
    long length=0l;
    if(head==NULL)
    {
        return 0;
    }
    while(head!=NULL)
    {
        length++;
        head=head->next;
    }
    return length;
}

// int getNth(TSortedList head,long position)
// {
//     TSortedList kontor;
//     long kontor_pos=1;
//     kontor=head;
//     if(head==NULL)
//         return 0;
//     while(kontor!=NULL)
//     {
//         if(kontor_pos==position)
//         {
//             return kontor->value;
//         }
//         kontor_pos++;
//         kontor=kontor->next;
//     }
// }

void freeTSortedList(TSortedList head)
{
    TSortedList current = head,next;
    while(current != NULL)
    {
        next=current->next;
        free(current);
        current=next;
    }
    head = NULL;
    return;
}

#endif
