#include "py/mpconfig.h"
#if MICROPY_VFS_NATIVE

#if !MICROPY_VFS
#error "with MICROPY_VFS_NATIVE enabled, must also enable MICROPY_VFS"
#endif

#include <string.h>
#include <errno.h>

#include "py/nlr.h"
#include "py/runtime.h"
#include "py/mperrno.h"
#include "extmod/vfs_native.h"
#include "lib/timeutils/timeutils.h"

#include "esp_vfs.h"
#include "src/esp_vfs_fat.h"
#include "esp_system.h"

#if _MAX_SS == _MIN_SS
#define SECSIZE(fs) (_MIN_SS)
#else
#define SECSIZE(fs) ((fs)->ssize)
#endif

#define mp_obj_native_vfs_t fs_user_mount_t

static const char *TAG = "vfs_native.c";
static wl_handle_t s_wl_handle = WL_INVALID_HANDLE;

/* esp-idf doesn't seem to have a cwd; create one. */
char cwd[256] = { 0 };
int
chdir(const char *path)
{
	strncpy(cwd, path, sizeof(cwd));
	cwd[255] = 0;
	return 0;
}
char *
getcwd(char *buf, size_t size)
{
	if (size <= strlen(cwd))
	{
		errno = ENAMETOOLONG;
		return NULL;
	}
	strcpy(buf, cwd);
	return buf;
}

STATIC mp_obj_t native_vfs_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *args) {
    mp_arg_check_num(n_args, n_kw, 1, 1, false);

    // create new object
    fs_user_mount_t *vfs = m_new_obj(fs_user_mount_t);
    vfs->base.type = type;
    vfs->flags = FSUSER_FREE_OBJ;

    ets_printf("DUMMY: VFS_MAKE_NEW\n");

    return MP_OBJ_FROM_PTR(vfs);
}

STATIC mp_obj_t native_vfs_mkfs(mp_obj_t bdev_in) {
    // create new object
    fs_user_mount_t *vfs = MP_OBJ_TO_PTR(native_vfs_make_new(&mp_native_vfs_type, 1, 0, &bdev_in));

    // make the filesystem
    /*uint8_t working_buf[_MAX_SS];
    FRESULT res = f_mkfs(&vfs->fatfs, FM_FAT | FM_SFD, 0, working_buf, sizeof(working_buf));
    if (res != FR_OK) {
        mp_raise_OSError(fresult_to_errno_table[res]);
    }*/
    
    ets_printf("DUMMY: VFS_MKFS\n");

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(native_vfs_mkfs_fun_obj, native_vfs_mkfs);
STATIC MP_DEFINE_CONST_STATICMETHOD_OBJ(native_vfs_mkfs_obj, MP_ROM_PTR(&native_vfs_mkfs_fun_obj));

STATIC MP_DEFINE_CONST_FUN_OBJ_3(native_vfs_open_obj, fatfs_builtin_open_self);

STATIC mp_obj_t native_vfs_ilistdir_func(size_t n_args, const mp_obj_t *args) {
    mp_obj_native_vfs_t *self = MP_OBJ_TO_PTR(args[0]);
    bool is_str_type = true;
    const char *path;
    if (n_args == 2) {
        if (mp_obj_get_type(args[1]) == &mp_type_bytes) {
            is_str_type = false;
        }
        path = mp_obj_str_get_str(args[1]);
    } else {
        path = "";
    }

    ets_printf("vfs_native: VFS_LISTDIR_FUNC\n");
    return native_vfs_ilistdir2(self, path, is_str_type);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR_BETWEEN(native_vfs_ilistdir_obj, 1, 2, native_vfs_ilistdir_func);

STATIC mp_obj_t native_vfs_remove(mp_obj_t vfs_in, mp_obj_t path_in) {
    mp_obj_native_vfs_t *self = MP_OBJ_TO_PTR(vfs_in);
    const char *path = mp_obj_str_get_str(path_in);

	int res = unlink(path);
	if (res < 0) {
		mp_raise_OSError(errno);
		return mp_const_none;
	}

	ets_printf("vfs_native: VFS_REMOVE_INTERNAL\n");
	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(native_vfs_remove_obj, native_vfs_remove);

STATIC mp_obj_t native_vfs_rmdir(mp_obj_t vfs_in, mp_obj_t path_in) {
    const char *path = mp_obj_str_get_str(path_in);
	int res = rmdir(path);
	if (res < 0) {
		mp_raise_OSError(errno);
		return mp_const_none;
	}

    ets_printf("vfs_native: VFS_MKDIR\n");
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(native_vfs_rmdir_obj, native_vfs_rmdir);

STATIC mp_obj_t native_vfs_rename(mp_obj_t vfs_in, mp_obj_t path_in, mp_obj_t path_out) {
    mp_obj_native_vfs_t *self = MP_OBJ_TO_PTR(vfs_in);
    const char *old_path = mp_obj_str_get_str(path_in);
    const char *new_path = mp_obj_str_get_str(path_out);

    int res = rename(old_path, new_path);
	/*
	// FIXME: have to check if we can replace files with this
	if (res < 0 && errno == EEXISTS) {
		res = unlink(new_path);
		if (res < 0) {
			mp_raise_OSError(errno);
			return mp_const_none;
		}
		res = rename(old_path, new_path);
	}
	*/
	if (res < 0) {
		mp_raise_OSError(errno);
		return mp_const_none;
	}

    ets_printf("vfs_native: VFS_RENAME\n");
	return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(native_vfs_rename_obj, native_vfs_rename);

STATIC mp_obj_t native_vfs_mkdir(mp_obj_t vfs_in, mp_obj_t path_o) {
    const char *path = mp_obj_str_get_str(path_o);
	int res = mkdir(path, 0755);
	if (res < 0) {
		mp_raise_OSError(errno);
		return mp_const_none;
	}

    ets_printf("vfs_native: VFS_MKDIR\n");
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(native_vfs_mkdir_obj, native_vfs_mkdir);

/// Change current directory.
STATIC mp_obj_t native_vfs_chdir(mp_obj_t vfs_in, mp_obj_t path_in) {
    const char *path = mp_obj_str_get_str(path_in);
	int res = chdir(path);
	if (res < 0) {
		mp_raise_OSError(errno);
		return mp_const_none;
	}

    ets_printf("vfs_native: VFS_CHDIR\n");

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(native_vfs_chdir_obj, native_vfs_chdir);

/// Get the current directory.
STATIC mp_obj_t native_vfs_getcwd(mp_obj_t vfs_in) {
    char buf[MICROPY_ALLOC_PATH_MAX + 1];
    char *ch = getcwd(buf, sizeof(buf));
	if (ch == NULL) {
		mp_raise_OSError(errno);
		return mp_const_none;
	}

    ets_printf("vfs_native: VFS_GETCWD\n");
    return mp_obj_new_str(buf, strlen(buf), false);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(native_vfs_getcwd_obj, native_vfs_getcwd);

/// \function stat(path)
/// Get the status of a file or directory.
STATIC mp_obj_t native_vfs_stat(mp_obj_t vfs_in, mp_obj_t path_in) {
    mp_obj_native_vfs_t *self = MP_OBJ_TO_PTR(vfs_in);
    const char *path = mp_obj_str_get_str(path_in);

	struct stat buf;
    FILINFO fno;
    if (path[0] == 0 || (path[0] == '/' && path[1] == 0)) {
        // stat root directory
        buf.st_size = 0;
		buf.st_atime = 946684800; // Jan 1, 2000
		buf.st_mode = MP_S_IFDIR;
    } else {
		int res = stat(path, &buf);
		if (res < 0) {
			mp_raise_OSError(errno);
			return mp_const_none;
		}
    }

    mp_obj_tuple_t *t = MP_OBJ_TO_PTR(mp_obj_new_tuple(10, NULL));
    t->items[0] = MP_OBJ_NEW_SMALL_INT(buf.st_mode);
    t->items[1] = MP_OBJ_NEW_SMALL_INT(0); // st_ino
    t->items[2] = MP_OBJ_NEW_SMALL_INT(0); // st_dev
    t->items[3] = MP_OBJ_NEW_SMALL_INT(0); // st_nlink
    t->items[4] = MP_OBJ_NEW_SMALL_INT(0); // st_uid
    t->items[5] = MP_OBJ_NEW_SMALL_INT(0); // st_gid
    t->items[6] = MP_OBJ_NEW_SMALL_INT(buf.st_size); // st_size
    t->items[7] = MP_OBJ_NEW_SMALL_INT(buf.st_atime); // st_atime
    t->items[8] = MP_OBJ_NEW_SMALL_INT(buf.st_atime); // st_mtime
    t->items[9] = MP_OBJ_NEW_SMALL_INT(buf.st_atime); // st_ctime

    ets_printf("vfs_native: VFS_STAT\n");
    return MP_OBJ_FROM_PTR(t);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(native_vfs_stat_obj, native_vfs_stat);

// Get the status of a VFS.
STATIC mp_obj_t native_vfs_statvfs(mp_obj_t vfs_in, mp_obj_t path_in) {
    /*mp_obj_native_vfs_t *self = MP_OBJ_TO_PTR(vfs_in);
    (void)path_in;

    DWORD nclst;
    FATFS *fatfs = &self->fatfs;
    FRESULT res = f_getfree(fatfs, &nclst);
    if (FR_OK != res) {
        mp_raise_OSError(fresult_to_errno_table[res]);
    }

    mp_obj_tuple_t *t = MP_OBJ_TO_PTR(mp_obj_new_tuple(10, NULL));

    t->items[0] = MP_OBJ_NEW_SMALL_INT(fatfs->csize * SECSIZE(fatfs)); // f_bsize
    t->items[1] = t->items[0]; // f_frsize
    t->items[2] = MP_OBJ_NEW_SMALL_INT((fatfs->n_fatent - 2)); // f_blocks
    t->items[3] = MP_OBJ_NEW_SMALL_INT(nclst); // f_bfree
    t->items[4] = t->items[3]; // f_bavail
    t->items[5] = MP_OBJ_NEW_SMALL_INT(0); // f_files
    t->items[6] = MP_OBJ_NEW_SMALL_INT(0); // f_ffree
    t->items[7] = MP_OBJ_NEW_SMALL_INT(0); // f_favail
    t->items[8] = MP_OBJ_NEW_SMALL_INT(0); // f_flags
    t->items[9] = MP_OBJ_NEW_SMALL_INT(_MAX_LFN); // f_namemax

    return MP_OBJ_FROM_PTR(t);*/
    ets_printf("DUMMY: VFS_STATVFS\n");
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(native_vfs_statvfs_obj, native_vfs_statvfs);

STATIC mp_obj_t vfs_fat_mount(mp_obj_t self_in, mp_obj_t readonly, mp_obj_t mkfs) {
    fs_user_mount_t *self = MP_OBJ_TO_PTR(self_in);

    // Read-only device indicated by writeblocks[0] == MP_OBJ_NULL.
    // User can specify read-only device by:
    //  1. readonly=True keyword argument
    //  2. nonexistent writeblocks method (then writeblocks[0] == MP_OBJ_NULL already)
    /*if (mp_obj_is_true(readonly)) {
        self->writeblocks[0] = MP_OBJ_NULL;
    }*/

    // mount the block device
    
    const esp_vfs_fat_mount_config_t mount_config = {
            .max_files = 4,
            .format_if_mount_failed = true
    };
    
    esp_err_t err = esp_vfs_fat_spiflash_mount("", "locfd", &mount_config, &s_wl_handle);
    
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to mount FATFS (0x%x)", err);
        mp_raise_OSError(MP_EIO);
    }
        ESP_LOGE(TAG, "mount succes!");
    
    //FRESULT res = f_mount(&self->fatfs);

    // check if we need to make the filesystem
    /*if (res == FR_NO_FILESYSTEM && mp_obj_is_true(mkfs)) {
        uint8_t working_buf[_MAX_SS];
        res = f_mkfs(&self->fatfs, FM_FAT | FM_SFD, 0, working_buf, sizeof(working_buf));
    }*/
    /*if (res != FR_OK) {
        mp_raise_OSError(fresult_to_errno_table[res]);
    }*/

    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_3(vfs_fat_mount_obj, vfs_fat_mount);

STATIC mp_obj_t vfs_fat_umount(mp_obj_t self_in) {
    /*fs_user_mount_t *self = MP_OBJ_TO_PTR(self_in);
    FRESULT res = f_umount(&self->fatfs);
    if (res != FR_OK) {
        mp_raise_OSError(fresult_to_errno_table[res]);
    }
    return mp_const_none;*/
    ets_printf("DUMMY: VFS_UMOUNT\n");
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(native_vfs_umount_obj, vfs_fat_umount);

STATIC const mp_rom_map_elem_t native_vfs_locals_dict_table[] = {
    { MP_ROM_QSTR(MP_QSTR_mkfs), MP_ROM_PTR(&native_vfs_mkfs_obj) },
    { MP_ROM_QSTR(MP_QSTR_open), MP_ROM_PTR(&native_vfs_open_obj) },
    { MP_ROM_QSTR(MP_QSTR_ilistdir), MP_ROM_PTR(&native_vfs_ilistdir_obj) },
    { MP_ROM_QSTR(MP_QSTR_mkdir), MP_ROM_PTR(&native_vfs_mkdir_obj) },
    { MP_ROM_QSTR(MP_QSTR_rmdir), MP_ROM_PTR(&native_vfs_rmdir_obj) },
    { MP_ROM_QSTR(MP_QSTR_chdir), MP_ROM_PTR(&native_vfs_chdir_obj) },
    { MP_ROM_QSTR(MP_QSTR_getcwd), MP_ROM_PTR(&native_vfs_getcwd_obj) },
    { MP_ROM_QSTR(MP_QSTR_remove), MP_ROM_PTR(&native_vfs_remove_obj) },
    { MP_ROM_QSTR(MP_QSTR_rename), MP_ROM_PTR(&native_vfs_rename_obj) },
    { MP_ROM_QSTR(MP_QSTR_stat), MP_ROM_PTR(&native_vfs_stat_obj) },
    { MP_ROM_QSTR(MP_QSTR_statvfs), MP_ROM_PTR(&native_vfs_statvfs_obj) },
    { MP_ROM_QSTR(MP_QSTR_mount), MP_ROM_PTR(&vfs_fat_mount_obj) },
    { MP_ROM_QSTR(MP_QSTR_umount), MP_ROM_PTR(&native_vfs_umount_obj) },
};
STATIC MP_DEFINE_CONST_DICT(native_vfs_locals_dict, native_vfs_locals_dict_table);

const mp_obj_type_t mp_native_vfs_type = {
    { &mp_type_type },
    .name = MP_QSTR_VfsNative,
    .make_new = native_vfs_make_new,
    .locals_dict = (mp_obj_dict_t*)&native_vfs_locals_dict,
};

#endif // MICROPY_VFS_FAT
