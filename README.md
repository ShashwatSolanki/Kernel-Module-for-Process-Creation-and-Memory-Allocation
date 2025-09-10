# Kernel Module for Process Creation and Memory Allocation

This Linux kernel module demonstrates the creation of multiple kernel threads (child processes) and dynamic memory allocation within those threads. Each child thread allocates memory, sleeps for a short period, then frees the memory before exiting.

---

## Features

- Creates 3 child kernel threads.
- Each child thread allocates 1 KB of memory using `kmalloc`.
- Child threads print their process IDs and allocated memory addresses.
- Memory is freed before thread termination.
- Proper cleanup of threads on module unload.

---

## Files

- `process_memory_module.c`: The kernel module source code.

---

## How to Build

1. Ensure you have the Linux kernel headers installed.
2. Run `make` to build the kernel module.
