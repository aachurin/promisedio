/*[clinic input]
preserve
[clinic start generated code]*/

PyDoc_STRVAR(ns_Stream_get_write_queue_size__doc__,
"get_write_queue_size($self, /)\n"
"--\n"
"\n"
"\n"
"Returns the size of the write queue.");

#define NS_STREAM_GET_WRITE_QUEUE_SIZE_METHODDEF    \
    {"get_write_queue_size", (PyCFunction)ns_Stream_get_write_queue_size, METH_NOARGS, ns_Stream_get_write_queue_size__doc__},

static inline PyObject *
ns_Stream_get_write_queue_size_impl(Stream *self);

static PyObject *
ns_Stream_get_write_queue_size(Stream *self, PyObject *Py_UNUSED(ignored))
{
    return ns_Stream_get_write_queue_size_impl(self);
}
/*[clinic end generated code: output=16bcc525338a880a input=a9049054013a1b77]*/
