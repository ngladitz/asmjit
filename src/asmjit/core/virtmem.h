// [AsmJit]
// Complete x86/x64 JIT and Remote Assembler for C++.
//
// [License]
// ZLIB - See LICENSE.md file in the package.

// [Guard]
#ifndef _ASMJIT_CORE_VIRTMEM_H
#define _ASMJIT_CORE_VIRTMEM_H

#include "../core/build.h"
#ifndef ASMJIT_DISABLE_JIT

// [Dependencies]
#include "../core/globals.h"

ASMJIT_BEGIN_NAMESPACE

//! \addtogroup asmjit_core_jit
//! \{

// ============================================================================
// [asmjit::VirtMem]
// ============================================================================

//! Virtual memory management.
namespace VirtMem {

//! Virtual memory access flags.
enum AccessFlags : uint32_t {
  //! No access flags.
  kAccessNone = 0x00000000u,
  //! Memory is readable.
  kAccessRead = 0x00000001u,
  //! Memory is writable (implies read access).
  kAccessWrite = 0x00000002u,
  //! Memory is executable (implies read access).
  kAccessExecute = 0x00000004u,

  //! A combination of `kAccessRead | kAccessWrite`
  kAccessReadWrite = 0x00000003u
};

//! Virtual memory information.
struct Info {
  //! Virtual memory page size.
  uint32_t pageSize;
  //! Virtual memory page granularity.
  uint32_t pageGranularity;
};

//! Dual memory mapping used to map an anonymous memory into two memory regions
//! where one region is read-only, but executable, and the second region is
//! read+write, but not executable. Please see \ref VirtMem::allocDualMapping()
//! for more details.
struct DualMapping {
  //! Pointer to data with 'Read' or 'Read+Execute' access.
  void* ro;
  //! Pointer to data with 'Read-Write' access, but never 'Write+Execute'.
  void* rw;
};

//! Get virtual memory information, see `VirtMem::Info` for more details.
ASMJIT_API Info info() noexcept;

//! Allocates virtual memory by either using `VirtualAlloc()` (Windows)
//! or `mmap()` (POSIX).
//!
//! NOTE: `size` should be aligned to a page size, use \ref VirtMem::info()
//! to obtain it. Invalid size will not be corrected by the implementation
//! and the allocation would not succeed in such case.
ASMJIT_API Error alloc(void** p, size_t size, uint32_t accessFlags) noexcept;

//! Releases virtual memory previously allocated by \ref  VirtMem::alloc() or
//! \ref VirtMem::allocDualMapping().
//!
//! NOTE: The size must be the same as used by \ref VirtMem::alloc(). If the
//! size is not the same value the call will fail on any POSIX system, but
//! pass on Windows, because of the difference of the implementation.
ASMJIT_API Error release(void* p, size_t size) noexcept;

//! A cross-platform wrapper around `mprotect()` (POSIX) and `VirtualProtect`
//! (Windows).
ASMJIT_API Error protect(void* p, size_t size, uint32_t accessFlags) noexcept;

//! Allocates virtual memory and creates two views of it where the first view
//! has no write access. This is an addition to the API that should be used
//! in cases in which the operating system either enforces W^X security policy
//! or the application wants to use this policy by default to improve security
//! and prevent an accidental (or purposed) self-modifying code.
//!
//! The memory returned in the `dm` are two independent mappings of the same
//! shared memory region. You must use \ref VirtMem::releaseDualMapping() to
//! release it when it's no longer needed. Never use `VirtMem::release()` to
//! release the memory returned by `allocDualMapping()` as that would fail on
//! Windows.
//!
//! \remarks Both pointers in `dm` would be set to `nullptr` if the function fails.
ASMJIT_API Error allocDualMapping(DualMapping* dm, size_t size, uint32_t accessFlags) noexcept;

//! Releases the virtual memory mapping previously allocated by
//! \ref VirtMem::allocDualMapping().
//!
//! \remarks Both pointers in `dm` would be set to `nullptr` if the function succeeds.
ASMJIT_API Error releaseDualMapping(DualMapping* dm, size_t size) noexcept;

} // VirtMem

//! \}

ASMJIT_END_NAMESPACE

#endif
#endif // _ASMJIT_CORE_VIRTMEM_H
