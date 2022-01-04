#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

/** @defgroup utils utils
 * @{
 *
 * Functions for handling utils
 */

#ifndef BIT
#define BIT(n) (1 << (n))
#endif

/**
 * @brief Returns the LSB of a 2 byte integer
 *
 * @param val input 2 byte integer
 * @param lsb address of memory location to be updated with val's LSB
 * @return Return 0 upon success and non-zero otherwise
 */
int(util_get_LSB)(uint16_t val, uint8_t *lsb);

/**
 * @brief Returns the MSB of a 2 byte integer
 *
 * @param val input 2 byte integer
 * @param msb address of memory location to be updated with val's LSB
 * @return Return 0 upon success and non-zero otherwise
 */
int(util_get_MSB)(uint16_t val, uint8_t *msb);

/**
 * @brief Invokes sys_inb() system call but reads the value into a uint8_t variable.
 *
 * @param port the input port that is to be read
 * @param value address of 8-bit variable to be update with the value read
 * @return Return 0 upon success and non-zero otherwise
 */
int(util_sys_inb)(int port, uint8_t *value);

/**
 * @brief Convert a 9-bit 2’s complement counter to 16-bit.
 * @param value 9-bit 2's complement counter.
 * @param sign_bit Sign of the most signficant bit.
 * @return Return 16-bit 2's complement counter.
 */
int16_t cpl2_9_to_16(uint8_t value, bool sign_bit);

#endif //UTILS_H
