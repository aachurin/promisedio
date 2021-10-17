// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

#ifndef PROMISEDIO_NS_SSL_H
#define PROMISEDIO_NS_SSL_H

#include "core/base.h"
#include "core/module.h"
#include "openssl/ssl.h"
#include "openssl/err.h"
//#include "openssl/evp.h"
//#include "openssl/x509.h"
//#include "openssl/rsa.h"
//#include "openssl/crypto.h"
//#include "openssl/x509.h"
//#include "openssl/x509v3.h"
//#include "openssl/pem.h"
//#include "openssl/rand.h"
//#include "openssl/bio.h"
//#include "openssl/dh.h"

typedef void _sslmodulestate;
#include PYSRC(ssl.h)

SSL *ssl_create_from_sslcontext(SSL_CTX *ctx);
SSL *ssl_create_from_pysslcontext(PySSLContext *ctx);

#endif
