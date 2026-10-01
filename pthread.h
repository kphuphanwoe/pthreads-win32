#ifndef _PTHREAD_H_INCLUDE
#define _PTHREAD_H_INCLUDE

#include <errno.h>
#include <process.h>
#include <stdbool.h>
#include <stdint.h>
#include <windows.h>

typedef CONDITION_VARIABLE pthread_cond_t;
typedef CRITICAL_SECTION pthread_mutex_t;
typedef HANDLE pthread_t;
typedef SYNCHRONIZATION_BARRIER pthread_barrier_t;

typedef void pthread_rwlock_t; // TODO
typedef void pthread_attr_t;
typedef void pthread_barrierattr_t;
typedef void pthread_condattr_t;
typedef void pthread_mutexattr_t;
typedef void pthread_rwlockattr_t;

/* essentials posix thread operation */
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
		   void *(*routine)(void *), void *arg);
__declspec(noreturn) void pthread_exit(void *retval);
int pthread_join(pthread_t thread, void **retval);
int pthread_equal(pthread_t t1, pthread_t t2);
pthread_t pthread_self(void);

/* barrier operations */
int pthread_barrier_destroy(pthread_barrier_t *barrier);
int pthread_barrier_init(pthread_barrier_t *barrier,
			 const pthread_barrierattr_t *attr, unsigned count);
int pthread_barrier_wait(pthread_barrier_t *barrier);

/* mutual exclusion operations */
int pthread_mutex_init(pthread_mutex_t *mutex,
		       const pthread_mutexattr_t *mutexattr);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
int pthread_mutex_destroy(pthread_mutex_t *mutex);

/* condition operations */
int pthread_cond_broadcast(pthread_cond_t *cond);
int pthread_cond_destroy(pthread_cond_t *cond);
int pthread_cond_init(pthread_cond_t *cond,
		      const pthread_condattr_t *cond_attr);
int pthread_cond_signal(pthread_cond_t *cond);
int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex,
			   const struct timespec *abstime);
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

/* read-write lock operations */
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);
int pthread_rwlock_init(pthread_rwlock_t *rwlock,
			const pthread_rwlockattr_t *attr);
int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_unlock(pthread_rwlock_t *rwlock);
int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock);

#endif /* !_PTHREAD_H_INCLUDE */
