# silence-engine

A C++/Lua video game engine.

## Dependencies

This project relies on the [Nix package manager](https://nixos.org/) to handle developer dependencies and build environments.

## Development Setup

Make sure `nix` is installed on your system before proceeding. 

Run the setup script to initialize Git submodules and compile the `shaderc` tool:

```bash
sh ./scripts/dev_install.sh
```

## Building
Generate the build system:

```bash
cmake -B build -G Ninja
```
Compile the engine executable:
```bash
cmake --build build
```

## Running
Run the engine using the custom target:

```bash
ninja -C build run
```

## Generating Documentation

You can generate the engine documentation using Doxygen via the provided script:

```bash
sh ./scripts/generate_doc.sh
```