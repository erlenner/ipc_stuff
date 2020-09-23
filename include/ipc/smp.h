#ifdef __cplusplus
#include <atomic>
#else
#include <stdatomic.h>
#endif

// Symmetric multiprocessing helpers, using a mix of inline asm from the Linux kernel, and C11 atomics

#define barrier() __asm__ __volatile__("": : :"memory")

#if __arm__
#define CACHELINE_BYTES 32
// L1: https://developer.arm.com/documentation/ddi0388/f/Level-1-Memory-System/About-the-L1-memory-system
// L2: https://community.nxp.com/thread/510105

// https://github.com/torvalds/linux/blob/master/arch/arm/include/asm/barrier.h
#define dmb(option) __asm__ __volatile__ ("dmb " #option : : : "memory")
#define smp_mb() dmb(ish);
#define smp_wmb() dmb(ishst);
#define smp_rmb() smp_mb()
//#define smp_rmb() dmb(ishld); // armv8
//#define smp_mb() atomic_thread_fence(memory_order_acq_rel);
//#define smp_wmb() smp_mb()
//#define smp_rmb() smp_mb()

#define cpu_relax() barrier()

#elif __x86_64__
#define CACHELINE_BYTES 64

#define smp_mb() barrier()
#define smp_wmb() barrier()
#define smp_rmb() barrier()

#define cpu_relax() barrier()
#endif

#ifdef __cplusplus
#define smp_load_acquire(x) std::atomic_load_explicit((std::atomic<typeof(x)>*)&x, std::memory_order_acquire)
#define smp_store_release(x, val) std::atomic_store_explicit((std::atomic<typeof(x)>*)&x, val, std::memory_order_release)
#else
#define smp_load_acquire(x) atomic_load_explicit(&x, memory_order_acquire)
#define smp_store_release(x, val) atomic_store_explicit(&x, val, memory_order_release)
#endif



// Variable accesses as per http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0124r5.html#Variable%20Access

// KERNEL
#define smp_read_once(x)  (*(const volatile typeof(x) *)&(x))
#define smp_write_once(x, val) do { *(volatile typeof(x) *)&(x) = (val); } while (0)

//// C11
//#ifdef __cplusplus
//#define smp_read_once(x)  (std::atomic_load_explicit((const volatile typeof(x) *)(&(x)), memory_order_relaxed))
//#define smp_write_once(x, val) std::atomic_store_explicit((std::atomic<typeof(x)>*)(volatile typeof(x) *)(&(x)), val, std::memory_order_relaxed)
//#else
//#define smp_read_once(x)  ((atomic_load_explicit((const volatile typeof(x) *)(&(x)), memory_order_relaxed)))
//#define smp_write_once(x, val) atomic_store_explicit((volatile typeof(x) *)(&(x)), val, memory_order_relaxed)
//#endif
