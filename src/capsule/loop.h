
// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_LOOP_API
#define PROMISEDIO_LOOP_API

#include "loop/loop.h"

#define LOOP_API loop_api_213e92bb8996a97c068288110bac7fb8

#define LOOP_GET_ID 0
#define Loop_Get() \
  (*(uv_loop_t * (*) (void*))(_ctx->loop_api_213e92bb8996a97c068288110bac7fb8__api[LOOP_GET_ID]))( \
    _ctx->loop_api_213e92bb8996a97c068288110bac7fb8__ctx)

#endif
