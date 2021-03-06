// Auto-generated

#ifndef PROMISEDIO_TIMER_API
#define PROMISEDIO_TIMER_API

#include "timer/timer.h"

static int timer_api_f2ceae52ebdf6921fd5c87857bd4ff2c__api_loaded = 0;
static void *timer_api_f2ceae52ebdf6921fd5c87857bd4ff2c__api[3];

#define TIMER_API timer_api_f2ceae52ebdf6921fd5c87857bd4ff2c

#define TIMER_TIMEOUT_ID 0
#define Timer_Timeout(...) \
  ((Promise * (*) (void*, uint64_t timeout))(timer_api_f2ceae52ebdf6921fd5c87857bd4ff2c__api[TIMER_TIMEOUT_ID]))( \
    _ctx->timer_api_f2ceae52ebdf6921fd5c87857bd4ff2c__ctx, __VA_ARGS__)

#define TIMER_START_ID 1
#define Timer_Start(...) \
  ((PyObject * (*) (void*, PyObject *func, uint64_t timeout, uint64_t repeat))(timer_api_f2ceae52ebdf6921fd5c87857bd4ff2c__api[TIMER_START_ID]))( \
    _ctx->timer_api_f2ceae52ebdf6921fd5c87857bd4ff2c__ctx, __VA_ARGS__)

#define TIMER_STOP_ID 2
#define Timer_Stop(...) \
  ((int (*) (void*, PyObject *ob))(timer_api_f2ceae52ebdf6921fd5c87857bd4ff2c__api[TIMER_STOP_ID]))( \
    _ctx->timer_api_f2ceae52ebdf6921fd5c87857bd4ff2c__ctx, __VA_ARGS__)

#endif
