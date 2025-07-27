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

#include <stdbool.h>

#include "unity.h"
#include "arena-allocator.h"
#include "arena-allocator-api.h"
#include "spec.h"
#include "spec-api.h"

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

/*! CONSTANTS */
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
const SpecParameter_t DEFAULT_CONFIG[CONFIG_LEN] = {
    { .data = (void *)&DEFAULT_I8, .type = SPEC_I8 },
    { .data = (void *)&DEFAULT_U8, .type = SPEC_U8 },
    { .data = (void *)&DEFAULT_I16, .type = SPEC_I16 },
    { .data = (void *)&DEFAULT_U16, .type = SPEC_U16 },
    { .data = (void *)&DEFAULT_I32, .type = SPEC_I32 },
    { .data = (void *)&DEFAULT_U32, .type = SPEC_U32 },
    { .data = (void *)&DEFAULT_I64, .type = SPEC_I64 },
    { .data = (void *)&DEFAULT_U64, .type = SPEC_U64 },
    { .data = (void *)&DEFAULT_FLOAT, .type = SPEC_FLOAT },
    { .data = (void *)&DEFAULT_BOOL, .type = SPEC_BOOL }
};

/*! GLOBAL VAR. */
ArenaAllocatorHandler_t harena;
SpecHandler_t hspec;

void setUp(void) {
    arena_allocator_api_init(&harena);
    spec_api_init(&hspec, &harena, DEFAULT_CONFIG, CONFIG_LEN, NULL, NULL);
}

void tearDown(void) {
    arena_allocator_api_free(&harena);
}

/*!
 * \defgroup        spec_init Test SPEC initialization.
 * @{
 */

void check_spec_init_null_spec_handler(void) {
    TEST_ASSERT_EQUAL_INT(SPEC_NULL_PTR, spec_api_init(NULL, &harena, DEFAULT_CONFIG, CONFIG_LEN, NULL, NULL));
}

void check_spec_init_null_arena_handler(void) {
    TEST_ASSERT_EQUAL_INT(SPEC_NULL_PTR, spec_api_init(&hspec, NULL, DEFAULT_CONFIG, CONFIG_LEN, NULL, NULL));
}

void check_spec_init_null_parameters(void) {
    SpecHandler_t hspec_loc;
    TEST_ASSERT_EQUAL_INT(SPEC_NULL_PTR, spec_api_init(&hspec_loc, &harena, NULL, CONFIG_LEN, NULL, NULL));
}

/*! @} */

/*!
 * \defgroup        spec_get Test SPEC get function.
 * @{
 */

void check_spec_get_with_null_handler(void) {
    int8_t data;
    TEST_ASSERT_EQUAL_INT(SPEC_NULL_PTR, spec_api_get(NULL, CONFIG_I8, &data, SPEC_I8));
}

void check_spec_get_with_null_var(void) {
    TEST_ASSERT_EQUAL_INT(SPEC_NULL_PTR, spec_api_get(&hspec, CONFIG_I8, NULL, SPEC_I8));
}

void check_spec_get_with_wrong_type(void) {
    float data;
    TEST_ASSERT_EQUAL_INT(SPEC_WRONG_TYPE, spec_api_get(&hspec, CONFIG_I8, &data, SPEC_FLOAT));
}

void check_spec_get_with_wrong_index(void) {
    int8_t data;
    TEST_ASSERT_EQUAL_INT(SPEC_IDX_OUT_OF_BOUNDS, spec_api_get(&hspec, CONFIG_LEN, &data, SPEC_I8));
}

void check_spec_get_with_int8_parameter(void) {
    int8_t data;
    spec_api_get(&hspec, CONFIG_I8, &data, SPEC_I8);
    TEST_ASSERT_EQUAL_INT8(DEFAULT_I8, data);
}

void check_spec_get_with_uint8_parameter(void) {
    uint8_t data;
    spec_api_get(&hspec, CONFIG_U8, &data, SPEC_U8);
    TEST_ASSERT_EQUAL_INT8(DEFAULT_U8, data);
}

void check_spec_get_with_int16_parameter(void) {
    int16_t data;
    spec_api_get(&hspec, CONFIG_I16, &data, SPEC_I16);
    TEST_ASSERT_EQUAL_INT16(DEFAULT_I16, data);
}

void check_spec_get_with_uint16_parameter(void) {
    uint16_t data;
    spec_api_get(&hspec, CONFIG_U16, &data, SPEC_U16);
    TEST_ASSERT_EQUAL_INT16(DEFAULT_U16, data);
}

void check_spec_get_with_int32_parameter(void) {
    int32_t data;
    spec_api_get(&hspec, CONFIG_I32, &data, SPEC_I32);
    TEST_ASSERT_EQUAL_INT32(DEFAULT_I32, data);
}

void check_spec_get_with_uint32_parameter(void) {
    uint32_t data;
    spec_api_get(&hspec, CONFIG_U32, &data, SPEC_U32);
    TEST_ASSERT_EQUAL_INT32(DEFAULT_U32, data);
}

void check_spec_get_with_int64_parameter(void) {
    int64_t data;
    spec_api_get(&hspec, CONFIG_I64, &data, SPEC_I64);
    TEST_ASSERT_EQUAL_INT64(DEFAULT_I64, data);
}

void check_spec_get_with_uint64_parameter(void) {
    uint64_t data;
    spec_api_get(&hspec, CONFIG_U64, &data, SPEC_U64);
    TEST_ASSERT_EQUAL_INT64(DEFAULT_U64, data);
}

void check_spec_get_with_float_parameter(void) {
    float data;
    spec_api_get(&hspec, CONFIG_FLOAT, &data, SPEC_FLOAT);
    TEST_ASSERT_EQUAL_FLOAT(DEFAULT_FLOAT, data);
}

void check_spec_get_with_bool_parameter(void) {
    bool data;
    spec_api_get(&hspec, CONFIG_BOOL, &data, SPEC_BOOL);

    if (DEFAULT_BOOL) {
        TEST_ASSERT_TRUE(data);
    } else {
        TEST_ASSERT_FALSE(data);
    }
}

/*! @} */

/*!
 * \defgroup        spec_set Test SPEC set function.
 * @{
 */

void check_spec_set_with_null_handler(void) {
    int8_t data = 4;
    TEST_ASSERT_EQUAL_INT(SPEC_NULL_PTR, spec_api_set(NULL, CONFIG_I8, &data, SPEC_I8));
}

void check_spec_set_with_null_var(void) {
    TEST_ASSERT_EQUAL_INT(SPEC_NULL_PTR, spec_api_set(&hspec, CONFIG_I8, NULL, SPEC_I8));
}

void check_spec_set_with_wrong_type(void) {
    int8_t data = 0xFF;
    TEST_ASSERT_EQUAL_INT(SPEC_WRONG_TYPE, spec_api_set(&hspec, CONFIG_FLOAT, &data, SPEC_I8));
}

void check_spec_set_with_wrong_index(void) {
    int8_t data = 0xFF;
    TEST_ASSERT_EQUAL_INT(SPEC_IDX_OUT_OF_BOUNDS, spec_api_set(&hspec, CONFIG_LEN, &data, SPEC_I8));
}

void check_spec_set_with_int8_parameter(void) {
    int8_t data = 3;
    TEST_ASSERT_EQUAL_INT(SPEC_OK, spec_api_set(&hspec, CONFIG_I8, &data, SPEC_I8));
}

void check_spec_set_with_uint8_parameter(void) {
    uint8_t data = 3;
    TEST_ASSERT_EQUAL_INT(SPEC_OK, spec_api_set(&hspec, CONFIG_U8, &data, SPEC_U8));
}

void check_spec_set_with_int16_parameter(void) {
    int16_t data = 3;
    TEST_ASSERT_EQUAL_INT(SPEC_OK, spec_api_set(&hspec, CONFIG_I16, &data, SPEC_I16));
}

void check_spec_set_with_uint16_parameter(void) {
    uint16_t data = 3;
    TEST_ASSERT_EQUAL_INT(SPEC_OK, spec_api_set(&hspec, CONFIG_U16, &data, SPEC_U16));
}

void check_spec_set_with_int32_parameter(void) {
    int32_t data = 3;
    TEST_ASSERT_EQUAL_INT(SPEC_OK, spec_api_set(&hspec, CONFIG_I32, &data, SPEC_I32));
}

void check_spec_set_with_uint32_parameter(void) {
    uint32_t data = 3;
    TEST_ASSERT_EQUAL_INT(SPEC_OK, spec_api_set(&hspec, CONFIG_U32, &data, SPEC_U32));
}

void check_spec_set_with_int64_parameter(void) {
    int64_t data = 3;
    TEST_ASSERT_EQUAL_INT(SPEC_OK, spec_api_set(&hspec, CONFIG_I64, &data, SPEC_I64));
}

void check_spec_set_with_uint64_parameter(void) {
    uint64_t data = 3;
    TEST_ASSERT_EQUAL_INT(SPEC_OK, spec_api_set(&hspec, CONFIG_U64, &data, SPEC_U64));
}

void check_spec_set_with_float_parameter(void) {
    float data = 2.14;
    TEST_ASSERT_EQUAL_INT(SPEC_OK, spec_api_set(&hspec, CONFIG_FLOAT, &data, SPEC_FLOAT));
}

void check_spec_set_with_bool_parameter(void) {
    bool data = true;
    TEST_ASSERT_EQUAL_INT(SPEC_OK, spec_api_set(&hspec, CONFIG_BOOL, &data, SPEC_BOOL));
}

/*! @} */

// int main(void) {
//     arena_allocator_api_init(&harena);
//     spec_api_init(&hspec, &harena, DEFAULT_CONFIG, CONFIG_LEN, NULL, NULL);
//
//     // int8_t d1, res;
//     // res = spec_api_get(&hspec, CONFIG_I8, (void *)&d1, SPEC_I8);
//     // printf("res=%d\td1=%d\n", res, d1);
//     //
//     // uint8_t d2;
//     // res = spec_api_get(&hspec, CONFIG_U8, (void *)&d2, SPEC_U8);
//     // printf("res=%d\td2=%d\n", res, d2);
//
//     arena_allocator_api_free(&harena);
// }

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
     * \defgroup        spec_get Run test for SPEC get function.
     * @{
     */

    RUN_TEST(check_spec_get_with_null_handler);
    RUN_TEST(check_spec_get_with_null_var);
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
    RUN_TEST(check_spec_set_with_null_var);
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
