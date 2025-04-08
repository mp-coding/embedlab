#!/usr/bin/env bash

set -euo pipefail

if (($# < 1)); then
  echo "Usage: $(basename "$0") DEVICE"
  echo "Prepare SD card ext4 filesystem (MBR + partitions)"
  exit 1
fi

check_exe() {
  if ! command -v "$1" &>/dev/null; then
    echo "Command not available in PATH: $1"
    exit 1
  fi
}

# By default, 4096bytes block size is used.
BLOCK_SIZE=4096

DEVICE="$1"
PART_SIZE="$2"
ROOT_DIR="$3"

if (($# < 3)); then
  echo "Usage: $(basename "$0") <device_or_image> <partition_size> <root_dir>"
  echo "Prepare SD card ext4 filesystem (MBR + partitions)"
  exit 1
fi

DEVICE_BLK_SIZE=512

PART1_START=1
PART1_SIZE=$((PART_SIZE / DEVICE_BLK_SIZE))
PART1_LABEL="volume0"

check_exe sfdisk

sfdisk "$DEVICE" <<==sfdisk
label: dos
label-id: 0x09650eb4
unit: sectors
start=    $PART1_START,  size=    $PART1_SIZE, type=83
==sfdisk

sync

# 64bit - support for filesystem bigger than 2^32
# flex_bg - per-block group metadata, not supported by lwext4
# huge_file - support for files larger than 2TB
# dir_nlink - support for more than 65000 hard links
# extra_isize - reserve extra space in each node for an extended metadata
# extent - due to the license issues, we cannot support extents
# ext_attr - due to the license issues, we cannot support ext attr
features='^64bit,^flex_bg,^resize_inode,^huge_file,^dir_nlink,^extra_isize,^extent,^metadata_csum'

check_exe mke2fs

make_partition() {
  label="$1"
  start="$2"
  size="$3"
  root="$4"
  mke2fs -F -L "$label" -d "$root" -E offset=$((start * DEVICE_BLK_SIZE)) -O "$features" -m 0 -t ext4 "$DEVICE" $((size * DEVICE_BLK_SIZE / BLOCK_SIZE)) >/dev/null
}

make_partition "$PART1_LABEL" "$PART1_START" "$PART1_SIZE" "$ROOT_DIR"
sync
