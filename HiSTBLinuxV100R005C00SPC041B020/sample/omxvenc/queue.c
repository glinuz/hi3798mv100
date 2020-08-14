/*--------------------------------------------------------------------------
Copyright (c) 2010-2011, Code Aurora Forum. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Code Aurora nor
      the names of its contributors may be used to endorse or promote
      products derived from this software without specific prior written
      permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NON-INFRINGEMENT ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
--------------------------------------------------------------------------*/
/*
	Queue with Linked list
*/

#include "queue.h"
#include <stdio.h>
#include <stdlib.h>


typedef struct Node
{
  void *element;
  struct Node *next;
} Node;

struct Queue
{
  Node *head;
  Node *tail;
  int  current_size;
};

Queue *alloc_queue()
{
  Queue *q = (Queue *) malloc(sizeof(Queue));
  if (q)
  {
    q->head = q->tail = NULL;
    q->current_size = 0;
  }
  return q;
}

void free_queue(Queue *q)
{
  while (q->current_size)
  {
    pop(q);
  }
  free(q);    //add by liminqi
}

void free_queue_and_qelement(Queue *q)
{
  while (q->current_size)
  {
    void *element = pop(q);
    if (element)
      free(element);
  }
}

int push(Queue *q, void * element)
{
  Node *new_node = (Node *) malloc(sizeof(Node));

  if (new_node == NULL)
    return -1;

  new_node->element = element;
  new_node->next = NULL;

  if (q->current_size == 0)
  {
    q->head = new_node;
  }
  else
  {
    q->tail->next = new_node;
  }

  q->tail = new_node;
  q->current_size++;

  return 0;
}

void *pop(Queue *q)
{
  Node *temp;
  void *element;

  if (q->current_size == 0)
    return NULL;

  temp = q->head;
  element = temp->element;

  if (q->current_size == 1)
  {
    q->head = q->tail = NULL;
  }
  else
  {
    q->head = q->head->next;
  }

  free(temp);
  q->current_size--;
  return element;
}
