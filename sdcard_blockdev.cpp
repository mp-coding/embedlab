/**
 * sdcard_blockdev
 * Created on: 03/03/2025
 * Author mateuszpiesta
 * Company: mprogramming
 */

#include "sdcard_blockdev.hpp"

#include "stm32746g_discovery.h"

extern "C" {

extern SD_HandleTypeDef uSdHandle;

/**
 * @brief  This function handles SDMMC1 global interrupt request.
 * @param  None
 * @retval None
 */
void BSP_SDMMC_IRQHandler(void) { HAL_SD_IRQHandler(&uSdHandle); }
}

SDCardBlockdev::SDCardBlockdev()
{
    if (BSP_SD_Init() != MSD_OK) { throw std::runtime_error("Failed to initialize SD card"); }
}

SDCardBlockdev::~SDCardBlockdev() { BSP_SD_DeInit(); }

std::error_code SDCardBlockdev::probe()
{
    BSP_SD_GetCardInfo(&info);

    if (BSP_SD_GetCardState() == SD_TRANSFER_OK) { return {}; }

    return vfs::from_errno(EIO);
}
std::error_code SDCardBlockdev::flush() { return {}; }
std::error_code SDCardBlockdev::write(const std::byte& buf, sector_t lba, std::size_t count)
{
    while (BSP_SD_GetCardState() != SD_TRANSFER_OK) { }
    auto input = reinterpret_cast<uint32_t*>(&const_cast<std::byte&>(buf));
    return BSP_SD_WriteBlocks(input, lba, count, 500) == MSD_OK ? std::error_code {} : vfs::from_errno(EIO);
}
std::error_code SDCardBlockdev::read(std::byte& buf, sector_t lba, std::size_t count)
{
    while (BSP_SD_GetCardState() != SD_TRANSFER_OK) { }
    return BSP_SD_ReadBlocks(reinterpret_cast<std::uint32_t*>(&buf), lba, count, 500) == MSD_OK ? std::error_code {} : vfs::from_errno(EIO);
}
vfs::result<std::size_t>                SDCardBlockdev::get_sector_size() const { return info.BlockSize; }
vfs::result<vfs::BlockDevice::sector_t> SDCardBlockdev::get_sector_count() const { return info.BlockNbr; }
std::string                             SDCardBlockdev::get_name() const { return "sdcard0"; }