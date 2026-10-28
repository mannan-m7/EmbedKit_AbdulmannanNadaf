
# EmbedKit - Ring Buffer

**Name:** Abdulmannan Nadaf

## Module: Ring Buffer (`ringbuf.c`)

Implements an 8-byte circular buffer in C using `uint8_t` data. It supports FIFO read and write operations, tracks the number of stored bytes, handles full and empty conditions, and uses bitwise AND for index wrap-around.

## Build Instructions

Compile using GCC with C99 support:

```bash
gcc -Wall -std=c99 ringbuf.c -o ringbuf.exe
```

Run on Windows:

```bash
ringbuf.exe
```
