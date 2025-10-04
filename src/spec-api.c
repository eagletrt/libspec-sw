/*!
 * \file            spec-api.c
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

#include "spec-api.h"
#include "arena-allocator-api.h"

#include <stdbool.h>
#include <string.h>
#include <stdint.h>

/*!
 * \brief           Check if there is a configuration saved in memory by
 *                  reading a magic number, if there any.
 *
 * \param[out]      hspec: The configuration handler structure.
 * \return          true if present, false otherwise.
 */
static bool prv_spec_api_check_magic_number(const SpecHandler_t *hspec) {
    return false;
}

/*!
 * \brief           A dummy read function.
 */
static SpecReturnCode_t prv_spec_dummy_read(size_t offset, void *data, size_t size) {
    return SPEC_OK;
}

/*!
 * \brief           A dummy write function.
 */
static SpecReturnCode_t prv_spec_dummy_write(size_t offset, const void *data, size_t size) {
    return SPEC_OK;
}

SpecReturnCode_t spec_api_init(SpecHandler_t *hspec,
                               ArenaAllocatorHandler_t *harena,
                               const SpecParameter_t *param_data,
                               size_t param_count,
                               SpecReturnCode_t (*read_nvm)(size_t offset, void *data, size_t size),
                               SpecReturnCode_t (*write_nvm)(size_t offset, const void *data, size_t size)) {
    if (hspec == NULL || harena == NULL || param_data == NULL) {
        return SPEC_NULL_PTR;
    }

    hspec->param_count = param_count;
    hspec->read_nvm = read_nvm == NULL ? prv_spec_dummy_read : read_nvm;
    hspec->write_nvm = write_nvm == NULL ? prv_spec_dummy_write : write_nvm;
    hspec->param_data = arena_allocator_api_calloc(harena, sizeof(SpecParameter_t), param_count);
    if (hspec->param_data == NULL) {
        return SPEC_NULL_PTR;
    }

    size_t size = 0;
    for (size_t i = 0; i < param_count; ++i) {
        hspec->param_data[i].data = arena_allocator_api_alloc(harena, param_data[i].size);
        if (hspec->param_data[i].data == NULL) {
            return SPEC_NULL_PTR;
        }
        hspec->param_data[i].size = param_data[i].size;

        SpecReturnCode_t ret = spec_api_set(hspec, i, param_data[i].data, param_data[i].size);
    }

    return SPEC_OK;
}

SpecReturnCode_t spec_api_load(SpecHandler_t *hspec) {
    if (hspec == NULL) {
        return SPEC_NULL_PTR;
    }

    if (prv_spec_api_check_magic_number(hspec)) {
        return SPEC_NO_CONFIG;
    }

    return SPEC_OK;
}

SpecReturnCode_t spec_api_store(const SpecHandler_t *hspec) {
    if (hspec == NULL) {
        return SPEC_NULL_PTR;
    }

    return SPEC_OK;
}

SpecReturnCode_t spec_api_get(const SpecHandler_t *hspec, size_t idx, void *out, size_t size) {
    if (hspec == NULL || out == NULL) {
        return SPEC_NULL_PTR;
    }

    if (idx >= hspec->param_count) {
        return SPEC_IDX_OUT_OF_BOUNDS;
    }

    if (size != hspec->param_data[idx].size) {
        return SPEC_WRONG_SIZE;
    }

    memcpy(out, hspec->param_data[idx].data, size);

    return SPEC_OK;
}

SpecReturnCode_t spec_api_set(SpecHandler_t *hspec, size_t idx, const void *data, size_t size) {
    if (hspec == NULL || data == NULL) {
        return SPEC_NULL_PTR;
    }

    if (idx >= hspec->param_count) {
        return SPEC_IDX_OUT_OF_BOUNDS;
    }

    if (size != hspec->param_data[idx].size) {
        return SPEC_WRONG_SIZE;
    }

    memcpy(hspec->param_data[idx].data, data, size);

    return SPEC_OK;
}
