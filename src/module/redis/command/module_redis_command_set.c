/**
 * Copyright (C) 2018-2022 Daniele Salvatore Albano
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 **/

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include <strings.h>
#include <arpa/inet.h>
#include <assert.h>

#include "misc.h"
#include "exttypes.h"
#include "log/log.h"
#include "clock.h"
#include "spinlock.h"
#include "xalloc.h"
#include "data_structures/small_circular_queue/small_circular_queue.h"
#include "data_structures/double_linked_list/double_linked_list.h"
#include "data_structures/queue_mpmc/queue_mpmc.h"
#include "slab_allocator.h"
#include "data_structures/hashtable/mcmp/hashtable.h"
#include "data_structures/hashtable/mcmp/hashtable_op_set.h"
#include "data_structures/hashtable/spsc/hashtable_spsc.h"
#include "protocol/redis/protocol_redis.h"
#include "protocol/redis/protocol_redis_reader.h"
#include "protocol/redis/protocol_redis_writer.h"
#include "module/module.h"
#include "network/io/network_io_common.h"
#include "config.h"
#include "fiber.h"
#include "network/channel/network_channel.h"
#include "storage/io/storage_io_common.h"
#include "storage/channel/storage_channel.h"
#include "storage/db/storage_db.h"
#include "module/redis/module_redis.h"
#include "module/redis/module_redis_connection.h"
#include "network/network.h"
#include "worker/worker_stats.h"
#include "worker/worker_context.h"

#define TAG "module_redis_command_set"

MODULE_REDIS_COMMAND_FUNCPTR_COMMAND_END(set) {
    module_redis_command_set_context_t *context = connection_context->command.context;

    storage_db_expiry_time_ms_t expiry_time_ms = STORAGE_DB_ENTRY_NO_EXPIRY;

    if (context->expiration.value.pxat_unix_time_milliseconds.has_token) {
        expiry_time_ms = (int64_t)context->expiration.value.pxat_unix_time_milliseconds.value;
    } else if (context->expiration.value.exat_unix_time_seconds.has_token) {
        expiry_time_ms = (int64_t)context->expiration.value.exat_unix_time_seconds.value * 1000;
    } else if (context->expiration.value.px_milliseconds.has_token) {
        expiry_time_ms = clock_realtime_coarse_int64_ms() + (int64_t)context->expiration.value.px_milliseconds.value;
    } else if (context->expiration.value.ex_seconds.has_token) {
        expiry_time_ms = clock_realtime_coarse_int64_ms() + ((int64_t)context->expiration.value.ex_seconds.value * 1000);
    } else if (context->expiration.value.keepttl_keepttl.has_token) {
        // TODO: not implemented, require a RMW operation
    }

    if (context->condition.value.nx_nx.has_token) {
        // TODO: not implemented, require a RMW operation
    } else if (context->condition.value.xx_xx.has_token) {
        // TODO: not implemented, require a RMW operation
    }

    if (context->get_get.has_token) {
        // TODO: check if the value is a string
        // TODO: not implemented, require a RMW operation
    }

    if (!storage_db_add_new_entry_index(
            connection_context->db,
            context->key.value.key,
            context->key.value.length,
            context->value.value.chunk_sequence,
            expiry_time_ms)) {
        module_redis_connection_error_message_printf_noncritical(connection_context, "ERR set failed");
        return true;
    }

    // Mark both the key and the chunk_sequence as NULL as the storage db now owns them, we don't want them to be
    // automatically freed at the end of the execution, especially the key as the hashtable might not need to hold
    // a reference to it, it might have already been freed
    context->key.value.key = NULL;
    context->value.value.chunk_sequence = NULL;

    return module_redis_connection_send_ok(connection_context);
}