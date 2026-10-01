#include "pthread.h"

struct execution_context_t {
	void *(*func)(void *);
	void *arg;
};
typedef struct execution_context_t execution_context_t;

unsigned __stdcall _pthread_routine_exec(void *arg)
{
	execution_context_t data = *(execution_context_t *)arg;
	free(arg);
	return (DWORD)(DWORD_PTR)data.func(data.arg);
}

int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
		   void *(*routine)(void *), void *arg)
{
	(void)attr;
	execution_context_t *context =
		(execution_context_t *)calloc(1, sizeof(*context));
	context->func = routine;
	context->arg = arg;
	*thread = (pthread_t)_beginthreadex(NULL, 0, &_pthread_routine_exec,
					    context, 0, NULL);
	return *thread == 0;
}

int pthread_join(pthread_t thread, void **retval)
{
	if (WaitForSingleObject(thread, INFINITE) == WAIT_OBJECT_0) {
		if (retval) {
			DWORD ex = 0;
			GetExitCodeThread(thread, &ex);
			*retval = (void *)(uint64_t)ex;
		}
		CloseHandle(thread);
		return 0;
	}
	return -1;
}

__declspec(noreturn) void pthread_exit(void *retval)
{
	_endthreadex((unsigned)(uintptr_t)retval);
}

int pthread_equal(pthread_t t1, pthread_t t2)
{
	return GetThreadId(t1) == GetThreadId(t2);
}

pthread_t pthread_self(void)
{
	return (pthread_t)GetCurrentThread();
}

/* barrier operations */
int pthread_barrier_destroy(pthread_barrier_t *barrier)
{
	if (!barrier)
		return EINVAL;

	return DeleteSynchronizationBarrier(barrier);
}

int pthread_barrier_init(pthread_barrier_t *barrier,
			 const pthread_barrierattr_t *attr, unsigned count)
{
	(void)attr;
	if (!barrier)
		return EINVAL;

	return InitializeSynchronizationBarrier(barrier, count, -1);
}

int pthread_barrier_wait(pthread_barrier_t *barrier)
{
	if (!barrier)
		return EINVAL;

	return EnterSynchronizationBarrier(barrier, -1);
}

/* mutual exclusion operations */
int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	if (!mutex)
		return EINVAL;

	LeaveCriticalSection(mutex);
	return 0;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
	if (!mutex)
		return EINVAL;

	return !TryEnterCriticalSection(mutex);
}

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
	if (!mutex)
		return EINVAL;

	EnterCriticalSection(mutex);
	return 0;
}

int pthread_mutex_init(pthread_mutex_t *mutex,
		       const pthread_mutexattr_t *mutexattr)
{
	InitializeCriticalSection(mutex);
	return 0;
}

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
	DeleteCriticalSection(mutex);
	return 0;
}

/* condition operations */
int pthread_cond_broadcast(pthread_cond_t *cond)
{
	WakeAllConditionVariable(cond);
	return 0;
}

int pthread_cond_destroy(pthread_cond_t *cond)
{
	/* InitializeConditionVariable() atomically release a lock. */
	return 0;
}

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *cond_attr)
{
	(void)cond_attr;
	InitializeConditionVariable(cond);
	return 0;
}

int pthread_cond_signal(pthread_cond_t *cond)
{
	WakeConditionVariable(cond);
	return 0;
}

int pthread_cond_timedwait(pthread_cond_t *cond, pthread_mutex_t *mutex,
			   const struct timespec *abstime)
{
	(void)cond;
	(void)mutex;
	(void)abstime;
	/*
	 * TODO: Need to implement
	 * ref: https://man7.org/linux/man-pages/man3/pthread_cond_signal.3.html
	 */
	return 0;
}

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
	return !SleepConditionVariableCS(cond, mutex, INFINITE);
}

/* read-write lock operations */
int pthread_rwlock_destroy(pthread_rwlock_t *rwlock)
{
	(void)rwlock;
	/*
	 * TODO: Need to implement
	 */
	return 0;
}

int pthread_rwlock_init(pthread_rwlock_t *rwlock,
			const pthread_rwlockattr_t *attr)
{
	(void)rwlock;
	(void)attr;
	/*
	 * TODO: Need to implement
	 */
	return 0;
}

int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock)
{
	(void)rwlock;
	/*
	 * TODO: Need to implement
	 */
	return 0;
}

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock)
{
	(void)rwlock;
	/*
	 * TODO: Need to implement
	 */
	return 0;
}

int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock)
{
	(void)rwlock;
	/*
	 * TODO: Need to implement
	 */
	return 0;
}

int pthread_rwlock_unlock(pthread_rwlock_t *rwlock)
{
	(void)rwlock;
	/*
	 * TODO: Need to implement
	 */
	return 0;
}

int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock)
{
	(void)rwlock;
	/*
	 * TODO: Need to implement
	 */
	return 0;
}
