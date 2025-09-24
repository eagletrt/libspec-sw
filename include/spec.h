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
#include <stdint.h>

/*!
 * \brief           Enumeration with all possible return code of the library.
 */
typedef enum SpecReturnCode {
    SPEC_OK,                /*!< Everything is fine */
    SPEC_NULL_PTR,          /*!< Unexpected NULL pointer detected */
    SPEC_IDX_OUT_OF_BOUNDS, /*!< The given index exceeds its bounds */
    SPEC_WRONG_SIZE,        /*!< Data size mismatch */
    SPEC_NO_CONFIG,         /*!< No configuration stored */
    SPEC_IO_ERR             /*!< I/O related error */
} SpecReturnCode_t;

/*!
 * \brief           A structure representing a configuration parameter.
 *
 * \description     An helper structure used to give a default configuration
 *                  to the handler via \ref spec_api_init function. It also
 *                  describes the structure of the configuration.
 */
typedef struct SpecParameter {
    void *data;  /*!< Parameter's data */
    size_t size; /*!< The data size */
} SpecParameter_t;

/*!
 * \brief           A structure that encapsulate data, functions and all
 *                  required information to handle a configuration.
 *
 * \attention       This structure should not be used directly.
 */
typedef struct SpecHandler {
    SpecParameter_t *param_data;                                                 /*!< The configuration data */
    size_t param_count;                                                          /*!< The number of parameters */
    SpecReturnCode_t (*read_nvm)(size_t offset, void *data, size_t size);        /*!< Function to write on NVM */
    SpecReturnCode_t (*write_nvm)(size_t offset, const void *data, size_t size); /*!< Function to read from NVM */
} SpecHandler_t;

#endif /* SPEC_H */
