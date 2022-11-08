/* lib.h - Defines for useful library functions
 * vim:ts=4 noexpandtab
 */

#ifndef _ETD_LIB_H
#define _ETD_LIB_H

#include "etd_types.h"

#define ETD_FOUR_MB (1 << 22)
#define ETD_FOUR_KB (1 << 12)
#define ETD_EIGHT_KB (FOUR_KB << 1)
#define ETD_EIGHT_MB (FOUR_MB << 1)

/* Userspace address-check functions */
etd_int32_t bad_userspace_addr(const void* addr, etd_int32_t len);
etd_int32_t safe_strncpy(etd_int8_t* dest, const etd_int8_t* src, etd_int32_t n);

/* TODO convenience? */
void die(etd_int8_t* s);
etd_uint32_t min(etd_uint32_t a, etd_uint32_t b);

void test_interrupts(void);

/* Port read functions */
/* Inb reads a byte and returns its value as a zero-extended 32-bit
 * unsigned int */
static inline etd_uint32_t inb(int port) {
    etd_uint32_t val;
    asm volatile ("             \n\
            xorl %0, %0         \n\
            inb  (%w1), %b0     \n\
            "
            : "=a"(val)
            : "d"(port)
            : "memory"
    );
    return val;
}

/* Reads two bytes from two consecutive ports, starting at "port",
 * concatenates them little-endian style, and returns them zero-extended
 * */
static inline etd_uint32_t inw(int port) {
    etd_uint32_t val;
    asm volatile ("             \n\
            xorl %0, %0         \n\
            inw  (%w1), %w0     \n\
            "
            : "=a"(val)
            : "d"(port)
            : "memory"
    );
    return val;
}

/* Reads four bytes from four consecutive ports, starting at "port",
 * concatenates them little-endian style, and returns them */
static inline etd_uint32_t inl(int port) {
    etd_uint32_t val;
    asm volatile ("inl (%w1), %0"
            : "=a"(val)
            : "d"(port)
            : "memory"
    );
    return val;
}

/* Writes a byte to a port */
#define outb(data, port)                \
do {                                    \
    asm volatile ("outb %b1, (%w0)"     \
            :                           \
            : "d"(port), "a"(data)      \
            : "memory", "cc"            \
    );                                  \
} while (0)

/* Writes two bytes to two consecutive ports */
#define outw(data, port)                \
do {                                    \
    asm volatile ("outw %w1, (%w0)"     \
            :                           \
            : "d"(port), "a"(data)      \
            : "memory", "cc"            \
    );                                  \
} while (0)

/* Writes four bytes to four consecutive ports */
#define outl(data, port)                \
do {                                    \
    asm volatile ("outl %1, (%w0)"     \
            :                           \
            : "d"(port), "a"(data)      \
            : "memory", "cc"            \
    );                                  \
} while (0)

/* Clear interrupt flag - disables interrupts on this processor */
#define cli()                           \
do {                                    \
    asm volatile ("cli"                 \
            :                           \
            :                           \
            : "memory", "cc"            \
    );                                  \
} while (0)

/* Save flags and then clear interrupt flag
 * Saves the EFLAGS register into the variable "flags", and then
 * disables interrupts on this processor */
#define cli_and_save(flags)             \
do {                                    \
    asm volatile ("                   \n\
            pushfl                    \n\
            popl %0                   \n\
            cli                       \n\
            "                           \
            : "=r"(flags)               \
            :                           \
            : "memory", "cc"            \
    );                                  \
} while (0)

/* Set interrupt flag - enable interrupts on this processor */
#define sti()                           \
do {                                    \
    asm volatile ("sti"                 \
            :                           \
            :                           \
            : "memory", "cc"            \
    );                                  \
} while (0)

/* Restore flags
 * Puts the value in "flags" into the EFLAGS register.  Most often used
 * after a cli_and_save_flags(flags) */
#define restore_flags(flags)            \
do {                                    \
    asm volatile ("                   \n\
            pushl %0                  \n\
            popfl                     \n\
            "                           \
            :                           \
            : "r"(flags)                \
            : "memory", "cc"            \
    );                                  \
} while (0)

#define save_esp(regs)					\
do {                                    \
    asm volatile ("                   \n\
            pushl %%esp				  \n\
            popl %0                   \n\
            "                           \
            : "=r"(regs)               \
            :                           \
            : "memory", "cc"            \
    );                                  \
} while (0)

#define restore_esp(regs)				\
do {                                    \
    asm volatile ("                   \n\
            pushl %0				  \n\
            popl %%esp                \n\
            "                           \
			:							\
            :"r"(regs)					\
            : "esp", "memory", "cc"            \
    );                                  \
} while (0)

#define save_ebp(regs)					\
do {                                    \
    asm volatile ("                   \n\
            pushl %%ebp				  \n\
            popl %0                   \n\
            "                           \
            : "=r"(regs)               \
            :                           \
            : "memory", "cc"            \
    );                                  \
} while (0)

#define restore_ebp(regs)				\
do {                                    \
    asm volatile ("                   \n\
            pushl %0				  \n\
            popl %%ebp                \n\
            "                           \
			:							\
            :"r"(regs)					\
            : "ebp", "memory", "cc"            \
    );                                  \
} while (0)

#endif /* _ETD_LIB_H */
