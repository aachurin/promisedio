
// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_PROMISE_API
#define PROMISEDIO_PROMISE_API

#include "promise/promise.h"

#define PROMISE_API promise_api_304cf823833a97ca179f3d8feb9178f7

#define PROMISE_NEW_ID 0
#define Promise_New() \
  (*(Promise * (*) (void*))(_ctx->promise_api_304cf823833a97ca179f3d8feb9178f7__api[PROMISE_NEW_ID]))( \
    _ctx->promise_api_304cf823833a97ca179f3d8feb9178f7__ctx)

#define PROMISE_CALLBACK_ID 1
#define Promise_Callback(...) \
  (*(void (*) (Promise *self, promisecb fulfilled, promisecb rejected))(_ctx->promise_api_304cf823833a97ca179f3d8feb9178f7__api[PROMISE_CALLBACK_ID]))( \
    __VA_ARGS__)

#define PROMISE_THEN_ID 2
#define Promise_Then(...) \
  (*(Promise * (*) (Promise *self))(_ctx->promise_api_304cf823833a97ca179f3d8feb9178f7__api[PROMISE_THEN_ID]))( \
    __VA_ARGS__)

#define PROMISE_NEWRESOLVED_ID 3
#define Promise_NewResolved(...) \
  (*(Promise * (*) (void*, PyObject *value))(_ctx->promise_api_304cf823833a97ca179f3d8feb9178f7__api[PROMISE_NEWRESOLVED_ID]))( \
    _ctx->promise_api_304cf823833a97ca179f3d8feb9178f7__ctx, __VA_ARGS__)

#define PROMISE_RESOLVE_ID 4
#define Promise_Resolve(...) \
  (*(void (*) (Promise *self, PyObject *value))(_ctx->promise_api_304cf823833a97ca179f3d8feb9178f7__api[PROMISE_RESOLVE_ID]))( \
    __VA_ARGS__)

#define PROMISE_REJECT_ID 5
#define Promise_Reject(...) \
  (*(void (*) (Promise *self, PyObject *value))(_ctx->promise_api_304cf823833a97ca179f3d8feb9178f7__api[PROMISE_REJECT_ID]))( \
    __VA_ARGS__)

#define PROMISE_REJECTARGS_ID 6
#define Promise_RejectArgs(...) \
  (*(void (*) (Promise *self, PyObject *exc, PyObject *args))(_ctx->promise_api_304cf823833a97ca179f3d8feb9178f7__api[PROMISE_REJECTARGS_ID]))( \
    __VA_ARGS__)

#define PROMISE_REJECTSTRING_ID 7
#define Promise_RejectString(...) \
  (*(void (*) (Promise *self, PyObject *exc, const char *msg))(_ctx->promise_api_304cf823833a97ca179f3d8feb9178f7__api[PROMISE_REJECTSTRING_ID]))( \
    __VA_ARGS__)

#define PROMISE_CLEARCHAIN_ID 8
#define Promise_ClearChain() \
  (*(void (*) (void*))(_ctx->promise_api_304cf823833a97ca179f3d8feb9178f7__api[PROMISE_CLEARCHAIN_ID]))( \
    _ctx->promise_api_304cf823833a97ca179f3d8feb9178f7__ctx)

#define PROMISE_PROCESSCHAIN_ID 9
#define Promise_ProcessChain() \
  (*(int (*) (void*))(_ctx->promise_api_304cf823833a97ca179f3d8feb9178f7__api[PROMISE_PROCESSCHAIN_ID]))( \
    _ctx->promise_api_304cf823833a97ca179f3d8feb9178f7__ctx)

#endif
