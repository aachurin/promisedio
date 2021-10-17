// Copyright (c) 2021 Andrey Churin (aachurin@gmail.com).
// This file is part of promisedio

Py_LOCAL_INLINE(int)
sslroutine_set_exception(StreamHandle *handle)
{
    _STATE_set(handle)
    if (!PyErr_ExceptionMatches(S(SSLWantReadError)) &&
        !PyErr_ExceptionMatches(S(SSLWantWriteError))) {
        stream_set_exception(handle, NULL);
        return 1;
    }
    return 0;
}

Py_LOCAL_INLINE(void)
sslroutine_stream_read(StreamHandle *handle, char *buffer, Py_ssize_t size)
{
    _Py_IDENTIFIER(write);
    PyObject *pybuffer = PyMemoryView_FromMemory(buffer, size, PyBUF_READ);
    if (!pybuffer) {
        sslroutine_set_exception(handle);
    } else {
        // should always success
        PyObject *ret = _PyObject_CallMethodIdOneArg(handle->inbio, &PyId_write, pybuffer);
        Py_DECREF(pybuffer);
        if (!ret) {
            sslroutine_set_exception(handle);
        } else {
            Py_XDECREF(ret);
            // now we can use internal buffer to read
            sslroutine_feedssl(handle, buffer);
        }
    }
}


//Py_LOCAL_INLINE(int)
//sslstream_feed_appdata(StreamHandle *handle, PyObject *bytes)
//{
//
//}
//
//Py_LOCAL_INLINE(int)
//sslstream_feed_ssldata(StreamHandle *handle, char **buffer_ptr, Py_ssize_t *nread_ptr, PyObject **buffer_keeper_ptr)
//{
//    struct sslstreamstate *_state = &handle->ssl;
//    switch (_state->status) {
//        case DO_HANDSHAKE:
//
//            break;
//        case WRAPPED:
//            break;
//        case SHUTDOWN:
//            break;
//    }
//
//    char *buffer = *buffer_ptr;
//    Py_ssize_t nread = *nread_ptr;
//    _Py_IDENTIFIER(write);
//    _Py_IDENTIFIER(write_eof);
//    if (nread < 0) {
//        _PyObject_CallMethodIdNoArgs(handle->ssl_inbio, &PyId_write_eof);
//    } else {
//        PyObject *ret = _PyObject_CallMethodId(handle->ssl_inbio, &PyId_write, "y#", buffer, nread);
//        if (!ret)
//            return -1;
//
//    }
//}

Py_LOCAL_INLINE(void)
sslstream_handle_connect(StreamHandle *handle)
{
//    handle->ssl.status = DO_HANDSHAKE;
//    sslstream_feed_ssldata(handle);
}