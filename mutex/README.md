






1. 关于std::unique_lock构造函数

(6)
template <class Rep, class Period> unique_lock(mutex_type& m, const chrono::duration<Rep, Period>& rel_time);

解释：unique_lock管理着mutex m，通过调用m.try_lock_for(rel_time)，在rel_time时间内尝试锁住mutex m。

关于std::timed_mutex::try_lock_for的prototype，

template <class Rep, class Period> bool try_lock_for(const chrono::duration<Rep, Period>& rel_time);

unique_lock尝试去锁住timed_mutex，最多被block rel_time时间，就是说过了 rel_time，calling thread无论无何都要锁住 timed_mutex。调用try_lock_for时候，有以下3种情况可能会发生：

 - 1. 如果timed_mutex当前没有被任何线程锁住，calling_thread就会立即锁住 timed_mutex

 - 2. 如果timed_mutex当前被另外一个线程占用了，要么2a. 另外一个线程通过调用unlock()释放了timed_mutex，要么2b. rel_time时间到了，2a或者2b谁先发生都会让calling thread获得timed_mutex

 - 3. 如果timed_mutex已经被calling_thread占用了，那么当calling thread再去尝试获得一个已经被自己占用的timed_mutex，此时会造成死锁。










2. 关于"std::shared_timed_mutex"
    a. 在C++ 14里面才开始引入

与其他更容易做互斥访问的锁相比，"shared_timed_mutex"有两种访问方式：
    A. *shared* - 好几个线程可以共同拥有对同一个锁的所有权
    B. *exclusive* - 只有一个线程能拥有这个锁

shared_timed_mutex适用的场景如下：
    有多个读者(reader)要同时访问同一个资源(*shared*)但是只有一个写着(writer)能够访问这个资源(*exculsive*)。

举个例子，

假如有个变量需要进行保护，
CircleBuffer<Radar77Data> data_buffer_;

于是，我就定义了一个std::shared_timed_mutex mt_;

当我要对*data_buffer_*做读操作时，就使用*shared_lock*,
    std::shared_lock<std::shared_timed_mutex> lg(mt_);
当我要对*data_buffer_*做写操作时，就使用*unique_lock*,
    std::unique_lock<std::shared_timed_mutex> lg(mt_);
