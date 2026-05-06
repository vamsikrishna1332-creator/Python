````md
# NewtonOS Runtime

NewtonOS Runtime is a closed-source operating system/runtime project designed to run Linux, Windows, and Android apps from one place.

The project includes a C++ launcher called `mini_os`, ISO packaging tools, and a bootable ISO recipe with an interactive desktop menu. The source files and build scripts are included inside this project folder, but the project is **not open source** and is not licensed for redistribution, modification, or public reuse without permission.

## Features

- Run Linux apps through Flatpak
- Install and run Windows `.exe` and `.msi` apps using Wine
- Install and run Android `.apk` apps using Waydroid
- Build a bootable NewtonOS ISO
- Build a legacy non-bootable runtime ISO
- Includes an interactive desktop launcher in the bootable ISO
- Chromium browser included in the bootable ISO

## Project Status

This project is experimental.

Windows and Android app support depends on Wine, Flatpak, Waydroid, drivers, hardware support, and the host Linux environment. Not every Windows or Android app is guaranteed to work.

## License

This project is proprietary and closed source.

You may not copy, redistribute, publish, sell, modify, or reuse this project or any of its files without explicit permission from the owner.

All rights reserved.

## Requirements

For building the runtime on Ubuntu or Debian:

```bash
sudo apt-get update
sudo apt-get install -y build-essential cmake wine flatpak
````

For Android app support:

```bash
sudo apt-get install -y waydroid
sudo waydroid init
```

For building the bootable ISO:

```bash
sudo apt-get update
sudo apt-get install -y live-build xorriso
```

## Build NewtonOS Runtime

From the project root, run:

```bash
cmake -S os_runtime -B os_runtime/build
cmake --build os_runtime/build
```

Install the launcher:

```bash
sudo install -m 755 os_runtime/build/mini_os /usr/local/bin/mini_os
```

Check that it works:

```bash
mini_os
```

## Install Windows Apps

To install a Windows `.exe` installer:

```bash
mini_os install setup.exe
```

To install a Windows `.msi` installer:

```bash
mini_os install installer.msi
```

## Run Windows Apps

To run a Windows executable:

```bash
mini_os run app.exe
```

## Install Android Apps

To install an Android APK:

```bash
mini_os install app.apk
```

## Run Android Apps

Use the Android package name:

```bash
mini_os run apk:com.example.app
```

Example:

```bash
mini_os run apk:com.android.chrome
```

## Install Linux Apps with Flatpak

To install a Flatpak app:

```bash
mini_os install flatpak:org.mozilla.firefox
```

## Run Linux Apps with Flatpak

To run a Flatpak app:

```bash
mini_os run flatpak:org.mozilla.firefox
```

You can also pass arguments:

```bash
mini_os run flatpak:org.mozilla.firefox --private-window
```

## Build Bootable ISO

The bootable ISO files are located inside:

```bash
os_runtime/bootable_iso/
```

To build the bootable ISO:

```bash
./os_runtime/bootable_iso/build_bootable_iso.sh
```

The output ISO will be created at:

```bash
os_runtime/bootable_iso/out/mini_os_bootable.iso
```

## Bootable ISO Includes

The bootable ISO includes:

* Chromium browser
* NewtonOS Control Center
* Xorg
* Openbox
* LightDM
* Wine
* Flatpak
* Waydroid
* Networking tools

## Build Legacy Runtime ISO

To build a non-bootable ISO containing runtime files:

```bash
./os_runtime/tools/build_iso.sh
```

This is useful when you only want to package the runtime files instead of creating a full bootable OS image.

## CMake Targets

The project includes these CMake targets:

```bash
cmake --build os_runtime/build
```

Builds the `mini_os` executable.

```bash
cmake --build os_runtime/build --target iso
```

Builds the runtime ISO package.

```bash
cmake --build os_runtime/build --target bootable-iso
```

Builds the bootable NewtonOS ISO using the bootable ISO script.

## Notes

* Wine is required for Windows app support.
* Waydroid is required for Android app support.
* Flatpak is required for Linux app installation and launching.
* Some apps may require extra permissions, graphics drivers, network access, or system configuration.
* Android apps may require Waydroid to be initialized and running.
* Windows apps may not all work perfectly because Wine compatibility varies by application.

## Disclaimer

NewtonOS Runtime is provided as-is.

The owner is not responsible for app compatibility issues, failed installations, data loss, system damage, or misuse of this software.

Use it carefully and only on systems where you understand the risks.

```

Based on the uploaded `CMakeLists.txt` and current README contents. :contentReference[oaicite:0]{index=0} :contentReference[oaicite:1]{index=1}
```
