// ライセンス: GPL2

// スレッド関係の関数

#ifndef _MISCTHREAD_H
#define _MISCTHREAD_H

#include <pthread.h>

typedef void* ( *STARTFUNC )( void * );

enum
{
    DEFAULT_STACKSIZE = 64
};

namespace MISC
{
    // スレッド作成
    int thread_create( pthread_t * thread, STARTFUNC func , void * arg, const int stack_kbyte = DEFAULT_STACKSIZE );
}

#endif
