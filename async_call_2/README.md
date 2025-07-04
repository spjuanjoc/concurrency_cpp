# Async call 2

Call async tasks

Example of the usage of std::future and std::promise

## Expected output

```text
First message, before sleep 5s      tid:132564063926080
Third message, before async call 5s tid:132564056536768
Second message, after sleep 5s      tid:132564063926080
after async
after set_value
future ready
async ready
```
