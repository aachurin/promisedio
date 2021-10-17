#ifndef PYSOURCE_3_10_0_SSL_H
#define PYSOURCE_3_10_0_SSL_H

enum py_ssl_error {
    PY_SSL_ERROR_NONE,
    PY_SSL_ERROR_SSL,
    PY_SSL_ERROR_WANT_READ,
    PY_SSL_ERROR_WANT_WRITE,
    PY_SSL_ERROR_WANT_X509_LOOKUP,
    PY_SSL_ERROR_SYSCALL,     /* look at error stack/return value/errno */
    PY_SSL_ERROR_ZERO_RETURN,
    PY_SSL_ERROR_WANT_CONNECT,
    PY_SSL_ERROR_EOF,         /* special case of SSL_ERROR_SYSCALL */
    PY_SSL_ERROR_NO_SOCKET,   /* socket has been GC'd */
    PY_SSL_ERROR_INVALID_ERROR_CODE
};

enum py_ssl_server_or_client {
    PY_SSL_CLIENT,
    PY_SSL_SERVER
};

enum py_ssl_version {
    PY_SSL_VERSION_SSL2,
    PY_SSL_VERSION_SSL3=1,
    PY_SSL_VERSION_TLS, /* SSLv23 */
    PY_SSL_VERSION_TLS1,
    PY_SSL_VERSION_TLS1_1,
    PY_SSL_VERSION_TLS1_2,
    PY_SSL_VERSION_TLS_CLIENT=0x10,
    PY_SSL_VERSION_TLS_SERVER,
};

enum py_proto_version {
    PY_PROTO_MINIMUM_SUPPORTED = -2,
    PY_PROTO_SSLv3 = SSL3_VERSION,
    PY_PROTO_TLSv1 = TLS1_VERSION,
    PY_PROTO_TLSv1_1 = TLS1_1_VERSION,
    PY_PROTO_TLSv1_2 = TLS1_2_VERSION,
#ifdef TLS1_3_VERSION
    PY_PROTO_TLSv1_3 = TLS1_3_VERSION,
#else
    PY_PROTO_TLSv1_3 = 0x304,
#endif
    PY_PROTO_MAXIMUM_SUPPORTED = -1,

#if defined(SSL3_VERSION) && !defined(OPENSSL_NO_SSL3)
    PY_PROTO_MINIMUM_AVAILABLE = PY_PROTO_SSLv3,
#elif defined(TLS1_VERSION) && !defined(OPENSSL_NO_TLS1)
    PY_PROTO_MINIMUM_AVAILABLE = PY_PROTO_TLSv1,
#elif defined(TLS1_1_VERSION) && !defined(OPENSSL_NO_TLS1_1)
    PY_PROTO_MINIMUM_AVAILABLE = PY_PROTO_TLSv1_1,
#elif defined(TLS1_2_VERSION) && !defined(OPENSSL_NO_TLS1_2)
    PY_PROTO_MINIMUM_AVAILABLE = PY_PROTO_TLSv1_2,
#elif defined(TLS1_3_VERSION) && !defined(OPENSSL_NO_TLS1_3)
    PY_PROTO_MINIMUM_AVAILABLE = PY_PROTO_TLSv1_3,
#else
    #error "PY_PROTO_MINIMUM_AVAILABLE not found"
#endif

#if defined(TLS1_3_VERSION) && !defined(OPENSSL_NO_TLS1_3)
    PY_PROTO_MAXIMUM_AVAILABLE = PY_PROTO_TLSv1_3,
#elif defined(TLS1_2_VERSION) && !defined(OPENSSL_NO_TLS1_2)
    PY_PROTO_MAXIMUM_AVAILABLE = PY_PROTO_TLSv1_2,
#elif defined(TLS1_1_VERSION) && !defined(OPENSSL_NO_TLS1_1)
    PY_PROTO_MAXIMUM_AVAILABLE = PY_PROTO_TLSv1_1,
#elif defined(TLS1_VERSION) && !defined(OPENSSL_NO_TLS1)
    PY_PROTO_MAXIMUM_AVAILABLE = PY_PROTO_TLSv1,
#elif defined(SSL3_VERSION) && !defined(OPENSSL_NO_SSL3)
    PY_PROTO_MAXIMUM_AVAILABLE = PY_PROTO_SSLv3,
#else
    #error "PY_PROTO_MAXIMUM_AVAILABLE not found"
#endif
};

typedef struct {
    PyObject_HEAD
    SSL_CTX *ctx;
    unsigned char *alpn_protocols;
    unsigned int alpn_protocols_len;
    PyObject *set_sni_cb;
    int check_hostname;
    unsigned int hostflags;
    int protocol;
#ifdef TLS1_3_VERSION
    int post_handshake_auth;
#endif
    PyObject *msg_cb;
    PyObject *keylog_filename;
    BIO *keylog_bio;
    _sslmodulestate *state;
} PySSLContext;

typedef struct {
    PyObject_HEAD
    PyObject *Socket; /* weakref to socket on which we're layered */
    SSL *ssl;
    PySSLContext *ctx; /* weakref to SSL context */
    char shutdown_seen_zero;
    enum py_ssl_server_or_client socket_type;
    PyObject *owner; /* Python level "owner" passed to servername callback */
    PyObject *server_hostname;
    _PySSLError err; /* last seen error from various sources */
    PyObject *exc_type;
    PyObject *exc_value;
    PyObject *exc_tb;
} PySSLSocket;

typedef struct {
    PyObject_HEAD
    BIO *bio;
    int eof_written;
} PySSLMemoryBIO;

#define newPySSLSocket_hash 0x4a012785

#endif