
// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_PROMISE_API
#define PROMISEDIO_PROMISE_API

#include "promise/promise.h"

#define PROMISE_API promise_api_1029ccc52f383b0c85163efaa925059c

#define PROMISE_STARTLOOP_ID 0
#define Promise_StartLoop(...) \
  (*(int (*) (void*, unlockloop unlock_func, void *ctx))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[PROMISE_STARTLOOP_ID]))( \
    _ctx->promise_api_1029ccc52f383b0c85163efaa925059c__ctx, __VA_ARGS__)

#define PROMISE_STOPLOOP_ID 1
#define Promise_StopLoop(...) \
  (*(int (*) (void*, unlockloop unlock_func, void *ctx))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[PROMISE_STOPLOOP_ID]))( \
    _ctx->promise_api_1029ccc52f383b0c85163efaa925059c__ctx, __VA_ARGS__)

#define PROMISE_NEW_ID 2
#define Promise_New() \
  (*(Promise * (*) (void*))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[PROMISE_NEW_ID]))( \
    _ctx->promise_api_1029ccc52f383b0c85163efaa925059c__ctx)

#define PROMISE_CALLBACK_ID 3
#define Promise_Callback(...) \
  (*(void (*) (Promise *self, promisecb fulfilled, promisecb rejected))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[PROMISE_CALLBACK_ID]))( \
    __VA_ARGS__)

#define PROMISE_THEN_ID 4
#define Promise_Then(...) \
  (*(Promise * (*) (Promise *self))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[PROMISE_THEN_ID]))( \
    __VA_ARGS__)

#define PROMISE_NEWRESOLVED_ID 5
#define Promise_NewResolved(...) \
  (*(Promise * (*) (void*, PyObject *value))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[PROMISE_NEWRESOLVED_ID]))( \
    _ctx->promise_api_1029ccc52f383b0c85163efaa925059c__ctx, __VA_ARGS__)

#define PROMISE_RESOLVEEX_ID 6
#define Promise_ResolveEx(...) \
  (*(void (*) (Promise *self, PyObject *value, int invoke_callback))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[PROMISE_RESOLVEEX_ID]))( \
    __VA_ARGS__)

#define PROMISE_RESOLVE_ID 7
#define Promise_Resolve(...) \
  (*(void (*) (Promise *self, PyObject *value))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[PROMISE_RESOLVE_ID]))( \
    __VA_ARGS__)

#define PROMISE_REJECTEX_ID 8
#define Promise_RejectEx(...) \
  (*(void (*) (Promise *self, PyObject *value, int invoke_callback))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[PROMISE_REJECTEX_ID]))( \
    __VA_ARGS__)

#define PROMISE_REJECT_ID 9
#define Promise_Reject(...) \
  (*(void (*) (Promise *self, PyObject *value))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[PROMISE_REJECT_ID]))( \
    __VA_ARGS__)

#define PROMISE_REJECTARGSEX_ID 10
#define Promise_RejectArgsEx(...) \
  (*(void (*) (Promise *self, PyObject *exc, PyObject *args, int invoke_callback))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[PROMISE_REJECTARGSEX_ID]))( \
    __VA_ARGS__)

#define PROMISE_REJECTARGS_ID 11
#define Promise_RejectArgs(...) \
  (*(void (*) (Promise *self, PyObject *exc, PyObject *args))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[PROMISE_REJECTARGS_ID]))( \
    __VA_ARGS__)

#define PROMISE_REJECTSTRINGEX_ID 12
#define Promise_RejectStringEx(...) \
  (*(void (*) (Promise *self, PyObject *exc, const char *msg, int invoke_callback))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[PROMISE_REJECTSTRINGEX_ID]))( \
    __VA_ARGS__)

#define PROMISE_REJECTSTRING_ID 13
#define Promise_RejectString(...) \
  (*(void (*) (Promise *self, PyObject *exc, const char *msg))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[PROMISE_REJECTSTRING_ID]))( \
    __VA_ARGS__)

#define PROMISE_CLEARCHAIN_ID 14
#define Promise_ClearChain() \
  (*(void (*) (void*))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[PROMISE_CLEARCHAIN_ID]))( \
    _ctx->promise_api_1029ccc52f383b0c85163efaa925059c__ctx)

#define PROMISE_PROCESSCHAIN_ID 15
#define Promise_ProcessChain() \
  (*(Py_ssize_t (*) (void*))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[PROMISE_PROCESSCHAIN_ID]))( \
    _ctx->promise_api_1029ccc52f383b0c85163efaa925059c__ctx)

#define LOCK_NEW_ID 16
#define Lock_New() \
  (*(Lock * (*) (void*))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[LOCK_NEW_ID]))( \
    _ctx->promise_api_1029ccc52f383b0c85163efaa925059c__ctx)

#define LOCK_ACQUIRE_ID 17
#define Lock_Acquire(...) \
  (*(Promise * (*) (Lock *self))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[LOCK_ACQUIRE_ID]))( \
    __VA_ARGS__)

#define LOCK_RELEASE_ID 18
#define Lock_Release(...) \
  (*(void (*) (Lock *self))(_ctx->promise_api_1029ccc52f383b0c85163efaa925059c__api[LOCK_RELEASE_ID]))( \
    __VA_ARGS__)

#endif
