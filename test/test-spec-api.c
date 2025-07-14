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

#include "unity.h"
#include "arena-allocator.h"
#include "arena-allocator-api.h"
#include "spec.h"
#include "spec-api.h"

ArenaAllocatorHandler_t harena;
SpecHandler_t hspec;

void setUp(void) {
    /* TODO */
}

void tearDown(void) {
    /* TODO */
}

int main(void) {
    UNITY_BEGIN();

    UNIITY_END();
}
