#ifndef PYSOURCE_3_10_0_CAPSULE_H
#define PYSOURCE_3_10_0_CAPSULE_H

typedef struct {
    PyObject_HEAD
    void *pointer;
    const char *name;
    void *context;
    PyCapsule_Destructor destructor;
} PyCapsule;

#endif