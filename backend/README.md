## Build

```bash
# uwebsocket submodule
git submodule update --init --recursive

# generate build files
cmake -S . -B build

# build project
cd build && make
```

## Editor Notes
```bash
# only needed if your editor/clangd is SHIT.
# (optional) editor happy = developer happy
ln -sf build/compile_commands.json compile_commands.json
```
