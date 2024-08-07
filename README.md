# wazza

## Requirements

For building wazza the only requirements is a Linux machine with glibc, gcc, and make. And it can be installed on most distributions with one commands.

```
# Ubuntu
apt install build-essential

# Fedora
dnf install @development-tools
```

## Building

```
make
```

This will create the `./wazza` binary file.

## Unit Test

Requires the check library (on fedora `dnf install check-devel`).

```
make check
```

## Run Locally

Well this is not a production service, will be no point in doing so, so actually so locally is actually the only way it will ever be executed in.

```
make
./wazza
```
