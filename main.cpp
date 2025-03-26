
#include "board.hpp"
#include "vfs/disk.hpp"
#include "vfs/stdstream.hpp"

extern "C" {
#include "main.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
}

#include "FreeRTOS.h"
#include "task.h"

#include "sdcard_blockdev.hpp"
#include "stdout.hpp"

#include <vfs/disk_mngr.hpp>
#include <vfs/vfs.hpp>
#include <vfs/logger.hpp>

#include <filesystem>
#include <cassert>
#include <sys/stat.h>
#include <sys/mount.h>
#include <dirent.h>
#include <unistd.h>
#include <cinttypes>

namespace {
    std::unique_ptr<vfs::VirtualFS> m_vfs;

    class DefaultStdStream : public vfs::StdStream {
    public:
        ~DefaultStdStream() override { syscalls::stdout_deinit(); }
        vfs::result<std::size_t> in(std::span<char>) override
        {
            // TODO
            return vfs::error(ENOTSUP);
        }
        vfs::result<std::size_t> out(std::span<const char> data) override
        {
            /// TODO: refactor stdout_write
            if (not syscalls::stdout_write(data)) { return data.size(); }
            return vfs::error(EIO);
        }
        vfs::result<std::size_t> err(std::span<const char> data) override
        {
            /// TODO: refactor stdout_write
            if (not syscalls::stdout_write(data)) { return data.size(); }
            return vfs::error(EIO);
        }
    };

} // namespace

namespace vfs {
    VirtualFS& borrow_vfs() { return *m_vfs; }
} // namespace vfs

[[noreturn]] void main_task(void*)
{
    vfs::logger::register_output_callback([](const auto lvl, const auto data) { printf("<%s> %s\r\n", vfs::logger::internal::level2str(lvl), data); });

    auto blockdev  = SDCardBlockdev();
    auto disk_mngr = vfs::DiskManager();

    const auto disk = disk_mngr.register_device(blockdev);
    assert(disk);

    const auto partition_name = (*disk)->borrow_partition(0)->get_name().c_str();

    m_vfs = std::make_unique<vfs::VirtualFS>(disk_mngr, std::make_unique<DefaultStdStream>());
    assert(m_vfs->register_filesystem(vfs::fstype::ext4).value() == 0);

    assert(mount(partition_name, "/mnt/vol0", "ext4", 0, nullptr) == 0);

    const auto lua_state = luaL_newstate();
    assert(lua_state != nullptr);

    luaopen_base(lua_state);

    const auto lua_entry_point = "/mnt/vol0/main.lua";
    if (luaL_dofile(lua_state, lua_entry_point) != LUA_OK) {
        printf("Error running Lua main entry point: %s\r\n", lua_tostring(lua_state, -1));
        lua_pop(lua_state, 1);
    }

    lua_close(lua_state);

    umount("/mnt/vol0");

    uint32_t counter {};
    while (true) {
        BSP_LED_Toggle(LED1);
        printf("Led blink: %" PRIu32 "\r\n", counter++);
        vTaskDelay(1000);
    }
}
int main()
{
    assert(board::init().value() == 0);
    xTaskCreate(main_task, "main", 1024*8, nullptr, tskIDLE_PRIORITY, nullptr);

    vTaskStartScheduler();
}
