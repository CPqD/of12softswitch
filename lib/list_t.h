/* Copyright (c) 2008 The Board of Trustees of The Leland Stanford
 * Junior University
 * 
 * We are making the OpenFlow specification and associated documentation
 * (Software) available for public use and benefit with the expectation
 * that others will use, modify and enhance the Software and contribute
 * those enhancements back to the community. However, since we would
 * like to make the Software available for broadest use, with as few
 * restrictions as possible permission is hereby granted, free of
 * charge, to any person obtaining a copy of this Software to deal in
 * the Software under the copyrights without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 * 
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT.  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * The name and trademarks of copyright holder(s) may NOT be used in
 * advertising or publicity pertaining to the Software or any
 * derivatives without specific, written prior permission.
 */
#ifndef LIST_T_H
#define LIST_T_H 1

/* Doubly linked list. */

#include <stdbool.h>
#include <stddef.h>
#include "util.h"

/* Doubly linked list head or element. */
typedef struct list_t
  {
    struct list_t *prev;     /* Previous list element. */
    struct list_t *next;     /* Next list element. */
}list_t;

#define LIST_INITIALIZER(LIST) { LIST, LIST }
#ifdef __cplusplus
  extern "C"
  {
#endif

void list_t_init(list_t *);

/* List insertion. */
void list_t_insert(list_t *, list_t *);
void list_t_splice(list_t *before, list_t *first, list_t *last);
void list_t_push_front(list_t *, list_t *);
void list_t_push_back(list_t *, list_t *);
void list_t_replace(list_t *, const list_t *);

/* List removal. */
list_t *list_t_remove(list_t *);
list_t *list_t_pop_front(list_t *);
list_t *list_t_pop_back(list_t *);

/* List elements. */
list_t *list_t_front(list_t *);
list_t *list_t_back(list_t *);

/* List properties. */
size_t list_t_size(const list_t *);
bool list_t_is_empty(const list_t *);

#ifdef __cplusplus
  }
#endif

#define LIST_FOR_EACH(ITER, STRUCT, MEMBER, LIST)                   \
    for (ITER = CONTAINER_OF((LIST)->next, STRUCT, MEMBER);         \
         &(ITER)->MEMBER != (LIST);                                 \
         ITER = CONTAINER_OF((ITER)->MEMBER.next, STRUCT, MEMBER))
#define LIST_FOR_EACH_REVERSE(ITER, STRUCT, MEMBER, LIST)           \
    for (ITER = CONTAINER_OF((LIST)->prev, STRUCT, MEMBER);         \
         &(ITER)->MEMBER != (LIST);                                 \
         ITER = CONTAINER_OF((ITER)->MEMBER.prev, STRUCT, MEMBER))
#define LIST_FOR_EACH_SAFE(ITER, NEXT, STRUCT, MEMBER, LIST)        \
    for (ITER = CONTAINER_OF((LIST)->next, STRUCT, MEMBER);         \
         (NEXT = CONTAINER_OF((ITER)->MEMBER.next, STRUCT, MEMBER), \
          &(ITER)->MEMBER != (LIST));                               \
         ITER = NEXT)

#endif /* list_t.h */
