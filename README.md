# ALEXTMap
Unreal Engine plugin that implements a wrapper for Microsoft's ML-enhanced map.
> https://github.com/microsoft/ALEX

This project uses git submodules, clone with `--recursive`

> ```git clone --recursive git@github.com:MarkJGx/ALEXTMap.git```

## Usage
```cpp
TAlexMap<int32, int32> Map;
Map.Add(1, 1)
int32 Value = Map.At(1);
int32 Value = Map[1];
```
