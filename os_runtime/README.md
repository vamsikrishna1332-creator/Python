# Mini OS Runtime (C++)

This project has two deliverables:

1. **`mini_os` launcher** (C++) for `.exe`, `.apk`, and `flatpak:<app-id>` targets.
2. **Bootable ISO recipe** with an interactive desktop menu and Chromium preinstalled.

## Build launcher

```bash
cmake -S os_runtime -B os_runtime/build
cmake --build os_runtime/build
```

Run example:

```bash
./os_runtime/build/mini_os run flatpak:org.mozilla.firefox
```

## Build bootable ISO (interactive + Chromium preinstalled)

The bootable ISO is defined under `os_runtime/bootable_iso/` and uses **Debian live-build**.

```bash
./os_runtime/bootable_iso/build_bootable_iso.sh
```

Output:

- `os_runtime/bootable_iso/out/mini_os_bootable.iso`

### What comes preinstalled in the ISO

- Chromium browser
- Interactive launcher (`Mini OS Control Center`) that opens automatically at login
- Basic desktop stack (Xorg + Openbox + LightDM)
- `wine`, `flatpak`, `waydroid`, and networking tools

### Requirements for ISO build host

Install live-build and ISO tools on your build machine:

```bash
sudo apt-get update
sudo apt-get install -y live-build xorriso
```

## Legacy data ISO packaging

If you only need a non-bootable ISO containing the runtime files, use:

```bash
./os_runtime/tools/build_iso.sh
```
