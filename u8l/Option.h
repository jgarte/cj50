/*
  Copyright (C) 2021-2023 Christian Jaeger, <ch@christianjaeger.ch>
  Published under the terms of the MIT License, see the LICENSE file.
*/

#ifndef OPTION_H_
#define OPTION_H_

#include <stdbool.h>
#include "util.h"
#include "macro-util.h"


#define Option_(T) XCAT(Option_,T)

#define DEFTYPE_Option_(T)                                      \
    typedef struct {                                            \
        bool is_none;                                           \
        T value;                                                \
    } Option_(T);                                               \
                                                                \
    /* We don't drop the `value` as it may have */              \
    /* changed ownership in the mean time! */                   \
    static inline UNUSED                                        \
    void XCAT(Option_(T),_drop)(const Option_(T) s) { }         \
                                                                \
    static UNUSED                                               \
    bool XCAT(Option_(T),_equal)(const Option_(T) *a,           \
                                 const Option_(T) *b) {         \
        return ((a->is_none == b->is_none) &&                   \
                (a->is_none ? true                              \
                 : XCAT(T, _equal)(&a->value, &b->value)));     \
    }

#define None(T)                                 \
    (Option_(T)) { .is_none = true }
#define Some(T, val)                            \
    (Option_(T)) { .is_none = false, .value = val }

#define Option_is_some(v) (!(v).is_none)


#endif /* OPTION_H_ */