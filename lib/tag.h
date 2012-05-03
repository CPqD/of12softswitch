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

#ifndef TAG_H
#define TAG_H 1

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include "util.h"

/*
 * Tagging support.
 *
 * A 'tag' represents an arbitrary category.  Currently, tags are used to
 * represent categories of flows and in particular the dependencies for a flow
 * switching decision.  For example, if a flow's output port is based on
 * knowledge that source MAC 00:02:e3:0f:80:a4 is on eth0, then a tag that
 * represents that dependency is attached to that flow in the flowtracking hash
 * table.
 *
 * As this example shows, the universe of possible categories is very large,
 * and even the number of categories that are in use at a given time can be
 * very large.  This means that keeping track of category membership via
 * conventional means (lists, bitmaps, etc.) is likely to be expensive.
 *
 * Tags are actually implemented via a "superimposed coding", as discussed in
 * Knuth TAOCP v.3 section 6.5 "Retrieval on Secondary Keys".  A tag is an
 * unsigned integer in which exactly 2 bits are set to 1 and the rest set to 0.
 * For 32-bit integers (as currently used) there are 32 * 31 / 2 = 496 unique
 * tags; for 64-bit integers there are 64 * 63 / 2 = 2,016.
 *
 * Because there is a small finite number of unique tags, tags must collide
 * after some number of them have been created.  In practice we generally
 * create tags by choosing bits randomly.
 *
 * The key property of tags is that we can combine them without increasing the
 * amount of data required using bitwise-OR, since the result has the 1-bits
 * from both tags set.  The necessary tradeoff is that the result is even more
 * ambiguous: if combining two tags yields a value with 4 bits set to 1, then
 * the result value will test as having 4 * 3 / 2 = 6 unique tags, not just the
 * two tags that we combined.
 *
 * The upshot is this: a value that is the bitwise-OR combination of a number
 * of tags will always include the tags that were combined, but it may contain
 * any number of additional tags as well.  This is acceptable for flowtracking,
 * since we want to be sure that we catch every flow that needs to be
 * revalidated, but it is OK if we revalidate a few extra flows as well.
 *
 * If we combine too many tags, then the result will have every bit set, so
 * that it will test as including every tag.  Fortunately, this is not a big
 * problem for us: although there are many flows overall, each individual flow
 * belongs only to a small number of categories.
 */

/* Represents a tag, or the combination of 0 or more tags. */
typedef uint32_t tag_type;

tag_type tag_create_random(void);
tag_type tag_create_deterministic(uint32_t seed);
static inline bool tag_intersects(tag_type, tag_type);
static inline bool tag_is_valid(tag_type);

/* Returns true if 'a' and 'b' have at least one tag in common,
 * false if their set of tags is disjoint. . */
static inline bool
tag_intersects(tag_type a, tag_type b)
{
    tag_type x = a & b;
    return (x & (x - 1)) != 0;
}

/* Returns true if 'tag' is a valid tag, that is, if exactly two bits are set
 * to 1 and the rest to 0.   Otherwise, returns false. */
static inline bool
tag_is_valid(tag_type tag)
{
    tag_type x = tag & (tag - 1);
    tag_type y = x & (x - 1);
    return x && !y;
}

/*
 * A tag set accumulates tags with reduced ambiguity compared to a single tag.
 * The flow tracking uses tag sets to keep track of tags that need to
 * revalidated after a number of packets have been processed.
 */
#define TAG_SET_SIZE 4
struct tag_set {
    tag_type total;
    tag_type tags[TAG_SET_SIZE];
    unsigned int n;
};

void tag_set_init(struct tag_set *);
void tag_set_add(struct tag_set *, tag_type);
static inline bool tag_set_is_empty(const struct tag_set *);
static inline bool tag_set_intersects(const struct tag_set *, tag_type);

/* Returns true if 'set' will match no tags at all,
 * false if it will match at least one tag. */
static inline bool
tag_set_is_empty(const struct tag_set *set)
{
    return !set->n;
}

/* Returns true if any of the tags in 'tags' are also in 'set',
 * false if the intersection is empty. */
static inline bool
tag_set_intersects(const struct tag_set *set, tag_type tags)
{
    BUILD_ASSERT_DECL(TAG_SET_SIZE == 4);
    return (tag_intersects(set->total, tags)
            && (tag_intersects(set->tags[0], tags)
                || tag_intersects(set->tags[1], tags)
                || tag_intersects(set->tags[2], tags)
                || tag_intersects(set->tags[3], tags)));
}

#endif /* tag.h */
