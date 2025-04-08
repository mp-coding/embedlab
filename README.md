# EmbedLab

EmbedLab is a playground for embedded development, providing a structured environment for running C++ examples on embedded platforms. The project is designed to support educational purposes, experimentations, and demonstrations of embedded concepts, particularly on STM32F7 Discovery boards.

## Getting Started

### Prerequisites
To work with EmbedLab, ensure you have the following tools installed:
- **GNU Arm Embedded Toolchain** (`arm-none-eabi-gcc`)
- **Meson Build System** (`meson`, `ninja`)
- **Git** (for version control)

### Cloning the Repository
```sh
git clone --recurse-submodules https://github.com/mp-coding/embedlab.git
cd embedlab
```
### Building the Project
```sh
meson setup build --cross-file toolchain/stm32f7.txt --optimization=g
ninja -C build
```

### Flashing to the STM32F7 Discovery Board
```sh
meson compile -C build program
```

### Printing detailed size report
```sh
meson compile -C build size
```

### Creating read-to-use SD image
```sh
./scripts/prepare_sdcard.sh <dev>    
```
Important: This script will wipe out all the data from the disk. Please check that:
1. You passed the correct block device
2. You backed up all the important data from SD card

## Contributing

Contributions are welcome! Please follow these steps:

1. **Fork the Repository**.
2. **Create a New Branch**:
   ```bash
   git checkout -b feature/YourFeatureName
   ```
3. **Commit Your Changes**:
   ```bash
   git commit -m 'Add some feature'
   ```
4. **Push to the Branch**:
   ```bash
   git push origin feature/YourFeatureName
   ```
5. **Open a Pull Request**.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.

---

## Contact
For any inquiries or discussions, feel free to open an issue or reach out to mp-coding.







