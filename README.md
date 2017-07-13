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

## Benchmark

```
ninja && export file="./benchmark/$(git log -n1 --format=%H | cut -c 1-5).result" && rm -f $file && echo "Queue Thread OPS" > $file && for i in {0..16}; do ./benchmark/benchmark ${i} >> $file; done && less $file
```

## References

### MS-Queue

included in Java's `concurrency` package

[The Art of Multiprocessor Programming](https://www.amazon.co.jp/Art-Multiprocessor-Programming-%E4%B8%A6%E8%A1%8C%E3%83%97%E3%83%AD%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0%E3%81%AE%E5%8E%9F%E7%90%86%E3%81%8B%E3%82%89%E5%AE%9F%E8%B7%B5%E3%81%BE%E3%81%A7/dp/4048679880)

Chapter 10.5.

### Optimistic Approarch to lock-free FIFO queue

[Paper](http://citeseerx.ist.psu.edu/viewdoc/summary?doi=10.1.1.94.8625)

## Benchmark

#### Lock-based Queue

```
Thread ms TPS
1 160 6250000
2 579 3454231
3 880 3409090
4 1456 2747252
5 2104 2376425
6 2607 2301495
7 3286 2130249
8 4012 1994017
9 4567 1970659
10 5288 1891074
11 5913 1860307
12 6569 1826762
13 7439 1747546
14 8100 1728395
15 8440 1777251
16 9161 1746534
```

#### Optimistic Approarch

```
Thread ms TPS        
1 104 9615384        
2 285 7017543        
3 466 6437768        
4 642 6230529        
5 818 6112469        
6 991 6054490        
7 1221 5733005       
8 1470 5442176       
9 1742 5166475       
10 1982 5045408      
11 2218 4959422      
12 2192 5474452      
13 2361 5506141      
14 2570 5447470      
15 2770 5415162      
16 2995 5342237      
```

## Memo

* エリミネーションのほうがスケールする気がする.
  * ボトルネックが一つのポインタである以上スレッド数の恩恵は受けない.
