
// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_NS_API
#define PROMISEDIO_NS_API

#include "ns/ns.h"

#define NS_API ns_api_4aaf83cd3102ad19b7f080e074b1b185

#define NS_GETADDRINFO_ID 0
#define Ns_GetAddrInfo(...) \
  (*(Promise * (*) (void*, const char *node, const char *service, int family, int type, int proto, int flags))(_ctx->ns_api_4aaf83cd3102ad19b7f080e074b1b185__api[NS_GETADDRINFO_ID]))( \
    _ctx->ns_api_4aaf83cd3102ad19b7f080e074b1b185__ctx, __VA_ARGS__)

#define NS_GETNAMEINFO_ID 1
#define Ns_GetNameInfo(...) \
  (*(Promise * (*) (void*, const struct sockaddr *addr, int flags))(_ctx->ns_api_4aaf83cd3102ad19b7f080e074b1b185__api[NS_GETNAMEINFO_ID]))( \
    _ctx->ns_api_4aaf83cd3102ad19b7f080e074b1b185__ctx, __VA_ARGS__)

#define TCPSTREAM_GETSOCKNAME_ID 2
#define TcpStream_GetSockName(...) \
  (*(PyObject * (*) (Stream *stream))(_ctx->ns_api_4aaf83cd3102ad19b7f080e074b1b185__api[TCPSTREAM_GETSOCKNAME_ID]))( \
    __VA_ARGS__)

#define TCPSTREAM_GETPEERNAME_ID 3
#define TcpStream_GetPeerName(...) \
  (*(PyObject * (*) (Stream *stream))(_ctx->ns_api_4aaf83cd3102ad19b7f080e074b1b185__api[TCPSTREAM_GETPEERNAME_ID]))( \
    __VA_ARGS__)

#define TCPSTREAM_SETTCPNODELAY_ID 4
#define TcpStream_SetTcpNoDelay(...) \
  (*(int (*) (Stream *stream, int enabled))(_ctx->ns_api_4aaf83cd3102ad19b7f080e074b1b185__api[TCPSTREAM_SETTCPNODELAY_ID]))( \
    __VA_ARGS__)

#define TCPSTREAM_SETTCPKEEPALIVE_ID 5
#define TcpStream_SetTcpKeepAlive(...) \
  (*(int (*) (Stream *stream, int delay))(_ctx->ns_api_4aaf83cd3102ad19b7f080e074b1b185__api[TCPSTREAM_SETTCPKEEPALIVE_ID]))( \
    __VA_ARGS__)

#define TCPSTREAM_CONNECT_ID 6
#define TcpStream_Connect(...) \
  (*(Promise * (*) (Stream *stream, const struct sockaddr *addr, PyObject *sslctx, const char *server_name))(_ctx->ns_api_4aaf83cd3102ad19b7f080e074b1b185__api[TCPSTREAM_CONNECT_ID]))( \
    __VA_ARGS__)

#define STREAM_CLOSE_ID 7
#define Stream_Close(...) \
  (*(Promise * (*) (Stream *stream))(_ctx->ns_api_4aaf83cd3102ad19b7f080e074b1b185__api[STREAM_CLOSE_ID]))( \
    __VA_ARGS__)

#define STREAM_SHUTDOWN_ID 8
#define Stream_Shutdown(...) \
  (*(Promise * (*) (Stream *stream))(_ctx->ns_api_4aaf83cd3102ad19b7f080e074b1b185__api[STREAM_SHUTDOWN_ID]))( \
    __VA_ARGS__)

#define STREAM_WRITE_ID 9
#define Stream_Write(...) \
  (*(Promise * (*) (Stream *stream, PyObject *data))(_ctx->ns_api_4aaf83cd3102ad19b7f080e074b1b185__api[STREAM_WRITE_ID]))( \
    __VA_ARGS__)

#define STREAM_READ_ID 10
#define Stream_Read(...) \
  (*(Promise * (*) (Stream *stream, Py_ssize_t n))(_ctx->ns_api_4aaf83cd3102ad19b7f080e074b1b185__api[STREAM_READ_ID]))( \
    __VA_ARGS__)

#define STREAM_READEXACTLY_ID 11
#define Stream_ReadExactly(...) \
  (*(Promise * (*) (Stream *stream, Py_ssize_t n))(_ctx->ns_api_4aaf83cd3102ad19b7f080e074b1b185__api[STREAM_READEXACTLY_ID]))( \
    __VA_ARGS__)

#define STREAM_READUNTIL_ID 12
#define Stream_ReadUntil(...) \
  (*(Promise * (*) (Stream *stream, char c))(_ctx->ns_api_4aaf83cd3102ad19b7f080e074b1b185__api[STREAM_READUNTIL_ID]))( \
    __VA_ARGS__)

#endif
