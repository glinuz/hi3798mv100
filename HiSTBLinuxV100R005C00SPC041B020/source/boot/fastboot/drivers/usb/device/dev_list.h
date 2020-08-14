/*	$OpenBSD: queue.h,v 1.26 2004/05/04 16:59:32 grange Exp $	*/
/*	$NetBSD: queue.h,v 1.11 1996/05/16 05:17:14 mycroft Exp $	*/

/*
 * Copyright (c) 1991, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)queue.h	8.5 (Berkeley) 8/20/94
 */

#ifndef _DWC_LIST_H_
#define _DWC_LIST_H_

#ifdef __cplusplus
extern "C" {
#endif

/** @file
 *
 * This file defines linked list operations.  It is derived from BSD with
 * only the MACRO names being prefixed with DWC_.  This is because a few of
 * these names conflict with those on Linux.  For documentation on use, see the
 * inline comments in the source code.  The original license for this source
 * code applies and is preserved in the dwc_list.h source file.
 */

/*
 * This file defines five types of data structures: singly-linked lists,
 * lists, simple queues, tail queues, and circular queues.
 *
 *
 * A singly-linked list is headed by a single forward pointer. The elements
 * are singly linked for minimum space and pointer manipulation overhead at
 * the expense of O(n) removal for arbitrary elements. New elements can be
 * added to the list after an existing element or at the head of the list.
 * Elements being removed from the head of the list should use the explicit
 * macro for this purpose for optimum efficiency. A singly-linked list may
 * only be traversed in the forward direction.  Singly-linked lists are ideal
 * for applications with large datasets and few or no removals or for
 * implementing a LIFO queue.
 *
 * A list is headed by a single forward pointer (or an array of forward
 * pointers for a hash table header). The elements are doubly linked
 * so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list before
 * or after an existing element or at the head of the list. A list
 * may only be traversed in the forward direction.
 *
 * A simple queue is headed by a pair of pointers, one the head of the
 * list and the other to the tail of the list. The elements are singly
 * linked to save space, so elements can only be removed from the
 * head of the list. New elements can be added to the list before or after
 * an existing element, at the head of the list, or at the end of the
 * list. A simple queue may only be traversed in the forward direction.
 *
 * A tail queue is headed by a pair of pointers, one to the head of the
 * list and the other to the tail of the list. The elements are doubly
 * linked so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list before or
 * after an existing element, at the head of the list, or at the end of
 * the list. A tail queue may be traversed in either direction.
 *
 * A circle queue is headed by a pair of pointers, one to the head of the
 * list and the other to the tail of the list. The elements are doubly
 * linked so that an arbitrary element can be removed without a need to
 * traverse the list. New elements can be added to the list before or after
 * an existing element, at the head of the list, or at the end of the list.
 * A circle queue may be traversed in either direction, but has a more
 * complex end of list detection.
 *
 * For details on the use of these macros, see the queue(3) manual page.
 */

/*
 * Circular queue definitions.
 */
#define DWC_CIRCLEQ_HEAD(name, type)					\
struct name {								\
	struct type *cqh_first;		/* first element */		\
	struct type *cqh_last;		/* last element */		\
}

#define DWC_CIRCLEQ_HEAD_INITIALIZER(head)				\
	{ DWC_CIRCLEQ_END(&head), DWC_CIRCLEQ_END(&head) }

#define DWC_CIRCLEQ_ENTRY(type)						\
struct {								\
	struct type *cqe_next;		/* next element */		\
	struct type *cqe_prev;		/* previous element */		\
}

/*
 * Circular queue access methods
 */
#define DWC_CIRCLEQ_FIRST(head)		((head)->cqh_first)
#define DWC_CIRCLEQ_LAST(head)		((head)->cqh_last)
#define DWC_CIRCLEQ_END(head)		((void *)(head))
#define DWC_CIRCLEQ_NEXT(elm, field)	((elm)->field.cqe_next)
#define DWC_CIRCLEQ_PREV(elm, field)	((elm)->field.cqe_prev)
#define DWC_CIRCLEQ_EMPTY(head)						\
	(DWC_CIRCLEQ_FIRST(head) == DWC_CIRCLEQ_END(head))

#define DWC_CIRCLEQ_EMPTY_ENTRY(elm, field) (((elm)->field.cqe_next == NULL) && ((elm)->field.cqe_prev == NULL))

#define DWC_CIRCLEQ_FOREACH(var, head, field)				\
	for((var) = DWC_CIRCLEQ_FIRST(head);				\
	    (var) != DWC_CIRCLEQ_END(head);				\
	    (var) = DWC_CIRCLEQ_NEXT(var, field))

#define DWC_CIRCLEQ_FOREACH_SAFE(var, var2, head, field)			\
	for((var) = DWC_CIRCLEQ_FIRST(head), var2 = DWC_CIRCLEQ_NEXT(var, field); \
	    (var) != DWC_CIRCLEQ_END(head);					\
	    (var) = var2, var2 = DWC_CIRCLEQ_NEXT(var, field))

#define DWC_CIRCLEQ_FOREACH_REVERSE(var, head, field)			\
	for((var) = DWC_CIRCLEQ_LAST(head);				\
	    (var) != DWC_CIRCLEQ_END(head);				\
	    (var) = DWC_CIRCLEQ_PREV(var, field))

/*
 * Circular queue functions.
 */
#define DWC_CIRCLEQ_INIT(head) do {					\
	(head)->cqh_first = DWC_CIRCLEQ_END(head);			\
	(head)->cqh_last = DWC_CIRCLEQ_END(head);			\
} while (0)

#define DWC_CIRCLEQ_INIT_ENTRY(elm, field) do {				\
	(elm)->field.cqe_next = NULL;					\
	(elm)->field.cqe_prev = NULL;					\
} while (0)

#define DWC_CIRCLEQ_INSERT_AFTER(head, listelm, elm, field) do {	\
	(elm)->field.cqe_next = (listelm)->field.cqe_next;		\
	(elm)->field.cqe_prev = (listelm);				\
	if ((listelm)->field.cqe_next == DWC_CIRCLEQ_END(head))		\
		(head)->cqh_last = (elm);				\
	else								\
		(listelm)->field.cqe_next->field.cqe_prev = (elm);	\
	(listelm)->field.cqe_next = (elm);				\
} while (0)

#define DWC_CIRCLEQ_INSERT_BEFORE(head, listelm, elm, field) do {	\
	(elm)->field.cqe_next = (listelm);				\
	(elm)->field.cqe_prev = (listelm)->field.cqe_prev;		\
	if ((listelm)->field.cqe_prev == DWC_CIRCLEQ_END(head))		\
		(head)->cqh_first = (elm);				\
	else								\
		(listelm)->field.cqe_prev->field.cqe_next = (elm);	\
	(listelm)->field.cqe_prev = (elm);				\
} while (0)

#define DWC_CIRCLEQ_INSERT_HEAD(head, elm, field) do {			\
	(elm)->field.cqe_next = (head)->cqh_first;			\
	(elm)->field.cqe_prev = DWC_CIRCLEQ_END(head);			\
	if ((head)->cqh_last == DWC_CIRCLEQ_END(head))			\
		(head)->cqh_last = (elm);				\
	else								\
		(head)->cqh_first->field.cqe_prev = (elm);		\
	(head)->cqh_first = (elm);					\
} while (0)

#define DWC_CIRCLEQ_INSERT_TAIL(head, elm, field) do {			\
	(elm)->field.cqe_next = DWC_CIRCLEQ_END(head);			\
	(elm)->field.cqe_prev = (head)->cqh_last;			\
	if ((head)->cqh_first == DWC_CIRCLEQ_END(head))			\
		(head)->cqh_first = (elm);				\
	else								\
		(head)->cqh_last->field.cqe_next = (elm);		\
	(head)->cqh_last = (elm);					\
} while (0)

#define DWC_CIRCLEQ_REMOVE(head, elm, field) do {			\
	if ((elm)->field.cqe_next == DWC_CIRCLEQ_END(head))		\
		(head)->cqh_last = (elm)->field.cqe_prev;		\
	else								\
		(elm)->field.cqe_next->field.cqe_prev =			\
		    (elm)->field.cqe_prev;				\
	if ((elm)->field.cqe_prev == DWC_CIRCLEQ_END(head))		\
		(head)->cqh_first = (elm)->field.cqe_next;		\
	else								\
		(elm)->field.cqe_prev->field.cqe_next =			\
		    (elm)->field.cqe_next;				\
} while (0)

#define DWC_CIRCLEQ_REMOVE_INIT(head, elm, field) do {			\
	DWC_CIRCLEQ_REMOVE(head, elm, field);				\
	DWC_CIRCLEQ_INIT_ENTRY(elm, field);				\
} while (0)

#define DWC_CIRCLEQ_REPLACE(head, elm, elm2, field) do {		\
	if (((elm2)->field.cqe_next = (elm)->field.cqe_next) ==		\
	    DWC_CIRCLEQ_END(head))					\
		(head).cqh_last = (elm2);				\
	else								\
		(elm2)->field.cqe_next->field.cqe_prev = (elm2);	\
	if (((elm2)->field.cqe_prev = (elm)->field.cqe_prev) ==		\
	    DWC_CIRCLEQ_END(head))					\
		(head).cqh_first = (elm2);				\
	else								\
		(elm2)->field.cqe_prev->field.cqe_next = (elm2);	\
} while (0)

#ifdef __cplusplus
}
#endif

#endif /* _DWC_LIST_H_ */
