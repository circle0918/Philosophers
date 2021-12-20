int pthread_create(pthread_t *tidp,const pthread_attr_t *attr, void *(*start_rtn)(void*),void *arg); 
: pthread_create是类Unix操作系统（Unix、Linux、Mac OS X等）的创建线程的函数。它的功能是创建线程（实际上就是确定调用该线程函数的入口点），在线程创建以后，就开始运行相关的线程函数。
                 pthread_create的返回值:若成功，返回0；若出错，返回出错编号。第一个参数为指向线程标识符的指针。第二个参数用来设置线程属性。第三个参数是线程运行函数的起始地址。最后一个参数是运行函数的参数。
pthread_detach : 用pthread join但是如线程没完成，一旦调用会被阻塞，所以可在子线程加入detach。或者父线程调用pthread_detach(thread_id)（非阻塞，可立即返回） 　　
这将该子线程的状态设置为detached,则该线程运行结束后会自动释放所有资源。
pthread_join()函数，以阻塞的方式等待thread指定的线程结束。当函数返回时，被等待线程的资源被收回。如果线程已经结束，那么该函数会立即返回。并且thread指定的线程必须是joinable的

https://docs.oracle.com/cd/E19253-01/819-7051/sync-12/index.html
pthread_mutex_init lock
pthread_mutex_destroy 
pthread_mutex_lock,
pthread_mutex_unlock

check data race :
valgrind --tool=helgrind
gcc -Wall -Wextra -Werror -I. -fsanitize=thread -g
