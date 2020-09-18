#ifdef __cplusplus
#include <atomic>
#else
#include <stdatomic.h>
#endif

#pragma GCC diagnostic ignored "-Wcomment"
#pragma GCC diagnostic push

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

//#define cpu_relax() \
//do {                \
//  smp_mb();         \
//  __asm__ __volatile__("nop; nop; nop; nop; nop; nop; nop; nop; nop; nop;");  \
//} while (0)

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

//#define __scalar_type_to_expr_cases(type)       \
//    unsigned type:  (unsigned type)0,           \
//    signed type:  (signed type)0
//
//#define __unqual_scalar_typeof(x) typeof(       \
//  _Generic((x),                               \
//    char:  (char)0,                          \
//    __scalar_type_to_expr_cases(char),       \
//    __scalar_type_to_expr_cases(short),      \
//    __scalar_type_to_expr_cases(int),        \
//    __scalar_type_to_expr_cases(long),       \
//    __scalar_type_to_expr_cases(long long),  \
//    default: (x)))
//
////#define ACCESS_ONCE(x) (*(volatile typeof(x) *)&(x))
//#define READ_ONCE(x)  (*(const volatile __unqual_scalar_typeof(x) *)&(x))
//#define WRITE_ONCE(x, val)            \
//do {                                    \
//  *(volatile typeof(x) *)&(x) = (val);  \
//} while (0)
//
//#define smp_load_acquire(p)                           \
//({                                                    \
//  __unqual_scalar_typeof(p) ___p1 = READ_ONCE(p);     \
//  smp_mb();                                           \
//  (typeof(p))___p1;                                   \
//})
//
//#define smp_store_release(p, v)   \
//do {                              \
//  smp_mb();                       \
//  WRITE_ONCE(p, v);               \
//} while (0)

#pragma GCC diagnostic pop
