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
    /* TODO: function body. */
    return false;
}

/*!
 * \brief           Get the size of the specified type.
 *
 * \param[in]       type: The type to get the size of.
 * \return          The size on success, 0 otherwise.
 */
static size_t prv_spec_api_sizeof(SpecType_t type) {
    switch (type) {
        case SPEC_I8:
        case SPEC_U8:
            return sizeof(int8_t);

        case SPEC_I16:
        case SPEC_U16:
            return sizeof(int16_t);

        case SPEC_I32:
        case SPEC_U32:
            return sizeof(int32_t);

        case SPEC_I64:
        case SPEC_U64:
            return sizeof(int64_t);

        case SPEC_FLOAT:
            return sizeof(float);

        case SPEC_BOOL:
            return sizeof(bool);

        default:
            return 0U;
    }
}

/*!
 * \brief           Get the alignment of the specified type.
 *
 * \param[in]       type: The type to get the alignment of.
 * \return          The alignment on success, 0 otherwise.
 */
static size_t prv_spec_api_alignof(SpecType_t type) {
    switch (type) {
        case SPEC_I8:
        case SPEC_U8:
            return _Alignof(int8_t);

        case SPEC_I16:
        case SPEC_U16:
            return _Alignof(int16_t);

        case SPEC_I32:
        case SPEC_U32:
            return _Alignof(int32_t);

        case SPEC_I64:
        case SPEC_U64:
            return _Alignof(int64_t);

        case SPEC_FLOAT:
            return _Alignof(float);

        case SPEC_BOOL:
            return _Alignof(bool);

        default:
            return 0U;
    }
}

/*!
 * \brief           Aligns the offset up to the nearest multiple of `alignment`.
 *
 * \param[in]       offset: The offset to be aligned.
 * \param[in]       alignment: The alignment.
 * \return          The offset aligned to `alignment`.
 *
 * \note            This is needed to ensure correct memory access based on
 *                  type alignment. Misaligned access can lead to fault and
 *                  unexpected behaviors.
 */
static size_t prv_spec_api_align_up(size_t offset, size_t alignment) {
    return (offset + alignment - 1) & ~(alignment - 1);
}

SpecReturnCode_t spec_api_init(SpecHandler_t *hspec,
                               ArenaAllocatorHandler_t *harena,
                               const SpecParameter_t *param,
                               size_t param_count,
                               SpecReturnCode_t (*read_nvm)(size_t offset, void *data, size_t size),
                               SpecReturnCode_t (*write_nvm)(size_t offset, const void *data, size_t size)) {
    size_t size, i;

    if (hspec == NULL || harena == NULL || param == NULL) {
        return SPEC_NULL_PTR;
    }

    hspec->param_count = param_count;
    hspec->read_nvm = read_nvm;   /* TODO: handle NULL */
    hspec->write_nvm = write_nvm; /* TODO: handle NULL */
    hspec->param_items = arena_allocator_api_calloc(harena, sizeof(SpecItem_t), param_count);
    if (hspec->param_items == NULL) {
        return SPEC_NULL_PTR;
    }

    size = 0;
    for (i = 0; i < param_count; ++i) {
        hspec->param_items[i].type = param[i].type;
        hspec->param_items[i].offset = size;
        size = prv_spec_api_align_up(size, prv_spec_api_alignof(param[i].type));
        size += prv_spec_api_sizeof(param[i].type);
    }

    hspec->param_data = arena_allocator_api_alloc(harena, size);
    if (hspec->param_data == NULL) {
        return SPEC_NULL_PTR;
    }

    for (i = 0; i < param_count; ++i) {
        int ret = spec_api_set(hspec, i, param[i].data, param[i].type);
        if (ret != SPEC_OK) {
            return ret;
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

SpecReturnCode_t spec_api_get(const SpecHandler_t *hspec, size_t idx, void *out, SpecType_t type) {
    if (hspec == NULL || out == NULL) {
        return SPEC_NULL_PTR;
    }

    if (idx >= hspec->param_count) {
        return SPEC_IDX_OUT_OF_BOUNDS;
    }

    if (type != hspec->param_items[idx].type) {
        return SPEC_WRONG_TYPE;
    }

    memcpy(out, hspec->param_data + hspec->param_items[idx].offset, prv_spec_api_sizeof(type));

    return SPEC_OK;
}

SpecReturnCode_t spec_api_set(SpecHandler_t *hspec, size_t idx, const void *data, SpecType_t type) {
    if (hspec == NULL || data == NULL) {
        return SPEC_NULL_PTR;
    }

    if (idx >= hspec->param_count) {
        return SPEC_IDX_OUT_OF_BOUNDS;
    }

    if (type != hspec->param_items[idx].type) {
        return SPEC_WRONG_TYPE;
    }

    memcpy(hspec->param_data + hspec->param_items[idx].offset, data, prv_spec_api_sizeof(type));

    return SPEC_OK;
}
