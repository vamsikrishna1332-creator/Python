# Mini OS Runtime (C++)

This project has two deliverables:

1. **`mini_os` launcher** (C++) for Windows (`.exe/.msi`), Android (`.apk` install + `apk:<package>` run), and `flatpak:<app-id>` targets.
2. **Bootable ISO recipe** with an interactive desktop menu and Chromium preinstalled.

## Install mini_os on Ubuntu/Debian

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake wine flatpak
```

For Android app support, install and initialize Waydroid:

```bash
sudo apt-get install -y waydroid
sudo waydroid init
```

Build and install `mini_os`:

```bash
cmake -S os_runtime -B os_runtime/build
cmake --build os_runtime/build
sudo install -m 755 os_runtime/build/mini_os /usr/local/bin/mini_os
```

Verify:

```bash
mini_os
```

## Can it install Windows and Android apps?

Yes.

### Install Windows apps

```bash
mini_os install setup.exe
mini_os install installer.msi
```

### Run Windows apps

```bash
mini_os run app.exe
```

### Install Android apps (APK)

```bash
mini_os install app.apk
```

### Run installed Android apps

Use the Android package name:

```bash
mini_os run apk:com.android.chrome
```

### Flatpak install/run

```bash
mini_os install flatpak:org.mozilla.firefox
mini_os run flatpak:org.mozilla.firefox --private-window
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
