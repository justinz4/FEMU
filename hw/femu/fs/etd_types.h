/* etd_types.h - Defines to use the familiar explicitly-sized types in this
 * OS (uint32_t, int8_t, etc.).  This is necessary because we don't want
 * to include <stdint.h> when building this OS
 * vim:ts=4 noexpandtab
 */

#ifndef _ETD_TYPES_H
#define _ETD_TYPES_H

#ifndef ETD_ASM

/* Types defined here just like in <stdint.h> */
typedef int etd_int32_t;
typedef unsigned int etd_uint32_t;

typedef short etd_int16_t;
typedef unsigned short etd_uint16_t;

typedef char etd_int8_t;
typedef unsigned char etd_uint8_t;

#endif /* ETD_ASM */

#endif /* _ETD_TYPES_H */
