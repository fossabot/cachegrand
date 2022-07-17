Configuration
=============

## Max lockable memory and Opened files limit

cachegrand uses io_uring, a standard kernel component, to perform disk and network I/O operations; this component shares
some memory directly with the kernel which can't get swapped and has to be locked in place once allocated.

The `max_clients` parameter (explained in the next section) is dependant on the amount of lockable memory, not having
enough might prevent cachegrand from starting or, as it's used by other software e.g. browsers, desktop managers, etc.,
it might cause other kind of failures

It is also important to have a value high enough for the `open files` as in Linux the network connections are counted
towards this limit as well, to manage 10000 connections the open files limit must be greater than 10000, it is
usually safe to set it to unlimited or to a very high value.

```shell
$ ulimit -a
...
max locked memory       (kbytes, -l) 64
...
open files                      (-n) 1024
...
```

In this case, both the `max locked memory` and the `open files` limits are too low and need to be increased.

These two limits can be safely set very high or to unlimited, cachegrand will try to set very high values at the start
but depending on the distribution or on the security settings the operation might not be allowed.

## Hugepages

The SLAB Allocator in cachegrand currently relies on 2MB hugepages, it's necessary to enable them otherwise it will
fail-back to a dummy malloc-based allocator that will perform extremely badly.

A simple way to enable them is to set `/sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages` to the desired number of
hugepages to allocate, bear in mind that each page is 2 MB.

Here an example on how initialize 512 hugepages (1 GB of memory).
```shell
echo 512 | sudo tee /sys/kernel/mm/hugepages/hugepages-2048kB/nr_hugepages
```

The hugepages can also be configured to at the system boot via sysctl, the specifics depend on the distribution in use. 

## Config file

cachegrand uses YAML for its configuration file, an example can be found in `etc/cachegrand.yaml.skel`.

| Parameter name                             | Value type                                                                                                                             | Default value                                                             | Documentation                                                                                                                                                                                    |
|--------------------------------------------|----------------------------------------------------------------------------------------------------------------------------------------|---------------------------------------------------------------------------|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------|
| cpus                                       | list (numeric, numeric ranges or all keyword)                                                                                          |                                                                           | List of cpus to bind on, values can be provided as numbers, numeric ranges or the special keyward `all` to bind on all the available cpus                                                        |
| workers_per_cpus                           | numeric                                                                                                                                | 1                                                                         | Number of worksers per cpu, suggested 1                                                                                                                                                          |
| run_in_foreground                          | bool                                                                                                                                   | false                                                                     | True/false flag to run cachegrand in the foregrand (currently unsupported)                                                                                                                       |
| pidfile_path                               | string                                                                                                                                 | /var/run/cachegrand/cachegrand.pid                                        | Path to the pid file                                                                                                                                                                             |
| use_slab_allocator                         | bool                                                                                                                                   | false                                                                     | Enable or disable the slab allocator, requires hugepages available                                                                                                                               |
| network.backend                            | enum (io_uring)                                                                                                                        | io_uring                                                                  | Set the backend for the network, allowed values *io_uring*                                                                                                                                       |
| network.max_clients                        | numeric                                                                                                                                | 250                                                                       | Max amount of clients that can connect                                                                                                                                                           |
| network.listen_backlog                     | numeric                                                                                                                                | 100                                                                       | Max listen backlog                                                                                                                                                                               |
| network.protocols                          | list                                                                                                                                   |                                                                           |                                                                                                                                                                                                  |
| network.protocols.type                     | enum (redis)                                                                                                                           | redis                                                                     | Set the type of protocol, allowed values *redis*                                                                                                                                                 |
| network.protocols.timeout.read_ms          | numeric                                                                                                                                | -1                                                                        | Read timeout in milliseconds, -1 to disable it or greater than 0 to enable it                                                                                                                    |
| network.protocols.timeout.write_ms         | numeric                                                                                                                                | 10000                                                                     | Write timeout in milliseconds, -1 to disable it or greater than 0 to enable it                                                                                                                   |
| network.protocols.keepalive.time           | numeric                                                                                                                                | 0                                                                         | Currently unsupported                                                                                                                                                                            |
| network.protocols.keepalive.interval       | numeric                                                                                                                                | 0                                                                         | Currently unsupported                                                                                                                                                                            |
| network.protocols.keepalive.probes         | numeric                                                                                                                                | 0                                                                         | Currently unsupported                                                                                                                                                                            |
| network.protocols.tls                      |                                                                                                                                        |                                                                           | Optional block of parameters to enable encryption for the protocol                                                                                                                               |
| network.protocols.tls.certificate_path     | string                                                                                                                                 |                                                                           | Path to the certificate in x509 format                                                                                                                                                           |
| network.protocols.tls.private_key_path:    | string                                                                                                                                 |                                                                           | Path to the private key                                                                                                                                                                          |
| network.protocols.tls.min_version          | enum (any, tls1.0, tls1.1, tls1.2, tls1.3)                                                                                             | any                                                                       | Max TLS version allowed, allowed options any:, tls1.0, tls1.1, tls1.2, tls1.3                                                                                                                    |
| network.protocols.tls.max_version          | enum (any, tls1.0, tls1.1, tls1.2, tls1.3)                                                                                             | any                                                                       | Max TLS version allowed, allowed options any:, tls1.0, tls1.1, tls1.2, tls1.3                                                                                                                    |
| network.protocols.tls.cipher_suites        | list                                                                                                                                   |                                                                           | Cipher suites allowed, run //path/to/cachegrand-server --list-tls-cipher-suites to get the full list                                                                                             |
| network.protocols.redis.max_key_length     | numeric                                                                                                                                | 8192                                                                      | Maximum allowed key length, it can't be greater than 65536 bytes                                                                                                                                 |
| network.protocols.redis.max_command_length | numeric                                                                                                                                | 536870912                                                                 | Maximum allowed command length                                                                                                                                                                   |
| network.protocols.bindings                 | list                                                                                                                                   |                                                                           | List of bindings to listen on (host / port tuples)                                                                                                                                               |
| network.protocols.bindings.host            | string                                                                                                                                 | 0.0.0.0                                                                   | IP Address to bind on, can be IPv4 or IPv6 if enabled in the system                                                                                                                              |
| network.protocols.bindings.port            | numeric                                                                                                                                | 6379                                                                      | Port to listen on, ports <= 1024 require root                                                                                                                                                    |
| network.protocols.bindings.tls             | bool                                                                                                                                   | false                                                                     | Enable or disable TLS for a specific binding                                                                                                                                                     |
| database.max_keys                          | numeric                                                                                                                                | 1000000                                                                   | cachegrand *currently* doesn't autoresize the hashtable, the amount of allowed keys is fixed                                                                                                     |
| database.backend                           | enum (memory/file)                                                                                                                     | memory                                                                    | Set the type of backend, allowed values *memory* and *file*                                                                                                                                      |
| database.file                              | list                                                                                                                                   |                                                                           | The current implementation of the file backend is a PoC and it's limited in performances and functionalities                                                                                     |
| database.file.path                         | string                                                                                                                                 | /var/lib/cachegrand                                                       | Path to a folder to be used for the shards                                                                                                                                                       |
| database.file.shard_size_mb                | numeric                                                                                                                                | 100                                                                       | Maximum size of a shard in MB                                                                                                                                                                    |
| database.file.max_opened_shards            | numeric                                                                                                                                | 1000                                                                      | Maximum number of shards opened (unsupported)                                                                                                                                                    |
| sentry.enable                              | bool                                                                                                                                   | false                                                                     | If enabled and if the dsn is provided, in case of a crash a minidump is automatically generated and uploaded to sentry.io - data stored in cachegrand get be uploaded if part of the stacktrace! |
| sentry.dsn                                 | string                                                                                                                                 | https://05dd54814d8149cab65ba2987d560340@o590814.ingest.sentry.io/5740234 | DSN to use with the sentry.io service                                                                                                                                                            |
| logs                                       | list                                                                                                                                   |                                                                           | List of log sinks                                                                                                                                                                                |
| logs.type                                  | enum (console/file)                                                                                                                    | console and file                                                          |                                                                                                                                                                                                  |
| logs.level                                 | list set (all, debug, verbose, info, warning, recoverable, error, no-debug, no-verbose, no-info, no-warning, no-recoverable, no-error) | all, no-verbose, no-debug                                                 | Log level                                                                                                                                                                                        |
| logs.file.path                             | string                                                                                                                                 | /var/log/cachegrand/cachegrand.log                                        | Path to the log file                                                                                                                                                                             |