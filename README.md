# zzmalloc1

A memory allocator in C++. Designed for singlethread. Uses freelist to manage small blocks(size <= 4KB). Requests larger than 4KB will be redirected to malloc.

I reallized that the interface and design of my allocator are too different from the standard ones (tc, mi, rp...) to compare the performance. Going to develop a new one in C which supports multithreads.

## License

[MIT](LICENSE) © Zixuan Zhao