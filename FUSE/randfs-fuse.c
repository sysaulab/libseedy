/*
 FUSE: Filesystem in Userspace
 Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

 This program can be distributed under the terms of the GNU GPL.
 See the file LICENSE.txt.

 */

/*
 * Loopback macFUSE file system in C. Uses the high-level FUSE API.
 * Based on the fusexmp_fh.c example from the Linux FUSE distribution.
 * Amit Singh <http://osxbook.com>
 */

#include <AvailabilityMacros.h>
//#include <cstdlib>
//#include <cstdio>
#include <stdio.h>
#include <time.h>

#define HAVE_EXCHANGE 0
#define HAVE_RENAMEX 1
#define HAVE_ACCESS 0

#define FUSE_USE_VERSION 26

#define _GNU_SOURCE

#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <fuse/fuse.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/attr.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mount.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/vnode.h>
#include <sys/xattr.h>
#include <unistd.h>

#if defined(_POSIX_C_SOURCE)
typedef unsigned char  u_char;
typedef unsigned short u_short;
typedef unsigned int   u_int;
typedef unsigned long  u_long;
#endif

#define G_PREFIX                       "org"
#define G_KAUTH_FILESEC_XATTR G_PREFIX ".apple.system.Security"
#define A_PREFIX                       "com"
#define A_KAUTH_FILESEC_XATTR A_PREFIX ".apple.system.Security"
#define XATTR_APPLE_PREFIX             "com.apple."

/* MY CODE V V V V V V V V V V V V V V V V V V V V V V V V V V */
#include "../src/libseedy/libseedy64.h"
#include "../src/tools/algos/cc2032.h"
#include "../src/tools/algos/nm80.h"

/*
 *   Intended to be the seeding RNG that seeds other RNGs.
 *   It is initialised randomnly with libseedy at each start.
 */
CC2032 RNG_ROOT;
NM80 RNG_FS;
char BASEPATH[1025];
char RNGPATH[1025];

#define RANDFS_OPT_DEBUG_ON



#if defined(RANDFS_OPT_DEBUG_ON)
void debug(char* key, char* val)
{
    static FILE* debug_file = NULL;
    if(debug_file == NULL)
    {
        char actualpath [PATH_MAX+1];
        sprintf(actualpath, "%s/debug.log", BASEPATH);
        debug_file = fopen(actualpath, "a");
        if(debug_file == NULL)
        {
            fprintf(stderr, "PANICKING. CANNOT OPEN DEBUG FILE!!!\n%s\n", actualpath);
            exit(0);
        }
    }
    fprintf(debug_file, "%s:%s\n", key, val);
    fflush(debug_file);
}
char VALUE[100];
#define debugU64x(label,value) sprintf(VALUE, "%016llx", (uint64_t)value);debug(label, VALUE)
#define debugU64(label,value) sprintf(VALUE, "%llu", (uint64_t)value);debug(label, VALUE)
#else
#define debug(a,b) ;
#define debugU64x(a,b) ;
#define debugU64(a,b) ;
#endif


/* To handle the opened directories. */






/* MY CODE A A A A A A A A A A A A A A A A A A A A A A A A A A A A A */


struct loopback {
    uint32_t blocksize;
    bool case_insensitive;
};

static struct loopback loopback;

static int
loopback_getattr( const char *path, struct stat *stbuf)
{
    /* https://www.man7.org/linux/man-pages/man7/inode.7.html */
    
    int res;

    if (0 == strcmp("/", path) )
    {
        res = lstat(BASEPATH, stbuf);
    }
    else if (0 == strcmp("/.metadata_never_index", path) )
    {
        res = lstat(RNGPATH, stbuf);
        stbuf->st_size = 0;
    }
    else /* is a file */
    {
        int16_t val;
        
        if(1 == sscanf(path, "/%hx.bin", &val))
        {
            res = lstat(RNGPATH, stbuf);
            stbuf->st_size = 0x7FFFFFFFFFFFFFFF; /* MACOS UPPER LIMIT: 0x7FFFFFFFFFFFFFFF. */
        }
        else /* is unknown */
        {
            res = lstat("/../bad", stbuf);
        }
    }

    return 0;
}

static int
loopback_fgetattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
{
debug("loopback_fgetattr()", (char*)path);
    int res;
    (void)path;

    res = fstat(fi->fh, stbuf);

    // Fall back to global I/O size. See loopback_getattr().
    stbuf->st_blksize = 0;

    if (res == -1) {
        return -errno;
    }

    return 0;
}

#if HAVE_ACCESS

static int
loopback_access(
    const char *path, 
    int mask)
{
    int res;

    /*
     * Standard access permission flags:
     * F_OK            test for existence of file
     * X_OK            test for execute or search permission
     * W_OK            test for write permission
     * R_OK            test for read permission
     *
     * Extended access permission flags that can be enabled by setting
     * FUSE_CAP_ACCESS_EXTENDED (See loopback_init()):
     * _READ_OK        read file data / read directory
     * _WRITE_OK       write file data / add file to directory
     * _EXECUTE_OK     execute file / search in directory
     * _DELETE_OK      delete file / delete directory
     * _APPEND_OK      append to file / add subdirectory to directory
     * _RMFILE_OK      remove file from directory
     * _RATTR_OK       read basic attributes
     * _WATTR_OK       write basic attributes
     * _REXT_OK        read extended attributes
     * _WEXT_OK        write extended attributes
     * _RPERM_OK       read permissions
     * _WPERM_OK       write permissions
     * _CHOWN_OK       change ownership
     */

    res = access(path, mask & (F_OK | X_OK | W_OK | R_OK));
    if (res == -1)
        return -errno;

    return 0;
}

#endif /* HAVE_ACCESS */

static int
loopback_readlink( const char *path, char *buf, size_t size)
{
debug("loopback_readlink()", (char*)path);

    int res;

    res = readlink(path, buf, size - 1);
    if (res == -1) 
    {
        return -errno;
    }

    buf[res] = '\0';

    return 0;
}

struct loopback_dirp
{
    DIR *dp;
    struct dirent *entry;
    off_t offset;
};
static inline struct loopback_dirp *
get_dirp(struct fuse_file_info *fi)
{
    return (struct loopback_dirp *)(uintptr_t)fi->fh;
}


static int
loopback_opendir(const char *path, struct fuse_file_info *fi)
{
    int result;
    struct loopback_dirp* i = malloc(sizeof(struct loopback_dirp)); 
    debug("opendir()", (char*)path);
    if(i == NULL){
        debug("FATAL", "Out of memory");
        exit(EXIT_FAILURE);
    }

    i->offset = 1;



    /* Make a new handle */
    if(path[1] != '\0')
    {
        debug("ERROR", "Path not found");
        return -ENOENT;
    }
        

    fi->fh = (uint64_t)i;

    return 0;
}

static int
loopback_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    struct loopback_dirp* handl = get_dirp(fi);
    debugU64("readdir()", handl->offset);
    
    (void)path;
    int res;

    while ( handl->offset < 256 )
    {
        char hex[32];
        struct stat st; 
        struct dirent dir;        
        handl->entry = &dir;
        lstat(RNGPATH, &st);
        st.st_ino = handl->offset;
        snprintf(hex, 10, "%02hx.bin", (uint16_t)handl->offset);
        strncpy(handl->entry->d_name, hex, 1024);
        
        if (filler(buf, hex, &st, handl->offset + 1)) 
        {
            break;
        }
        handl->offset = handl->offset + 1;
        //debugU64("ITER-D", handl->offset);

    }
    return 0;
}

static int
loopback_releasedir(const char *path, struct fuse_file_info *fi)
{ /* Should be OK, Untested. */
debugU64x("releasedir()", fi->fh);
    free((struct loopback_dirp*)fi->fh);
    return 0;
}

static int
loopback_mknod(const char *path, mode_t mode, dev_t rdev)
{
debug("loopback_mknod()", (char*)path);
    int res;

    if (S_ISFIFO(mode)) {
        res = mkfifo(path, mode);
    } else {
        res = mknod(path, mode, rdev);
    }

    if (res == -1) {
        return -errno;
    }

    return 0;
}

static int
loopback_mkdir(const char *path, mode_t mode)
{
debug("loopback_mkdir()", (char*)path);
    return -EROFS;
    return 0;
}

static int
loopback_unlink(const char *path)
{
debug("loopback_unlink()", (char*)path);
    return -EROFS;
    return 0;
}

static int
loopback_rmdir(const char *path)
{
debug("loopback_rmdir()", (char*)path);
    return -EROFS;
    return 0;
}

static int
loopback_symlink(const char *from, const char *to)
{
debug("loopback_symlink()", (char*)from);
    return -EROFS;
    return 0;
}

static int
loopback_rename(const char *from, const char *to)
{
debug("loopback_rename()", (char*)from);
    return -EROFS;
    return 0;
}

#if HAVE_EXCHANGE

static int
loopback_exchange(const char *path1, const char *path2, unsigned long options)
{
debug("loopback_exchange()", (char*)path1);
    int res;

    res = exchangedata(path1, path2, options);
    if (res == -1) {
        return -errno;
    }

    return 0;
}

#endif /* HAVE_EXCHANGE */

static int
loopback_link(const char *from, const char *to)
{
debug("loopback_link()", (char*)from);
    return -EROFS;
    return 0;
}

static int
loopback_fsetattr_x(const char *path, struct setattr_x *attr, struct fuse_file_info *fi)
{
debug("loopback_fsetattr_x()", (char*)path);
    return -EROFS;
    return 0;
}

static int
loopback_setattr_x(const char *path, struct setattr_x *attr)
{
debug("loopback_setattr_x()", (char*)path);
    return -EROFS;
    return 0;
}

static int
loopback_getxtimes(const char *path, struct timespec *bkuptime,
                   struct timespec *crtime)
{
debug("loopback_getxtimes()", (char*)path);
    return -EROFS;
    return 0;
}

static int
loopback_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
debug("loopback_create()", (char*)path);
    return -EROFS;
    return 0;
}

static int
loopback_open(const char *path, struct fuse_file_info *fi)
{
debug("open()", (char*)path);
    FILE* fd = calloc(1, sizeof(FILE));
    if( fd == NULL )
    {
        debug("FATAL", "Out of memory");
        exit(EXIT_FAILURE);
    }
    sscanf(path, "/%hx.bin", &fd->_file);
    if (fd->_file < 0 || fd->_file > 255)
    {
        return -ENOENT;
    }

    fi->fh = (uint64_t)fd;
    return 0;
}

static int
loopback_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    debug("read()", (char*)path);
    nm80_fill(&RNG_FS, (uint8_t*)buf, size, ((FILE*)fi->fh)->_file, offset);
    return size;
}

static int
loopback_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    debug("write()", (char*)path);
    return -EROFS;
}

static int
loopback_flush(const char *path, struct fuse_file_info *fi)
{
debugU64("flush()", ((FILE*)fi->fh)->_file);
    return 0;
}

static int
loopback_release(const char *path, struct fuse_file_info *fi)
{
debugU64("release()", ((FILE*)fi->fh)->_file);
    free((FILE*)fi->fh);
    return 0;
}

static int
loopback_fsync(const char *path, int isdatasync, struct fuse_file_info *fi)
{
debugU64("fsync()", ((FILE*)fi->fh)->_file);
    return 0;
}

static int
loopback_setxattr(const char *path, const char *name, const char *value, size_t size, int flags, uint32_t position)
{
    debug("setxattr()", (char*)path);
    return 0;
}

static int
loopback_getxattr(const char *path, const char *name, char *value, size_t size,
                  uint32_t position)
{
    debug("setxattr()", (char*)path);
    return 0;
}

static int
loopback_listxattr(const char *path, char *list, size_t size)
{
    debug("listxattr()", (char*)path);
    return 0;
}

static int
loopback_removexattr(const char *path, const char *name)
{
    debug("removexattr()", (char*)path);
    return 0;
}

static int
loopback_fallocate(const char *path, int mode, off_t offset, off_t length, struct fuse_file_info *fi)
{
    debug("fallocate()", (char*)path);
    return 0;
}

static int
loopback_setvolname(const char *name)
{
    debug("setvolname()", (char*)name);
    return 0;
}

static int
loopback_statfs_x(const char *path, struct statfs *stbuf)
{
debug("loopback_statfs_x()", (char*)path);

    stbuf->f_bsize = 4096;    /* optimal transfer block size */
    stbuf->f_blocks = 0x0;    /* total data blocks in file system */
    stbuf->f_bavail = 0;    /* free blocks available to unprivileged user */
    stbuf->f_bfree = 0;     /* free blocks in fs */
    stbuf->f_files = 0;     /* total file nodes in file system */
    stbuf->f_ffree = 0;  /* free file nodes in fs */
    return 0;
}

#if HAVE_RENAMEX

static int
loopback_renamex(const char *path1, const char *path2, unsigned int flags)
{
    debug("renamex()", "start");
    return 0;
}

#endif /* HAVE_RENAMEX */

void *
loopback_init(struct fuse_conn_info *conn)
{
    debug("init()", "start");
    conn->want |= FUSE_CAP_VOL_RENAME | FUSE_CAP_XTIMES ;

#if HAVE_ACCESS
    conn->want |= FUSE_CAP_ACCESS_EXTENDED;
#endif

#ifdef FUSE_ENABLE_CASE_INSENSITIVE
    if (loopback.case_insensitive) {
        conn->want |= FUSE_CAP_CASE_INSENSITIVE;
    }
#endif
    return NULL;
}

void
loopback_destroy(void *userdata)
{
    /* nothing */
}

static struct fuse_operations loopback_oper = {
    .init        = loopback_init,
    .destroy     = loopback_destroy,
    .getattr     = loopback_getattr,
    .fgetattr    = loopback_fgetattr,
#if HAVE_ACCESS
    .access      = loopback_access,
#endif
    .readlink    = loopback_readlink,
    .opendir     = loopback_opendir,
    .readdir     = loopback_readdir,
    .releasedir  = loopback_releasedir,
    .mknod       = loopback_mknod,
    .mkdir       = loopback_mkdir,
    .symlink     = loopback_symlink,
    .unlink      = loopback_unlink,
    .rmdir       = loopback_rmdir,
    .rename      = loopback_rename,
    .link        = loopback_link,
    .create      = loopback_create,
    .open        = loopback_open,
    .read        = loopback_read,
    .write       = loopback_write,
    .flush       = loopback_flush,
    .release     = loopback_release,
    .fsync       = loopback_fsync,
    .setxattr    = loopback_setxattr,
    .getxattr    = loopback_getxattr,
    .listxattr   = loopback_listxattr,
    .removexattr = loopback_removexattr,
#if HAVE_EXCHANGE
    .exchange    = loopback_exchange,
#endif
    .getxtimes   = loopback_getxtimes,
    .setattr_x   = loopback_setattr_x,
    .fsetattr_x  = loopback_fsetattr_x,
    .fallocate   = loopback_fallocate,
    .setvolname  = loopback_setvolname,
    .statfs_x    = loopback_statfs_x,
#if HAVE_RENAMEX
    .renamex     = loopback_renamex,
#endif

    .flag_nullpath_ok = 1,
    .flag_nopath = 1,
};

static const struct fuse_opt loopback_opts[] = {
    { "blocksize=%u", offsetof(struct loopback, blocksize), 0 },
    { "case_insensitive", offsetof(struct loopback, case_insensitive), true },
    FUSE_OPT_END
};



int
main(int argc, char *argv[])
{
    int new = 0;
    int res = 0;
    char* home = getenv("HOME");
    struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
    cc2032_init(&RNG_ROOT,seedy64);
    snprintf(BASEPATH, 1024, "%s/.randfs", home);
    mkdir(BASEPATH, 0740);
    debug("START", "##### ##### ##### ##### ##### ##### ##### #####\n\n");
    snprintf(RNGPATH, 1024, "%s/state.bin", BASEPATH);
    nm80_init(&RNG_FS, RNGPATH);




debug("home", home);
debug("BASEPATH", BASEPATH);



    loopback.blocksize = 4096;
    loopback.case_insensitive = 0;
    if (fuse_opt_parse(&args, &loopback, loopback_opts, NULL) == -1) {
        exit(1);
    }

    umask(0);
    res = fuse_main(args.argc, args.argv, &loopback_oper, NULL);
    
    fuse_opt_free_args(&args);
    return res;
}

