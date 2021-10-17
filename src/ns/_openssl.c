#include <openssl/ssl.h>
#include <openssl/err.h>


#ifdef WITH_LIBSSL
    PyTypeObject *PySSLContextType;
    PyObject *PySSLErrorObject;
    PyObject *PySSLCertVerificationErrorObject;
    PyObject *PySSLZeroReturnErrorObject;
    PyObject *PySSLSyscallErrorObject;
    PyObject *err_codes_to_names;
    PyObject *err_names_to_codes;
    PyObject *lib_codes_to_names;

static PyObject *
_setSSLError(_STATE_var, const char *errstr, int errcode, const char *filename, int lineno)
{
    if (errstr == NULL)
        errcode = ERR_peek_last_error();
    else
        errcode = 0;
    fill_and_set_sslerror(state, NULL, _state->PySSLErrorObject, errcode, errstr, lineno, errcode);
    ERR_clear_error();
    return NULL;
}

static BIO *
create_bio()
{
    BIO *bio = BIO_new(BIO_s_mem());
    if (!bio) {
        PyErr_SetString(PyExc_MemoryError, "failed to allocate BIO");
        return NULL;
    }
    BIO_set_retry_read(bio);
    BIO_set_mem_eof_return(bio, -1);
    return bio;
}

SSL *
ssl_create_from_sslcontext(SSL_CTX *ctx)
{
    SSL *ssl;
    BIO *incoming;
    BIO *outgoing;
    incoming = create_bio();
    if (!incoming)
        return NULL;
    outgoing = create_bio();
    if (!outgoing) {
        BIO_free(incoming);
        return NULL;
    }
    ssl = SSL_new(ctx);
    SSL_set_bio(ssl, incoming, outgoing);
    return ssl
}

SSL *
ssl_create_from_pysslcontext(PySSLContext *ctx)
{
    return ssl_create_from_sslcontext(ctx->ctx);
}


static void
fill_and_set_sslerror(_STATE_var,
                      SSL *ssl, PyObject *type, int ssl_errno,
                      const char *errstr, int lineno, unsigned long errcode)
{
    PyObject *err_value = NULL, *reason_obj = NULL, *lib_obj = NULL;
    PyObject *verify_obj = NULL, *verify_code_obj = NULL;
    PyObject *init_value, *msg, *key;
    _Py_IDENTIFIER(reason);
    _Py_IDENTIFIER(library);
    _Py_IDENTIFIER(verify_message);
    _Py_IDENTIFIER(verify_code);

    if (errcode != 0) {
        int lib, reason;

        lib = ERR_GET_LIB(errcode);
        reason = ERR_GET_REASON(errcode);
        key = Py_BuildValue("ii", lib, reason);
        if (key == NULL)
            goto fail;
        reason_obj = PyDict_GetItemWithError(state->err_codes_to_names, key);
        Py_DECREF(key);
        if (reason_obj == NULL && PyErr_Occurred()) {
            goto fail;
        }
        key = PyLong_FromLong(lib);
        if (key == NULL)
            goto fail;
        lib_obj = PyDict_GetItemWithError(state->lib_codes_to_names, key);
        Py_DECREF(key);
        if (lib_obj == NULL && PyErr_Occurred()) {
            goto fail;
        }
        if (errstr == NULL)
            errstr = ERR_reason_error_string(errcode);
    }
    if (errstr == NULL)
        errstr = "unknown error";

    /* verify code for cert validation error */
    if ((ssl != NULL) && (type == state->PySSLCertVerificationErrorObject)) {
        const char *verify_str = NULL;
        long verify_code;

        verify_code = SSL_get_verify_result(ssl);
        verify_code_obj = PyLong_FromLong(verify_code);
        if (verify_code_obj == NULL) {
            goto fail;
        }

        switch (verify_code) {
        case X509_V_ERR_HOSTNAME_MISMATCH:
            verify_obj = PyUnicode_FromFormat(
                "Hostname mismatch, certificate is not valid for '%S'.",
                sslsock->server_hostname
            );
            break;
        case X509_V_ERR_IP_ADDRESS_MISMATCH:
            verify_obj = PyUnicode_FromFormat(
                "IP address mismatch, certificate is not valid for '%S'.",
                sslsock->server_hostname
            );
            break;
        default:verify_str = X509_verify_cert_error_string(verify_code);
            if (verify_str != NULL) {
                verify_obj = PyUnicode_FromString(verify_str);
            } else {
                verify_obj = Py_None;
                Py_INCREF(verify_obj);
            }
            break;
        }
        if (verify_obj == NULL) {
            goto fail;
        }
    }

    if (verify_obj && reason_obj && lib_obj)
        msg = PyUnicode_FromFormat("[%S: %S] %s: %S (_ssl.c:%d)",
                                   lib_obj, reason_obj, errstr, verify_obj,
                                   lineno);
    else if (reason_obj && lib_obj)
        msg = PyUnicode_FromFormat("[%S: %S] %s (_ssl.c:%d)",
                                   lib_obj, reason_obj, errstr, lineno);
    else if (lib_obj)
        msg = PyUnicode_FromFormat("[%S] %s (_ssl.c:%d)",
                                   lib_obj, errstr, lineno);
    else
        msg = PyUnicode_FromFormat("%s (_ssl.c:%d)", errstr, lineno);
    if (msg == NULL)
        goto fail;

    init_value = Py_BuildValue("iN", ERR_GET_REASON(ssl_errno), msg);
    if (init_value == NULL)
        goto fail;

    err_value = PyObject_CallObject(type, init_value);
    Py_DECREF(init_value);
    if (err_value == NULL)
        goto fail;

    if (reason_obj == NULL)
        reason_obj = Py_None;
    if (_PyObject_SetAttrId(err_value, &PyId_reason, reason_obj))
        goto fail;

    if (lib_obj == NULL)
        lib_obj = Py_None;
    if (_PyObject_SetAttrId(err_value, &PyId_library, lib_obj))
        goto fail;

    if ((sslsock != NULL) && (type == state->PySSLCertVerificationErrorObject)) {
        /* Only set verify code / message for SSLCertVerificationError */
        if (_PyObject_SetAttrId(err_value, &PyId_verify_code,
                                verify_code_obj))
            goto fail;
        if (_PyObject_SetAttrId(err_value, &PyId_verify_message, verify_obj))
            goto fail;
    }

    PyErr_SetObject(type, err_value);
fail:
    Py_XDECREF(err_value);
    Py_XDECREF(verify_code_obj);
    Py_XDECREF(verify_obj);
}

static PyObject *
set_ssl_error(_STATE_var, const char *errstr, int errcode, const char *filename, int lineno)
{
    if (errstr == NULL)
        errcode = ERR_peek_last_error();
    else
        errcode = 0;
    fill_and_set_sslerror(_state, NULL, _state->PySSLErrorObject, errcode, errstr, lineno, errcode);
    ERR_clear_error();
    return NULL;
}

static inline BIO *
create_bio()
{
    BIO *bio = BIO_new(BIO_s_mem());
    if (!bio) {
        PyErr_SetString(PyExc_MemoryError, "failed to allocate BIO");
        return NULL;
    }
    BIO_set_retry_read(bio);
    BIO_set_mem_eof_return(bio, -1);
    return bio;
}

SSL *
create_ssl(_STATE_var, PySSLContext *sslctx, enum py_ssl_server_or_client socket_type, char *server_hostname)
{

    SSL *ssl;
    BIO *incoming;
    BIO *outgoing;
    SSL_CTX *ctx = sslctx->ctx;
    if ((socket_type == PY_SSL_SERVER) &&
        (sslctx->protocol == PY_SSL_VERSION_TLS_CLIENT)) {
        set_ssl_error(_state,
                     "Cannot create a server socket with a "
                     "PROTOCOL_TLS_CLIENT context", 0, __FILE__, __LINE__);
        return NULL;
    }
    if ((socket_type == PY_SSL_CLIENT) &&
        (sslctx->protocol == PY_SSL_VERSION_TLS_SERVER)) {
        set_ssl_error(_state,
                     "Cannot create a client socket with a "
                     "PROTOCOL_TLS_SERVER context", 0, __FILE__, __LINE__);
        return NULL;
    }
    incoming = create_bio();
    if (!incoming)
        return NULL;
    outgoing = create_bio();
    if (!outgoing) {
        BIO_free(incoming);
        return NULL;
    }
    Py_BEGIN_ALLOW_THREADS
        ssl = SSL_new(ctx->ctx);
    Py_END_ALLOW_THREADS
    if (!ssl) {
        BIO_free(incoming);
        BIO_free(outgoing);
        set_ssl_error(_state, NULL, 0, __FILE__, __LINE__);
        return NULL;
    }
    SSL_set_bio(ssl, incoming, outgoing);
    return ssl
}

static PySSLSocket *
newPySSLSocket(PySSLContext *sslctx, PySocketSockObject *sock,
               enum py_ssl_server_or_client socket_type,
               char *server_hostname,
               PyObject *owner, PyObject *session,
               PySSLMemoryBIO *inbio, PySSLMemoryBIO *outbio)
{
    PySSLSocket *self;
    SSL_CTX *ctx = sslctx->ctx;
    _PySSLError err = {0};

    if ((socket_type == PY_SSL_SERVER) &&
        (sslctx->protocol == PY_SSL_VERSION_TLS_CLIENT)) {
        _setSSLError(get_state_ctx(sslctx),
                     "Cannot create a server socket with a "
                     "PROTOCOL_TLS_CLIENT context", 0, __FILE__, __LINE__);
        return NULL;
    }
    if ((socket_type == PY_SSL_CLIENT) &&
        (sslctx->protocol == PY_SSL_VERSION_TLS_SERVER)) {
        _setSSLError(get_state_ctx(sslctx),
                     "Cannot create a client socket with a "
                     "PROTOCOL_TLS_SERVER context", 0, __FILE__, __LINE__);
        return NULL;
    }

    self = PyObject_GC_New(PySSLSocket,
                           get_state_ctx(sslctx)->PySSLSocket_Type);
    if (self == NULL)
        return NULL;

    self->ssl = NULL;
    self->Socket = NULL;
    self->ctx = sslctx;
    Py_INCREF(sslctx);
    self->shutdown_seen_zero = 0;
    self->owner = NULL;
    self->server_hostname = NULL;
    self->err = err;
    self->exc_type = NULL;
    self->exc_value = NULL;
    self->exc_tb = NULL;

    /* Make sure the SSL error state is initialized */
    ERR_clear_error();

    PySSL_BEGIN_ALLOW_THREADS
    self->ssl = SSL_new(ctx);
    PySSL_END_ALLOW_THREADS
    if (self->ssl == NULL) {
        Py_DECREF(self);
        _setSSLError(get_state_ctx(self), NULL, 0, __FILE__, __LINE__);
        return NULL;
    }
    /* bpo43522 and OpenSSL < 1.1.1l: copy hostflags manually */
#if !defined(LIBRESSL_VERSION_NUMBER) && OPENSSL_VERSION < 0x101010cf
    X509_VERIFY_PARAM *ssl_params = SSL_get0_param(self->ssl);
    X509_VERIFY_PARAM_set_hostflags(ssl_params, sslctx->hostflags);
#endif
    SSL_set_app_data(self->ssl, self);
    if (sock) {
        SSL_set_fd(self->ssl, Py_SAFE_DOWNCAST(sock->sock_fd, SOCKET_T, int));
    } else {
        /* BIOs are reference counted and SSL_set_bio borrows our reference.
         * To prevent a double free in memory_bio_dealloc() we need to take an
         * extra reference here. */
        BIO_up_ref(inbio->bio);
        BIO_up_ref(outbio->bio);
        SSL_set_bio(self->ssl, inbio->bio, outbio->bio);
    }
    SSL_set_mode(self->ssl,
                 SSL_MODE_ACCEPT_MOVING_WRITE_BUFFER | SSL_MODE_AUTO_RETRY);

#ifdef TLS1_3_VERSION
    if (sslctx->post_handshake_auth == 1) {
        if (socket_type == PY_SSL_SERVER) {
            /* bpo-37428: OpenSSL does not ignore SSL_VERIFY_POST_HANDSHAKE.
             * Set SSL_VERIFY_POST_HANDSHAKE flag only for server sockets and
             * only in combination with SSL_VERIFY_PEER flag. */
            int mode = SSL_get_verify_mode(self->ssl);
            if (mode & SSL_VERIFY_PEER) {
                int (*verify_cb)(int, X509_STORE_CTX *) = NULL;
                verify_cb = SSL_get_verify_callback(self->ssl);
                mode |= SSL_VERIFY_POST_HANDSHAKE;
                SSL_set_verify(self->ssl, mode, verify_cb);
            }
        } else {
            /* client socket */
            SSL_set_post_handshake_auth(self->ssl, 1);
        }
    }
#endif

    if (server_hostname != NULL) {
        if (_ssl_configure_hostname(self, server_hostname) < 0) {
            Py_DECREF(self);
            return NULL;
        }
    }
    /* If the socket is in non-blocking mode or timeout mode, set the BIO
     * to non-blocking mode (blocking is the default)
     */
    if (sock && sock->sock_timeout >= 0) {
        BIO_set_nbio(SSL_get_rbio(self->ssl), 1);
        BIO_set_nbio(SSL_get_wbio(self->ssl), 1);
    }

    PySSL_BEGIN_ALLOW_THREADS
    if (socket_type == PY_SSL_CLIENT)
        SSL_set_connect_state(self->ssl);
    else
        SSL_set_accept_state(self->ssl);
    PySSL_END_ALLOW_THREADS

    self->socket_type = socket_type;
    if (sock != NULL) {
        self->Socket = PyWeakref_NewRef((PyObject *) sock, NULL);
        if (self->Socket == NULL) {
            Py_DECREF(self);
            return NULL;
        }
    }
    if (owner && owner != Py_None) {
        if (PySSL_set_owner(self, owner, NULL) == -1) {
            Py_DECREF(self);
            return NULL;
        }
    }
    if (session && session != Py_None) {
        if (PySSL_set_session(self, session, NULL) == -1) {
            Py_DECREF(self);
            return NULL;
        }
    }

    PyObject_GC_Track(self);
    return self;
}

