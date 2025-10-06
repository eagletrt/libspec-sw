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
enum SpecReturnCode {
    SPEC_RC_OK,                /*!< Everything is fine */
    SPEC_RC_NULL_PTR,          /*!< Unexpected NULL pointer detected */
    SPEC_RC_IDX_OUT_OF_BOUNDS, /*!< The given index exceeds its bounds */
    SPEC_RC_WRONG_SIZE,        /*!< Data size mismatch */
    SPEC_RC_NO_CONFIG,         /*!< No configuration stored */
    SPEC_RC_IO_ERR             /*!< I/O related error */
};

/*!
 * \brief           Type definition for a function pointer for reading data from NVM.
 *
 * \param[in]       offset: Offset in the NVM from which to read data.
 * \param[out]      data: Pointer to the buffer where the data will be stored.
 * \param[in]       size: Size of the data to read.
 * \return          SPEC_RC_OK on success, SPEC_RC_NULL_PTR if data is NULL,
 *                  SEPC_RC_IO_ERR otherwise
 */
typedef enum SpecReturnCode (*spec_read_fn)(size_t offset, void *data, size_t size);

/*!
 * \brief           Type definition for a function pointer for writing data to NVM.
 *
 * \param[in]       offset: Offset in the NVM where data will be written.
 * \param[in]       data: Pointer to the data to be written to NVM.
 * \param[in]       size: Size of the data to write.
 * \return          SPEC_RC_OK on success, SPEC_RC_NULL_PTR if data is NULL,
 *                  SEPC_RC_IO_ERR otherwise
 */
typedef enum SpecReturnCode (*spec_write_fn)(size_t offset, const void *data, size_t size);

struct SpecParameter {
    void *data;  /*!< Parameter's data */
    size_t size; /*!< The data size */
};

/*!
 * \brief           A structure that encapsulate data, functions and all
 *                  required information to handle a configuration.
 *
 * \attention       This structure should not be used directly.
 */
struct SpecHandler {
    struct SpecParameter *param_data; /*!< The configuration data */
    size_t param_count;               /*!< The number of parameters */
    spec_read_fn read_nvm;            /*!< Function to write on NVM */
    spec_write_fn write_nvm;          /*!< Function to read from NVM */
};

#endif /* SPEC_H */
