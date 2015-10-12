#ifndef __OBJECT_H__
#define __OBJECT_H__

/* Object type */
typedef int object_t;

/* Max size of the buffer */
#define MAX 100

/* Creates a new object and returns it. */
object_t produce_object();

/* Store the specified object inside the buffer. */
void store_object(object_t obj);

/* Takes off the last stored object from the buffer */
object_t take_object();

/* Uses the specified object. */
void use_object(object_t obj);

#endif