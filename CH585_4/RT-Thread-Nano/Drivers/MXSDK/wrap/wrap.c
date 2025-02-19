/*
 * Copyright (c) 2024, smartmx <smartmx@qq.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-07-05     smartmx      the first version
 *
 */

/*
 * 通过增加ld选项来在link环节替换掉默认的库函数，格式：-Wl,--wrap=function_name 不同选项之间以空格隔开。
 * -Wl,--wrap=malloc
 * -Wl,--wrap=free
 * -Wl,--wrap=calloc
 * -Wl,--wrap=realloc
 * -Wl,--wrap=memcpy
 * -Wl,--wrap=memset
 * -Wl,--wrap=memcmp
 * -Wl,--wrap=vsnprintf
 * -Wl,--wrap=vprintf
 * -Wl,--wrap=vsprintf
 * -Wl,--wrap=printf
 * -Wl,--wrap=sprintf
 * -Wl,--wrap=snprintf
 * -Wl,--wrap=puts
 * -Wl,--wrap=strlen
 * -Wl,--wrap=strcpy
 */

#include "mx_sdk.h"
#include "CONFIG.h"
#include "printf/printf.h"

/**
 * @brief   used to malloc a size of memory
 *
 * @param   size, flag can not be 0.
 *
 * @return  void * a pointer to the memory.
 */
extern void *tmos_memory_allocate(uint16_t size, uint16_t flag);


/**
 * @brief   used to free a size of memory
 *
 * @param   the pointer of memory.
 *
 * @return  void *.
 */
extern void tmos_memory_free(void *ptr);

__HIGH_CODE
void *__wrap_malloc(size_t size)
{
    return tmos_memory_allocate(size, 1);
}

__HIGH_CODE
void __wrap_free(void *ptr)
{
    tmos_memory_free(ptr);
}

__HIGH_CODE
void *__wrap_calloc(size_t nitems, size_t size)
{
    void *mem;
    mem = tmos_memory_allocate(nitems *size, 1);
    if (mem != NULL)
    {
        tmos_memset(mem, 0, nitems *size);
    }
    return mem;
}

__HIGH_CODE
void *__wrap_realloc(void *ptr, size_t size)
{
    struct _tmos_mem_info_header
    {
        uint16_t len;
        uint16_t used;
    } *mem_info;
    void *mem;
    size_t old_size, copy_size;

    mem = tmos_memory_allocate(size, 1);
    if (mem != NULL)
    {
        mem_info = (struct _tmos_mem_info_header *)(((uint32_t)ptr) - 4);
        old_size = mem_info->len;
        if (old_size > size)
        {
            copy_size = size;
        }
        else
        {
            copy_size = old_size;
        }
        tmos_memcpy(mem, ptr, copy_size);
        tmos_memory_free(ptr);
    }
    return mem;
}

__HIGH_CODE
void *__wrap_memset(void *src, int value, size_t size)
{
    tmos_memset(src, value, size);
    return src;
}

__HIGH_CODE
int __wrap_memcmp(const void *src, const void *dst, size_t size)
{
    int ret;
    ret = tmos_memcmp(src, dst, size);
    if (ret != FALSE)
    {
        ret = FALSE;
    }
    else
    {
        ret = TRUE;
    }
    return ret;
}

int __wrap_vprintf(const char *format, va_list arg)
{
    return vprintf_(format, arg);
}

int __wrap_vsnprintf(char *s, size_t n, const char *format, va_list arg)
{
    return vsnprintf_(s, n, format, arg);
}

int __wrap_vsprintf(char *s, const char *format, va_list arg)
{
    return vsprintf_(s, format, arg);
}

int __wrap_printf(const char *format, ...)
{
    va_list args;
    va_start(args, format);
    const int ret = vprintf_(format, args);
    va_end(args);
    return ret;
}

int __wrap_puts(const char *str)
{
    const int ret = printf_("%s\n", str);
    return ret;
}

int __wrap_sprintf(char *s, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    const int ret = vsprintf_(s, format, args);
    va_end(args);
    return ret;
}

int __wrap_snprintf(char *s, size_t n, const char *format, ...)
{
    va_list args;
    va_start(args, format);
    const int ret = vsnprintf_(s, n, format, args);
    va_end(args);
    return ret;
}

size_t __wrap_strlen(const char *s)
{
    return tmos_strlen((char *)s);
}

char *__wrap_strcpy(char *dest, const char *src)
{
    char *original_dest = dest;
    while ( *src != '\0')
    {
        *dest = *src;
        src++;
        dest++;
    }
    *dest = '\0';
    return original_dest;
}
