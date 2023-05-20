## 线程

### 线程对于信号的处理

每个线程可以使用`int pthread_sigmask(int how,const sigset_t *restrict set,sigset_t *restrict oset);`设置自己的信号屏蔽字。设置完后相应的信号不会导致线程进入信号处理程序。

线程可以调用`int sigwait(const sigset_t *restrict set,int *restrict signop);`等待set集合中的信号，在调用该函数前应该阻塞对应的信号。

虽然线程有独立的信号屏蔽字，但是所有线程共享信号处理函数。进程会将信号发送到每一个线程，除了与硬件故障有关的它会直接发送到引发这个事件的线程中去。

kill用于发送信号到进程，`int pthread_kill(pthread_t thread,int signo);`用于发送信号到特定的线程。

### 有关线程属性，同步原语，同步原语属性的init和destroy的使用

对于静态分配的同步原语，一般都是全局性的，生命期包含整个程序运行期间，所以不会destroy。而对于动态分配的同步原语，一般其生命期都不包含整个程序生命期，需要在一定时刻释放，所以需要调用destroy。而对于线程属性和同步原语属性，它们在初始化后就不在有用，所以即使是静态声明的，也需要调用destroy释放空间。

### 线程与fork

多线程的进程，在调用fork的时候，fork只复制当前调用fork函数的线程，如果其他线程中占有锁将对程序状态造成影响。所以需要进行处理。

`int pthread_atfork(void ( *preapare)(void),void ( *parent)(void),void ( *child)(void));`

prepare在创建子进程前获取所有的锁，然后创建子进程。此时子进程和父进程都分别占有所有的锁，父进程调用parent函数解锁，子进程调用啊child函数解锁。

### pthread_create

头文件：pthread.h

创建线程

`int pthread_create(pthread_t *tid,const pthread_attr_t *attr,void *(*func)(void *),void *arg);`

**tid**:用来返回线程ID

**attr**:设置线程属性，可以设置线程是detach的还是join的，控制线程栈的大小/位置等与线程栈相关的属性。线程属性是一种类型的变量，有函数对其进行处理。使用默认属性则传入NULL。

**func**:线程执行的函数。其形参对应着arg，返回值将在pthread_join函数中返回。

**arg**:对应func函数的形参

#### 返回值

成功：返回0

失败：返回正的错误值

### pthread_setconcurrency

告知系统我们希望并发运行多少线程

`int pthread_setconcurrency(int new_level);`

**new_level**:我们在当前进程中使用的线程个数，编程者一般是可以确定这个数值的。

#### 说明

在linux下，如果你忽略了这个函数的使用，那么能够并发的线程数目由实现者来控制，对于系统调度的效率而言往往不是什么好的事情，因为默认的设置往往不是最佳的。
     更为糟糕的是，如果在某些系统中，如果你不调用pthread_setconcurrency()函数，那么系统中的运行的线程仅仅是第一个被创建的线程，其他线程根本不会被运行。比如在solaris 2。6中就有这些情况。为了在unix或者是linux系统上使移植更加的容易，请不要忘记在适当的地方调用次函数，清晰的告诉系统我们使用的线程个数。虽然在某些系统上，这个调用是徒劳的，但是它的使用增强的移植性！

### pthread_join

等待给定线程终止（阻塞）

`int pthread_join(pthreat_t tid,void **status);`

**tid**:等待线程的ID

**status**:用于获取指定线程函数的返回值,若不需要置NULL

#### 返回值

成功:返回0
失败:返回正的错误值

### pthread_cancel

等待给定线程终止（阻塞）

`int pthread_cancel(pthreat_t tid);`

**tid**:要取消的线程的ID，取消是指不等待线程终止，向线程发送要求进程终止请求，不过线程可以选择忽略或者控制如何被取消。

#### 返回值

成功:返回0
失败:返回正的错误值

### 与线程取消相关的其他函数

设置线程是否可被取消`int pthread_setcancelstate(int state,int *oldstate);`函数为原子操作

设置线程取消检测点：`void pthread_testcancel(void);`线程取消，需要在特定的取消检测点进行，处理posix规定的，还可以自己设置。

设置取消的类型：`int pthread_setcanceltype(int type,int *oldtype);`上面说的是“推迟取消”，还有“异步取消”不必等到取消点。


### pthread_cleanup_push

设置线程退出时，其调用的函数。线程退出时调用的函数称为线程清理处理程序。

`void pthread_cleanup_push(void (*rtn)(void *),void *arg)`

**rtn**：为线程退出时，想要调用的函数

**arg**：为传递给函数rtn的参数

#### 说明

其中设置的函数，只在线程1、自身调用pthread_exit()退出时。2、其他线程调用pthread_cancel取消线程时。3、如果上面两个没有使用，则在pthread_clean_up_pop的参数非零的时候，会调用清理处理程序。

调用线程清理处理程序的顺序，和该函数设置的顺序相反。

### pthread_cleanup_pop

`void pthread_cleanup_pop(int execute)`

**execute**:当线程运行该函数时，如果execute不为零，则运行线程清理处理程序，如果为零，则不运行，但是同样会删除push建立的线程清理处理程序。

### pthread_self

获取自己的线程ID

`pthread_t pthread_self(void);`

#### 返回值

自己的ID

### pthread_detach

将线程转变为脱离状态

`int pthread_detach(pthread_t tid);`

**tid**:需要设置为脱离状态的线程。

#### 返回值

成功：返回0

失败：返回正的错误值

####  常见操作

线程将自己设置为脱离的：pthread_detach(pthread_self());

### pthread_exit

终止线程

`void pthread_exit(void *status);`

**status**:返回线程终止状态，不能是线程函数的局部变量，否则会随着线程一起消失。

线程也可以通过其他方式终止。1、启动线程的函数返回。其返回值就是线程的终止状态。2、进程main函数返回，或者任何线程调用了exit，整个进程将终止，此时该进程的所有线程也随之终止。


### 线程特定数据函数

### pthread_once

用于控制在多线程程序中，其指定的函数只在第一个调用的线程中运行。其指定的函数不单独调用，只和他列在一起就可以。

`int pthread_once(pthread_once_t *onceptr,void (\*init) (void));`

**onceptr**:once = PTHREAD_ONCE_INIT;onceptr = &once;其中的宏是一个固定值。once一般定义为一个全局变量。

**init**:运行一次的函数指针，如果函数需要参数，可以用一个不需要的参数封装。对于线程特定数据应用运行一次的函数为pthread_key_create().这个函数就需要封装。

### pthread_key_create

用于获取进程，线程中维护特定数据数组 的下标。

`int pthread_key_create(pthread_key_t *keyptr,void (\*destructor)(void *value));`

**keyptr**:用于返回数组下标。一般其对应的变量定义为全局变量，该变量应该被所有线程共享，且所有线程只读该变量，所以不存在线程不安全的问题。

**destructor**:析构函数。用来释放数组在键值处，存储的数据地址指向的数据。

#### 返回值

以上两个函数返回值相同

成功：返回0

失败：返回正的错误值

### pthread_key_delete

取消键与线程特定数据值之间的关联关系。但是不会激活与键关联的析构函数。

`int  pthread_key_delete(pthread_key_t key);`

### 返回值

同上

### pthread_getspecific

获取线程自身维护数组中存储的指针值，传入参数为pthread_key_create()返回的下标

`void *pthread_getspecific(pthread_key_t key);`

#### 返回值

见函数功能说明

### pthread_setspecific

设置线程自身维护数组中的指针值

`int pthread_setspecific(pthread_key_t key,const void *value);`

**key**:下标

**value**:设置的数组元素值

#### 返回值

成功：0

失败：正的错误值

#### 说明

value一般指向一段动态分配的内存区

### 互斥锁

互斥锁变量应该在主线程中定义和初始化。互斥锁在一定程度上会增加系统开销，但是并不大。

判断是否使用互斥锁的一种方法就是线程是否需要访问同一个全局变量或静态变量。

加锁的位置也是根据访问全局变量或者静态变量的情况.

如果多个线程阻塞在同一个互斥锁上，解锁时，将优先唤醒优先级高的线程（优先级有调度方法确定）

使用多个互斥锁相互嵌套时，防止死锁发生。1、将资源访问排序，即对互斥锁访问排序。2、在上锁前，使用pthread_mutex_trylock测试，如果不能获得锁，就先释放所有已获得锁。如果能够获得则继续运行。

### pthread_mutex_init

用来初始化互斥量

`int pthread_mutex_init(pthread_mutex_t *restrict mutex,const pthread_mutexattr_t *restrict attr);`

**mutex**:需要初始化的互斥量的指针，可以是静态声明的变量取地址得到的（静态声明的也可以直接使用PTHREAD_MUTEX_INITIALIZER常量赋值初始化），也可以是使用malloc动态分配的。对于动态分配的互斥量，在free之前，需要调用pthread_mutex_destroy。对于静态则不需要调用destroy函数。静态分配是指不用malloc分配的。

**attr**:互斥量的属性，如果设置为NULL，则使用默认的进行初始化。

#### 返回值

成功：0

失败：正的错误值

### pthread_mutex_destroy

用来destroy互斥量（析构）

`int pthread_mutex_destroy(pthread_mutex_t *mutex);`

**mutex**:需要destroy的互斥量的指针

#### 返回值

成功：0

失败：正的错误值

### pthread_mutex_lock

用来加锁，在线程解锁前，其间的代码和数据不能被另一个线程访问。

`int pthread_mutex_lock(pthread_mutex_t *mptr);`

**mptr**:互斥锁变量的指针。互斥锁变量应该使用PTHREAD_MUTEX_INITIALIZE这个宏变量进行初始化。

#### 返回值

成功：0

失败：正的错误值

### pthread_mutex_unlock

`int pthread_mutex_unlock(pthread_mutex_t *mptr);`

用于解锁，其余同上。

### pthread_mutex_trylock

当互斥锁锁住的时候，不阻塞，直接返回EBUSY错误。没锁住的时候则上锁，返回成功。

`int pthread_mutex_trylock(pthread_mutex_t *mutex);`

#### 返回值

成功：0

失败：正的错误值

### 互斥量属性

互斥量的属性使用pthread_mutexattr_t结构进行设置。

`int pthread_mutexattr_init(pthread_mutexattr_t *attr);`使用默认初始化

`int pthread_mutexattr_destroy(pthread_mutexattr_t *attr);`反初始化

获取或设置attr中的有关share的值，可以为PTHREAD_PROCESS_PRIVATE（默认，线程间），PTHREAD_PROCESS_SHARED（表示进程间共享）

`int pthread_mutexattr_getpshared (const pthread_mutexattr_t *restrict attr,int *restrict pshared);`

`int pthread_mutexattr_setpshared (pthread_mutexattr_t *attr,int pshared);`

设置健壮性：当互斥量定义为shared时，其在进程间使用，当一个持有锁的进程终止时，由于他的锁没有释放，会影响其他进程/线程工作，如果设置属性为健壮，这种情况下会有通知，即在pthread_mutex_lock函数调用时，返回EOWNERDEAD。对于返回EOWNERDEAD的线程，如果直接解锁互斥量会导致该互斥量不再可用，如果想继续使用该互斥量，需要在解锁之前调用pthread_mutex_consistent函数，然后再解锁这样，互斥量就可以继续使用了。

`int pthread_mutexattr_getrobust(const pthread_mutexattr_t *restrict attr,int *restrict robust);`

`int pthread_mutexattr_setrobust(const pthread_mutexattr_t *restrict attr,int robust);`

`int pthread_mutex_consistent(pthread_mutex_t *mutex);`

定义类型：决定在进行，对没有解锁的互斥量加锁，对其他线程上锁的进行解锁，对已经解锁的进行解锁，三种操作时，会产生什么结果。四种类型。

`int pthread_mutexattr_gettype(const pthread_mutexattr_t *restrict attr,int *restrict type);`

`int pthread_mutexattr_settype(const pthread_mutexattr_t *restrict attr,int type);`

### 条件变量

条件变量也定义为一个全局变量，需要和互斥锁搭配使用。对于静态分配的不用调用destroy析构，对于动态分配的需要在free之前调用 destroy。但是对于静态分配的可以使用init进行初始化。

pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;	//这里是静态初始化，还可以使用malloc分配，然后调用函数初始化

pthread_cond_t c = PTHREAD_COND_INITIALIZER;	//这里是静态初始化，还可以使用malloc分配，然后调用函数初始化

### pthread_cond_init/destroy

说明参见mutex和读写锁。

属性有进程共享属性和时钟属性，时钟属性用来指定timedwait函数的计时时钟。

### pthread_cond_wait

等待条件的发生。在获取互斥锁之后调用，该函数会先解锁然后阻塞等待条件发生（原子操作），解锁的目的是让其他线程访问互斥锁保护的变量。当条件发生时，退出阻塞并重新上锁。

如果在调用该函数前发生条件，则条件丢失。

`int pthread_cond_wait(pthread_cond_t *cptr,pthread_mutex_t *mptr);`

返回值如前。

### pthread_cond_timedwait

等待条件发生，如果超过abstime指定的绝对时间（自1970.01.01到现在）则退出。

`int pthread_cond_timedwait(pthread_cond_t *cptr,pthread_mutex_t *mptr,const struct timespec *abstime);`

返回值如前

### pthread_cond_signal

解阻塞等待条件的线程中的一个，

`int pthread_cond_signal(pthread_cond_t *cptr);`

返回值如前。

### pthread_cond_broadcast

解阻塞所有等待该条件的线程。

`int pthread_cond_broadcast(pthread_cond_t *cptr);`

#### 说明

这种情况发生在读出者和写入者时，当写入者完成写入后，通知所有的读出者可读。

### 读写锁

也叫共享互斥锁，共享指的是读模式，互斥指的是写模式。

存在读加锁，写加锁，不加锁三种状态。读加锁状态下，不阻塞其他读加锁，但会阻塞写加锁请求，并同时不允许新的读请求，当现有读加锁处理完后，进入写加锁状态。写加锁状态下，会阻塞所有读/写加锁请求。

对于静态分配的不用调用destroy析构，对于动态分配的需要在free之前调用 destroy。但是对于静态分配的可以使用init进行初始化。

### pthread_rwlock_init

用来初始化读写锁，读写锁使用之前必须进行初始化。对于静态分配的读写锁，如果默认属性足够的话，可以使用PTHREAD_RWLOCK_INITTIALIZER进行初始化。

读写锁的属性只要进程共享属性，设置类似mutex。

`int pthread_rwlock_init(pthread_rwlock_t *restrict rwlock,const pthread_rwlockattr_t *restrict attr);`

**rwlock**:需要初始化的读写锁的指针，可以是静态声明的变量取地址得到的，也可以是使用malloc动态分配的。需要调用pthread_mutex_destroy。

**attr**:读写锁的属性，如果设置为NULL，则使用默认的进行初始化。

#### 返回值

成功：0

失败：正的错误值

### pthread_rwlock_destroy

用来destroy读写锁（析构）

`int pthread_rwlock_destroy(pthread_rwlock_t *rwlock);`

**rwlock**:需要destroy的读写锁的指针

#### 返回值

成功：0

失败：正的错误值

### pthread_rwlockattr_init/destroy

用来使用默认属性初始化属性变量/析构一个属性变量

`int pthread_rwlockattr_init(pthread_rwlockattr_t *attr);`

`int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr);`

#### 返回值
同上

### pthread_rwlockattr_get/setpshared

设置属性的值，当前只有PTHREAD_PROCESS_PRIVATE/SHARED

`int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *attr,int *valptr);`

`int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr,int value);`

#### 返回值
同上

### pthread_rwlock_rdlock/wrlock/unlock

用来加读锁，加写锁，解锁（包括读锁和写锁）

`int pthread_rwlock_rdlock/wrlock/unlock(pthread_rwlock_t *rwlock);`

**rwlock**:读写锁变量的指针。

#### 返回值

成功：0

失败：正的错误值

#### 说明

`int pthread_rwlock_rdlock/wrlock(pthread_rwlock_t *rwlock);`

功能类似于pthread_mutex_trylock函数。

`int pthread_rwlock_timedrdlock/wrlock(pthread_rwlock_t *restrict rwlock,const struct timespec *restrict tsptr);`

带有超时的读写锁，指定的是绝对的时间。

上述两个函数的返回值，和上面其他函数的返回值一样。

### 自旋锁

和互斥锁类似，使用方法和函数接口也基本相同，只要将mutex更换为spin即可。

二者的区别在于，在等待锁时mutex会进入睡眠状态，不占用CPU；而spin会进入一个循环等待，还会占用CPU，所以只适合等待锁时间不长的应用中。

### 屏障

等待指定数量的线程到达某一点，然后会对其中的一个线程返回PTHREAD_BARRIER_SERIAL_THREAD（其他返回0），该线程可用于集中结果处理。也可以不用返回PTHREAD_BARRIER_SERIAL_THREAD的线程做结果处理，可以使用主线程做处理。

属性：只有进程共享属性。

### pthread_barrier_init

`int pthread_barrier_init(pthread_barrier_t *restrict barrier,const pthread_barrierattr_t *restrict attr,unsigned int cout);`

**count**:指定允许所有线程继续运行之前，必须到达屏障的线程数目。

### pthread_barrier_destroy

###  pthread_barrier_wait

用来增加屏障技术，如果增加后没有达到要求，那么进入休眠状态。

`int pthread_barrier_wait(pthread_barrier_t *barrier);`

#### 返回值

成功：0或PTHREAD_BARRIER_SERIAL_THREAD

失败：正的错误值

#### 说明

到达屏障计数值后，并且线程处于非阻塞状态，则屏障可以被重用。除非destroy并进行新的初始化，否则屏障计数不会改变。

