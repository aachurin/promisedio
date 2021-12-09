
// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_SIGNAL_API
#define PROMISEDIO_SIGNAL_API

#define SIGNAL_API signal_api_4e247f2dad1d0a8160b909b158b7b95e

#define SIGNAL_SETSIGEVENT_ID 0
#define Signal_SetSigEvent(...) \
  (*(void (*) (sigevent cb))(_ctx->signal_api_4e247f2dad1d0a8160b909b158b7b95e__api[SIGNAL_SETSIGEVENT_ID]))( \
    __VA_ARGS__)

#endif
