# templtr

[![CMake](https://github.com/saile515/templtr/actions/workflows/cmake.yml/badge.svg?branch=master)](https://github.com/saile515/templtr/actions/workflows/cmake.yml)

**templtr (pronounced templator) is a lightweight framework for statically generating dynamic html pages, written in C++.**

*Caution: Project is still very early in development, so expect many new features and breaking api changes.*

## Features:
* Lightweight
* Easy to use
* Powerful
* Fast

## Installation:
### Option 1: Use prebuilt binaries
* Download and extract the latest release.
* Add to path. (optional)
### Option 2: Build from source
* Download and extract source from the latest release.
* Run `/usr/local/bin/cmake --build /path/to/source/build --config Release --target all --`
* Add to path. (optional)

## Usage:
### Creating a templtr project:
Run `templtr init` in the project root to create a templtr project.

### Build the project:
Run `templtr build`
**Options:**
* `-o` sets the out directory, default is `/out`
* `-b` sets the base url of links, default is an empty string

### Start dev server:
Run `templtr dev`
**Options:**
* `-o` sets the out directory, default is `/out`
* `-b` sets the base url of links, default is an empty string

## Documentation
Documentation for templtr is available at [saile515.github.io/templtr](https://saile515.github.io/templtr/)

## Example:
This repository contains an example project. You can clone the repository and run `templtr build` to build the project.
