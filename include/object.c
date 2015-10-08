#include <stdlib.h>
#include <stdio.h>

#include <object.>

/* Buffer containing all the object */
static object_t[MAX] buffer;

/* Index */
static int i = 0;

/* Creates a new object and returns it. */
object_t produce_object() {
    object_t obj;
    obj = rand();
    return obj;
}

/* Store the specified object inside the buffer. */
void store_object(object_t obj) {
    buffer[i++] = obj;
}

/* Takes off the last stored object from the buffer */
object_t take_object() {
    object_t obj = buffer[i--];
    return obj;
}

/* Uses the specified object. */
use_object(object_t obj) {
    
}