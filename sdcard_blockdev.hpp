/**
 * sdcard_blockdev
 * Created on: 03/03/2025
 * Author mateuszpiesta
 * Company: mprogramming
 */

#pragma once

#include <vfs/blockdev.hpp>

#include "stm32746g_discovery_sd.h"

class SDCardBlockdev : public vfs::BlockDevice {
public:
    SDCardBlockdev();
    ~SDCardBlockdev() override;
    [[nodiscard]] std::error_code          probe() override;
    [[nodiscard]] std::error_code          flush() override;
    [[nodiscard]] std::error_code          write(const std::byte& buf, sector_t lba, std::size_t count) override;
    [[nodiscard]] std::error_code          read(std::byte& buf, sector_t lba, std::size_t count) override;
    [[nodiscard]] vfs::result<std::size_t> get_sector_size() const override;
    [[nodiscard]] vfs::result<sector_t>    get_sector_count() const override;
    [[nodiscard]] std::string              get_name() const override;

private:
    HAL_SD_CardInfoTypeDef info {};
};
