#include "Utils/ThreadPool.h"
#include <pthread.h>

struct RGP_ThreadWork_s
{
    RGP_RoutineFunc_t startRoutine;
    void* arg;
    struct RGP_ThreadWork_s* next;
};

struct RGP_ThreadPool_s
{
    RGP_ThreadWork_t* workFirst;
    RGP_ThreadWork_t* workLast;
    pthread_mutex_t workMutex;
    pthread_cond_t  workCond;
    pthread_cond_t workingCond;
    size_t workingCount;
    size_t threadCount;
    Bool isStarted;
};

static RGP_ThreadWork_t* RGP_ThreadWrokCreate(RGP_RoutineFunc_t routine, void* arg)
{
    RGP_ASSERT(routine != NULL, "routine must not be NULL");
    RGP_ThreadWork_t* work;

    //switch to list to have something contiguous
    work = malloc(sizeof(RGP_ThreadWork_t));
    work->startRoutine = routine;
    work->arg = arg;
    work->next = NULL;

    return work;
}

static void RGP_ThreadWrokDestroy(RGP_ThreadWork_t* work)
{
    RGP_ASSERT(work != NULL, "work must not be NULL");

    free(work);
}

static RGP_ThreadWork_t* RGP_ThreadWorkRetreive(RGP_ThreadPool_t* pool)
{
    RGP_ASSERT(pool != NULL, "pool must not be NULL");

    RGP_ThreadWork_t* work = pool->workFirst;

    if(work == NULL)
    {
        return NULL;
    }

    if(work->next == NULL)
    {
        pool->workFirst = NULL;
        pool->workLast = NULL;
    }
    else
    {
        pool->workFirst = work->next;
    }

    return work;
}

static void* RGP_ThreadWorker(void* arg)
{
    RGP_ASSERT(arg != NULL, "pool must not be NULL");
    RGP_ThreadPool_t* pool = arg;
    RGP_ThreadWork_t* work = NULL;

    while(1)
    {
        pthread_mutex_lock(&pool->workMutex);

        while(pool->workFirst == NULL && pool->isStarted)
        {
            pthread_cond_wait(&pool->workCond, &pool->workMutex);
        }

        if(!pool->isStarted)
        {
            break;
        }

        work = RGP_ThreadWorkRetreive(pool);
        ++pool->workingCount;
        pthread_mutex_unlock(&pool->workMutex);

        if(work != NULL)
        {
            work->startRoutine(work->arg);
            RGP_ThreadWrokDestroy(work);
        }

        pthread_mutex_lock(&pool->workMutex);
        --pool->workingCount;
        if(pool->isStarted && pool->workingCount == 0 && pool->workFirst == NULL)
        {
            pthread_cond_signal(&pool->workingCond);
        }
        pthread_mutex_unlock(&pool->workMutex);
    }

    --pool->threadCount;
    pthread_cond_signal(&pool->workingCond);
    pthread_mutex_unlock(&pool->workMutex);

    return NULL;
}

Bool RGP_ThreadAddWork(RGP_ThreadPool_t* pool, RGP_RoutineFunc_t routine, void* arg)
{
    RGP_ASSERT(pool != NULL && routine != NULL, "pool and routine must not be NULL");

    RGP_ThreadWork_t* work = RGP_ThreadWrokCreate(routine, arg);
    if(work == NULL)
    {
        return 0;
    }

    pthread_mutex_lock(&pool->workMutex);
    if(pool->workFirst == NULL)
    {
        pool->workFirst = work;
        pool->workLast = pool->workFirst;
    }
    else
    {
        pool->workLast->next = work;
        pool->workLast = work;
    }

    pthread_cond_broadcast(&pool->workCond);
    pthread_mutex_unlock(&pool->workMutex);

    return 1;
}

void RGP_ThreadWait(RGP_ThreadPool_t* pool)
{
    pthread_mutex_lock(&pool->workMutex);
    while(1)
    {
        if(pool->workFirst != NULL || (pool->isStarted && pool->workingCount != 0) || (!pool->isStarted && pool->threadCount != 0))
        {
            pthread_cond_wait(&pool->workingCond, &pool->workMutex);
        }
        else
        {
            break;
        }
    }
    pthread_mutex_unlock(&pool->workMutex);
}

RGP_ThreadPool_t* RGP_ThreadCreate(size_t amount)
{
    RGP_ASSERT(amount > 0, "amount must be > 0");
    RGP_ThreadPool_t* pool;
    pthread_t thread;

    pool = calloc(1, sizeof(RGP_ThreadPool_t));
    pool->threadCount = amount;

    pthread_mutex_init(&pool->workMutex, NULL);
    pthread_cond_init(&pool->workCond, NULL);
    pthread_cond_init(&pool->workingCond, NULL);

    pool->workFirst = NULL;
    pool->workLast = NULL;
    pool->isStarted = 1;

    for(size_t i = 0; i < amount; ++i)
    {
        pthread_create(&thread, NULL, RGP_ThreadWorker, pool);
        pthread_detach(thread);
    }

    RGP_LOG_SUCCESS("Thread pool created with %d threads", amount);

    return pool;
}

void RGP_ThreadDestroy(RGP_ThreadPool_t* pool)
{
    RGP_ASSERT(pool != NULL, "pool should not be NULL");

    RGP_ThreadWork_t* work;
    RGP_ThreadWork_t* workNext;

    pthread_mutex_lock(&pool->workMutex);
    work = pool->workFirst;
    pool->isStarted = 0;

    while(work != NULL)
    {
        workNext = work->next;
        RGP_ThreadWrokDestroy(work);
        work = workNext;
    }

    pool->workFirst = NULL;
    pthread_cond_broadcast(&pool->workCond);
    pthread_mutex_unlock(&pool->workMutex);

    RGP_ThreadWait(pool);

    pthread_mutex_destroy(&pool->workMutex);
    pthread_cond_destroy(&pool->workCond);
    pthread_cond_destroy(&pool->workingCond);

    free(pool);

    RGP_LOG_SUCCESS("Thread pool destroyed");
}
