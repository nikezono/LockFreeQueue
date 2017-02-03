LockFreeQueue
---

## Build(Ninja)

```bash
mkdir build; cd $_
cmake .. -G Ninja
ninja

```

## Test

```
ctest
```

## References

### MS-Queue

included in Java's `concurrency` package

[The Art of Multiprocessor Programming](https://www.amazon.co.jp/Art-Multiprocessor-Programming-%E4%B8%A6%E8%A1%8C%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0%E3%81%AE%E5%8E%9F%E7%90%86%E3%81%8B%E3%82%89%E5%AE%9F%E8%B7%B5%E3%81%BE%E3%81%A7/dp/4048679880)

Chapter 10.5.

### Optimistic Approarch to lock-free FIFO queue

[Paper](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.94.8625)
