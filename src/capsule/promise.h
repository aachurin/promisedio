// Auto-generated

#ifndef PROMISEDIO_PROMISE_API
#define PROMISEDIO_PROMISE_API

#include "promise/promise.h"

static int promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api_loaded = 0;
static void *promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[21];

#define PROMISE_API promise_api_2afead2f9b2edc52f3cf55b2ae227e32

#define PROMISE_STARTLOOP_ID 0
#define Promise_StartLoop(...) \
  ((int (*) (void*, unlockloop unlock_func, void *ctx))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[PROMISE_STARTLOOP_ID]))( \
    _ctx->promise_api_2afead2f9b2edc52f3cf55b2ae227e32__ctx, __VA_ARGS__)

#define PROMISE_STOPLOOP_ID 1
#define Promise_StopLoop(...) \
  ((int (*) (void*, unlockloop unlock_func, void *ctx))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[PROMISE_STOPLOOP_ID]))( \
    _ctx->promise_api_2afead2f9b2edc52f3cf55b2ae227e32__ctx, __VA_ARGS__)

#define PROMISE_NEW_ID 2
#define Promise_New() \
  ((Promise * (*) (void*))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[PROMISE_NEW_ID]))( \
    _ctx->promise_api_2afead2f9b2edc52f3cf55b2ae227e32__ctx)

#define PROMISE_CALLBACK_ID 3
#define Promise_Callback(...) \
  ((void (*) (Promise *self, promisecb fulfilled, promisecb rejected))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[PROMISE_CALLBACK_ID]))( \
    __VA_ARGS__)

#define PROMISE_PYCALLBACK_ID 4
#define Promise_PyCallback(...) \
  ((void (*) (Promise *self, PyObject *fulfilled, PyObject *rejected))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[PROMISE_PYCALLBACK_ID]))( \
    __VA_ARGS__)

#define PROMISE_NEWRESOLVED_ID 5
#define Promise_NewResolved(...) \
  ((Promise * (*) (void*, PyObject *value, PyObject *func))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[PROMISE_NEWRESOLVED_ID]))( \
    _ctx->promise_api_2afead2f9b2edc52f3cf55b2ae227e32__ctx, __VA_ARGS__)

#define PROMISE_THEN_ID 6
#define Promise_Then(...) \
  ((Promise * (*) (Promise *self))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[PROMISE_THEN_ID]))( \
    __VA_ARGS__)

#define PROMISE_RESOLVEEX_ID 7
#define Promise_ResolveEx(...) \
  ((void (*) (Promise *self, PyObject *value, int invoke_callback))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[PROMISE_RESOLVEEX_ID]))( \
    __VA_ARGS__)

#define PROMISE_RESOLVE_ID 8
#define Promise_Resolve(...) \
  ((void (*) (Promise *self, PyObject *value))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[PROMISE_RESOLVE_ID]))( \
    __VA_ARGS__)

#define PROMISE_REJECTEX_ID 9
#define Promise_RejectEx(...) \
  ((void (*) (Promise *self, PyObject *value, int invoke_callback))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[PROMISE_REJECTEX_ID]))( \
    __VA_ARGS__)

#define PROMISE_REJECT_ID 10
#define Promise_Reject(...) \
  ((void (*) (Promise *self, PyObject *value))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[PROMISE_REJECT_ID]))( \
    __VA_ARGS__)

#define PROMISE_REJECTARGSEX_ID 11
#define Promise_RejectArgsEx(...) \
  ((void (*) (Promise *self, PyObject *exc, PyObject *args, int invoke_callback))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[PROMISE_REJECTARGSEX_ID]))( \
    __VA_ARGS__)

#define PROMISE_REJECTARGS_ID 12
#define Promise_RejectArgs(...) \
  ((void (*) (Promise *self, PyObject *exc, PyObject *args))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[PROMISE_REJECTARGS_ID]))( \
    __VA_ARGS__)

#define PROMISE_REJECTSTRINGEX_ID 13
#define Promise_RejectStringEx(...) \
  ((void (*) (Promise *self, PyObject *exc, const char *msg, int invoke_callback))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[PROMISE_REJECTSTRINGEX_ID]))( \
    __VA_ARGS__)

#define PROMISE_REJECTSTRING_ID 14
#define Promise_RejectString(...) \
  ((void (*) (Promise *self, PyObject *exc, const char *msg))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[PROMISE_REJECTSTRING_ID]))( \
    __VA_ARGS__)

#define PROMISE_CLEARCHAIN_ID 15
#define Promise_ClearChain() \
  ((void (*) (void*))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[PROMISE_CLEARCHAIN_ID]))( \
    _ctx->promise_api_2afead2f9b2edc52f3cf55b2ae227e32__ctx)

#define PROMISE_PROCESSCHAIN_ID 16
#define Promise_ProcessChain() \
  ((int (*) (void*))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[PROMISE_PROCESSCHAIN_ID]))( \
    _ctx->promise_api_2afead2f9b2edc52f3cf55b2ae227e32__ctx)

#define PROMISE_GETSTATS_ID 17
#define Promise_GetStats(...) \
  ((int (*) (void*, Py_ssize_t *active_promises))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[PROMISE_GETSTATS_ID]))( \
    _ctx->promise_api_2afead2f9b2edc52f3cf55b2ae227e32__ctx, __VA_ARGS__)

#define LOCK_NEW_ID 18
#define Lock_New() \
  ((Lock * (*) (void*))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[LOCK_NEW_ID]))( \
    _ctx->promise_api_2afead2f9b2edc52f3cf55b2ae227e32__ctx)

#define LOCK_ACQUIRE_ID 19
#define Lock_Acquire(...) \
  ((Promise * (*) (Lock *self))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[LOCK_ACQUIRE_ID]))( \
    __VA_ARGS__)

#define LOCK_RELEASE_ID 20
#define Lock_Release(...) \
  ((void (*) (Lock *self))(promise_api_2afead2f9b2edc52f3cf55b2ae227e32__api[LOCK_RELEASE_ID]))( \
    __VA_ARGS__)

#endif
