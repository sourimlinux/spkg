# SPKG (Sourim Package Manager)

## Installing:

Dependencies: `bash curl coreutils tar xz`

Install:
```sh
make install
```

Install to the specified root directory:
```sh
make install DESTDIR=${ROOT}
```

## Usage:

Install the packages:
```sh
spkg-install -S PACKAGE(s)
```