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

/*!
 * \brief           Check if the magic number is set or not.
 *
 * \param[out]      hspec: The configuration handler structure.
 * \return          true if set, false otherwise.
 */
static bool prv_spec_api_check_magic_number(const SpecHandler_t *hspec) {
    /* TODO: function body. */
    return false;
}

SpecReturnCode_t spec_api_init(SpecHandler_t *hspec,
                               ArenaAllocatorHandler_t *harena,
                               const SpecParameter_t *param,
                               size_t param_num,
                               int (*read_nvm)(size_t offset, void *data, size_t size),
                               int (*write_nvm)(size_t offset, const void *data, size_t size)) {
    if (hspec == NULL || harena == NULL) {
        return SPEC_NULL_PTR;
    }

    hspec->param_num = param_num;
    hspec->read_nvm = read_nvm;   /* TODO: handle NULL */
    hspec->write_nvm = write_nvm; /* TODO: handle NULL */
    hspec->param = (SpecParameter_t *)arena_allocator_api_calloc(
        harena, param_num, sizeof(SpecParameter_t));
    if (hspec->param == NULL) {
        return SPEC_NULL_PTR;
    }

    for (size_t i = 0; i < param_num; ++i) {
        if (memcpy(&hspec->param[i], &param[i], sizeof(param[i])) == NULL) {
            return SPEC_NULL_PTR;
        }
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

    /* TODO: function body. */

    return SPEC_OK;
}

SpecReturnCode_t spec_api_store(const SpecHandler_t *hspec) {
    if (hspec == NULL) {
        return SPEC_NULL_PTR;
    }

    /* TODO: function body. */

    return SPEC_OK;
}

SpecReturnCode_t spec_api_get(const SpecHandler_t *hspec, size_t idx, void *out, size_t size) {
    if (hspec == NULL || out == NULL) {
        return SPEC_NULL_PTR;
    }

    if (idx >= hspec->param_num) {
        return SPEC_IDX_OUT_OF_BOUNDS;
    }

    if (size < hspec->param[idx].size) {
        return SPEC_WRONG_SIZE;
    }

    if (memcpy(out, hspec->param[idx].data, hspec->param[idx].size) == NULL) {
        return SPEC_NULL_PTR;
    }

    return SPEC_OK;
}

SpecReturnCode_t spec_api_set(SpecHandler_t *hspec, size_t idx, const void *data, size_t size) {
    if (hspec == NULL || data == NULL) {
        return SPEC_NULL_PTR;
    }

    if (idx >= hspec->param_num) {
        return SPEC_IDX_OUT_OF_BOUNDS;
    }

    if (size > hspec->param[idx].size) {
        return SPEC_WRONG_SIZE;
    }

    if (memcpy(hspec->param[idx].data, data, size) == NULL) {
        return SPEC_NULL_PTR;
    }

    return SPEC_OK;
}
