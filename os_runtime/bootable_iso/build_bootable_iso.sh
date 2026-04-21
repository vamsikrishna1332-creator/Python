#!/usr/bin/env bash
set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
WORK_DIR="$SCRIPT_DIR/work"
OUT_DIR="$SCRIPT_DIR/out"

if ! command -v lb >/dev/null 2>&1; then
  echo "error: live-build (lb) is required. Install: sudo apt-get install live-build" >&2
  exit 2
fi

rm -rf "$WORK_DIR"
mkdir -p "$WORK_DIR" "$OUT_DIR"
cp -a "$SCRIPT_DIR/config" "$WORK_DIR/"

pushd "$WORK_DIR" >/dev/null

lb config \
  --distribution bookworm \
  --architectures amd64 \
  --binary-images iso-hybrid \
  --archive-areas "main contrib non-free non-free-firmware" \
  --linux-flavours amd64

lb build

ISO_PATH="$(find . -maxdepth 2 -name '*.iso' | head -n 1 || true)"
if [[ -z "$ISO_PATH" ]]; then
  echo "error: live-build did not produce an ISO" >&2
  exit 3
fi

cp "$ISO_PATH" "$OUT_DIR/mini_os_bootable.iso"
popd >/dev/null

echo "Bootable ISO ready: $OUT_DIR/mini_os_bootable.iso"
