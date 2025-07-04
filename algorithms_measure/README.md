# Algorithms measure

Measure the elapsed time of callables using decorators.


## Build

```shell
mkcd build
cmake .. -DCMAKE_PROJECT_TOP_LEVEL_INCLUDES=conan_provider.cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja
cmake --build .
```


## Run

```shell
./algorithms_measure
```

## Expected output

```text
Measure algorithms elapsed time
|   Algorithm   |Time (ms) |  Result  |
---------------------------------------
|std::accumulate|22.515716 |3000000   |
|std::find      |27.993639 |false     |
|std::find      |0.000432  |true      |
```
