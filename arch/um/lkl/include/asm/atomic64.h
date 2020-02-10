/* SPDX-License-Identifier: GPL-2.0 */
#ifndef __LKL_ATOMIC64_H
#define __LKL_ATOMIC64_H

#include <linux/types.h>

#ifdef CONFIG_SMP
#error "SMP is not supported on this platform"
#else
#define ATOMIC64_OP(op, c_op)						\
static inline void atomic64_##op(s64 i, atomic64_t *v)			\
{									\
	unsigned long flags;						\
									\
	raw_local_irq_save(flags);					\
	v->counter = v->counter c_op i;					\
	raw_local_irq_restore(flags);					\
}

#define ATOMIC64_OP_RETURN(op, c_op)					\
static inline s64 atomic64_##op##_return(s64 i, atomic64_t *v)		\
{									\
	unsigned long flags;						\
	s64 ret;							\
									\
	raw_local_irq_save(flags);					\
	ret = (v->counter = v->counter c_op i);				\
	raw_local_irq_restore(flags);					\
									\
	return ret;							\
}

#define ATOMIC64_FETCH_OP(op, c_op)					\
static inline s64 atomic64_fetch_##op(s64 i, atomic64_t *v)			\
{									\
	unsigned long flags;						\
	s64 ret;							\
									\
	raw_local_irq_save(flags);					\
	ret = v->counter;						\
	v->counter = v->counter c_op i;					\
	raw_local_irq_restore(flags);					\
									\
	return ret;							\
}
#endif /* CONFIG_SMP */

#ifndef atomic64_add_return
ATOMIC64_OP_RETURN(add, +)
#endif

#ifndef atomic64_sub_return
ATOMIC64_OP_RETURN(sub, -)
#endif

#ifndef atomic64_fetch_add
ATOMIC64_FETCH_OP(add, +)
#endif

#ifndef atomic64_fetch_sub
ATOMIC64_FETCH_OP(sub, -)
#endif

#ifndef atomic64_fetch_and
ATOMIC64_FETCH_OP(and, &)
#endif

#ifndef atomic64_fetch_or
ATOMIC64_FETCH_OP(or, |)
#endif

#ifndef atomic64_fetch_xor
ATOMIC64_FETCH_OP(xor, ^)
#endif

#ifndef atomic64_and
ATOMIC64_OP(and, &)
#endif

#ifndef atomic64_or
ATOMIC64_OP(or, |)
#endif

#ifndef atomic64_xor
ATOMIC64_OP(xor, ^)
#endif

#undef ATOMIC64_FETCH_OP
#undef ATOMIC64_OP_RETURN
#undef ATOMIC64_OP


#define ATOMIC64_INIT(i)	{ (i) }

static inline void atomic64_add(s64 i, atomic64_t *v)
{
	atomic64_add_return(i, v);
}

static inline void atomic64_sub(s64 i, atomic64_t *v)
{
	atomic64_sub_return(i, v);
}

#ifndef atomic64_read
#define atomic64_read(v)	READ_ONCE((v)->counter)
#endif

#define atomic64_set(v, i) WRITE_ONCE(((v)->counter), (i))

#define atomic64_xchg(ptr, v)		(xchg(&(ptr)->counter, (v)))
#define atomic64_cmpxchg(v, old, new)	(cmpxchg(&((v)->counter), (old), (new)))

#endif /* __LKL_ATOMIC64_H */
