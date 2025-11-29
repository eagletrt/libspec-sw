/*!
 * \file            test-spec-api.c
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

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "unity.h"
#include "arena-allocator.h"
#include "arena-allocator-api.h"
#include "spec.h"
#include "spec-api.h"

#define NVM_SIZE 0xFFFU
#define MAGIC_NUM 0xB16B00B5U

/*!
 * \brief           Enumeration with all configuration parameters.
 */
typedef enum ConfigParameters {
    CONFIG_I8,
    CONFIG_U8,
    CONFIG_I16,
    CONFIG_U16,
    CONFIG_I32,
    CONFIG_U32,
    CONFIG_I64,
    CONFIG_U64,
    CONFIG_FLOAT,
    CONFIG_BOOL,
    CONFIG_LEN
} ConfigParameters_t;

const int8_t DEFAULT_I8 = 0xD6;
const int8_t DEFAULT_U8 = 0xC4;
const int16_t DEFAULT_I16 = 0xCFC7;
const int16_t DEFAULT_U16 = 0x5FC7;
const int32_t DEFAULT_I32 = 0x85D3B2CD;
const int32_t DEFAULT_U32 = 0xA5DBB3C0;
const int64_t DEFAULT_I64 = 0xC1C1C1C1C1C1C1C1;
const int64_t DEFAULT_U64 = 0xFFFFFFFFFFFFFFFF;
const float DEFAULT_FLOAT = 3.14;
const bool DEFAULT_BOOL = false;
const struct SpecParameter DEFAULT_CONFIG[CONFIG_LEN] = {
    { .data = (void *)&DEFAULT_I8, .size = sizeof(DEFAULT_I8) },
    { .data = (void *)&DEFAULT_U8, .size = sizeof(DEFAULT_U8) },
    { .data = (void *)&DEFAULT_I16, .size = sizeof(DEFAULT_I16) },
    { .data = (void *)&DEFAULT_U16, .size = sizeof(DEFAULT_U16) },
    { .data = (void *)&DEFAULT_I32, .size = sizeof(DEFAULT_I32) },
    { .data = (void *)&DEFAULT_U32, .size = sizeof(DEFAULT_U32) },
    { .data = (void *)&DEFAULT_I64, .size = sizeof(DEFAULT_I64) },
    { .data = (void *)&DEFAULT_U64, .size = sizeof(DEFAULT_U64) },
    { .data = (void *)&DEFAULT_FLOAT, .size = sizeof(DEFAULT_FLOAT) },
    { .data = (void *)&DEFAULT_BOOL, .size = sizeof(DEFAULT_BOOL) }
};

ArenaAllocatorHandler_t harena;
struct SpecHandler hspec;
uint8_t fake_nvm[NVM_SIZE] = { 0 };
uint8_t fake_nvm_copy[NVM_SIZE] = { 0 };

void init_fake_nvm(void) {
    uint32_t magic_num = MAGIC_NUM;
    memcpy(fake_nvm, &magic_num, sizeof(magic_num));
    size_t offset = sizeof(magic_num);
    for (size_t i = 0; i < CONFIG_LEN; i++) {
        memcpy(&fake_nvm[offset], DEFAULT_CONFIG[i].data, DEFAULT_CONFIG[i].size);
        offset += DEFAULT_CONFIG[i].size;
    }
}

enum SpecReturnCode read_nvm_array(size_t offset, void *data, size_t size) {
    if (data == NULL)
        return SPEC_RC_NULL_PTR;

    if (offset + size > NVM_SIZE)
        return SPEC_RC_IO_ERR;

    memcpy(data, &fake_nvm[offset], size);
    return SPEC_RC_OK;
}

enum SpecReturnCode read_empty_nvm_array(size_t offset, void *data, size_t size) {
    if (data == NULL)
        return SPEC_RC_NULL_PTR;

    if (offset + size > NVM_SIZE)
        return SPEC_RC_IO_ERR;

    memset(data, 0xFF, size);
    return SPEC_RC_OK;
}

enum SpecReturnCode write_nvm_array(size_t offset, const void *data, size_t size) {
    if (data == NULL)
        return SPEC_RC_NULL_PTR;

    if (offset + size > NVM_SIZE)
        return SPEC_RC_IO_ERR;

    memcpy(&fake_nvm_copy[offset], data, size);
    return SPEC_RC_OK;
}

void setUp(void) {
    init_fake_nvm();
    memset(fake_nvm_copy, 0U, NVM_SIZE);
    arena_allocator_api_init(&harena);
    spec_api_init(&hspec, &harena, DEFAULT_CONFIG, CONFIG_LEN, read_nvm_array, write_nvm_array, MAGIC_NUM);
}

void tearDown(void) {
    arena_allocator_api_free(&harena);
}

/*!
 * \defgroup        spec_init Test SPEC initialization.
 * @{
 */

void check_spec_init_null_spec_handler(void) {
    TEST_ASSERT_EQUAL_INT(SPEC_RC_NULL_PTR, spec_api_init(NULL, &harena, DEFAULT_CONFIG, CONFIG_LEN, NULL, NULL, MAGIC_NUM));
}

void check_spec_init_null_arena_handler(void) {
    TEST_ASSERT_EQUAL_INT(SPEC_RC_NULL_PTR, spec_api_init(&hspec, NULL, DEFAULT_CONFIG, CONFIG_LEN, NULL, NULL, MAGIC_NUM));
}

void check_spec_init_null_parameters(void) {
    struct SpecHandler hspec_loc;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_NULL_PTR, spec_api_init(&hspec_loc, &harena, NULL, CONFIG_LEN, NULL, NULL, MAGIC_NUM));
}

/*! @} */

/*!
 * \defgroup        spec_load Test SPEC get function.
 * @{
 */

void check_spec_load_with_null_handler(void) {
    TEST_ASSERT_EQUAL_INT(SPEC_RC_NULL_PTR, spec_api_load(NULL));
}

void check_spec_load_with_empty_nvm(void) {
    hspec.read_nvm = read_empty_nvm_array;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_NO_CONFIG, spec_api_load(&hspec));
}

void check_spec_load_with_right_nvm(void) {
    TEST_ASSERT_EQUAL_INT_MESSAGE(SPEC_RC_OK, spec_api_load(&hspec), "SPEC initialization failed!");

    for (int i = 0; i < CONFIG_LEN; ++i)
        TEST_ASSERT_EQUAL_MEMORY_MESSAGE(hspec.param_data[i].data, DEFAULT_CONFIG[i].data, hspec.param_data[i].size, "One configuration parameter is wrong!");
}

void check_spec_load_with_different_versions(void) {
    hspec.magic_num = 0x128E57C;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_NO_CONFIG, spec_api_load(&hspec));
}

/*! @} */

/*!
 * \defgroup        spec_store Test SPEC get function.
 * @{
 */

void check_spec_store_with_null_handler(void) {
    TEST_ASSERT_EQUAL_INT(SPEC_RC_NULL_PTR, spec_api_store(NULL));
}

void check_spec_store_with_right_nvm(void) {
    TEST_ASSERT_EQUAL_INT_MESSAGE(SPEC_RC_OK, spec_api_store(&hspec), "spec_api_store failed!");
    TEST_ASSERT_EQUAL_UINT8_ARRAY_MESSAGE(fake_nvm, fake_nvm_copy, NVM_SIZE, "Stored configuration is different!");
}

/*! @} */

/*!
 * \defgroup        spec_get Test SPEC get function.
 * @{
 */

void check_spec_get_with_null_handler(void) {
    int8_t data;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_NULL_PTR, spec_api_get(NULL, CONFIG_I8, &data, sizeof(DEFAULT_I8)));
}

void check_spec_get_with_null_variable(void) {
    TEST_ASSERT_EQUAL_INT(SPEC_RC_NULL_PTR, spec_api_get(&hspec, CONFIG_I8, NULL, sizeof(DEFAULT_I8)));
}

void check_spec_get_with_wrong_type(void) {
    float data;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_WRONG_SIZE, spec_api_get(&hspec, CONFIG_I8, &data, sizeof(DEFAULT_FLOAT)));
}

void check_spec_get_with_wrong_index(void) {
    int8_t data;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_IDX_OUT_OF_BOUNDS, spec_api_get(&hspec, CONFIG_LEN, &data, sizeof(DEFAULT_I8)));
}

void check_spec_get_with_int8_parameter(void) {
    int8_t data;
    TEST_ASSERT_EQUAL_INT_MESSAGE(SPEC_RC_OK, spec_api_get(&hspec, CONFIG_I8, &data, sizeof(DEFAULT_I8)), "spec_api_get failed!");
    TEST_ASSERT_EQUAL_INT8_MESSAGE(DEFAULT_I8, data, "Wrong data value!");
}

void check_spec_get_with_uint8_parameter(void) {
    uint8_t data;
    TEST_ASSERT_EQUAL_INT_MESSAGE(SPEC_RC_OK, spec_api_get(&hspec, CONFIG_U8, &data, sizeof(DEFAULT_U8)), "spec_api_get failed!");
    TEST_ASSERT_EQUAL_UINT8_MESSAGE(DEFAULT_U8, data, "Wrong data value!");
}

void check_spec_get_with_int16_parameter(void) {
    int16_t data;
    TEST_ASSERT_EQUAL_INT_MESSAGE(SPEC_RC_OK, spec_api_get(&hspec, CONFIG_I16, &data, sizeof(DEFAULT_I16)), "spec_api_get failed!");
    TEST_ASSERT_EQUAL_INT16_MESSAGE(DEFAULT_I16, data, "Wrong data value!");
}

void check_spec_get_with_uint16_parameter(void) {
    uint16_t data;
    TEST_ASSERT_EQUAL_INT_MESSAGE(SPEC_RC_OK, spec_api_get(&hspec, CONFIG_U16, &data, sizeof(DEFAULT_U16)), "spec_api_get failed!");
    TEST_ASSERT_EQUAL_UINT16_MESSAGE(DEFAULT_U16, data, "Wrong data value!");
}

void check_spec_get_with_int32_parameter(void) {
    int32_t data;
    TEST_ASSERT_EQUAL_INT_MESSAGE(SPEC_RC_OK, spec_api_get(&hspec, CONFIG_I32, &data, sizeof(DEFAULT_I32)), "spec_api_get failed!");
    TEST_ASSERT_EQUAL_INT32_MESSAGE(DEFAULT_I32, data, "Wrong data value!");
}

void check_spec_get_with_uint32_parameter(void) {
    uint32_t data;
    TEST_ASSERT_EQUAL_INT_MESSAGE(SPEC_RC_OK, spec_api_get(&hspec, CONFIG_U32, &data, sizeof(DEFAULT_U32)), "spec_api_get failed!");
    TEST_ASSERT_EQUAL_UINT32_MESSAGE(DEFAULT_U32, data, "Wrong data value!");
}

void check_spec_get_with_int64_parameter(void) {
    int64_t data;
    TEST_ASSERT_EQUAL_INT_MESSAGE(SPEC_RC_OK, spec_api_get(&hspec, CONFIG_I64, &data, sizeof(DEFAULT_I64)), "spec_api_get failed!");
    TEST_ASSERT_EQUAL_INT64_MESSAGE(DEFAULT_I64, data, "Wrong data value!");
}

void check_spec_get_with_uint64_parameter(void) {
    uint64_t data;
    TEST_ASSERT_EQUAL_INT_MESSAGE(SPEC_RC_OK, spec_api_get(&hspec, CONFIG_U64, &data, sizeof(DEFAULT_U64)), "spec_api_get failed!");
    TEST_ASSERT_EQUAL_UINT64_MESSAGE(DEFAULT_U64, data, "Wrong data value!");
}

void check_spec_get_with_float_parameter(void) {
    float data;
    TEST_ASSERT_EQUAL_INT_MESSAGE(SPEC_RC_OK, spec_api_get(&hspec, CONFIG_FLOAT, &data, sizeof(DEFAULT_FLOAT)), "spec_api_get failed!");
    TEST_ASSERT_EQUAL_FLOAT_MESSAGE(DEFAULT_FLOAT, data, "Wrong data value!");
}

void check_spec_get_with_bool_parameter(void) {
    bool data;
    TEST_ASSERT_EQUAL_INT_MESSAGE(SPEC_RC_OK, spec_api_get(&hspec, CONFIG_BOOL, &data, sizeof(DEFAULT_BOOL)), "spec_api_get failed!");

    if (DEFAULT_BOOL) {
        TEST_ASSERT_TRUE_MESSAGE(data, "Wrong data value!");
    } else {
        TEST_ASSERT_FALSE_MESSAGE(data, "Wrong data value!");
    }
}

/*! @} */

/*!
 * \defgroup        spec_set Test SPEC set function.
 * @{
 */

void check_spec_set_with_null_handler(void) {
    int8_t data = 4;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_NULL_PTR, spec_api_set(NULL, CONFIG_I8, &data, sizeof(DEFAULT_I8)));
}

void check_spec_set_with_null_variable(void) {
    TEST_ASSERT_EQUAL_INT(SPEC_RC_NULL_PTR, spec_api_set(&hspec, CONFIG_I8, NULL, sizeof(DEFAULT_I8)));
}

void check_spec_set_with_wrong_type(void) {
    int8_t data = 0xFF;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_WRONG_SIZE, spec_api_set(&hspec, CONFIG_FLOAT, &data, sizeof(DEFAULT_I8)));
}

void check_spec_set_with_wrong_index(void) {
    int8_t data = 0xFF;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_IDX_OUT_OF_BOUNDS, spec_api_set(&hspec, CONFIG_LEN, &data, sizeof(DEFAULT_I8)));
}

void check_spec_set_with_int8_parameter(void) {
    int8_t data = 3;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_OK, spec_api_set(&hspec, CONFIG_I8, &data, sizeof(DEFAULT_I8)));
}

void check_spec_set_with_uint8_parameter(void) {
    uint8_t data = 3;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_OK, spec_api_set(&hspec, CONFIG_U8, &data, sizeof(DEFAULT_U8)));
}

void check_spec_set_with_int16_parameter(void) {
    int16_t data = 3;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_OK, spec_api_set(&hspec, CONFIG_I16, &data, sizeof(DEFAULT_I16)));
}

void check_spec_set_with_uint16_parameter(void) {
    uint16_t data = 3;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_OK, spec_api_set(&hspec, CONFIG_U16, &data, sizeof(DEFAULT_U16)));
}

void check_spec_set_with_int32_parameter(void) {
    int32_t data = 3;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_OK, spec_api_set(&hspec, CONFIG_I32, &data, sizeof(DEFAULT_I32)));
}

void check_spec_set_with_uint32_parameter(void) {
    uint32_t data = 3;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_OK, spec_api_set(&hspec, CONFIG_U32, &data, sizeof(DEFAULT_U32)));
}

void check_spec_set_with_int64_parameter(void) {
    int64_t data = 3;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_OK, spec_api_set(&hspec, CONFIG_I64, &data, sizeof(DEFAULT_I64)));
}

void check_spec_set_with_uint64_parameter(void) {
    uint64_t data = 3;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_OK, spec_api_set(&hspec, CONFIG_U64, &data, sizeof(DEFAULT_U64)));
}

void check_spec_set_with_float_parameter(void) {
    float data = 2.14;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_OK, spec_api_set(&hspec, CONFIG_FLOAT, &data, sizeof(DEFAULT_FLOAT)));
}

void check_spec_set_with_bool_parameter(void) {
    bool data = true;
    TEST_ASSERT_EQUAL_INT(SPEC_RC_OK, spec_api_set(&hspec, CONFIG_BOOL, &data, sizeof(DEFAULT_BOOL)));
}

/*! @} */

int main(void) {
    UNITY_BEGIN();

    /*!
     * \defgroup        spec_init Run test for SPEC initialization.
     * @{
     */

    RUN_TEST(check_spec_init_null_spec_handler);
    RUN_TEST(check_spec_init_null_arena_handler);
    RUN_TEST(check_spec_init_null_parameters);

    /*! @} */

    /*!
     * \defgroup        spec_load Test SPEC get function.
     * @{
     */

    RUN_TEST(check_spec_load_with_null_handler);
    RUN_TEST(check_spec_load_with_empty_nvm);
    RUN_TEST(check_spec_load_with_different_versions);
    RUN_TEST(check_spec_load_with_right_nvm);

    /*! @} */

    /*!
     * \defgroup        spec_store Test SPEC get function.
     * @{
     */

    RUN_TEST(check_spec_store_with_null_handler);
    RUN_TEST(check_spec_store_with_right_nvm);

    /*! @} */

    /*!
     * \defgroup        spec_get Run test for SPEC get function.
     * @{
     */

    RUN_TEST(check_spec_get_with_null_handler);
    RUN_TEST(check_spec_get_with_null_variable);
    RUN_TEST(check_spec_get_with_wrong_type);
    RUN_TEST(check_spec_get_with_wrong_index);
    RUN_TEST(check_spec_get_with_int8_parameter);
    RUN_TEST(check_spec_get_with_uint8_parameter);
    RUN_TEST(check_spec_get_with_int16_parameter);
    RUN_TEST(check_spec_get_with_uint16_parameter);
    RUN_TEST(check_spec_get_with_int32_parameter);
    RUN_TEST(check_spec_get_with_uint32_parameter);
    RUN_TEST(check_spec_get_with_int64_parameter);
    RUN_TEST(check_spec_get_with_uint64_parameter);
    RUN_TEST(check_spec_get_with_float_parameter);
    RUN_TEST(check_spec_get_with_bool_parameter);

    /*! @} */

    /*!
     * \defgroup        spec_set Test SPEC set function.
     * @{
     */

    RUN_TEST(check_spec_set_with_null_handler);
    RUN_TEST(check_spec_set_with_null_variable);
    RUN_TEST(check_spec_set_with_wrong_type);
    RUN_TEST(check_spec_set_with_wrong_index);
    RUN_TEST(check_spec_set_with_int8_parameter);
    RUN_TEST(check_spec_set_with_uint8_parameter);
    RUN_TEST(check_spec_set_with_int16_parameter);
    RUN_TEST(check_spec_set_with_uint16_parameter);
    RUN_TEST(check_spec_set_with_int32_parameter);
    RUN_TEST(check_spec_set_with_uint32_parameter);
    RUN_TEST(check_spec_set_with_int64_parameter);
    RUN_TEST(check_spec_set_with_uint64_parameter);
    RUN_TEST(check_spec_set_with_float_parameter);
    RUN_TEST(check_spec_set_with_bool_parameter);

    /*! @} */

    UNITY_END();
}
