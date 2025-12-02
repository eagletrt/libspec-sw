 # Simple Persistent Embedded Configurator (S.P.E.C.)
 This library provides a simple way to load, store and change MCUs configuration parameters run-time.

## Dependencies
This library relies on the [ArenaAllocator](https://github.com/eagletrt/libarena-allocator-sw.git) for memory management. Make sure to 
initialize the allocator handler before the S.P.E.C. handler initialization.

## Usage
The first thing you must do is declare and initialize the arena allocator handler
as shown in the usage section [here](https://github.com/eagletrt/libring-buffer-sw/#usage). After that, declare the configuration handler
using `struct SpecHandler` and initialize it by using the `spec_api_init()` function.

```c
const uint32_t CFG_VERSION = 0xB16B00B5U;
const struct SpecParameter DEFAULT_CFG[3U] = {
	{ .data = (int8_t[]){0x1F}, .size = 1U },
	{ .data = (int8_t[]){0x2F}, .size = 1U },
	{ .data = (int8_t[]){0x3F}, .size = 1U },
};

ArenaAllocatorHandler_t arena;
struct SpecHandler cfg;

arena_allocator_api_init(&arena);
spec_api_init(&spec, &area, DEFAULT_CFG, 3U, NULL, NULL, CFG_VERSION);
```
We need to declare the `DEFAULT_CFG`  because it will be used if no configuration
is stored inside the MCU. `DEFAULT_CFG` is an array of  parameters; it contains 
addresses to some data and their sizes.

Setting or getting parameters is quite simple. Use `spec_api_get()` and
`spec_api_set()` as follows.

```c
/*! Getting the 1st parameter */
int8_t var = 0;
spec_api_get(&spec, 0, &var, sizeof(var));

/*! Setting the 1st parameter */
var = get_new_var_value();
spec_api_set(&spec, 0, &var, sizeof(var));
```

In order to load or store configurations, you need to give the appropriate functions
to read and write data to the desired NVM.

```c
/*!
 * 'read_nvm' and 'write_nvm' are implemented by the user.
 *   - enum SpecReturnCode read_nvm(size_t, void*, size_t) { }
 *   - enum SpecReturnCode write_nvm(size_t, const void*, size_t) { }
 **/
spec_api_init(&cfg, &arena, DEFAULT_CFG, 3U, read_nvm, write_nvm);
```
## Examples
For more examples check the [examples](./examples) folder.
