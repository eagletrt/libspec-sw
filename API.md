# API Reference

### Typedefs

<details>
<summary><strong><code>typedef enum SpecReturnCode (*spec_read_fn)(...)</code></strong></summary>

```c
typedef enum SpecReturnCode (*spec_read_fn)(size_t offset,
                             void *data,
                             size_t size);
```

#### Description:
> Type definition for a function pointer for reading data from NVM.

#### Function Parameters
> | Parameter | Type      | Description                                          |
> |-----------|-----------|------------------------------------------------------|
> | `offset`  | `size_t`  | Offset in the NVM from which to read data.           |
> | `data`    | `void*`   | Pointer to the buffer where the data will be stored. |
> | `size`    | `size_t`  | Size of the data to read.                            |

#### Return Values
> Returns SPEC_RC_OK on success, SPEC_RC_NULL_PTR if data is NULL,
> SEPC_RC_IO_ERR otherwise
    
</details>

<details>
<summary><strong><code>typedef enum SpecReturnCode (*spec_write_fn)(...)</code></strong></summary>

```c
typedef enum SpecReturnCode (*spec_write_fn)(size_t offset,
                                             const void *data,
                                             size_t size);
```

#### Description:
> Type definition for a function pointer for writing data to NVM.

#### Function Parameters
> | Parameter | Type            | Description                                   |
> |-----------|-----------------|-----------------------------------------------|
> | `offset`  | `size_t`        | Offset in the NVM where data will be written. |
> | `data`    | `const void*`   | Pointer to the data to be written to NVM.     |
> | `size`    | `size_t`        | Size of the data to write.                    |

#### Return Values
> Returns SPEC_RC_OK on success, SPEC_RC_NULL_PTR if data is NULL,
> SEPC_RC_IO_ERR otherwise
    
</details>

### Enums

<details>
<summary><strong><code>enum SpecReturnCode</code></strong></summary>

#### Enumeration Values
> | Value                       | Description                         |
> |-----------------------------|-------------------------------------|
> | `SPEC_RC_OK`                | Everything is fine.                 |
> | `SPEC_RC_NULL_PTR`          | Unexpected NULL pointer detected.   |
> | `SPEC_RC_IDX_OUT_OF_BOUNDS` | The given index exceeds its bounds. |
> | `SPEC_RC_WRONG_SIZE`        | Data size mismatch.                 |
> | `SPEC_RC_NO_CONFIG`         | No configuration stored.            |
> | `SPEC_RC_IO_ERR`            | I/O related error.                  |

#### Description:
> Enumeration with all possible return code of the library.
    
</details>
 
### Data Structures

<details>
<summary><strong><code>struct SpecParameter</code></strong></summary>

#### Structure Members
> | Member | Type     | Description       |
> |--------|----------|-------------------|
> | `data` | `void*`  | Parameter's data. |
> | `size` | `size_t` | The data size.    |

#### Description:
> An helper structure used to give a default configuration to the handler
> via `spec_api_init()` function. It also describes the structure of the
> configuration.
    
</details>

<details>
<summary><strong><code>struct SpecParameter</code></strong></summary>
  
#### Structure Members
> | Member         | Type                    | Description               |
> |----------------|-------------------------|---------------------------|
> | `param_data`  | `struct SpecParameter*` | The configuration data.    |
> | `param_count` | `size_t`                | The number of parameters.  |
> | `read_nvm`    | `spec_read_fn`          | Function to write on NVM.  |
> | `write_nvm`   | `spec_write_fn`         | Function to read from NVM. |

#### Description:
> A structure that encapsulate data, functions and all required information
> to handle a configuration. This structure **SHOULD NOT** be used directly.

</details>

### Functions

<details>
<summary><strong><code>enum SpecReturnCode spec_api_init(...)</code></strong></summary>

```c
enum SpecReturnCode spec_api_init(struct SpecHandler *hspec,
                                  ArenaAllocatorHandler_t *harena,
                                  const struct SpecParameter *param_data,
                                  size_t param_count,
                                  spec_read_fn read_nvm,
                                  spec_write_fn write_nvm);
```

#### Description:
> Initialize the configuration handler.

#### Function Parameters
> | Parameter     | Type                          | Description                            |
> |-------------- |-------------------------------|----------------------------------------|
> | `hspec`       | `struct SpecHandler*`         | The configuration handler structure.   |
> | `harena`      | `ArenaAllocatorHandler_t*`    | The arena allocator handler structure. |
> | `param_data`  | `const struct SpecParameter*` | An array of SPEC parameters.           |
> | `param_count` | `size_t`                      | The number of parameters.              |
> | `read_nvm`    | `spec_read_fn`                | The function used to read from NVM.    |
> | `write_nvm`   | `spec_write_fn`               | The function used to write into NVM.   |

#### Return Values
> Returns SPEC_RC_OK on success, SPEC_RC_NULL_PTR otherwise.

</details>

<details>
<summary><strong><code>enum SpecReturnCode spec_api_load(...)</code></strong></summary>

```c
enum SpecReturnCode spec_api_load(struct SpecHandler *hspec);
```

#### Description:
> Loads the configuration from NVM into RAM.

#### Function Parameters
> | Parameter | Type                   | Description                          |
> |-----------|------------------------|--------------------------------------|
> | `hspec`   | `struct SpecHandler*`  | The configuration handler structure. |

#### Return Values
> Returns SPEC_RC_OK on success, SPEC_RC_NO_CONFIG if there is
> no configuration stored, SPEC_RC_NULL_PTR if `hspec` is NULL,
> SPEC_RC_IO_ERR otherwise.

</details>

<details>
<summary><strong><code>enum SpecReturnCode spec_api_store(...)</code></strong></summary>

```c
enum SpecReturnCode spec_api_store(struct SpecHandler *hspec);
```

#### Description:
> Stores the configuration into the NVM.

#### Function Parameters
> | Parameter | Type                         | Description                          |
> |-----------|------------------------------|--------------------------------------|
> | `hspec`   | `const struct SpecHandler*`  | The configuration handler structure. |

#### Return Values
> Returns SPEC_RC_OK on success, SPEC_RC_NULL_PTR if `hspec` is
> NULL, SPEC_RC_IO_ERR otherwise.

</details>

<details>
<summary><strong><code>enum SpecReturnCode spec_api_get(...)</code></strong></summary>

```c
enum SpecReturnCode spec_api_get(const struct SpecHandler *hspec,
                                 size_t idx,
                                 void *out,
                                 size_t size);

```

#### Description:
> Get a copy of the parameter at idx position.

#### Function Parameters
> | Parameter | Type                         | Description                                                   |
> |-----------|------------------------------|---------------------------------------------------------------|
> | `hspec`   | `const struct SpecHandler*`  | The configuration handler structure                           |
> | `idx`     | `size_t`                     | The parameter position.                                       |
> | `out`     | `void*`                      | A pointer to the variable where the parameter is copied into. |
> | `size`    | `size_t`                     | The output data size.                                         |

#### Return Values
> Returns SPEC_RC_OK on success, SPEC_RC_NULL_PTR if `hspec` or 
> `out` is NULL and if `memcpy` fails, SPEC_RC_IDX_OUT_OF_BOUNDS
> if `idx` is higher than the number of parameters, SPEC_RC_WRONG_SIZE
> if `size` is not equal as idx-th parameter's size.


</details>

<details>
<summary><strong><code>enum SpecReturnCode spec_api_set(...)</code></strong></summary>

```c
enum SpecReturnCode spec_api_get(const struct SpecHandler *hspec,
                                 size_t idx,
                                 const void *data,
                                 size_t size);
```

#### Description:
> Stores the configuration into the NVM.

#### Function Parameters
> | Parameter | Type                         | Description                                     |
> |-----------|------------------------------|-------------------------------------------------|
> | `hspec`   | `const struct SpecHandler*`  | The configuration handler structure             |
> | `idx`     | `size_t`                     | The parameter position.                         |
> | `data`    | `const void*`                | A pointer to the variable containing the value. |
> | `size`    | `size_t`                     | The output data size.                           |

#### Return Values
> Returns SPEC_RC_OK on success, SPEC_RC_NULL_PTR if `hspec` or 
> `out` is NULL and if `memcpy` fails, SPEC_RC_IDX_OUT_OF_BOUNDS
> if `idx` is higher than the number of parameters, SPEC_RC_WRONG_SIZE
> if `size` is not equal as idx-th parameter's size.

</details>

