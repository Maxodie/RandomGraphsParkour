#include "Utils/ThreadPool.h"
#include <pthread.h>

struct RGP_ThreadWork_s
{
    RGP_RoutineFunc_t startRoutine;
    void* arg;

    //scheduling
    //isPending tells that the worker is doing nothing and can be used
    Bool isPending;
    //isProcessing tells the retreiver it has already be taken by a thread and it is currently running (so it's obviously not pending)
    Bool isProcessing;
};

struct RGP_ThreadPool_s
{
    RGP_ThreadWork_t* workList;
    size_t workReadyCount;
    size_t totalWorkPoolCount;
    pthread_mutex_t workMutex;
    pthread_cond_t  workCond;
    pthread_cond_t workingCond;
    size_t workingCount;
    size_t threadCount;
    Bool isStarted;
};

static RGP_ThreadWork_t* RGP_ThreadWorkRetreive(RGP_ThreadPool_t* pool)
{
    RGP_ASSERT(pool != NULL, "pool must not be NULL");

    if(pool->workReadyCount == 0)
    {
        return NULL;
    }

    RGP_ThreadWork_t* work = NULL;
    for(size_t i = 0; i < pool->totalWorkPoolCount; ++i)
    {
        work = &pool->workList[i];
        if(!work->isProcessing && !work->isPending)
        {
            work->isProcessing = 1;
            break;
        }
        RGP_ASSERT(pool->totalWorkPoolCount - 1 != i, "no available work found but workReadyCount says it exists");
    }
    --pool->workReadyCount;

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

        while(pool->workReadyCount == 0 && pool->isStarted)
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
            work->isPending = 1;
            work->isProcessing = 0;
        }

        pthread_mutex_lock(&pool->workMutex);
        --pool->workingCount;
        if(pool->isStarted && pool->workingCount == 0 && pool->workReadyCount == 0)
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

    pthread_mutex_lock(&pool->workMutex);

    RGP_ThreadWork_t* work = NULL;
    for(size_t i = 0; i < pool->totalWorkPoolCount; ++i)
    {
        RGP_ThreadWork_t* searched = &pool->workList[i];
        if(searched->isPending)
        {
            work = searched;
            break;
        }
    }

    if(work == NULL)
    {
        RGP_ListAddUb(pool->workList);
        work = &pool->workList[pool->totalWorkPoolCount];
        pool->totalWorkPoolCount++;
    }

    work->arg = arg;
    work->startRoutine = routine;
    work->isPending = 0;
    work->isProcessing = 0;
    ++pool->workReadyCount;

    pthread_cond_broadcast(&pool->workCond);
    pthread_mutex_unlock(&pool->workMutex);

    return 1;
}

void RGP_ThreadWait(RGP_ThreadPool_t* pool)
{
    pthread_mutex_lock(&pool->workMutex);
    while(1)
    {
        if(pool->workReadyCount > 0 || (pool->isStarted && pool->workingCount != 0) || (!pool->isStarted && pool->threadCount != 0))
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

    pool->workList = RGP_ListInit(amount, RGP_ThreadWork_t);
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

    RGP_ThreadWork_t* workNext;

    pthread_mutex_lock(&pool->workMutex);
    pool->isStarted = 0;

    RGP_ListDelete(pool->workList);
    pthread_cond_broadcast(&pool->workCond);
    pthread_mutex_unlock(&pool->workMutex);

    RGP_ThreadWait(pool);

    pthread_mutex_destroy(&pool->workMutex);
    pthread_cond_destroy(&pool->workCond);
    pthread_cond_destroy(&pool->workingCond);

    free(pool);

    RGP_LOG_SUCCESS("Thread pool destroyed");
}
