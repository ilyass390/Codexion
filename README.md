*This project has been created as part of the 42 curriculum by iamessag.*

# Codexion

## Description

Codexion is a concurrency simulation inspired by the classic Dining Philosophers problem. Multiple coders sit in a circular co-working hub, each needing two USB dongles simultaneously to compile their quantum code. The challenge is to orchestrate resource sharing fairly and efficiently — avoiding deadlocks, starvation, and burnout — using POSIX threads, mutexes, and smart scheduling.

The simulation stops either when a coder burns out (fails to compile within the allowed time) or when all coders have compiled the required number of times.

## Instructions

### Compilation

```bash
make
```

### Usage

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug \
time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

### Arguments

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coders and dongles (>= 1) |
| `time_to_burnout` | Time in ms before a coder burns out without compiling |
| `time_to_compile` | Time in ms to compile (requires 2 dongles) |
| `time_to_debug` | Time in ms to debug |
| `time_to_refactor` | Time in ms to refactor |
| `number_of_compiles_required` | Simulation stops when all coders reach this count |
| `dongle_cooldown` | Time in ms a dongle is unavailable after release |
| `scheduler` | Arbitration policy: `fifo` or `edf` |

### Examples

```bash
# 5 coders, feasible parameters, FIFO scheduling
./codexion 5 910 200 100 100 5 100 fifo

# 5 coders, feasible parameters, EDF scheduling
./codexion 5 910 200 100 100 5 100 edf

# Single coder — will burn out (only 1 dongle, needs 2)
./codexion 1 800 200 200 200 5 100 fifo
```

### Feasibility Check

Parameters are mathematically fair if:
- Even coders: `time_to_burnout > 2 * (time_to_compile + dongle_cooldown)`
- Odd coders: `time_to_burnout > 3 * (time_to_compile + dongle_cooldown)`

## Blocking Cases Handled

### Deadlock Prevention
Deadlock is prevented through **resource ordering** — each coder always acquires their two dongles in order of dongle ID (lowest ID first, highest ID second). This breaks Coffman's circular wait condition: no two coders can hold one dongle and wait for the other in a circular chain.

### Starvation Prevention
Each dongle maintains a **priority queue (min-heap)** of waiting coders. Access is granted strictly according to the scheduler policy:
- **FIFO**: coders served in arrival order (`request_time`)
- **EDF**: coders with the earliest burnout deadline served first (`last_compile_ms + time_to_burnout`)

Tie-breaking uses `request_time` then coder `id` for determinism.

### Cooldown Handling
After a coder releases a dongle, it is marked unavailable until `dongle_cooldown` milliseconds have elapsed via `available_at` timestamp. A coder only proceeds when both dongles are available AND their cooldown has passed.

### Precise Burnout Detection
A dedicated monitor thread checks every 300 microseconds whether any coder has exceeded `time_to_burnout` milliseconds without compiling. The burnout log is printed within 10ms of the actual burnout time.

### Log Serialization
All log output is protected by a `print_mutex`. The `stop` flag is always written under both `state_mutex` and `print_mutex`, guaranteeing that no state message can be printed after a burnout message.

## Thread Synchronization Mechanisms

### `state_mutex` (pthread_mutex_t)
Protects all shared simulation state: `stop`, `last_compile_ms`, `compile_count`, `coders_ready`, `simulation_started`. Any thread reading or writing these fields must hold `state_mutex`.

### `print_mutex` (pthread_mutex_t)
Serializes all output to stdout. The `stop` flag is also written under this mutex, ensuring that once burnout is logged, no further state messages can be printed.

### Per-dongle `mutex` and `cond` (pthread_mutex_t / pthread_cond_t)
Each dongle has its own mutex protecting its heap and `available_at` field. The condition variable is broadcast when a dongle is released, waking waiting coders to retry acquisition.

### `start_cond` (pthread_cond_t) — Barrier
A barrier ensures all coder threads and the monitor thread are created before the simulation starts. Each coder increments `coders_ready` and waits on `start_cond`. The main thread waits until `coders_ready == nb_coders`, then sets `start_ms`, updates all coders' `last_compile_ms` and `deadline`, sets `simulation_started = 1`, and broadcasts — releasing everyone simultaneously for a fair start.

### Race Condition Prevention
- **Double push**: `in_queue` flag ensures a coder is never pushed to a heap twice
- **Post-burnout logs**: `stop` checked under `print_mutex` before every printf
- **Partial acquisition**: coders only proceed when both dongles are granted atomically under their respective mutexes
- **Lock order**: always `state_mutex` → `print_mutex` → dongle mutexes, never reversed

### Thread-safe Communication Between Coders and Monitor
The monitor reads `last_compile_ms` and `compile_count` under `state_mutex`. Coders write these fields under the same mutex via `update_compile_state`. The monitor sets `stop = 1` under both `state_mutex` and `print_mutex` before logging burnout, guaranteeing coders see the stop signal and halt printing immediately.

## Resources

### References
- [The linux programing interface](https://broman.dev/download/The%20Linux%20Programming%20Interface.pdf)
- [Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [pthread_cond_wait — Linux man page](https://man7.org/linux/man-pages/man3/pthread_cond_wait.3p.html)
- [Earliest Deadline First Scheduling — Wikipedia](https://en.wikipedia.org/wiki/Earliest_deadline_first_scheduling)

### AI Usage
--helpet in understanding some concepts.
