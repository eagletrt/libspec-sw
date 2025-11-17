# API Reference

## Header files

- [include/spec-api.h](#file-includespec-apih)
- [include/spec.h](#file-includespech)

## File include/spec-api.h

_Library to dynamically configure MCUs parameters via different types of peripherals._

**Date:**

2025-07-10 



**Authors:**

Mirko Lana [[mirko.lana@eagletrt.it](mailto:mirko.lana@eagletrt.it)]



This library loads a configuration into the RAM by reading it from a NVM, such as FLASH. During program execution, the library listens for commands from one or more connected peripherals. Upon receiving a command, it can update the value of the specific parameter. If necessary, the updated configuration can be saved back to the NVM.



**Warning:**

The allocated memory will not be freed automatically but has to be deallocated by using the arena allocator.


## Functions

| Type | Name |
| ---: | :--- |
|  enum [**SpecReturnCode**](#enum-specreturncode) | [**spec\_api\_get**](#function-spec_api_get) (const struct [**SpecHandler**](#struct-spechandler) \*hspec, size\_t idx, void \*out, size\_t size) <br>_Get a copy of the parameter at idx position._ |
|  enum [**SpecReturnCode**](#enum-specreturncode) | [**spec\_api\_init**](#function-spec_api_init) (struct [**SpecHandler**](#struct-spechandler) \*hspec, ArenaAllocatorHandler\_t \*harena, const struct [**SpecParameter**](#struct-specparameter) \*param\_data, size\_t param\_count, [**spec\_read\_fn**](#typedef-spec_read_fn) read\_nvm, [**spec\_write\_fn**](#typedef-spec_write_fn) write\_nvm) <br>_Initialize the configuration handler._ |
|  enum [**SpecReturnCode**](#enum-specreturncode) | [**spec\_api\_load**](#function-spec_api_load) (struct [**SpecHandler**](#struct-spechandler) \*hspec) <br>_Loads the configuration from NVM into RAM._ |
|  enum [**SpecReturnCode**](#enum-specreturncode) | [**spec\_api\_set**](#function-spec_api_set) (struct [**SpecHandler**](#struct-spechandler) \*hspec, size\_t idx, const void \*data, size\_t size) <br>_Set the value of the parameter at idx position._ |
|  enum [**SpecReturnCode**](#enum-specreturncode) | [**spec\_api\_store**](#function-spec_api_store) (const struct [**SpecHandler**](#struct-spechandler) \*hspec) <br>_Stores the configuration into the NVM._ |



## Functions Documentation

### function `spec_api_get`

_Get a copy of the parameter at idx position._
```c
enum SpecReturnCode spec_api_get (
    const struct SpecHandler *hspec,
    size_t idx,
    void *out,
    size_t size
) 
```


**Parameters:**


* `hspec` The configuration handler structure. 
* `idx` The parameter position. 
* `out` A pointer to the variable where the parameter is copied into. 
* `size` The output data size. 


**Returns:**

SPEC\_RC\_OK on success, SPEC\_RC\_NULL\_PTR if `hspec` or`out` is NULL and if`memcpy` fails, SPEC\_RC\_IDX\_OUT\_OF\_BOUNDS if`idx` is higher than the number of parameters, SPEC\_RC\_WRONG\_SIZE if`size` is not equal as idx-th parameter's size.
### function `spec_api_init`

_Initialize the configuration handler._
```c
enum SpecReturnCode spec_api_init (
    struct SpecHandler *hspec,
    ArenaAllocatorHandler_t *harena,
    const struct SpecParameter *param_data,
    size_t param_count,
    spec_read_fn read_nvm,
    spec_write_fn write_nvm
) 
```


**Parameters:**


* `hspec` The configuration handler structure. 
* `harena` The arena allocator handler structure. 
* `param_data` An array of SPEC parameters. 
* `param_count` The number of parameters. 
* `read_nvm` The function used to read from NVM. 
* `write_nvm` The function used to write into NVM. 


**Returns:**

SPEC\_RC\_OK on success, SPEC\_RC\_NULL\_PTR otherwise.
### function `spec_api_load`

_Loads the configuration from NVM into RAM._
```c
enum SpecReturnCode spec_api_load (
    struct SpecHandler *hspec
) 
```


**Parameters:**


* `hspec` The configuration handler structure. 


**Returns:**

SPEC\_RC\_OK on success, SPEC\_RC\_NO\_CONFIG if there is no configuration stored, SPEC\_RC\_NULL\_PTR if `hspec` is NULL, SPEC\_RC\_IO\_ERR otherwise.
### function `spec_api_set`

_Set the value of the parameter at idx position._
```c
enum SpecReturnCode spec_api_set (
    struct SpecHandler *hspec,
    size_t idx,
    const void *data,
    size_t size
) 
```


**Parameters:**


* `hspec` The configuration handler structure. 
* `idx` The parameter position. 
* `data` A pointer to the variable containing the new value. 
* `size` The input data size.


**Returns:**

SPEC\_RC\_OK on success, SPEC\_RC\_NULL\_PTR if `hspec` or`data` is NULL and if`memcpy` fails, SPEC\_RC\_IDX\_OUT\_OF\_BOUNDS if`idx` is higher than the number of parameters, SPEC\_RC\_WRONG\_TYPE if`size` is not equal as idx-th parameter's size.
### function `spec_api_store`

_Stores the configuration into the NVM._
```c
enum SpecReturnCode spec_api_store (
    const struct SpecHandler *hspec
) 
```


**Parameters:**


* `hspec` The configuration handler structure. 


**Returns:**

SPEC\_RC\_OK on success, SPEC\_RC\_NULL\_PTR if `hspec` is NULL, SPEC\_RC\_IO\_ERR otherwise.


## File include/spec.h

_Library to dynamically configure MCUs parameters via different types of peripherals._

**Date:**

2025-07-10 



**Authors:**

Mirko Lana [[mirko.lana@eagletrt.it](mailto:mirko.lana@eagletrt.it)]



This library loads a configuration into the RAM by reading it from a NVM, such as FLASH. During program execution, the library listens for commands from one or more connected peripherals. Upon receiving a command, it can update the value of the specific parameter. If necessary, the updated configuration can be saved back to the NVM.



**Warning:**

The allocated memory will not be freed automatically but has to be deallocated by using the arena allocator.

## Structures and Types

| Type | Name |
| ---: | :--- |
| struct | [**SpecHandler**](#struct-spechandler) <br>_A structure that encapsulate data, functions and all required information to handle a configuration._ |
| struct | [**SpecParameter**](#struct-specparameter) <br> |
| enum  | [**SpecReturnCode**](#enum-specreturncode)  <br>_Enumeration with all possible return code of the library._ |
| typedef enum [**SpecReturnCode**](#enum-specreturncode)(\* | [**spec\_read\_fn**](#typedef-spec_read_fn)  <br>_Type definition for a function pointer for reading data from NVM._ |
| typedef enum [**SpecReturnCode**](#enum-specreturncode)(\* | [**spec\_write\_fn**](#typedef-spec_write_fn)  <br>_Type definition for a function pointer for writing data to NVM._ |



## Structures and Types Documentation

### struct `SpecHandler`

_A structure that encapsulate data, functions and all required information to handle a configuration._

**Attention:**

This structure should not be used directly.

Variables:

-  size\_t param_count  <br>The number of parameters

-  struct [**SpecParameter**](#struct-specparameter) \* param_data  <br>The configuration data

-  [**spec\_read\_fn**](#typedef-spec_read_fn) read_nvm  <br>Function to write on NVM

-  [**spec\_write\_fn**](#typedef-spec_write_fn) write_nvm  <br>Function to read from NVM

### struct `SpecParameter`


Variables:

-  void \* data  <br>Parameter's data

-  size\_t size  <br>The data size

### enum `SpecReturnCode`

_Enumeration with all possible return code of the library._
```c
enum SpecReturnCode {
    SPEC_RC_OK,
    SPEC_RC_NULL_PTR,
    SPEC_RC_IDX_OUT_OF_BOUNDS,
    SPEC_RC_WRONG_SIZE,
    SPEC_RC_NO_CONFIG,
    SPEC_RC_IO_ERR
};
```

### typedef `spec_read_fn`

_Type definition for a function pointer for reading data from NVM._
```c
typedef enum SpecReturnCode(* spec_read_fn) (size_t offset, void *data, size_t size);
```


**Parameters:**


* `offset` Offset in the NVM from which to read data. 
* `data` Pointer to the buffer where the data will be stored. 
* `size` Size of the data to read. 


**Returns:**

SPEC\_RC\_OK on success, SPEC\_RC\_NULL\_PTR if data is NULL, SEPC\_RC\_IO\_ERR otherwise
### typedef `spec_write_fn`

_Type definition for a function pointer for writing data to NVM._
```c
typedef enum SpecReturnCode(* spec_write_fn) (size_t offset, const void *data, size_t size);
```


**Parameters:**


* `offset` Offset in the NVM where data will be written. 
* `data` Pointer to the data to be written to NVM. 
* `size` Size of the data to write. 


**Returns:**

SPEC\_RC\_OK on success, SPEC\_RC\_NULL\_PTR if data is NULL, SEPC\_RC\_IO\_ERR otherwise



