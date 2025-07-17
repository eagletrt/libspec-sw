/*!
 * \file            spec-api.h
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

#ifndef SPEC_API_H
#define SPEC_API_H

#include "spec.h"
#include "arena-allocator.h"

/*!
 * \brief           Initialize the configuration handler.
 *
 * \param[out]      hspec: The configuration handler structure.
 * \param[in]       harena: The arena allocator handler structure.
 * \param[in]       param: An array of SPEC parameters
 * \param[in]       read_nvm: The function used to read from NVM.
 * \param[in]       write_nvm: The function used to write into NVM.
 * \return          SPEC_OK on success, SPEC_NULL_PTR otherwise.
 */
SpecReturnCode_t spec_api_init(SpecHandler_t *hspec,
                               ArenaAllocatorHandler_t *harena,
                               const SpecParameter_t *param,
                               size_t param_num,
                               int (*read_nvm)(size_t offset, void *data, size_t size),
                               int (*write_nvm)(size_t offset, const void *data, size_t size));

/*!
 * \brief           Loads the configuration from NVM into RAM.
 *
 * \param[out]      hspec: The configuration handler structure.
 * \return          SPEC_OK on success, SPEC_NO_CONFIG if there is no
 *                  configuration stored, SPEC_NULL_PTR if `hspec` is NULL,
 *                  SPEC_IO_ERR otherwise.
 */
SpecReturnCode_t spec_api_load(SpecHandler_t *hspec);

/*!
 * \brief           Stores the configuration into the NVM.
 *
 * \param[in]       hspec: The configuration handler structure.
 * \return          SPEC_OK on success, SPEC_NULL_PTR if `hspec` is NULL,
 *                  SPEC_IO_ERR otherwise.
 */
SpecReturnCode_t spec_api_store(const SpecHandler_t *hspec);

/*!
 * \brief           Get a copy of the parameter at idx position.
 *
 * \param[in]       hspec: The configuration handler structure.
 * \param[in]       idx: The parameter position.
 * \param[out]      out: A pointer to the variable where the parameter is
 *                  copied into.
 * \param[in]       size: The output data size.
 * \return          SPEC_OK on success, SPEC_NULL_PTR if `hspec` or `out` is
 *                  NULL and if `memcpy` fails, SPEC_IDX_OUT_OF_BOUNDS if `idx`
 *                  is higher than the number of parameters, SPEC_WRONG_SIZE
 *                  if `size` bigger than parameter's size.
 */
SpecReturnCode_t spec_api_get(const SpecHandler_t *hspec, size_t idx, void *out, size_t size);

/*!
 * \brief           Get a copy of the parameter at idx position.
 *
 * \param[out]      hspec: The configuration handler structure.
 * \param[in]       idx: The parameter position.
 * \param[in]       data: A pointer to the variable containing the new value.
 * \param[in]       size: The input data size.
 *
 * \return          SPEC_OK on success, SPEC_NULL_PTR if `hspec` or `data` is
 *                  NULL and if `memcpy` fails, SPEC_IDX_OUT_OF_BOUNDS if `idx`
 *                  is higher than the number of parameters, SPEC_WRONG_SIZE
 *                  if `size` is smaller than parameter's size.
 */
SpecReturnCode_t spec_api_set(SpecHandler_t *hspec, size_t idx, const void *data, size_t size);

#endif /* SPEC_API_H */
