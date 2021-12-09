// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_CORE_CHAIN_H
#define PROMISEDIO_CORE_CHAIN_H

#include <stddef.h>

#define Chain__HEAD_FIELD _c_head
#define Chain__TAIL_FIELD _c_tail
#define Chain__NEXT_FIELD _c_next

#define Chain_ROOT(t) t * Chain__HEAD_FIELD; t * Chain__TAIL_FIELD;
#define Chain_NODE(t) t * Chain__NEXT_FIELD;

#define Chain_INIT(c) (c)->Chain__HEAD_FIELD = NULL
#define Chain_NODEINIT(n) (n)->Chain__NEXT_FIELD = NULL

#define Chain_HEAD(c) ((c)->Chain__HEAD_FIELD)
#define Chain_NEXT(n) ((n)->Chain__NEXT_FIELD)
#define Chain_TAIL(c) ((c)->Chain__HEAD_FIELD ? (c)->Chain__TAIL_FIELD: NULL)

#define Chain__ADD(c, h, t)                                     \
    do {                                                        \
        if ((c)->Chain__HEAD_FIELD == NULL) {                   \
            (c)->Chain__HEAD_FIELD = (h);                       \
        } else {                                                \
            (c)->Chain__TAIL_FIELD->Chain__NEXT_FIELD = (h);    \
        }                                                       \
        (c)->Chain__TAIL_FIELD = (t);                           \
    } while (0)

#define Chain__ADDLEFT(c, n)                                \
    do {                                                    \
        (n)->Chain__NEXT_FIELD = (c)->Chain__HEAD_FIELD;    \
        (c)->Chain__HEAD_FIELD = (n);                       \
    } while (0)

#define Chain_APPEND(c, n)      \
    do {                        \
        Chain__ADD(c, n, n);    \
        Chain_NODEINIT(n);      \
    } while(0)

#define Chain_EXTEND(c, cc)     \
    Chain__ADD(c, (cc)->Chain__HEAD_FIELD, (cc)->Chain__TAIL_FIELD)

#define Chain_APPENDLEFT(c, n)  \
    do {                        \
        Chain_NODEINIT(n);      \
        Chain__ADDLEFT(c, n);   \
    } while(0)

#define Chain_MOVE(c, cc)       \
    do {                        \
        Chain_EXTEND(c, cc);    \
        Chain_INIT(cc);         \
    } while(0)

#define Chain_FOREACH(it, c)    \
  for ((it) = Chain_HEAD(c); (it) != NULL; (it) = Chain_NEXT(it))

#define Chain_PULLALL(it, c)                            \
  while ((it) = Chain_HEAD(c),                          \
         Chain_HEAD(c) = (it) ? Chain_NEXT(it): NULL,   \
         (it) ? Chain_NEXT(it) = NULL : (void) 0,       \
         (it))

#define Chain_PULLONE(it, c)                            \
     if ((it) = Chain_HEAD(c),                          \
        Chain_HEAD(c) = (it) ? Chain_NEXT(it): NULL,    \
        (it) ? Chain_NEXT(it) = NULL : (void) 0,        \
        (it))

#endif /* CHAIN_H */
