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
 * \brief           Configuration magic number.
 */
static const uint32_t SPEC_RC_MAGIC_NUM = 0xB16B00B5U;

/*!
 * \brief           A dummy read function.
 */
static enum SpecReturnCode prv_spec_api_dummy_read(size_t offset, void *data, size_t size) {
    return SPEC_RC_OK;
}

/*!
 * \brief           A dummy write function.
 */
static enum SpecReturnCode prv_spec_api_dummy_write(size_t offset, const void *data, size_t size) {
    return SPEC_RC_OK;
}

/*!
 * \brief           Check if there is a configuration saved in memory by
 *                  reading a magic number, if there any.
 *
 * \param[out]      hspec: The configuration handler structure.
 * \return          true if present, false otherwise.
 */
static bool prv_spec_api_check_magic_number(const struct SpecHandler *hspec) {
    uint32_t magic_num = 0U;
    enum SpecReturnCode ret = hspec->read_nvm(0U, &magic_num, sizeof(magic_num));
    if (ret != SPEC_RC_OK) {
        return false;
    }

    return magic_num == SPEC_RC_MAGIC_NUM;
}

enum SpecReturnCode spec_api_init(struct SpecHandler *hspec,
                                  ArenaAllocatorHandler_t *harena,
                                  const struct SpecParameter *param_data,
                                  size_t param_count,
                                  spec_read_fn read_nvm,
                                  spec_write_fn write_nvm) {
    if (hspec == NULL || harena == NULL || param_data == NULL) {
        return SPEC_RC_NULL_PTR;
    }

    hspec->param_count = param_count;
    hspec->read_nvm = read_nvm == NULL ? prv_spec_api_dummy_read : read_nvm;
    hspec->write_nvm = write_nvm == NULL ? prv_spec_api_dummy_write : write_nvm;
    hspec->param_data = arena_allocator_api_calloc(harena, sizeof(struct SpecParameter), param_count);
    if (hspec->param_data == NULL) {
        return SPEC_RC_NULL_PTR;
    }

    for (size_t i = 0U; i < param_count; ++i) {
        hspec->param_data[i].data = arena_allocator_api_alloc(harena, param_data[i].size);
        if (hspec->param_data[i].data == NULL) {
            return SPEC_RC_NULL_PTR;
        }
        hspec->param_data[i].size = param_data[i].size;

        enum SpecReturnCode ret = spec_api_set(hspec, i, param_data[i].data, param_data[i].size);
        if (ret != SPEC_RC_OK) {
            return ret;
        }
    }

    return SPEC_RC_OK;
}

enum SpecReturnCode spec_api_load(struct SpecHandler *hspec) {
    if (hspec == NULL) {
        return SPEC_RC_NULL_PTR;
    }

    if (!prv_spec_api_check_magic_number(hspec)) {
        return SPEC_RC_NO_CONFIG;
    }

    size_t offset = sizeof(SPEC_RC_MAGIC_NUM);
    for (size_t i = 0U; i < hspec->param_count; ++i) {
        enum SpecReturnCode ret = hspec->read_nvm(offset, hspec->param_data[i].data, hspec->param_data[i].size);
        if (ret != SPEC_RC_OK) {
            return ret;
        }

        offset += hspec->param_data[i].size;
    }

    return SPEC_RC_OK;
}

enum SpecReturnCode spec_api_store(const struct SpecHandler *hspec) {
    if (hspec == NULL) {
        return SPEC_RC_NULL_PTR;
    }

    if (!prv_spec_api_check_magic_number(hspec)) {
        enum SpecReturnCode ret = hspec->write_nvm(0U, (void *)&SPEC_RC_MAGIC_NUM, sizeof(SPEC_RC_MAGIC_NUM));
        if (ret != SPEC_RC_OK) {
            return ret;
        }
    }

    size_t offset = sizeof(SPEC_RC_MAGIC_NUM);
    for (size_t i = 0U; i < hspec->param_count; ++i) {
        enum SpecReturnCode ret = hspec->write_nvm(offset, hspec->param_data[i].data, hspec->param_data[i].size);
        if (ret != SPEC_RC_OK) {
            return ret;
        }

        offset += hspec->param_data[i].size;
    }

    return SPEC_RC_OK;
}

enum SpecReturnCode spec_api_get(const struct SpecHandler *hspec, size_t idx, void *out, size_t size) {
    if (hspec == NULL || out == NULL) {
        return SPEC_RC_NULL_PTR;
    }

    if (idx >= hspec->param_count) {
        return SPEC_RC_IDX_OUT_OF_BOUNDS;
    }

    if (size != hspec->param_data[idx].size) {
        return SPEC_RC_WRONG_SIZE;
    }

    memcpy(out, hspec->param_data[idx].data, size);

    return SPEC_RC_OK;
}

enum SpecReturnCode spec_api_set(struct SpecHandler *hspec, size_t idx, const void *data, size_t size) {
    if (hspec == NULL || data == NULL) {
        return SPEC_RC_NULL_PTR;
    }

    if (idx >= hspec->param_count) {
        return SPEC_RC_IDX_OUT_OF_BOUNDS;
    }

    if (size != hspec->param_data[idx].size) {
        return SPEC_RC_WRONG_SIZE;
    }

    memcpy(hspec->param_data[idx].data, data, size);

    return SPEC_RC_OK;
}
