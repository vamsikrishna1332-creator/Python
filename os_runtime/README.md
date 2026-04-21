# Mini OS Runtime (C++)

This is a **starter runtime shell** written in C++ that can launch:

- `.exe` apps through `wine`
- `.apk` apps through `waydroid`
- Flatpak IDs through `flatpak run`

> Note: this is not a full operating system kernel. It is a small userspace launcher that gives one command interface to run app formats from different ecosystems.

## Build

```bash
cmake -S os_runtime -B os_runtime/build
cmake --build os_runtime/build
```

## Run

```bash
./os_runtime/build/mini_os run game.exe
./os_runtime/build/mini_os run mobile.apk
./os_runtime/build/mini_os run flatpak:org.mozilla.firefox --private-window
```

## Make ISO package

This project can also be packaged as an `.iso` image containing the launcher binary + docs.

```bash
./os_runtime/tools/build_iso.sh
```

Output:

- `os_runtime/dist/mini_os-runtime.iso`

Optional arguments:

```bash
./os_runtime/tools/build_iso.sh <build_dir> <out_dir> <iso_name>
```

ISO builder tools accepted (auto-detected):

- `xorriso` (preferred)
- `genisoimage`
- `mkisofs`

## Requirements

Install tools needed for the target app type:

- `wine` for `.exe`
- `waydroid` for `.apk`
- `flatpak` for Flatpak apps
