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
#include <config.h>
#include "list_t.h"
#include <assert.h>

/* Initializes 'list' as an empty list. */
void
list_t_init(struct list_t *list)
{
    list->next = list->prev = list;
}

/* Inserts 'elem' just before 'before'. */
void
list_t_insert(struct list_t *before, struct list_t *elem)
{
  elem->prev = before->prev;
  elem->next = before;
  before->prev->next = elem;
  before->prev = elem;
}

/* Removes elements 'first' though 'last' (exclusive) from their current list,
   then inserts them just before 'before'. */
void
list_t_splice(struct list_t *before, struct list_t *first, struct list_t *last)
{
  if (first == last)
    return;
  last = last->prev;

  /* Cleanly remove 'first'...'last' from its current list. */
  first->prev->next = last->next;
  last->next->prev = first->prev;

  /* Splice 'first'...'last' into new list. */
  first->prev = before->prev;
  last->next = before;
  before->prev->next = first;
  before->prev = last;
}

/* Inserts 'elem' at the beginning of 'list', so that it becomes the front in
   'list'. */
void
list_t_push_front(struct list_t *list, struct list_t *elem)
{
  list_t_insert(list->next, elem);
}

/* Inserts 'elem' at the end of 'list', so that it becomes the back in
 * 'list'. */
void
list_t_push_back(struct list_t *list, struct list_t *elem)
{
  list_t_insert(list, elem);
}

/* Puts 'elem' in the position currently occupied by 'position'.
 * Afterward, 'position' is not part of a list. */
void
list_t_replace(struct list_t *element, const struct list_t *position)
{
    element->next = position->next;
    element->next->prev = element;
    element->prev = position->prev;
    element->prev->next = element;
}

/* Removes 'elem' from its list and returns the element that followed it.
   Undefined behavior if 'elem' is not in a list. */
struct list_t *
list_t_remove(struct list_t *elem)
{
  elem->prev->next = elem->next;
  elem->next->prev = elem->prev;
  return elem->next;
}

/* Removes the front element from 'list' and returns it.  Undefined behavior if
   'list' is empty before removal. */
struct list_t *
list_t_pop_front(struct list_t *list)
{
  struct list_t *front = list->next;
  list_t_remove(front);
  return front;
}

/* Removes the back element from 'list' and returns it.
   Undefined behavior if 'list' is empty before removal. */
struct list_t *
list_t_pop_back(struct list_t *list)
{
  struct list_t *back = list->prev;
  list_t_remove(back);
  return back;
}

/* Returns the front element in 'list'.
   Undefined behavior if 'list' is empty. */
struct list_t *
list_t_front(struct list_t *list)
{
  assert(!list_t_is_empty(list));
  return list->next;
}

/* Returns the back element in 'list'.
   Undefined behavior if 'list' is empty. */
struct list_t *
list_t_back(struct list_t *list)
{
  assert(!list_t_is_empty(list));
  return list->prev;
}

/* Returns the number of elements in 'list'.
   Runs in O(n) in the number of elements. */
size_t
list_t_size(const struct list_t *list)
{
  const struct list_t *e;
  size_t cnt = 0;

  for (e = list->next; e != list; e = e->next)
    cnt++;
  return cnt;
}

/* Returns true if 'list' is empty, false otherwise. */
bool
list_t_is_empty(const struct list_t *list)
{
  return list->next == list;
}
