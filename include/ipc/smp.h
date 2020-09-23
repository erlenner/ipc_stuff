#ifdef __cplusplus
#include <atomic>
#else
#include <stdatomic.h>
#endif

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
#define smp_load_acquire(p) std::atomic_load_explicit((std::atomic<typeof(p)>*)&p, std::memory_order_acquire)
#define smp_store_release(p, v) std::atomic_store_explicit((std::atomic<typeof(p)>*)&p, v, std::memory_order_release)
#else
#define smp_load_acquire(p) atomic_load_explicit(&p, memory_order_acquire)
#define smp_store_release(p, v) atomic_store_explicit(&p, v, memory_order_release)
#endif
