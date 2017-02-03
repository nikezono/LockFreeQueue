# ConcurrencyControl


## Build(Ninja)

```bash
mkdir build; cd $_
cmake .. -G Ninja
ninja

```

## Test

```
./ci.sh
```

## Benchmark

```
./plot_bench.sh
```

## References

### Silo

[Speedy transactions in multicore in-memory databases](http://dl.acm.org/citation.cfm?id=2522713)

OCC and Silo implements **decentralized tid allocation** of this paper.

### Aether

[Aether](http://dl.acm.org/citation.cfm?id=1920928)

SiloLogger, Logger implements **Group Commit, Flush Pipelining and Early Lock Release** of this paper.

### 2PL

出典を探してない.
