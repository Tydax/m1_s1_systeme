#include <stdlib.h>
#include <stdio.h>

#include <object.h>

/* Buffer containing all the object */
static object_t buffer[MAX];

/* Index */
static unsigned int i = 0;

/* Creates a new object and returns it. */
object_t produce_object() {
    object_t obj;
    obj = i;
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
void use_object(object_t obj) {
    printf("Object n°%d consumed\n", obj);
}