1. 关于"std::shared_timed_mutex"
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
