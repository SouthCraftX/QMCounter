#ifndef QMC_H
#   define QMC_H

#   include <stdlib.h>
#   include <wchar.h>

#   if !defined(QMC_DEBUG) && (defined(_DEBUG) || defined(DEBUG)) && !defined(NDEBUG)
#       define QMC_DEBUG
#   endif 

// platform
#   if defined(_MSDOS)
#       error "libQMC does not support MS-DOS"
#   elif (defined(_WIN16) || defined(_WIN32) || defined(_WIN64)) && !defined(__QMC_WINDOWS__)
#       define __QMC_WINDOWS__
#   endif

// 64-bit or 32-bit
#   if defined(__QMC_WINDOWS__)
#       if defined(_WIN64)
#           define __QMC_64BIT__
#       endif // defined(_WIN64)
#   elif defined(POSIX)
#       if (defined(__x86_64__) || defined(__LP64__) || defined(__amd64) || defined(__aarch64__))
#           define __QMC_64BIT__
#       endif // (defined(__x86_64__) || defined(__LP64__) || defined(__amd64) || defined(__aarch64__))
#   endif // defined(__QMC_WINDOWS__)

//endian
#   if defined(__BYTE_ORDER__)
#       if (__BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__)
#           define __QMC_LITTLE_ENDIAN__
#       elif (__BYTE_ORDER__ == __ORDER_BIG_ENDIAN__)
#           define __QMC_BIG_ENDIAN__
#       endif // defined(__BYTE_ORDER__)
#   elif (defined(__linux__) || defined(__OpenBSD__) || defined(__DragonFly__))
#       include <endian.h>
#       if (__BYTE_ORDER == __LITTLE_ENDIAN)
#           define __QMC_LITTLE_ENDIAN__
#       elif (__BYTE_ORDER == __BIG_ENDIAN)
#           define __QMC_BIG_ENDIAN__
#       endif // (defined(__linux__) || defined(__OpenBSD__) || defined(__DragonFly__))
#   elif defined(__hppa__) || defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || \
        (defined(__MIPS__) && defined(__MIPSEB__)) || defined(__ppc__) || defined(__POWERPC__) ||\
        defined(__powerpc__) || defined(__PPC__) || defined(__sparc__)
#       define __QMC_BIG_ENDIAN__
#   endif // defined(__BYTE_ORDER__)

#   if !(defined(__QMC_BIG_ENDIAN__) || defined(__QMC_LITTLE_ENDIAN__))
#       error "libQMC:unsupported endianness!"
#   endif

// Force Inline
#   if defined(__GNUC__)
#       define __QMC_FORCEINLINE__ __attribute__((always_inline))
#   elif defined(_MSC_VER)
#       define __QMC_FORCEINLINE__ __forceinline
#   else
#       define __QMC_FORCEINLINE__ inline
#       if __STDC_VER__ >= 202305L
#           warning "libQMC: inline may fail"
#       endif // __STDC_VER__ >= 202305L
#   endif // defined(__GNUC__)

// Noreturn Attribude 
#   if defined(__GNUC__)
#       define __QMC_NORETURN__ __attribute__((noreturn))
#   elif defined(_MSC_VER)
#       define __QMC_NORETURN__ __declspec(noreturn)
#   elif __STDC_VERSION__ >= 201112L
#       define __QMC_NORETURN__ _Noreturn
#   else
#       error "libQMC:Cannot define __QMC_NORETURN__ macro!"
#   endif // defined(__GNUC__)

// Fallthrough 
#   if defined(__GNUC__)
#       define __QMC_FALLTHROUGH__ __attribute__((fallthrough))
#   elif defined(_MSC_VER)
#       define __QMC_FALLTHROUGH__ __QMC_FALLTHROUGH__
#   elif __STDC_VERSION__ > 202300L /* C23 Future Std*/
#       define __QMC_FALLTHROUGHT__ [[fallthrough]]
#   else
#       error "libQMC:Cannot define __QMC_FALLTHROUGH__"
#   endif // defined(__GNUC__)

#   define __QMC_ABSTRACT__ 
#   define __QMC_EXPORT__ extern
#   define __QMC_INTERNAL__ static
#   if defined(__GNUC__)
#       define __QMC_GLOBAL_CONSTRUCTOR__ __attribute__((constructor)) 
#   endif // defined(__GNUC__)

#   if defined(__cplusplus)
extern "C" {
#   endif // defined(__cplusplus)

// Define your own __QMC_ASSERT__ to override them
#   if !defined(__QMC_ASSERT__)
#       define __QMC_ASSERT__(e)
#       define __QMC_REPORT_BUG__() 
#       if defined(QMC_DEBUG)
#           undef __QMC_ASSERT__
#           undef __QMC_REPORT_BUG__ 
__QMC_NORETURN__ __QMC_INTERNAL__
void __qmc_asserted_0_handler(qmc_int32_t line , qmc_cstr_t func_name)
{
    __builtin_printf("libQMC Debug:Fatal Error:Asserted false "\
                      "in line %i , function %s" , line , func_name);
    __builtin_abort();
}

__QMC_NORETURN__ __QMC_INTERNAL__
void __qmc_report_bug(qmc_int32_t line , qmc_cstr_t func_name)
{
    __builtin_printf("libQMC Debug:Fatal Error:Buggy code occurred "\
                      "in line %i , function %s" , line , func_name);
    __builtin_abort();
}

#          define __QMC_ASSERT__(e) (\
    if(!(e)) \ 
        __qmc_asserted_0_handler(__LINE__ , __func__) \
)

#          define __QMC_REPORT_BUG__() ( \
    __qmc_report_bug(__LINE__ , __func__) \
)
#       endif // defined(QMC_DEBUG)
#   endif // !defined(__QMC_ASSERT__)

#   define __QMC_IN_RANGE__(l,x,r) ((l <= x) && (x <= r))

#   if !defined(QMC_DISABLE_LIBC_TYPE_LIMITS)
#       include <limits.h>
#   endif // !defined(QMC_USER_TYPE_LIMITS)

#   if !defined(QMC_DISABLE_LIBC_SJLJ)
#       include <setjmp.h>
#   endif // !defined(QMC_DISABLE_LIBC_SJLJ)

// Define your own qmc_uint8_t to override them
#   if !defined(qmc_uint8_t)
#
#       include <stdatomic.h>
#       include <stdint.h>
#       include <stdbool.h>
#       include <math.h>

typedef     uint8_t         qmc_uint8_t;
typedef     int8_t          qmc_int8_t;
typedef     uint16_t        qmc_uint16_t;
typedef     int16_t         qmc_int16_t;
typedef     uint32_t        qmc_uint32_t;
typedef     int32_t         qmc_int32_t;
typedef     uint64_t        qmc_uint64_t;
typedef     int64_t         qmc_int64_t;

typedef     _Atomic uint8_t         qmc_auint8_t;
typedef     _Atomic int8_t          qmc_aint8_t;
typedef     _Atomic uint16_t        qmc_auint16_t;
typedef     _Atomic int16_t         qmc_aint16_t;
typedef     _Atomic uint32_t        qmc_auint32_t;
typedef     _Atomic int32_t         qmc_aint32_t;
typedef     _Atomic uint64_t        qmc_auint64_t;
typedef     _Atomic int64_t         qmc_aint64_t;

typedef     qmc_int8_t      qmc_flag8_t;
typedef     qmc_int16_t     qmc_flag16_t;
typedef     qmc_int32_t     qmc_flag32_t;

typedef     qmc_aint32_t    qmc_aflag32_t;

typedef     qmc_uint8_t     qmc_byte_t;

typedef     char*           qmc_str_t;
typedef     const char*     qmc_cstr_t;
typedef     wchar_t*        qmc_wstr_t;
typedef     const wchar_t*  qmc_cwstr_t;

typedef     float_t         qmc_fp32_t;
typedef     double_t        qmc_fp64_t;

typedef     qmc_flag32_t    qmc_errno_t;
typedef     qmc_uint32_t    qmc_size_t;
#       if defined(__QMC_64BIT__)
typedef     qmc_uint64_t    qmc_longsize_t;
#       else
typedef     qmc_size_t      qmc_longsize_t; 
#       endif // defined(__QMC_64BIT__)
typedef     qmc_uint16_t    qmc_thrd_count_t;

typedef     qmc_int32_t(*qmc_func_t)(void *);
typedef     void*           qmc_abstract_obj_t;

typedef     qmc_byte_t*(*qmc_alloc_fn_t)(qmc_abstract_obj_t,qmc_longsize_t);
typedef     qmc_byte_t*(*qmc_dealloc_fn_t)(qmc_abstract_obj_t,qmc_byte_t*,qmc_longsize_t);

#   endif // !defined(qmc_uint8_t) 

#   if defined(__QMC_64BIT__)
typedef     qmc_uint64_t    qmc_event_count_t;
typedef     qmc_uint64_t    qmc_tick_t;
#   else
typedef     qmc_uint32_t    qmc_event_count_t;
typedef     qmc_uint32_t    qmc_tick_t;
#   endif // defined(__QMC_64BIT__)

typedef     qmc_uint16_t    qmc_track_count_t;
typedef     qmc_uint16_t    qmc_resolution_t;
typedef     qmc_flag32_t    qmc_midi_format_t;

// Error codes
enum
{
    QMC_OK = 0,
    QMC_BAD_ARG,
    QMC_BAD_ARCH_PATH,
    QMC_BAD_FILE_PATH,
    QMC_BAD_PATH,
    QMC_PERMISSION_DENIED,
    QMC_OPEN_FAILED,
    QMC_GET_FILE_SIZE_FAILED,
    QMC_BAD_MIDI_HEADER_SIZE,
    QMC_BAD_MIDI_HEADER,
    QMC_FILE_TOO_BIG,
    QMC_UNSUPPORTED_FORMAT,
    QMC_BAD_TRACK_SIZE,
    QMC_BAD_TRACK_HEADER,
    QMC_BAD_MIDI,
    QMC_OUT_OF_MEM,
    QMC_LOAD_DLL_FAILED,
    QMC_DLL_ALREADY_LOADED,
    QMC_CREATE_THRD_FAILED,
    QMC_UNKNOWN_ERR
};

// Midi Formats
enum
{
    QMC_FMT_SINGLETRACK = 0,
    QMC_FMT_MUILTTRACK,
    QMC_FMT_SMPTETIME
};

enum
{
    QMC_CANCEL = 1000,
    QMC_COMPLETED
};

#   define __QMC_LOCAL_TRACK_BUF_MAX__ 1048576 // 1MB
#   define __QMC_RES_FIRST_ALLOC_COUNT__ 524288 // 512*sizeof(struct qmc_thrd_temp_result) KB
#   define __QMC_TEXT_EVENT_SET_INIT_SIZE__  2048
#   define __QMC_TEXT_event_ptr_count__ 64

__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_int32_t qmc_quick_log2(qmc_int32_t x)
{
    qmc_fp32_t fp32_data = (qmc_fp32_t)x;
    return (qmc_uint32_t)((*((qmc_uint32_t*)&fp32_data) >> 23) & 0xFF) - 127;
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_uint16_t qmc_u16_be_to_le(qmc_uint16_t x)
{
    return (x >> 8) + (x << 8);
}



struct qmc_global_pool{
    //sizeof(pool_bitmap) : sizeof(pool_mem) = 1 : 64
    qmc_uint64_t pool_bitmap[8]; // 64bytes/512bits/512slices/4096bytes
    qmc_byte_t pool_mem[4096];
} __qmc_global_mem_pool;

qmc_uint8_t qmc_align_to_slice(qmc_uint8_t size)
{
    return size >> 3 + ((size % 8) ? 1 : 0);
}



// @slice_count can only be 1,2,4 or 8
qmc_byte_t* qmc_find_free_slices_in_pool(qmc_uint8_t slice_count)
{
    bool res;
    qmc_uint64_t* now_pos = __qmc_global_mem_pool.pool_bitmap;
    do
    {
        res = *now_pos & UINT64_MAX;
        ++now_pos;
    } while (res && now_pos < __qmc_global_mem_pool.pool_bitmap+8 );
    
    // Poll is full
    if(now_pos == __qmc_global_mem_pool.pool_bitmap+8)
    {
        return NULL;
    }

    if (*(qmc_uint32_t*)now_pos & UINT32_MAX)
    {
        /* code */
    }
    else
    {

    }

}

// Max size= 248 bytes
__QMC_INTERNAL__
qmc_byte_t* qmc_get_free_slices_in_pool(qmc_uint8_t slice_count)
{
    __QMC_ASSERT__(4);
    return qmc_find_free_slices_in_pool(slice_count);
}

// Error Descriptor
struct qmc_errdesc
{
    qmc_errno_t id;
    qmc_str_t further_info;
};

// <===========Backends===========>
// ----Input File Stream----
__QMC_ABSTRACT__
struct qmc_ifile
{
    bool is_prefetched_stream;
    qmc_longsize_t file_size;
    qmc_abstract_obj_t context;   // Could be the handle of anything , likes ordinary file, pipe, even file in zip
    qmc_errno_t(*fn_open)(struct qmc_ifile*,qmc_cstr_t);
    qmc_longsize_t(*fn_read)(qmc_abstract_obj_t,qmc_byte_t**,qmc_longsize_t);
    void(*fn_close)(qmc_abstract_obj_t);
};

#   define qmc_fopen    qmc_open_ifile
#   define qmc_fread    qmc_read_ifile
#   define qmc_fclose   qmc_close_ifile
#   define qmc_finit    qmc_init_ifile

#   define QMC_DEFINE_IFILE(var_name) (struct qmc_ifile var_name[1] ; qmc_finit(var_name)) 

__QMC_FORCEINLINE__ 
qmc_errno_t qmc_open_ifile(struct qmc_ifile* p_ifile , qmc_cstr_t path)
{
    return (*p_ifile->fn_open)(p_ifile , path);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_longsize_t qmc_read_ifile(struct qmc_ifile* p_ifile , qmc_byte_t** p_buf ,qmc_longsize_t size)
{
    return (*p_ifile->fn_read)(p_ifile->context , p_buf , size);
}

__QMC_FORCEINLINE__
void qmc_close_ifile(struct qmc_ifile* p_ifile)
{
    return (*p_ifile->fn_close)(p_ifile->context);
}

#   if !defined(QMC_DISABLE_SYSTEM_FILE_API)
#       if defined(__QMC_WINDOWS__) 
#           include <fileapi.h>         // CreateFile , ReadFile , ...
#           include <handleapi.h>       // CloseHandle
#           include <errhandlingapi.h>  // GetLastError
#           include <stringapiset.h>    // MultiByteToWideChar 

__QMC_INTERNAL__
qmc_errno_t __qmc_win_open_file(struct qmc_ifile* p_ifile , qmc_cstr_t path)
{
    HANDLE file_handle;

    // Convert char* to wchar_t*
    wchar_t wc_path[MAX_PATH] = { };
    MultiByteToWideChar(CP_THREAD_ACP , 0 , path , -1 , wc_path , MAX_PATH);

    file_handle = CreateFileW(wc_path , GENERIC_READ , FILE_SHARE_READ , NULL ,
                        OPEN_EXISTING , FILE_FLAG_SEQUENTIAL_SCAN , NULL);

    if(file_handle)
    {
        LARGE_INTEGER file_size;
        if(GetFileSizeEx(file_handle , &file_size))
        {
            p_ifile->context = file_handle;
#           if defined(__QMC_64BIT__)
            p_ifile->file_size = file_size.QuadPart;
#           else // 32-bit version cannot handle files that larger than 4GiB
            if(file_size.u.HighPart)
            {
                CloseHandle(file_handle);
                return QMC_FILE_TOO_BIG;
            }
            p_ifile->file_size = file_size.u.LowPart;
#           endif // defined(__QMC_64BIT__)
            return QMC_OK;
        }
        CloseHandle(file_handle);
        return QMC_GET_FILE_SIZE_FAILED;
    }

    switch (GetLastError())
    {
        case ERROR_FILE_NOT_FOUND: 
        case ERROR_INVALID_DRIVE:  
        case ERROR_PATH_NOT_FOUND:  return QMC_BAD_PATH;
        case ERROR_ACCESS_DENIED:   return QMC_PERMISSION_DENIED;
    
        default:
            return QMC_OPEN_FAILED;
    }
}

__QMC_INTERNAL__ __QMC_FORCEINLINE__
qmc_size_t __qmc_win_read_file_32(void* handle , qmc_byte_t* buf , qmc_size_t size)
{
    qmc_size_t total_read;
    return ReadFile(handle , buf , size , &total_read , NULL) ? total_read : 0;
}

#           if defined(__QMC_64BIT__)
__QMC_INTERNAL__
qmc_longsize_t __qmc_win_read_file(void* handle , qmc_byte_t** p_buf ,qmc_longsize_t size)
{
    qmc_longsize_t n_read = 0;
    qmc_byte_t* buf = *p_buf;

    while(size > UINT32_MAX)
    {
        n_read += __qmc_win_read_file_32(handle , buf , UINT32_MAX);
        buf += UINT32_MAX , size -= UINT32_MAX;
    }
    n_read += __qmc_win_read_file_32(handle , buf , UINT32_MAX);
    return n_read;
}
#           endif // defined(__QMC_64BIT__)

__QMC_INTERNAL__
void __qmc_win_close_file(void* handle)
{
    CloseHandle(handle);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_init_ifile(struct qmc_ifile* p_ifile)
{
    p_ifile->is_prefetched_stream = false;
    p_ifile->fn_close = __qmc_win_close_file;
    p_ifile->fn_open  = __qmc_win_open_file;
    p_ifile->fn_read  = __qmc_win_read_file;
}

#       elif defined(POSIX)
#           include <unistd.h>
#           include <fcntl.h>
#           include <sys/stat.h>

__QMC_INTERNAL__
qmc_errno_t __qmc_posix_open_file(struct qmc_ifile* p_ifile , qmc_cstr_t path)
{
    int file_handle = open(path , O_RDONLY | O_LARGEFILE);
    if(file_handle == -1)
    {
        switch ( errno )
        {

            case EACCES:
                return QMC_PERMISSION_DENIED;
            case EISDIR: __QMC_FALLTHROUGH__
            case EINVAL: __QMC_FALLTHROUGH__
            case EFAULT: __QMC_FALLTHROUGH__
            case ENOENT: __QMC_FALLTHROUGH__
            case ENAMETOOLONG:
                return QMC_BAD_PATH;
            case ELOOP: __QMC_REPORT_BUG__();
            case ENOMEN:
                return QMC_OUT_OF_MEM;
            default:
                return QMC_OPEN_FAILED;
        }
    }
    struct stat file_stat;
    if(fstat(file_handle , &file_stat))
    {
        close(file_handle);
        return QMC_GET_FILE_SIZE_FAILED;
    }
    p_ifile->file_size = file_stat.st_size;
    p_ifile->context = (qmc_abstract_obj_t)file_handle;
    return QMC_OK;
}

__QMC_INTERNAL__
qmc_longsize_t __qmc_posix_read_file(qmc_abstract_obj_t file_handle , qmc_byte_t** p_buf , 
                                     qmc_longsize_t size)
{
    read((int)file_handle , *p_buf , size);
}

__QMC_INTERNAL__
void __qmc_posix_close_file(qmc_abstract_obj_t file_handle)
{
    close((int)file_handle);
}

__QMC_FORCEINLINE__
void qmc_init_ifile(struct qmc_ifile* p_ifile)
{
    p_ifile->fn_open = __qmc_posix_open_file;
    p_ifile->fn_read = __qmc_posix_read_file;
    p_ifile->fn_close = __qmc_posix_close_file;
}

#       else
#           error "libQMC:No system macros detected.Have you configured your build environment properly?"
#       endif // defined(__QMC_WINDOWS__)

#   elif !defined(QMC_DISABLE_LIBC_FILE_API)
#       include <stdio.h>
#       include <errno.h>

#       if defined(_MSC_VER)
#           define __FSEEK  _fseeki64_nolock
#           define __FTELL  _ftelli64_nolock
#           define __FREAD  _fread_nolock
#           define __FCLOSE _fclose_nolock
#       else
#           define __FSEEK  fseek
#           define __FTELL  ftell
#           define __FREAD  fread
#           define __FCLOSE fclose
#       endif // defined(_MSC_VER)

__QMC_FORCEINLINE__
void qmc_init_ifile(struct qmc_ifile* p_ifile)
{
    p_ifile->fn_open = __qmc_libc_open_file;
    p_ifile->fn_read = __qmc_libc_read_file;
    p_ifile->fn_close = __FCLOSE; 
}

__QMC_INTERNAL__
qmc_errno_t __qmc_libc_open_file(struct qmc_ifile* p_ifile , qmc_cstr_t path)
{
    FILE* file_handle = fopen(path , "rb");
    if(file_handle)
    {
        if(__FSEEK(file_handle , 0 , SEEK_END))
        {
            __FCLOSE(file_handle);
            return QMC_GET_FILE_SIZE_FAILED;
        }
        p_ifile->file_size = __FTELL(file_handle);
        if(__FSEEK(file_handle , 0 , SEEK_SET)) // Move FPI back
        {
            __FCLOSE(file_handle);
            return QMC_OPEN_FAILED;
        }
        p_ifile->context = file_handle;
        return QMC_OK;
    }
    return QMC_OPEN_FAILED;
}

qmc_longsize_t __qmc_libc_read_file(void* handle , qmc_byte_t** p_buf , qmc_longsize_t size)
{
    return __FREAD((void*)*p_buf , 1 , size , (FILE*)handle);
}

__QMC_FORCEINLINE__
qmc_errno_t qmc_open_ifile(struct qmc_ifile* p_ifile , qmc_cstr_t path)
{
    (*p_ifile->fn_open)(p_ifile , path);
}

__QMC_FORCEINLINE__
qmc_longsize_t qmc_read_ifile(struct void* handle , qmc_byte_t** p_buf , qmc_longsize_t size)
{
    (*p_ifile->fn_read)(handle , p_buf , size);
}

__QMC_FORCEINLINE__
void qmc_close_ifile(void* handle)
{
    (*p_ifile->fn_close)(handle);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_init_ifile(struct qmc_ifile* p_ifile)
{
    p_ifile->is_prefetched_stream = false;
    p_ifile->fn_close = __FCLOSE;
    p_ifile->fn_open  = __qmc_libc_open_file;
    p_ifile->fn_read  = __qmc_libc_read_file;
}

#       undef __FSEEK 
#       undef __FCLOSE
#       undef __FREAD
#       undef __FTELL

#   else
#       if !(defined(qmc_fopen) && defined(qmc_fread) && defined(qmc_fclose) && defined(qmc_finit))
#           error 
#       endif 
#   endif

// ----Compressed File Stream----
#   if !defined(QMC_DISABLE_COMPRESSED_FILE_STREAM)
__QMC_INTERNAL__

#       if defined(__QMC_WINDOWS__)
#           include <libloaderapi.h>
struct qmc_lib7z_api
{
    HMODULE dll_handle;
} __qmc_lib7z_api;
qmc_errno_t qmc_load_lib7z(qmc_cstr_t dll_path)
{
    if(__qmc_lib7z_api.dll_handle)
    {
        return QMC_DLL_ALREADY_LOADED;
    }

    __qmc_lib7z_api.dll_handle = LoadLibrary(dll_path);
    if(__qmc_lib7z_api.dll_handle)
    {

    }
    return QMC_BAD_PATH;
}

void qmc_unload_lib7z()
{
    if(__qmc_lib7z_api.dll_handle)
    {
        FreeLibrary(__qmc_lib7z_api.dll_handle);
        __qmc_lib7z_api.dll_handle = NULL;
    }
}
#       else
#           include <dlfcn.h>

qmc_errno_t qmc_load_lib7z(qmc_cstr_t dll_path)
{
    qmc_abstract_obj_t dll_handle = dloepn(dll_path , RTLD_NOW);
    if(dll_handle)
    {

    }
    return QMC_BAD_PATH;
}
#       endif // defined(__QMC_WINDOWS__)





struct qmc_compressed_file_context
{
    

};



qmc_errno_t qmc_cf_open(qmc_abstract_obj_t context , qmc_cstr_t path , qmc_size_t total_size)
{
    // Init
    struct qmc_compressed_file_context* cf_context = context;
    char arch_path_and_file_name[total_size];
    memcpy(arch_path_and_file_name , path , total_size);
    qmc_cstr_t arch_path = arch_path_and_file_name;
    qmc_str_t file_name = strchr(arch_path_and_file_name , '\n');
    file_name[-1] = 0;

    

}

#   endif // !defined(QMC_DISABLE_COMPRESSED_FILE_STREAM)

// ----Prefetched File Stream----
struct qmc_pf_ifile
{
    struct qmc_track_allocator* allocator; // Isn't NULL if file is opened
    qmc_byte_t* file_buf_begin;
    qmc_byte_t* file_buf_cur;
    qmc_longsize_t remain_size;
}; // sizeof(struct qmc_pf_ifile) == 32    (In 64bit platform)


qmc_errno_t qmc_pf_ifile_open(struct qmc_pf_ifile* p_pf_ifile , qmc_cstr_t path , 
                              struct qmc_track_allocator* allocator)
{
    struct qmc_ifile ordinary_file[1];
    qmc_finit(ordinary_file);

    qmc_errno_t open_ret;
    if(open_ret = qmc_fopen(ordinary_file , path))
    {
        return open_ret;
    }

    p_pf_ifile->file_buf_begin = qmc_alloc_track(allocator , ordinary_file->file_size);
    if(p_pf_ifile->file_buf_begin)
    {
        qmc_fread(ordinary_file , &p_pf_ifile->file_buf_begin , ordinary_file->file_size);
        p_pf_ifile->remain_size = ordinary_file->file_size;
        p_pf_ifile->file_buf_cur = p_pf_ifile->file_buf_begin;
        qmc_fclose(ordinary_file);
        return QMC_OK;
    }
    return QMC_OUT_OF_MEM;
}

qmc_longsize_t qmc_pf_ifile_read(struct qmc_pf_ifile* p_pf_ifile , qmc_byte_t** p_buf , qmc_longsize_t size)
{
    *p_buf = p_pf_ifile->file_buf_cur;
    if(p_pf_ifile->remain_size >= size)
    {
        return size;
    }
    return p_pf_ifile->remain_size;
}

void qmc_pf_ifile_close(struct qmc_pf_ifile* p_pf_ifile)
{
    if(p_pf_ifile->allocator)
    {
        qmc_free_track(p_pf_ifile->allocator , p_pf_ifile->file_buf_begin);
        p_pf_ifile->allocator = NULL;
    }
}

// ----Threading----
__QMC_INTERNAL__ qmc_thrd_count_t __qmc_cpu_thrd_n = 0;

#   if !defined(QMC_DISABLE_SYSTEM_THREADING_API)
#       if defined(__QMC_WINDOWS__)

#           include <sysinfoapi.h>  // GetSystemInfo
#           include <processthreadsapi.h> // CreateThread , ExitThread , ...

#           define __QMC_WAIT_FOR_MEMORY_TIME__ 1 // 5ms

#           if defined(__QMC_GLOBAL_CONSTRUCTOR__)
__QMC_GLOBAL_CONSTRUCTOR__ __QMC_INTERNAL__
void qmc_get_cpu_thrd_count_from_system()
{
    SYSTEM_INFO sys_info;
    GetNativeSystemInfo(&sys_info);
    __qmc_cpu_thrd_n = sys_info.dwNumberOfProcessors;
}

qmc_thrd_count_t qmc_get_cpu_thrd_count()
{
    return __qmc_cpu_thrd_n;
}
#           else
__QMC_INTERNAL__
void qmc_get_cpu_thrd_count_from_system()
{
    SYSTEM_INFO sys_info;
    GetNativeSystemInfo(&sys_info);
    __qmc_cpu_thrd_n = sys_info.dwNumberOfProcessors;
}

qmc_thrd_count_t qmc_get_cpu_thrd_count()
{
    if(__qmc_cpu_thrd_n)
        return __qmc_cpu_thrd_n;

    qmc_get_cpu_thrd_count_from_system();
    return __qmc_cpu_thrd_n;
}
#           endif // defined(__QMC_GLOBAL_CONSTRUCTOR__)

struct qmc_thrd
{
    HANDLE thread_handle;
};

__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_errno_t qmc_thrd_start(struct qmc_thrd* p_thrd , qmc_func_t func , void* arg)
{
    p_thrd->thread_handle = CreateThread(NULL , 0 , (LPTHREAD_START_ROUTINE)func , arg , 0 , NULL);
    return (p_thrd->thread_handle ? QMC_OK : QMC_CREATE_THRD_FAILED);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_thrd_exit(qmc_errno_t ret)
{
    ExitThread(ret);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_wait_for_memory()
{
    Sleep(1);
}

#       else
#           include <pthread.h> // pthread_*
#           include <sys/time.h> // setitimer

#           define __QMC_WAIT_FOR_MEMORY_TIME__ 5000 // 5ms

struct qmc_thrd
{
    pthread_t thread_handle;
};

__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_errno_t qmc_thrd_start(struct qmc_thrd* p_thrd ,qmc_func_t func , void* arg)
{
    return (pthread_create(&p_thrd->thread_handle , NULL , func , arg)) ?  
           QMC_CREATE_THRD_FAILED : QMC_OK;
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_thrd_wait_for_memory()
{
    struct itimerval delay_time = { 
        (struct timerval){0 , 0},
        (struct timerval){0 , 1000000}
    };
    setitimer(ITIMER_PROF , &delay_time);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_thrd_exit(qmc_errno_t ret)
{
    pthread_exit(ret);
}

#       endif // defined(__QMC_WINDOWS__)
#   elif !define(QMC_DISABLE_LIBC_THREADING_API)
#       include <threads.h>

struct qmc_thrd
{
    thrd_t thread;
};

__QMC_INTERNAL__
qmc_errno_t qmc_thrd_create(struct qmc_thrd* p_thrd , qmc_func_t func , void* arg)
{
    switch(thrd_create(&p_thrd->thread , func , arg ))
    {
        case thrd_success:  return QMC_OK;
        case thrd_nomem:    return QMC_OUT_OF_MEM;
        case thrd_error:    return QMC_CREATE_THRD_FAILED;
    }
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_thrd_exit(qmc_errno_t ret)
{
    thrd_exit(ret);
}

#   elif defind(qmc_thrd) && defined(qmc_thrd_create) && defined(qmc_thrd_stop)

#   else
#       error "libQMC:Threading-related methods are not impled!"
#   endif

struct qmc_thrd_group
{
    
    struct qmc_thrd* thread_set;
};

#   if !defined(QMC_DISABLE_SYSTEM_MEMORY_API)
#       if defined(__QMC_WINDOWS__)
#           include <winbase.h>
#           include <heapapi.h>
struct qmc_base_allocator
{
    HANDLE heap;
};

typedef struct qmc_base_allocator qmc_track_allocator;
typedef struct qmc_base_allocator qmc_small_allocator;

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_track_allocator_init(qmc_track_allocator* track_allocator , qmc_longsize_t max_size)
{
    track_allocator->heap = HeapCreate(HEAP_NO_SERIALIZE , 0 , max_size);
    // Enable heap terminate-on-corruption. 
    (void)HeapSetInformation(track_allocator->heap , HeapEnableTerminationOnCorruption , NULL , 0);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_byte_t* qmc_alloc_track(qmc_track_allocator* track_allocator , qmc_longsize_t size)
{
    return (qmc_byte_t*)HeapAlloc(track_allocator->heap , HEAP_NO_SERIALIZE , size);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_free_track(qmc_track_allocator* track_allocator , qmc_byte_t* ptr , qmc_size_t size)
{
    (void)size;
    HeapFree(track_allocator->heap , 0 , ptr);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_track_allocator_destory(qmc_track_allocator* track_allocator)
{
    HeapDestroy(track_allocator->heap);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_small_allocator_init(qmc_small_allocator* track_allocator , qmc_longsize_t max_size)
{
    track_allocator->heap = HeapCreate(0 , 0 , max_size);
    // Enable heap terminate-on-corruption. 
    (void)HeapSetInformation(track_allocator->heap , HeapEnableTerminationOnCorruption , NULL , 0);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_byte_t* qmc_small_alloc(qmc_small_allocator* track_allocator , qmc_longsize_t size)
{
    return (qmc_byte_t*)HeapAlloc(track_allocator->heap , 0 , size);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_byte_t* qmc_small_calloc(qmc_small_allocator* track_allocator , qmc_longsize_t size)
{
    return (qmc_byte_t*)HeapAlloc(track_allocator->heap , HEAP_ZERO_MEMORY , size);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_byte_t* qmc_small_realloc(qmc_small_allocator* small_allocator , qmc_byte_t* old_ptr ,
                              qmc_longsize_t old_size , qmc_longsize_t new_size)
{
    (void)old_size;
    return (qmc_byte_t*)HeapReAlloc(small_allocator->heap , 0 , old_ptr ,  new_size);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_byte_t* qmc_small_recalloc(qmc_small_allocator* small_allocator , qmc_byte_t* old_ptr ,
                               qmc_longsize_t old_size , qmc_longsize_t new_size)
{
    (void)old_size;
    return (qmc_byte_t*)HeapReAlloc(small_allocator->heap , HEAP_ZERO_MEMORY , old_ptr ,  new_size);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_small_free(qmc_small_allocator* small_allocator , qmc_byte_t* ptr , qmc_longsize_t size)
{
    (void)size;
    HeapFree(small_allocator->heap , 0 , ptr);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_allocator_destory(struct qmc_base_allocator* allocator)
{
    HeapDestroy(allocator->heap);
}

#       elif defined(__linux__)
#           include <stdlib.h> // malloc , realloc , free
#           include <linux/vmalloc.h> // __vmalloc , vfree_atomic

struct qmc_base_allocator
{
    qmc_longsize_t free_size;
};

typedef struct qmc_base_allocator qmc_track_allocator;
typedef struct qmc_base_allocator qmc_small_allocator;

__QMC_FORCEINLINE__ __QMC_INTERNAL__
bool qmc_allocatable(struct qmc_base_allocator* track_allocator , qmc_size_t desired_size)
{
    return track_allocator->free_size >= desired_size;
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_track_allocator_init(qmc_track_allocator* track_allocator , qmc_longsize_t max_size)
{
    track_allocator->free_size = max_size;
}

qmc_byte_t* qmc_alloc_track(qmc_track_allocator* track_allocator , qmc_size_t size)
{
    if(qmc_allocatable(track_allocator , size))
    {
        qmc_byte_t* ptr = __vmalloc(size , __GFP_HIGH | GFP_NOWAIT );
        if(ptr)
        {
            track_allocator->free_size -= size;
        }
        return ptr;
    }
    return NULL;
}



__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_free_track(qmc_track_allocator* track_allocator , qmc_byte_t* ptr , qmc_size_t size)
{
    vfree_atomic(ptr);
    track_allocator->free_size += size;
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_small_allocator_init(qmc_small_allocator* small_allocator , qmc_longsize_t max_size)
{
    qmc_track_allocator_init(small_allocator , max_size);
}

qmc_byte_t* qmc_small_alloc(qmc_small_allocator* small_allocator , qmc_size_t size)
{
    if(qmc_allocatable(small_allocator , size))
    {
        qmc_byte_t* ptr = malloc(size);
        if(ptr)
        {
            small_allocator->free_size -= size;
        }
        return ptr;
    }
    return NULL;
}

qmc_byte_t* qmc_small_calloc(qmc_small_allocator* small_allocator , qmc_size_t size)
{
    if(qmc_allocatable(small_allocator , size))
    {
        qmc_byte_t* ptr = calloc(size);
        if(ptr)
        {
            small_allocator->free_size -= size;
        }
        return ptr;
    }
    return NULL;
}

qmc_byte_t* qmc_small_realloc(qmc_small_allocator* small_allocator , qmc_byte_t* old_ptr ,
                              qmc_longsize_t old_size , qmc_longsize_t new_size)
{
    qmc_longsize_t delta_size = new_size - old_size;
    if(qmc_allocatable(small_allocator , delta_size))
    {
        qmc_byte_t* ptr = realloc(old_ptr , new_size);
        if(ptr)
        {
            small_allocator->free_size -= delta_size;
        }
        return ptr;
    }
    return NULL;
}

qmc_byte_t* qmc_small_recalloc(qmc_small_allocator* small_allocator , qmc_byte_t* old_ptr ,
                               qmc_longsize_t old_size , qmc_longsize_t new_size)
{
    qmc_byte_t* ptr = qmc_small_realloc(small_allocator , old_ptr , old_size , new_size);
    if(ptr)
    {
        memset(ptr + old_size , 0 , new_size - old_size);
    }
    return ptr;
}

void qmc_small_free(qmc_small_allocator* small_allocator , qmc_byte_t* ptr , qmc_longsize_t size)
{
    free(ptr);
    small_allocator->free_size += size;
}

#           define qmc_allocator_destroy(x)

#       else
#           error "Unsupported OS,please define QMC_DISABLE_SYSTEM_MEMORY_API"
#       endif // defined(__QMC_WINDOWS__)

#   elif !defined(QMC_DISABLE_LIBC_MEMORY_API)

#   elif defined(qmc_track_allocator) && defined(qmc_small_allocator) && defined(qmc_track_alloc) \\
         defined(qmc_small_alloc)

#   else
#       error
#   endif // !defined(QMC_DISABLE_SYSTEM_MEMORY_API)

// ---- Constant Object Allocator ----
struct qmc_const_object_allocator
{
    qmc_small_allocator* upstream_allocator;
    qmc_str_t* pool;
    qmc_str_t* free_point;
    qmc_longsize_t free_size;
    qmc_longsize_t acquired_size;
};

__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_errno_t qmc_init_cobj_allocator(struct qmc_const_object_allocator* cobj_allocator ,
                                    qmc_small_allocator* upstream_allocator , 
                                    qmc_longsize_t init_size)
{
    cobj_allocator->upstream_allocator = upstream_allocator;
    return ( cobj_allocator->pool = qmc_small_alloc(upstream_allocator , init_size) ) ?
           (cobj_allocator->free_size = init_size , QMC_OK) : 
           QMC_OUT_OF_MEM;
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_str_t qmc_alloc_cobj(struct qmc_const_object_allocator* cobj_allocator , 
                           qmc_longsize_t size)
{
#   if defined(QMC_DEBUG)
    if(cobj_allocator->pool)
    {
        return qmc_alloc_cobj_nocheck(cobj_allocator , size);
    }
    __QMC_REPORT_BUG__();
#   else
    return qmc_alloc_cobj_nocheck(cobj_allocator , size);
#   endif
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_str_t qmc_alloc_cobj_nocheck(struct qmc_const_object_allocator* cobj_allocator , 
                                   qmc_longsize_t size)
{
    if(cobj_allocator->free_size < size)
    {
        qmc_longsize_t new_size = cobj_allocator->acquired_size * 1.2;
        qmc_str_t new_ptr = qmc_small_realloc(cobj_allocator->upstream_allocator , 
                                              cobj_allocator->pool , 
                                              cobj_allocator->acquired_size , 
                                              new_size);        
        if(new_ptr)
        {
            cobj_allocator->acquired_size = new_size;
            cobj_allocator->pool = new_size;
        }
        else
        {
            return NULL;
        }
    }
    qmc_str_t* ret_ptr = cobj_allocator->free_point;
    cobj_allocator->free_point += size;
    cobj_allocator->free_size -= size;
    return ret_ptr;
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_destory_cobj_allocator(struct qmc_const_object_allocator* cobj_allocator)
{
    qmc_small_free(cobj_allocator->upstream_allocator , 
                   cobj_allocator->pool , 
                   cobj_allocator->free_size + cobj_allocator->acquired_size);
    cobj_allocator->pool = NULL;
}

// ---- Lock ----
typedef void(*qmc_lock_fn_t)(void*);

// ----Conditional Lock----
#   if !defined(QMC_DIABLE_SYSTEM_CONDLOCK_API)

#       if defined(__QMC_WINDOWS__)
#           include <synchapi.h>
struct qmc_condlock
{
    _Atomic bool is_resource_busy;
    CRITICAL_SECTION lock;      // Lock
    CONDITION_VARIABLE cond;    // Condition Variable
};

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_condlock_init(struct qmc_condlock* p_lock)
{
    p_lock->is_resource_busy = false;
    InitializeCriticalSection(&p_lock->lock);
    InitializeConditionVariable(&p_lock->cond);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_condlock_acquire(struct qmc_condlock* p_lock)
{
    EnterCriticalSection(&p_lock->lock);
    qmc_condlock_wait_for_cond(p_lock);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_condlock_release(struct qmc_condlock* p_lock)
{
    LeaveCriticalSection(&p_lock->lock);
    p_lock->is_resource_busy = false;
    qmc_condlock_wake_others(p_lock);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_condlock_wait_for_cond(struct qmc_condlock* p_lock)
{
    while (p_lock->is_resource_busy)
    {
        SleepConditionVariableCS(&p_lock->cond , &p_lock->lock , INFINITY);  
    }
    p_lock->is_resource_busy = true;
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_condlock_wake_others(struct qmc_condlock* p_lock)
{
    WakeConditionVariable(&p_lock->cond);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_condlock_destory(struct qmc_condlock* p_lock)
{
    /* Do Nothing */
}
#       else
#           include <pthread.h>

struct qmc_condlock
{
    _Atomic bool is_resource_busy;
    pthread_mutex_t lock;   // Lock
    pthread_cond_t cond;    // Condition Variable
};

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_condlock_init(struct qmc_condlock* p_lock)
{
    pthread_mutex_init(&p_lock->lock , NULL);
    pthread_cond_init(&p_lock->cond , NULL);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_condlock_acquire(struct qmc_condlock* p_lock)
{
    pthread_mutex_lock(&p_lock->lock);
    qmc_condlock_wait_for_cond(p_lock);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_condlock_release(struct qmc_condlock* p_lock)
{
    pthread_mutex_unlock(&p_lock->lock);
    p_lock->is_resource_busy = false;
    qmc_condlock_wake_others(p_lock);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_condlock_wait_for_cond(struct qmc_condlock* p_lock)
{
    while (p_lock->is_resource_busy)
    {
        pthread_cond_wait(&p_lock->cond , &p_lock->lock);
    }
    p_lock->is_resource_busy = true;
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_condlock_wake_others(struct qmc_condlock* p_lock)
{
    pthread_cond_signal(&p_lock->cond);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_condlock_destory(struct qmc_condlock* p_lock)
{
    pthread_mutex_destory(&p_lock->lock);
    pthread_cond_destory(&p_lock->cond);
}

#       endif // defiend(__QMC_WINDOWS__)

#   elif defined(qmc_condlock) && defined(qmc_condlock_acquire) && \\
         defined(qmc_condlock_init) && defined(qmc_condlock_release)

#   else

#   endif // !defined(QMC_DIABLE_SYSTEM_CONDLOCK_API)


// Define your own qmc_spinlock , relative methods , 
// QMC_DISABLE_SYSTEM_SPINLOCK_API , QMC_DISABLE_BUILDIN_SPINLOCK to override them
#   if !defined(QMC_DISABLE_SYSTEM_SPINLOCK_API)
#       if defined(__QMC_WINDOWS__)
#           include<wdm.h>

struct qmc_spinlock
{
    KSPIN_LOCK lock;
    KIRQL irql;
};

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_init_lock_as_spinlock(struct qmc_lock* p_lock)
{
    p_lock->lock_context = qmc_get_free_slices_in_pool(qmc_align_to_slice(sizeof qmc_spinlock));
    p_lock->fn_init = (void(*)(void*))qmc_spinlock_init;
    p_lock->fn_acquire = (void(*)(void*))qmc_spinlock_acquire;
    p_lock->fn_release = (void(*)(void*))qmc_spinlock_release;
}

void qmc_spinlock_init(struct qmc_spinlock* p_lock)
{
    KeInitializeSpinLock(&p_lock->lock);
}

void qmc_spinlock_acquire(struct qmc_spinlock* p_lock)
{
    KeAcquireSpinLock(&p_lock->lock , &p_lock->irql);
}

void qmc_spinlock_release(struct qmc_spinlock* p_lock)
{
    KeReleaseSpinLock(&p_lock->lock , p_lock->irql);
}
#       elif defined(__linux__)
#           include <linux/spinlock.h>
struct qmc_spinlock
{
    raw_spinlock_t lock;
};

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_spinlock_init(struct qmc_spinlock* p_lock)
{
    p_lock->lock = __RAW_SPIN_LOCK_UNLOCKED(p_lock->lock);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_spinlock_acquire(struct qmc_spinlock* p_lock)
{
    raw_spin_lock(&p_lock->lock);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_spin_lock_release(struct qmc_spinlock* p_lock)
{
    raw_spin_unlock(&p_lock->lock);
}
#       endif // defined(__QMC_WINDOWS__)
#   elif !defined(QMC_DISABLE_BUILDIN_SPINLOCK)



#   elif defined(qmc_spinlock) && defined(qmc_spinlock_init) && \
         defined(qmc_spinlock_acquire) && defined(qmc_spinlock_release)

#   else
#       error "libQMC:No spinlock relatived methods impled!"
#   endif

// <===========QMCounter===========>

struct qmc_midi_global_meta
{
    qmc_midi_format_t format;
    qmc_track_count_t track_count;
    qmc_resolution_t resolution;
};

struct qmc_text_event
{
    qmc_tick_t tick;
    qmc_str_t text[0];
};

struct qmc_scan_track_context
{
    qmc_size_t track_size;
    qmc_byte_t* track_buf;
    jmp_buf rollback_point;
};

struct qmc_text_event_set
{
    qmc_event_count_t count;
    qmc_longsize_t free_count;
    qmc_longsize_t free_size;
    qmc_longsize_t acquired_size;
    struct qmc_text_event** array;
    struct qmc_text_event* event_pool;
    struct qmc_text_event* free_point;
};

__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_longsize_t qmc_sizeof_text_event(qmc_longsize_t text_len)
{
    return sizeof(qmc_tick_t) + text_len + 1;
}

__QMC_INTERNAL__
qmc_errno_t qmc_init_text_event_set(struct qmc_text_event_set* text_event_set, qmc_small_allocator* small_allocator)
{
    text_event_set->event_pool = qmc_small_alloc(small_allocator , __QMC_TEXT_EVENT_SET_INIT_SIZE__);
    text_event_set->array = qmc_small_alloc(small_allocator , __QMC_TEXT_event_ptr_count__);
    if(text_event_set->event_pool && text_event_set->array)
    {
        text_event_set->free_size = __QMC_TEXT_EVENT_SET_INIT_SIZE__;
        text_event_set->acquired_size = __QMC_TEXT_EVENT_SET_INIT_SIZE__;
        text_event_set->count = 0;
        return QMC_OK;
    }
    return QMC_OUT_OF_MEM;
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_longsize_t qmc_text_event_auto_realloc(struct qmc_text_event_set* text_event_set , 
                                qmc_longsize_t str_len , struct qmc_scan_track_context* scan_ctx ,
                                qmc_small_allocator* small_allocator  )
{
    qmc_longsize_t event_len = qmc_sizeof_text_event(str_len);
    if(event_len > text_event_set->free_size)
    {
        qmc_longsize_t new_size = (text_event_set->acquired_size + event_len) * 1.25;
        struct qmc_text_event* new_pool =  qmc_small_realloc(   
            small_allocator , text_event_set->event_pool ,      
            text_event_set->acquired_size , new_size            
        );
        if(new_pool)
        {
            text_event_set->free_size += new_size - text_event_set->acquired_size;
            text_event_set->acquired_size = new_size;
            text_event_set->free_point = text_event_set->event_pool + 
                (text_event_set->acquired_size - text_event_set->free_size);
        }
        longjmp(scan_ctx->rollback_point , QMC_OUT_OF_MEM);
    }

    if(!text_event_set->free_count)
    {
        qmc_longsize_t new_size = text_event_set->count << 1;
        struct qmc_text_event** new_array = qmc_small_realloc(   
            small_allocator , text_event_set->array ,      
            text_event_set->acquired_size , new_size            
        );
        if(new_array)
        {
            text_event_set->array = new_array;
            text_event_set->free_count = text_event_set->count;
        }
        longjmp(scan_ctx->rollback_point , QMC_OUT_OF_MEM);
    }

    return event_len;
}

// Roll back if it's out of memory
__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_text_event_set_add(struct qmc_text_event_set* text_event_set , 
                            qmc_byte_t* str_begin , qmc_longsize_t str_len , 
                            qmc_tick_t tick , 
                            qmc_small_allocator* small_allocator , 
                            struct qmc_scan_track_context* scan_ctx)
{ 
    qmc_longsize_t event_len = qmc_text_event_auto_realloc(
        text_event_set , str_len , scan_ctx , small_allocator
    );
    *(qmc_tick_t*)text_event_set->free_point = tick;
    memcpy((qmc_tick_t*)text_event_set->free_point + 1 , str_begin , str_len);
    text_event_set->free_point += event_len;
    *((qmc_byte_t*)text_event_set->free_point - 1) = 0;
    text_event_set->free_count--;
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_longsize_t qmc_text_event_set_calc_merge_size(
    struct qmc_text_event_set** src_array , qmc_longsize_t src_count , 
    qmc_longsize_t* p_event_ptr_count
)
{
    qmc_longsize_t event_ptr_count = 0 , total_pool_size = 0;
    for(struct qmc_text_event_set** p = src_array ; p != src_array + src_count ; ++p)
    {
        event_ptr_count += (*p)->count;
        total_pool_size += (*p)->acquired_size - (*p)->free_size;
    }
    *p_event_ptr_count = event_ptr_count;
    return total_pool_size;
}

__QMC_INTERNAL__
void qmc_text_event_set_change_array_offset(
    struct qmc_text_event** begin , struct qmc_text_event** end ,
    ptrdiff_t offset
){
    for(struct qmc_text_event** p = begin ; p != end ; ++p)
    {
        *p += offset;
    }
}

__QMC_INTERNAL__
void qmc_text_event_set_perform_copy(
    struct qmc_text_event_set* dst , struct qmc_text_event_set* src_array ,
    qmc_longsize_t src_count 
){
    struct qmc_text_event* cur_dst_pool = dst->event_pool;
    qmc_longsize_t cur_pool_size , cur_array_size;
    struct qmc_text_event** cur_dst_array = dst->array;
    ptrdiff_t pool_offset;

    for (struct qmc_text_event_set* p = src_array ; p < src_array + src_count; p++)
    {
        pool_offset = dst->event_pool - p->event_pool;
        cur_pool_size =  p->acquired_size - p->free_size;
        cur_array_size = p->count * sizeof(struct qmc_text_event_set**);
        memcpy(cur_dst_pool , p->event_pool , cur_pool_size); // Copy Pool
        memcpy(cur_dst_array , p->array , cur_array_size);
        qmc_text_event_set_change_array_offset(
            cur_dst_array , cur_dst_array + cur_array_size , pool_offset
        ); 
        cur_dst_pool += cur_pool_size;
    }
    
}



__QMC_INTERNAL__
qmc_errno_t qmc_text_event_set_merge(
    struct qmc_text_event_set* dst , struct qmc_text_event_set* src_array , 
    qmc_longsize_t src_count , qmc_small_allocator* small_allocator
){
    qmc_longsize_t event_ptr_count;
    qmc_longsize_t pool_size = qmc_text_event_set_calc_merge_size(
        src_array , src_count , &event_ptr_count
    );
    struct qmc_text_event_set temp_set = {
        .event_pool = qmc_small_alloc(small_allocator , pool_size) ,
        .array = qmc_small_alloc(small_allocator , sizeof(struct qmc_text_event_set*) * event_ptr_count) ,
        .free_size = 0 , 
        .free_count = 0,
        .acquired_size = pool_size ,
        .count = event_ptr_count
    };
    if(temp_set.event_pool && temp_set.array)
    {
        qmc_text_event_set_perform_copy(dst , src_array , src_count);
        /* Sort */
        *dst = temp_set;
    }
    if(temp_set.event_pool)
        qmc_small_free(small_allocator , temp_set.event_pool , pool_size);
    
    return QMC_OUT_OF_MEM;
}


__QMC_INTERNAL__
void qmc_text_event_set_destory(struct qmc_text_event_set* text_event_set , 
                                qmc_small_allocator* small_allocator)
{
    qmc_small_free(small_allocator , text_event_set->array , 
                   text_event_set->count + text_event_set->free_count);
    qmc_small_free(small_allocator , text_event_set->event_pool , 
                   text_event_set->acquired_size);
}

typedef struct qmc_text_event qmc_text_event_t;         // FF 01 len text
typedef struct qmc_text_event qmc_copyright_event_t;    // FF 02 len text
typedef struct qmc_text_event qmc_track_name_event_t;   // FF 03 len text

typedef struct qmc_text_event qmc_lyrics_event_t;       // FF 05 len text
typedef struct qmc_text_event qmc_marker_event_t;       // FF 06 len text
typedef struct qmc_text_event qmc_cue_point_event_t;    // FF 07 len text

union qmc_midi_meta
{
    
    struct 
    {
        // All event end with NULL
        struct qmc_text_event_set   text;      
        struct qmc_text_event_set   copyright; 
        struct qmc_text_event_set   track_name;
        struct qmc_text_event_set   __reserved; // Reserve for Instrument Name
        struct qmc_text_event_set   lyrics;    
        struct qmc_text_event_set   marker;    
        struct qmc_text_event_set   cue_point;

    } in_name;
    struct 
    {
        struct qmc_text_event* event_set[7];
    } in_array;
};

struct qmc_working_stat
{
    qmc_aflag32_t working_flag;
    qmc_aflag32_t working_thrd_count;
};

struct qmc_midi_stream
{
    qmc_auint16_t loaded_track;
    qmc_auint16_t remain_track;   
    qmc_track_allocator track_allocator;
    struct qmc_ifile* file;
    qmc_abstract_obj_t io_lock;
    qmc_errno_t(*fn_load_track)(struct qmc_midi_stream* , qmc_byte_t** , 
                                qmc_size_t* , qmc_uint16_t);
};

struct qmc_tickframe_result
{
    qmc_uint32_t noteon_n;
    qmc_uint32_t noteoff_n;
    qmc_uint32_t tempo_microsec; // It will be 0 if there is no tempo change event
};

struct qmc_thrd_tickframe_result
{
    struct qmc_tickframe_result* tf_res;
    qmc_size_t count;
};

struct qmc_temp_result
{
    struct qmc_thrd_tickframe_result* temp_res;
};

struct qmcounter
{
    struct qmc_midi_stream midi_stream;
};



__QMC_FORCEINLINE__ __QMC_INTERNAL__
qmc_size_t qmc_midi_stream_read_header(struct qmc_midi_stream* midi_stream ,
                                        qmc_byte_t** p_buf)
{
    return qmc_fread(midi_stream->file , p_buf , 6);
}

__QMC_INTERNAL__
qmc_errno_t qmc_load_midi_header(struct qmc_midi_stream* midi_stream , struct qmc_midi_global_meta* meta)
{
    qmc_flag16_t midi_format;
    qmc_byte_t header_buf[6];
    qmc_byte_t* header = header_buf;
    
    if(qmc_midi_stream_read_header(midi_stream , &header) < 6)
        return QMC_BAD_MIDI;

    if(*(qmc_uint32_t*)header != 'MThd')
        return QMC_BAD_MIDI_HEADER;

    if(*((qmc_uint32_t*)header + 1) != 6)
        return QMC_BAD_MIDI_HEADER_SIZE;

#   if defined(__QMC_BIG_ENDIAN__)
    midi_format = *((uint16_t*)header + 4);
#   else
    midi_format = qmc_u16_be_to_le(*((uint16_t*)header_buf+4));
#   endif // defined(__QMC_BIG_ENDIAN__)

    switch(midi_format)
    {
        case QMC_FMT_SINGLETRACK: __QMC_FALLTHROUGH__
        case QMC_FMT_MUILTTRACK:
            meta->format = midi_format; 
            break;
        default:
            return QMC_UNSUPPORTED_FORMAT;
    }

#   if defined(__QMC_BIG_ENDIAN__)
    meta->track_count = *((uint16_t*)header + 5);
    meta->resolution = *((uint16_t*)header + 6);
#   else
    meta->track_count = qmc_u16_be_to_le(*((uint16_t*)header + 5));
    meta->resolution = qmc_u16_be_to_le(*((uint16_t*)header + 6));
#   endif // defined(__QMC_BIG_ENDIAN__)
}

// Return 1 if the EOT event is OK
__QMC_FORCEINLINE__ __QMC_INTERNAL__
bool qmc_test_track_last_4_bytes(qmc_byte_t** p_buf , qmc_size_t track_size)
{
    return qmc_test_end_of_track_event(*(qmc_uint32_t*)(*p_buf + track_size - 4));
}

// Return 1 if the EOT event is OK
__QMC_FORCEINLINE__ __QMC_INTERNAL__
bool qmc_test_end_of_track_event(qmc_uint32_t track_last_4_bytes)
{
    return (track_last_4_bytes << 8) == 0xFF2F0000;
}

// Return 0 if the track header is OK
__QMC_FORCEINLINE__ __QMC_INTERNAL__
bool qmc_test_track_header(struct qmc_midi_stream* midi_stream , qmc_uint32_t* header_buf)
{
    return !((qmc_fread(midi_stream->file , &header_buf , 8) == 8) && *header_buf == 'Mtrk' );
}

__QMC_INTERNAL__
qmc_errno_t qmc_load_track(struct qmc_midi_stream* midi_stream , qmc_byte_t** p_buf ,
                          qmc_size_t* p_track_size , qmc_uint16_t* p_track_id)
{
    // track_header[0] : MTrk Flag
    // track_header[1] : Track Size
    qmc_uint32_t track_header[2];
    qmc_flag32_t ret;
    bool allocated = false;

    if(midi_stream->remain_track)
    {
        qmc_condlock_acquire((struct qmc_condlock*)midi_stream->io_lock);
        // Critical Section BEGIN
        if(qmc_test_track_header(midi_stream , track_header))
        {
            ret = QMC_BAD_TRACK_HEADER;
            goto JMPFLAG_UNLOCK_AND_RETURN;
        }
        if(track_header[1] > __QMC_LOCAL_TRACK_BUF_MAX__)
        {
            if(!(*p_buf = qmc_alloc_track(&midi_stream->track_allocator , track_header[1])))
            {
                ret = QMC_OUT_OF_MEM;
                goto JMPFLAG_UNLOCK_AND_RETURN;
            }
            allocated = true;
        }
        if(qmc_fread(midi_stream->file , p_buf , track_header[1]) < track_header[1])
        {
            ret = QMC_BAD_MIDI;
            goto JMPFLAG_UNLOCK_AND_RETURN;
        }
        if(qmc_test_track_last_4_bytes(p_buf , track_header[1])) // Is it EOT event?
        {
            *p_track_size = track_header[1];
            midi_stream->remain_track--;
            *p_track_id = midi_stream->loaded_track;
            midi_stream->loaded_track++;
            ret = QMC_OK;
            goto JMPFLAG_UNLOCK_AND_RETURN;
        }
        ret = QMC_BAD_TRACK_SIZE;

JMPFLAG_UNLOCK_AND_RETURN:
        if(allocated && ret)
        {
            qmc_free_track(&midi_stream->track_allocator , *p_buf , track_header[1]);
        }
        qmc_condlock_release((struct qmc_condlock*)midi_stream->io_lock);
        return ret;
    }
    return QMC_COMPLETED;
}

qmc_errno_t qmc_load_track_with_pfs(struct qmc_midi_stream* midi_stream , qmc_byte_t** p_buf , 
                                    qmc_size_t* p_track_size , qmc_uint16_t* p_track_id)
{
    // track_header[0] : MTrk Flag
    // track_header[1] : Track Size
    qmc_uint32_t track_header[2];
    qmc_flag32_t ret;

    if(midi_stream->remain_track)
    {
        qmc_spinlock_acquire((struct qmc_spinlock*)midi_stream->io_lock);
        // Critical Section BEGIN
        if(qmc_test_track_header(midi_stream , track_header))
        {
            ret = QMC_BAD_TRACK_HEADER;
            goto JMPFLAG_UNLOCK_AND_RETURN;
        }
        if(qmc_fread(midi_stream->file , p_buf , track_header[1]) < track_header[1])
        {
            ret = QMC_BAD_MIDI;
            goto JMPFLAG_UNLOCK_AND_RETURN;
        }
        if(qmc_test_track_last_4_bytes(p_buf , track_header[1])) // Is it EOT event?
        {
            *p_track_size = track_header[1];
            midi_stream->remain_track--;
            *p_track_id = midi_stream->loaded_track;
            midi_stream->loaded_track++;
            ret = QMC_OK;
            goto JMPFLAG_UNLOCK_AND_RETURN;
        }
        /*(*p_buf[track_header[1] - 3] == 0xFF) && (*p_buf[track_header[1] - 2] == 0x2F) && (*p_buf[track_header[1] - 1] == 0x00)*/
        ret = QMC_BAD_TRACK_SIZE;

JMPFLAG_UNLOCK_AND_RETURN:
        // Critical Section END
        qmc_spinlock_release((struct qmc_spinlock*)midi_stream->io_lock);
        return ret;
    }
    return QMC_COMPLETED;
}

qmc_uint32_t vlq_to_u32_move(qmc_byte_t** p)
{
    qmc_uint32_t i = **p & 0x7Fu;
	while (*(*p)++ & 0x80u)
	{
		(i <<= 7);
        i |= **p & 0x7Fu;
	}
	return i;
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_auto_free_track(qmc_track_allocator* track_allocator , qmc_size_t track_size , 
                         qmc_byte_t* local_buf , qmc_byte_t* track_buf)
{           
    if(local_buf == track_buf) return;
    qmc_free_track(track_allocator , track_buf , track_size);
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_auto_handle_cancel_flag(
    qmc_size_t track_size ,
    qmc_byte_t* local_buf ,
    qmc_byte_t* track_buf ,
    struct qmc_midi_stream* midi_stream , 
    qmc_small_allocator* small_allocator ,
    struct qmc_thrd_tickframe_result* res ,
    struct qmc_working_stat* working_stat
){
    if(working_stat->working_flag == QMC_CANCEL)
    {
        qmc_auto_free_track(&midi_stream->track_allocator ,track_size , local_buf , track_buf);
        qmc_small_free(small_allocator , res->tf_res , res->count);
        qmc_thrd_exit(QMC_CANCEL);
    }
}  

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_handle_out_of_mem_before_scan(struct qmc_working_stat* working_stat , 
                                   qmc_small_allocator* small_allocator , 
                                   struct qmc_thrd_tickframe_result* basic_res)
{
    if(working_stat->working_thrd_count == 1)
    {
        // Give up the whole task if no else threads are working 
        working_stat->working_flag = QMC_CANCEL;
        qmc_small_free(small_allocator , basic_res->tf_res , basic_res->count);
        qmc_thrd_exit(QMC_OUT_OF_MEM);
    }
    working_stat->working_thrd_count--;
    qmc_wait_for_memory();
    working_stat->working_thrd_count++; 
}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
void qmc_analyse_sysex_event()
{

}

// @qmc_analyse_FN_event , @qmc_analyse_meta_event , @qmc_analyse_event :
// Arg @event : The begin byte of the event
// Return true if it is a EOT event

__QMC_FORCEINLINE__ __QMC_INTERNAL__
bool qmc_analyse_meta_event(qmc_byte_t** p_event , struct qmc_tickframe_result* basic_res ,
                            union qmc_midi_meta* meta , qmc_track_count_t cur_track_id ,
                            struct qmc_const_object_allocator* cobj_allocator)
{
    p_event++;
    if(__QMC_IN_RANGE__(1 , *p_event , 2) || __QMC_IN_RANGE__(5 , *p_event , 7))
    {
        qmc_uint8_t index = *(++p_event) - 1;
        ((meta->in_array.event[index]) + (meta->in_array.end_index[index])) = 

    }
    else if (*(qmc_uint16_t*)p_event == 0x5103) // Tempo Change
    {
        /* code */
    }
    

}

__QMC_FORCEINLINE__ __QMC_INTERNAL__
bool qmc_analyse_FN_event(qmc_byte_t** p_event , struct qmc_tickframe_result* basic_res ,
                          union qmc_midi_meta* meta , qmc_uint16_t cur_track ,
                          struct qmc_const_object_allocator* cobj_allocator)
{
    switch (**p_event)
    {
        case 0xFF:
            qmc_analyse_meta_event(p_event , basic_res , meta , cobj_allocator , cur_track);
            break;
        
        case 0xF0:
            qmc_analyse_sysex_event();
            break;

        default:
            __QMC_REPORT_BUG__();
    }
}


__QMC_FORCEINLINE__ __QMC_INTERNAL__
bool qmc_analyse_event(qmc_byte_t** p_event , struct qmc_tickframe_result* basic_res ,
                       union qmc_midi_meta* meta , qmc_uint16_t cur_track ,
                       struct qmc_const_object_allocator* cobj_allocator)
{
    switch (**p_event >> 4)
    {
        case 0x09: // noteon
            *p_event += 2;
            if(**p_event) basic_res->noteon_n++;
            else // Regard noteon(vel=0) as noteoff
                basic_res->noteoff_n++;
            break;
        
        case 0x08: // noteoff
            basic_res->noteoff_n++;
            *p_event += 4;
            break;

        case 0x0F:  // meta-event
            return qmc_analyse_FN_event(p_event , basic_res , meta , cobj_allocator , cur_track);

        default:
            break;
    }
}

__QMC_INTERNAL__
qmc_errno_t qmc_scan_loop(qmc_byte_t* track_buf , qmc_size_t track_size , 
                          struct qmc_thrd_tickframe_result* basic_res , 
                          )
{
    qmc_byte_t* cur_buf_pos = track_buf;
    jmp_buf jmp_point;
    qmc_errno_t ret = setjmp(jmp_point);
    if(!ret)
    {
        while (cur_buf_pos < track_buf + track_size)
        {
            qmc_uint32_t delta_time = vlq_to_u32_move(&cur_buf_pos);
            cur_tf_res_pos += delta_time;
            qmc_analyse_event(&cur_buf_pos , basic_res , midi_meta , track_id ,  cobj_allocator);
        }
    }
    else
    {
        return ret;
    }
}

__QMC_INTERNAL__
qmc_errno_t qmc_scan_section(struct qmc_midi_stream* midi_stream , qmc_byte_t* track_buf , 
                             qmc_byte_t* local_buf , qmc_size_t track_size , 
                             struct qmc_thrd_tickframe_result* basic_res , 
                             union qmc_midi_meta* midi_meta , qmc_track_count_t track_id ,
                             qmc_small_allocator* small_allocator)
{
    qmc_byte_t* cur_buf_pos = track_buf;
    struct qmc_tickframe_result* cur_tf_res_pos;
    qmc_errno_t exit_ret = QMC_OK;
    // Fist Alloc
    cur_tf_res_pos = qmc_small_calloc(
        small_allocator , 
        __QMC_RES_FIRST_ALLOC_COUNT__ * sizeof(struct qmc_tickframe_result)
    ); 
    if(cur_tf_res_pos)
    {   
        qmc_scan_loop();
    }
    else
    {
        exit_ret = QMC_OUT_OF_MEM;
    }

    qmc_auto_free_track(&midi_stream->track_allocator , track_size , local_buf , track_buf);
    qmc_thrd_exit(exit_ret);
}

__QMC_INTERNAL__
qmc_errno_t qmc_scan_track(struct qmc_midi_stream* midi_stream , 
                           qmc_small_allocator* small_allocator , 
                           struct qmc_const_object_allocator* cobj_allocator , 
                           struct qmc_thrd_tickframe_result* basic_res , 
                           union qmc_midi_meta** midi_meta ,
                           struct qmc_working_stat* working_stat)
{
    qmc_byte_t local_buf[__QMC_LOCAL_TRACK_BUF_MAX__];
    qmc_byte_t* track_buf;
    qmc_size_t track_size;
    qmc_flag32_t load_ret;
    qmc_uint16_t track_id = 0;

    while (true)
    {
        qmc_auto_handle_cancel_flag(track_size , local_buf , track_buf , midi_stream , 
                                    small_allocator , basic_res , working_stat);

        track_buf = local_buf;
        load_ret = (*midi_stream->fn_load_track)(midi_stream , &track_buf , &track_size , &track_id);
        if(load_ret)
        {
            switch (load_ret)
            {   
                case QMC_COMPLETED:
                    qmc_thrd_exit(QMC_OK);
                    break;

                case QMC_OUT_OF_MEM:
                    qmc_handle_out_of_mem_before_scan(working_stat , small_allocator , basic_res);
                    continue;

                default:
                    break;
            }
        }
    }
        qmc_byte_t* cur_buf_pos = track_buf;
        struct qmc_tickframe_result* cur_tf_res_pos;
        qmc_errno_t exit_ret = QMC_OK;

        // Fist Alloc
        cur_tf_res_pos = qmc_small_calloc(
            small_allocator , 
            __QMC_RES_FIRST_ALLOC_COUNT__ * sizeof(struct qmc_tickframe_result)
        ); 
        if(cur_tf_res_pos)
        {   
            while (cur_buf_pos < track_buf + track_size)
            {
                qmc_uint32_t delta_time = vlq_to_u32_move(&cur_buf_pos);
                cur_tf_res_pos += delta_time;
                qmc_analyse_event(&cur_buf_pos , basic_res , midi_meta , track_id ,  cobj_allocator);
            }
        }
        else
        {
            exit_ret = QMC_OUT_OF_MEM;
        }
    
        qmc_auto_free_track(&midi_stream->track_allocator , track_size , local_buf , track_buf);
        qmc_thrd_exit(exit_ret);

    
    
}
#   if defined(__cplusplus)
}
#   endif // extern "C"

#endif //