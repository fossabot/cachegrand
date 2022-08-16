#ifndef CACHEGRAND_MODULE_REDIS_H
#define CACHEGRAND_MODULE_REDIS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "module_redis_autogenerated_commands_enum.h"

#define MODULE_REDIS_COMMAND_FUNCPTR_NAME(COMMAND_FUNC_PTR, TYPE) \
    module_redis_process_command_##COMMAND_FUNC_PTR##_##TYPE

#define MODULE_REDIS_COMMAND_FUNCPTR_NAME_AUTOGEN(COMMAND_FUNC_PTR, TYPE) \
    module_redis_process_command_##COMMAND_FUNC_PTR##_##TYPE##_autogen

#define MODULE_REDIS_COMMAND_FUNCPTR_GENERIC(COMMAND_FUNC_PTR, TYPE, ARGUMENTS) \
    module_redis_command_funcptr_retval_t MODULE_REDIS_COMMAND_FUNCPTR_NAME(COMMAND_FUNC_PTR, TYPE) (ARGUMENTS)

#define MODULE_REDIS_COMMAND_FUNCPTR_GENERIC_AUTOGEN(COMMAND_FUNC_PTR, TYPE, ARGUMENTS) \
    module_redis_command_funcptr_retval_t MODULE_REDIS_COMMAND_FUNCPTR_NAME_AUTOGEN(COMMAND_FUNC_PTR, TYPE) (ARGUMENTS)

#define MODULE_REDIS_COMMAND_FUNCPTR_ARGUMENTS_BASE \
    module_redis_connection_context_t *connection_context

#define MODULE_REDIS_COMMAND_FUNCPTR_ARGUMENTS_COMMAND_END \
    MODULE_REDIS_COMMAND_FUNCPTR_ARGUMENTS_BASE

#define MODULE_REDIS_COMMAND_FUNCPTR_ARGUMENTS_COMMAND_FREE \
    MODULE_REDIS_COMMAND_FUNCPTR_ARGUMENTS_BASE

#define MODULE_REDIS_COMMAND_FUNCPTR_COMMAND_END(COMMAND_FUNC_PTR) \
    MODULE_REDIS_COMMAND_FUNCPTR_GENERIC( \
        COMMAND_FUNC_PTR, \
        command_end, \
        MODULE_REDIS_COMMAND_FUNCPTR_ARGUMENTS_COMMAND_END)

#define MODULE_REDIS_COMMAND_FUNCPTR_COMMAND_FREE_AUTOGEN(COMMAND_FUNC_PTR) \
    MODULE_REDIS_COMMAND_FUNCPTR_GENERIC_AUTOGEN( \
        COMMAND_FUNC_PTR, \
        command_free, \
        MODULE_REDIS_COMMAND_FUNCPTR_ARGUMENTS_COMMAND_FREE)

#define MODULE_REDIS_COMMAND_AUTOGEN(ID, COMMAND, COMMAND_FUNC_PTR, REQUIRED_ARGS_COUNT, HAS_VARIABLE_ARGUMENTS, ARGS_COUNT) \
    { \
        .command = MODULE_REDIS_COMMAND_##ID, \
        .string = (COMMAND), \
        .string_len = strlen(COMMAND), \
        .context_size = sizeof(CONCAT(CONCAT(module_redis_command, COMMAND_FUNC_PTR), context_t)), \
        .arguments = CONCAT(CONCAT(module_redis_command, COMMAND_FUNC_PTR), arguments), \
        .arguments_count = (ARGS_COUNT), \
        .command_end_funcptr = MODULE_REDIS_COMMAND_FUNCPTR_NAME(COMMAND_FUNC_PTR, command_end), \
        .command_free_funcptr = MODULE_REDIS_COMMAND_FUNCPTR_NAME_AUTOGEN(COMMAND_FUNC_PTR, command_free), \
        .required_arguments_count = (REQUIRED_ARGS_COUNT), \
        .has_variable_arguments = (HAS_VARIABLE_ARGUMENTS), \
        .tokens_hashtable = NULL, \
    }

#define MODULE_REDIS_COMMAND(ID, COMMAND, COMMAND_FUNC_PTR, REQUIRED_ARGS_COUNT, HAS_VARIABLE_ARGUMENTS, ARGS_COUNT) \
    { \
        .command = MODULE_REDIS_COMMAND_##ID, \
        .string = (COMMAND), \
        .string_len = strlen(COMMAND), \
        .context_size = sizeof(CONCAT(CONCAT(module_redis_command, COMMAND_FUNC_PTR), context_t)), \
        .arguments = CONCAT(CONCAT(module_redis_command, COMMAND_FUNC_PTR), arguments), \
        .arguments_count = (ARGS_COUNT), \
        .command_end_funcptr = MODULE_REDIS_COMMAND_FUNCPTR_NAME(COMMAND_FUNC_PTR, command_end), \
        .command_free_funcptr = MODULE_REDIS_COMMAND_FUNCPTR_NAME(COMMAND_FUNC_PTR, command_free), \
        .required_arguments_count = (REQUIRED_ARGS_COUNT), \
        .has_variable_arguments = (HAS_VARIABLE_ARGUMENTS), \
        .tokens_hashtable = NULL, \
    }

typedef void module_redis_command_context_t;
typedef bool module_redis_command_funcptr_retval_t;

// This typedef is needed before the declaration of the function pointers as it's used in there
// the entire struct can't be moved because of the dependencies
typedef struct module_redis_connection_context module_redis_connection_context_t;

typedef module_redis_command_funcptr_retval_t (module_redis_command_end_funcptr_t)(
        MODULE_REDIS_COMMAND_FUNCPTR_ARGUMENTS_COMMAND_END);
typedef module_redis_command_funcptr_retval_t (module_redis_command_free_funcptr_t)(
        MODULE_REDIS_COMMAND_FUNCPTR_ARGUMENTS_COMMAND_FREE);

typedef struct module_redis_key module_redis_key_t;
struct module_redis_key {
    char *key;
    size_t length;
};

typedef struct module_redis_pattern module_redis_pattern_t;
struct module_redis_pattern {
    char *pattern;
    size_t length;
};

typedef struct module_redis_long_string module_redis_long_string_t;
struct module_redis_long_string {
    storage_db_chunk_sequence_t *chunk_sequence;
    struct {
        storage_db_chunk_index_t index;
        off_t offset;
    } current_chunk;
};

typedef struct module_redis_short_string module_redis_short_string_t;
struct module_redis_short_string {
    char *short_string;
    size_t length;
};

enum module_redis_command_argument_type {
    MODULE_REDIS_COMMAND_ARGUMENT_TYPE_UNSUPPORTED,
    MODULE_REDIS_COMMAND_ARGUMENT_TYPE_KEY,
    MODULE_REDIS_COMMAND_ARGUMENT_TYPE_SHORT_STRING,
    MODULE_REDIS_COMMAND_ARGUMENT_TYPE_LONG_STRING,
    MODULE_REDIS_COMMAND_ARGUMENT_TYPE_INTEGER,
    MODULE_REDIS_COMMAND_ARGUMENT_TYPE_DOUBLE,
    MODULE_REDIS_COMMAND_ARGUMENT_TYPE_UNIXTIME,
    MODULE_REDIS_COMMAND_ARGUMENT_TYPE_BOOL,
    MODULE_REDIS_COMMAND_ARGUMENT_TYPE_PATTERN,
    MODULE_REDIS_COMMAND_ARGUMENT_TYPE_BLOCK,
    MODULE_REDIS_COMMAND_ARGUMENT_TYPE_ONEOF,
};
typedef enum module_redis_command_argument_type module_redis_command_argument_type_t;

typedef struct module_redis_command_argument module_redis_command_argument_t;
struct module_redis_command_argument {
    char *name;
    module_redis_command_argument_type_t type;
    module_redis_command_argument_t *parent_argument;
    char *since;
    char *token;
    module_redis_command_argument_t *sub_arguments;
    uint16_t sub_arguments_count;
    bool is_positional;
    bool is_optional;
    bool is_sub_argument;
    bool has_sub_arguments;
    bool has_multiple_occurrences;
    bool has_multiple_token;
    size_t argument_context_member_size;
    off_t argument_context_member_offset;
};

typedef struct module_redis_command_info module_redis_command_info_t;
struct module_redis_command_info {
    // The longest Redis command is 23 chars
    char string[24];
    uint8_t string_len;
    module_redis_commands_t command;
    uint16_t context_size;
    uint16_t arguments_count;
    uint8_t required_arguments_count;
    bool has_variable_arguments;
    module_redis_command_argument_t *arguments;
    module_redis_command_end_funcptr_t *command_end_funcptr;
    module_redis_command_free_funcptr_t *command_free_funcptr;
    hashtable_spsc_t *tokens_hashtable;
};

typedef struct module_redis_command_parser_context_token_map_item module_redis_command_parser_context_argument_token_entry_t;
struct module_redis_command_parser_context_token_map_item {
    char *token;
    module_redis_command_argument_t *argument;
    char *one_of_tokens[6];
    uint16_t one_of_token_count;
    uint8_t token_length;
};

typedef struct module_redis_command_parser_context module_redis_command_parser_context_t;
struct module_redis_command_parser_context {
    uint16_t positional_arguments_parsed_count;
    struct {
        bool require_stream;
        void *member_context_addr;
        module_redis_command_argument_t *expected_argument;
        module_redis_command_argument_t *next_expected_argument;
        uint16_t block_argument_index;
    } current_argument;
};

struct module_redis_connection_context {
    protocol_redis_resp_version_t resp_version;
    char *client_name;
    protocol_redis_reader_context_t reader_context;
    network_channel_t *network_channel;
    network_channel_buffer_t read_buffer;
    storage_db_t *db;
    size_t current_argument_token_data_offset;
    bool terminate_connection;
    struct {
        char *message;
    } error;
    struct {
        size_t data_length;
        module_redis_command_info_t *info;
        module_redis_command_context_t *context;
        module_redis_command_parser_context_t parser_context;
        bool skip;
    } command;
};

#include "module_redis_autogenerated_commands_contexts.h"

void module_redis_accept(
        network_channel_t *channel);

bool module_redis_process_data(
        module_redis_connection_context_t *connection_context,
        network_channel_buffer_t *read_buffer);

#ifdef __cplusplus
}
#endif

#endif //CACHEGRAND_MODULE_REDIS_H