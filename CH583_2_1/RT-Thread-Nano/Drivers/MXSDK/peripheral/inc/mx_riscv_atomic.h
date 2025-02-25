/*
 * Copyright (c) 2025, smartmx <smartmx@qq.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2025-02-21     smartmx      the first version
 *
 */

#ifndef _MX_RISCV_ATOMIC_H_
#define _MX_RISCV_ATOMIC_H_

#include "core_riscv.h"

/********************************************use riscv atomic *****************************************************/
/* 原子<加>操作，返回操作后的值 */
__attribute__((always_inline)) RV_STATIC_INLINE  int32_t __MX_AMOADD_W(volatile int32_t *addr, int32_t value)
{
    register int32_t result;

    __asm volatile ("amoadd.w %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/* 原子<加>操作，不返回操作后的值 */
__attribute__((always_inline)) RV_STATIC_INLINE  void __MX_VOID_AMOADD_W(volatile int32_t *addr, int32_t value)
{
    __asm volatile ("amoadd.w zero, %1, %0" : \
            "+A"(*addr) : "r"(value) : "memory");
}

/* 原子<与>操作，返回操作后的值 */
__attribute__((always_inline)) RV_STATIC_INLINE int32_t __MX_AMOAND_W(volatile int32_t *addr, int32_t value) {
    register int32_t result;

    __asm volatile ("amoand.w %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/* 原子<与>操作，不返回操作后的值 */
__attribute__((always_inline)) RV_STATIC_INLINE void __MX_VOID_AMOAND_W(volatile int32_t *addr, int32_t value) {
    __asm volatile ("amoand.w zero, %1, %0" : \
            "+A"(*addr) : "r"(value) : "memory");
}

/* 原子<或>操作，返回操作后的值 */
__attribute__((always_inline)) RV_STATIC_INLINE int32_t __MX_AMOOR_W(volatile int32_t *addr, int32_t value) {
    register int32_t result;
    __asm volatile ("amoor.w %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/* 原子<或>操作，不返回操作后的值 */
__attribute__((always_inline)) RV_STATIC_INLINE void __MX_VOID_AMOOR_W(volatile int32_t *addr, int32_t value) {
    __asm volatile ("amoor.w zero, %1, %0" : \
            "+A"(*addr) : "r"(value) : "memory");
}

/* 原子<异或>操作，返回操作后的值 */
__attribute__((always_inline)) RV_STATIC_INLINE uint32_t __MX_AMOXOR_W(volatile int32_t *addr,uint32_t value) {
    uint32_t result;
    __asm volatile ("amoxor.w %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/* 原子<异或>操作，不返回操作后的值 */
__attribute__((always_inline)) RV_STATIC_INLINE void __MX_VOID_AMOXOR_W(volatile int32_t *addr,uint32_t value) {
    __asm volatile ("amoxor.w zero, %1, %0" : \
            "+A"(*addr) : "r"(value) : "memory");
}

/* 原子<交换内存>操作，赋新值并返回原始值 */
__attribute__((always_inline)) RV_STATIC_INLINE uint32_t __MX_AMOSWAP_W(volatile uint32_t *addr, uint32_t newval)
{
    register uint32_t result;

    __asm volatile ("amoswap.w %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(newval) : "memory");
    return result;
}

/* 原子<交换内存>操作，赋新值,不返回原始值 */
__attribute__((always_inline)) RV_STATIC_INLINE void __MX_VOID_AMOSWAP_W(volatile uint32_t *addr, uint32_t newval)
{
    __asm volatile ("amoswap.w zero, %1, %0" : \
            "+A"(*addr) : "r"(newval) : "memory");
}

/* 原子<无符号比大小>操作，返回大的值  */
__attribute__((always_inline)) RV_STATIC_INLINE uint32_t __MX_AMOMAXU_W(volatile uint32_t *addr, uint32_t value)
{
    register uint32_t result;

    __asm volatile ("amomaxu.w %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/* 原子<有符号比大小>操作，返回大的值  */
__attribute__((always_inline)) RV_STATIC_INLINE int32_t __MX_AMOMAX_W(volatile int32_t *addr, int32_t value)
{
    register int32_t result;

    __asm volatile ("amomax.w %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/* 原子<无符号比大小>操作，返回小的值  */
__attribute__((always_inline)) RV_STATIC_INLINE uint32_t __MX_AMOMINU_W(volatile uint32_t *addr, uint32_t value)
{
    register uint32_t result;

    __asm volatile ("amominu.w %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

/* 原子<有符号比大小>操作，返回小的值  */
__attribute__((always_inline)) RV_STATIC_INLINE int32_t __MX_AMOMIN_W(volatile int32_t *addr, int32_t value)
{
    register int32_t result;

    __asm volatile ("amomin.w %0, %2, %1" : \
            "=r"(result), "+A"(*addr) : "r"(value) : "memory");
    return *addr;
}

#define MX_ATOMIC_SETBITS(ADDR, BITS)                   do{__MX_VOID_AMOOR_W(ADDR, BITS);}while(0)

#define MX_ATOMIC_RESETBITS(ADDR, BITS)                 do{__MX_VOID_AMOAND_W(ADDR, (~(BITS)));}while(0)

#endif /* _MX_RISCV_ATOMIC_H_ */
