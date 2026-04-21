#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"
BUILD_DIR="${1:-$ROOT_DIR/build}"
OUT_DIR="${2:-$ROOT_DIR/dist}"
ISO_NAME="${3:-mini_os-runtime.iso}"
STAGING_DIR="$OUT_DIR/iso_root"

mkdir -p "$OUT_DIR"
rm -rf "$STAGING_DIR"
mkdir -p "$STAGING_DIR/mini_os"

if [[ ! -x "$BUILD_DIR/mini_os" ]]; then
  echo "error: $BUILD_DIR/mini_os not found. Build first:" >&2
  echo "  cmake -S os_runtime -B os_runtime/build && cmake --build os_runtime/build" >&2
  exit 1
fi

cp "$BUILD_DIR/mini_os" "$STAGING_DIR/mini_os/"
cp "$ROOT_DIR/README.md" "$STAGING_DIR/mini_os/"

cat > "$STAGING_DIR/mini_os/RUN.sh" <<'EOF'
#!/usr/bin/env sh
set -e
./mini_os "$@"
EOF
chmod +x "$STAGING_DIR/mini_os/RUN.sh"

ISO_PATH="$OUT_DIR/$ISO_NAME"

if command -v xorriso >/dev/null 2>&1; then
  xorriso -as mkisofs -V "MINI_OS_RUNTIME" -o "$ISO_PATH" "$STAGING_DIR"
elif command -v genisoimage >/dev/null 2>&1; then
  genisoimage -V "MINI_OS_RUNTIME" -o "$ISO_PATH" "$STAGING_DIR"
elif command -v mkisofs >/dev/null 2>&1; then
  mkisofs -V "MINI_OS_RUNTIME" -o "$ISO_PATH" "$STAGING_DIR"
else
  echo "error: need xorriso, genisoimage, or mkisofs to create ISO" >&2
  exit 2
fi

echo "ISO created: $ISO_PATH"
