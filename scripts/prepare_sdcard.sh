#!/usr/bin/env bash

set -euo pipefail

SCRIPT_DIR="$(realpath "$(dirname "$0")")"

DEVICE="$1"
IMAGE_NAME="sdcard.img"
PARTITION_SIZE=$((1024 * 1024 * 128)) #128MB
DISK_IMAGE_SIZE=$((PARTITION_SIZE + 512))

if [ -z "$DEVICE" ] || [ ! -b "$DEVICE" ]; then
  echo "Error: Invalid or missing device."
  echo "Usage: $0 <device>"
  exit 1
fi

if [ -f "$IMAGE_NAME" ]; then
  echo "Warning: $IMAGE_NAME already exists and will be overwritten."
  rm -f "$IMAGE_NAME"
fi

truncate -s "$DISK_IMAGE_SIZE" "$IMAGE_NAME"

"$SCRIPT_DIR/mkdisk.sh" "$IMAGE_NAME" "$PARTITION_SIZE" "$SCRIPT_DIR/../assets"

dd if="$IMAGE_NAME" of="$DEVICE" bs=4M status=progress

rm -f "$IMAGE_NAME"
