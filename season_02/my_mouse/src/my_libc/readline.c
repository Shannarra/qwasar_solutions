#include "readline.h"
#include <stdlib.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>

#define READLINE_READ_SIZE 512
#define NEWLINE '\n'

typedef unsigned short ushort;
typedef struct {
    ushort offset;
    ushort length;
    char array[BUFSIZ];
} ReadBuffer;

static ushort fill(ReadBuffer* buffer, int fd);
static void reset_index(ReadBuffer* buffer);
static ushort newline_index(ReadBuffer* buffer, int fd);
static ushort scan_until_newline(const ReadBuffer* buffer, ushort i);
static void add_newline(ReadBuffer* buffer, ushort i);
static char* pop(ReadBuffer* buffer, ushort n);
static void offset(ReadBuffer* buffer, ushort n);

char* readline(int fd)
{
    static ReadBuffer buffer = {
            .offset = 0,
            .length = 0,
            .array = {0}
    };
    if (!buffer.length && !fill(&buffer, fd))
    {
        return NULL;
    }
    return pop(&buffer, newline_index(&buffer, fd));
}

ushort fill(ReadBuffer* buffer, int fd)
{
    reset_index(buffer);
    ushort chars_read = read(fd, &buffer->array[buffer->length], READLINE_READ_SIZE);
    if (chars_read == 0) return 0;
    buffer->length += chars_read;
    buffer->array[buffer->length] = 0;
    return chars_read;
}

void reset_index(ReadBuffer* buffer)
{
    ushort i;
    for (i = 0; i < buffer->length; i++)
    {
        buffer->array[i] = buffer->array[i + buffer->offset];
    }
    buffer->offset = 0;
}

ushort newline_index(ReadBuffer* buffer, int fd)
{
    ushort i = scan_until_newline(buffer, 0);
    char last = buffer->array[buffer->offset + i];
    while (last != NEWLINE)
    {
        if (!fill(buffer, fd))
        {
            add_newline(buffer, i);
            break;
        }
        i = scan_until_newline(buffer, i);
        last = buffer->array[buffer->offset + i];
    }
    return i;
}

ushort scan_until_newline(const ReadBuffer* buffer, ushort i)
{
    const char* startingPoint = &buffer->array[buffer->offset];
    for (; i < buffer->length && startingPoint[i] != NEWLINE; i++);
    return i;
}

void add_newline(ReadBuffer* buffer, ushort i)
{
    buffer->array[buffer->offset + i] = NEWLINE;
    buffer->length++;
}

char* pop(ReadBuffer* buffer, ushort n)
{
    char* line = malloc(n + 1);
    const char* bufferArray = &buffer->array[buffer->offset];
    ushort i;
    for (i = 0; i < n; i++)
    {
        line[i] = bufferArray[i];
    }
    line[i] = 0;
    offset(buffer, n + 1);
    return line;
}

void offset(ReadBuffer* buffer, ushort n)
{
    buffer->offset += n;
    buffer->length -= n;
}
