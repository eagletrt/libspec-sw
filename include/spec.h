/*!
 * \file            spec.h
 * \date            2025-07-10
 * \authors         Mirko Lana [mirko.lana@eagletrt.it]
 *
 * \brief           Library to dynamically configure MCUs parameters via
 *                  different types of peripherals.
 *
 * \details         This library loads a configuration into the RAM by reading
 *                  it from a NVM, such as FLASH. During program execution, the
 *                  library listens for commands from one or more connected
 *                  peripherals. Upon receiving a command, it can update the
 *                  value of the specific parameter. If necessary, the updated
 *                  configuration can be saved back to the NVM.
 *
 * \warning         The allocated memory will not be freed automatically but has
 *                  to be deallocated by using the arena allocator.
 */

#ifndef SPEC_H
#define SPEC_H

#include <stddef.h>

/*!
 * \brief           A structure representing a configuration parameter.
 */
typedef struct SpecParameter {
    void *data;  /*!< Parameter's data */
    size_t size; /*!< Data size */
} SpecParameter_t;

/*!
 * \brief           A structure that encapsulate data, functions and all
 *                  required information to handle a configuration.
 *
 * \attention       This structure should not be used directly.
 */
typedef struct SpecHandler {
    SpecParameter_t *param;                                         /*!< An array of parameters */
    size_t param_num;                                               /*!< The number of parameters */
    int (*read_nvm)(size_t offset, void *data, size_t size);        /*!< Function to write on NVM */
    int (*write_nvm)(size_t offset, const void *data, size_t size); /*!< Function to read from NVM */
} SpecHandler_t;

/*!
 * \brief           Enumeration with all possible return code of the library.
 */
typedef enum SpecReturnCode {
    SPEC_OK,
    SPEC_NULL_PTR,
    SPEC_IDX_OUT_OF_BOUNDS,
    SPEC_WRONG_SIZE,
    SPEC_NO_CONFIG,
    SPEC_IO_ERR
} SpecReturnCode_t;

#endif /* SPEC_H */
