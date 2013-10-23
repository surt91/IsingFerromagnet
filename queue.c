#include "queue.h"

queue_t *create_queue()
{
    queue_t *queue;
    queue = (queue_t *) malloc (sizeof(queue_t));

    queue->first = NULL;
    queue->last = NULL;
    queue->size = 0;

    return queue;
}

void enqueue(queue_t *queue, int value)
{
    queue_node_t *entry;
    entry = (queue_node_t *) malloc (sizeof(queue_node_t));

    entry->value = value;
    entry->next = NULL;
    entry->prev = queue->last;

    if(queue->last != NULL)
        queue->last->next = entry;

    queue->last = entry;

    if(queue->first == NULL)
        queue->first = queue->last;

    queue->size++;
}

int dequeue(queue_t *queue)
{
    int value;
    queue_node_t *tmp;

    if(queue->size >=0)
    {
        value = queue->first->value;
        tmp = queue->first;
        queue->first = queue->first->next;

        free(tmp);
        queue->size--;
        return value;
    }
    else
        exit(1);
}

void clear_queue(queue_t *queue)
{
    while(queue->size)
    {
        dequeue(queue);
    }
    free(queue);
}

void test_queue()
{
    queue_t *q;
    q = create_queue();
    enqueue(q,13);
    enqueue(q,7);
    enqueue(q,666);

    printf("%d\n",dequeue(q));
    printf("%d\n",dequeue(q));
    enqueue(q,42);
    printf("%d\n",dequeue(q));

    clear_queue(q);
}
