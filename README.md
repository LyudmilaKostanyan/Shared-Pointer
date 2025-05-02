# Shared Pointer

## Overview

This project implements a simplified, thread-safe `SharedPtr<T>` in C++ with manual reference counting.

## Problem Description

`std::shared_ptr` provides shared ownership of dynamically allocated objects with automatic deletion when the last owner is destroyed. Manual implementation exposes internal mechanics. Key elements include:

* A heap-allocated control block that holds an atomic strong reference count.
* A `SharedPtr<T>` class that manages increments and decrements of that count.
* Automatic destruction of both the managed object and its control block when the strong count reaches zero.

## Implemented Features

* **ControlBlock**

  * A small `struct` with `std::atomic<std::size_t> strong_count{1}`.
  * Allocated alongside the managed object and shared by all copies.

* **Constructors & Destructor**

  * **Default ctor** sets both `ptr` and `ctrl` to `nullptr`.
  * **Pointer-ctor** (`SharedPtr(T* p)`) allocates a new `ControlBlock` if `p != nullptr`.
  * **Copy ctor** increments the shared `strong_count` with `fetch_add(1)`.
  * **Destructor** calls `release()`, which does `fetch_sub(1)` and, if the old count was 1, `delete`s both `ptr` and `ctrl`.

* **Copy Assignment**

  * Releases current ownership (`release()`), then copies `ptr` and `ctrl` and increments `strong_count`.

* **`release()`**

  * Internal helper: atomically decrements the count and destroys resources when it hits zero.

* **`reset(T* p = nullptr)`**

  * Calls `release()` on the old object, then (if `p`) sets `ptr = p` and allocates a fresh `ControlBlock`, or else sets both to `nullptr`.

* **`swap(SharedPtr& other)`**

  * Exchanges `ptr` and `ctrl` via `std::swap`.

* **Element Access**

  * `get()`, `operator*()`, `operator->()`, and `operator[](size_t)` forward to the managed `T*`.

* **Observers**

  * `use_count()` returns the current strong count (or 0 if `ctrl == nullptr`).
  * `unique()` returns `true` if `use_count() == 1`.

## Example Output

```
=== SharedPtr Unit Tests ===

== Running Basic Reference Counting Test ==
  After creation: p.use_count() = 1 (expected 1)
  After copy-ctor: p.use_count() = 2, q.use_count() = 2 (expected 2, 2)
  After assignment: p.use_count() = 3, r.use_count() = 3 (expected 3, 3)
  After r.reset(): r.use_count() = 0, p.use_count() = 2 (expected 0, 2)
  After p.reset() & q.reset(): p.use_count() = 0, q.use_count() = 0 (expected 0, 0)
[PASS] Basic Reference Counting

== Running Self-Assignment Test ==
  After creation: a.use_count() = 1 (expected 1)
  After self-assignment: a.use_count() = 1 (expected 1)
[PASS] Self-Assignment

== Running Swap Test ==
  Before swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
  After swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
[PASS] Swap Operation

=== All tests completed successfully ===

  Before swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
  After swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
[PASS] Swap Operation

  Before swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
  After swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
[PASS] Swap Operation
  Before swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
  After swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
  Before swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
  After swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
  Before swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
  After swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
  Before swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
  After swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
  Before swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
  After swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
  Before swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
  Before swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
  After swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
  After swap: a.use_count() = 1, b.use_count() = 1 (expected 1, 1)
[PASS] Swap Operation

=== All tests completed successfully ===
```

## Explanation of Output

* **Test headers** mark each scenario.
* **“After …” lines** show both actual and expected `use_count()`.
* **`[PASS]`** confirms each test’s assertions all succeeded.
* Any failed `assert` will abort immediately, exposing the bug.

## How to Compile and Run

### 1. Clone the Repository

```bash
git clone https://github.com/LyudmilaKostanyan/Shared-Pointer.git
cd Shared-Pointer
```

### 2. Build

```bash
cmake -S . -B build
cmake --build build
```

### 3. Run

#### Linux/macOS

```bash
cd build
./main
```

#### Windows

```powershell
cd build
.\main.exe
```
