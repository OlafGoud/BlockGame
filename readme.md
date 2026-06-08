# Blockgame
game/gameengine for rendering blocks.
## Getting started

Donload? versions

## Compiled setup

### Prerequisites
- C++ compiler
- Cmake

### Building
```sh
git clone
cmake -DCMAKE_POLICY_VERSION_MINIMUM=3.5 -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build -S source
cmake --build ./build --parallel

# for clangd (need clang installed)
ln -s build/compile_commands.json compile_commands.json 

```

**Dependencies**

> [!CAUTION]
> This project is created on linux. Windows support isn't guaranteed. 
