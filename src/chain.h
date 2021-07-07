// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef CHAIN_H
#define CHAIN_H

#include <stddef.h>

#define Chain__HEAD_FIELD _c_head
#define Chain__TAIL_FIELD _c_tail
#define Chain__NEXT_FIELD _c_next

#define Chain_PROTOCOL(t) t * Chain__HEAD_FIELD; t * Chain__TAIL_FIELD;
#define Chain_NODE_PROTOCOL(t) t * Chain__NEXT_FIELD;

// generic chain type
typedef struct {
    Chain_PROTOCOL(void)
} chain_t;

// generic chain node type
typedef struct {
    Chain_NODE_PROTOCOL(void)
} chain_node_t;

#define Chain_INIT(c) (c)->Chain__HEAD_FIELD = NULL
#define Chain_NODE_INIT(n) (n)->Chain__NEXT_FIELD = NULL

#define Chain__ADD(c, h, t)                                         \
    if ((c)->Chain__HEAD_FIELD == NULL) {                           \
        (c)->Chain__HEAD_FIELD = (h);                               \
    } else {                                                        \
        (c)->Chain__TAIL_FIELD->Chain__NEXT_FIELD = (h);            \
    }                                                               \
    (c)->Chain__TAIL_FIELD = (t)

#define Chain_APPEND(c, n) Chain__ADD(c, n, n)
#define Chain_EXTEND(c, cc) Chain__ADD(c, (cc)->Chain__HEAD_FIELD, (cc)->Chain__TAIL_FIELD)
#define Chain_HEAD(c) ((c)->Chain__HEAD_FIELD)
#define Chain_NODE_NEXT(n) ((n)->Chain__NEXT_FIELD)
#define Chain_FOREACH(n, c)                                         \
  for ((n) = Chain_HEAD(c); (n) != NULL; (n) = Chain_NODE_NEXT(n))

#define Chain_CLEAR(n, c)                                           \
  while ((n) = Chain_HEAD(c),                                       \
         Chain_HEAD(c) = (n) ? Chain_NODE_NEXT(n): NULL,            \
         (n) ? Chain_NODE_NEXT(n) = NULL : 0,                       \
         (n))

#endif /* CHAIN_H */
