
#include <stdio.h>
#include <stdint.h>

#define SIZE 8

typedef struct
{
    uint8_t arr[SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} RingBuffer;

void init(RingBuffer *b)
{
    b->head = 0;
    b->tail = 0;
    b->count = 0;
}

int buf_write(RingBuffer *b, uint8_t data)
{
    if (b->count == SIZE)
        return 0;

    b->arr[b->head] = data;

    /* Bitwise AND can avoid modulo division on MCUs without
       a hardware divider. This works because SIZE is a power of 2. */
    b->head = (b->head + 1) & (SIZE - 1);

    b->count++;

    return 1;
}

int buf_read(RingBuffer *b, uint8_t *data)
{
    if (b->count == 0)
        return 0;

    *data = b->arr[b->tail];
    b->tail = (b->tail + 1) & (SIZE - 1);

    b->count--;

    return 1;
}

uint8_t buf_count(RingBuffer *b)
{
    return b->count;
}

int buf_is_full(RingBuffer *b)
{
    return b->count == SIZE;
}

int buf_is_empty(RingBuffer *b)
{
    return b->count == 0;
}

int main(void)
{
    RingBuffer b;
    uint8_t data;
    uint8_t i;

    uint8_t a[] = {
        0x41, 0x42, 0x43, 0x44,
        0x45, 0x46, 0x47, 0x48
    };

    uint8_t c[] = {0x49, 0x4A, 0x4B};

    init(&b);

    printf("Initial writes:\n");

    for (i = 0; i < SIZE; i++)
    {
        if (buf_write(&b, a[i]))
        {
            printf("[WRITE] 0x%02X -> OK (count=%u)\n",
                   (unsigned int)a[i],
                   (unsigned int)buf_count(&b));
        }
    }

    printf("Full: %s\n", buf_is_full(&b) ? "YES" : "NO");

    if (!buf_write(&b, 0x99))
        printf("[WRITE] 0x99 -> FAIL (buffer full)\n");

    printf("\nReading three bytes:\n");

    for (i = 0; i < 3; i++)
    {
        if (buf_read(&b, &data))
        {
            printf("[READ] -> 0x%02X (count=%u)\n",
                   (unsigned int)data,
                   (unsigned int)buf_count(&b));
        }
    }

    printf("\nWriting three new bytes:\n");

    for (i = 0; i < 3; i++)
    {
        if (buf_write(&b, c[i]))
        {
            printf("[WRITE] 0x%02X -> OK (count=%u)\n",
                   (unsigned int)c[i],
                   (unsigned int)buf_count(&b));
        }
    }

    printf("\nReading remaining bytes:\n");

    while (!buf_is_empty(&b))
    {
        if (buf_read(&b, &data))
        {
            printf("[READ] -> 0x%02X (count=%u)\n",
                   (unsigned int)data,
                   (unsigned int)buf_count(&b));
        }
    }

    printf("Empty: %s\n", buf_is_empty(&b) ? "YES" : "NO");

    if (!buf_read(&b, &data))
        printf("[READ] (empty) -> FAIL (buffer empty)\n");

    return 0;
}
