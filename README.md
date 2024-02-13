# gba-raycast-engine

A very much work in progress 3d raycast game for the Game Boy Advance. Written in C.

## Compiling

### Local

* Install devkitARM. See [devkitPro](https://devkitpro.org/wiki/Getting_Started) for instructions.
* Set necessary paths to DEVKITARM & DEVKITPRO environment variables. On Linux default locations would be DEVKITARM=/opt/devkitpro/devkitARM & DEVKITPRO=/opt/devkitpro)
* Run `make`, after compiling a `.gba` file can be found in the project root directory.

### With docker

* Run `builddocker.sh` script to build the project in a container and export the `.gba` file to the root directory