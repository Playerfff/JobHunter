# TCMalloc

1. What? 什么是TCMalloc？它与标准内存分配器有何不同？

​	TCMalloc（Thread-Caching Malloc）是一种用于动态内存分配的内存分配器，由Google开发。它专为多线程环境设计，旨在提高多线程程序中的内存分配和释放性能。以下是与标准内存分配器的一些主要区别：

1. **线程本地缓存：** TCMalloc使用线程本地缓存，这意味着每个线程都维护自己的一小块内存池。这有助于减少线程间的竞争，因为不同线程可以并发地执行内存分配和释放，而不需要对全局内存池进行争夺。
2. **中心缓存：** TCMalloc还有一个中心缓存，用于处理大块内存的分配。当一个线程需要分配较大的内存块时，它会从中心缓存中获取，而不是使用线程本地缓存。
3. **小对象优化：** TCMalloc对小对象的内存分配进行了优化。它使用了一种称为"size class"的技术，将不同大小的对象分配到不同的内存块中，以减少碎片化。
4. **高性能：** 由于使用了线程本地缓存和其他优化技术，TCMalloc通常在多线程环境中表现出色，能够更有效地处理内存分配和释放，从而提高整体性能。

总体而言，TCMalloc旨在提供更好的多线程性能和更低的内存碎片化，特别适用于大型、高并发的系统。然而，它并不是标准的C或C++库，而是由Google开发和维护的一个独立的库。

---

2. How? TCMalloc是如何管理内存的？

   1. TCMalloc 有三个核心概念: ThreadCache, CentralFreeList (Managed by TransferCacheManager) 和 PageHeap;
   2. **PageHeap**: 维护两个数组: free\_和large\_:
      1. free\_: 
         1. `free_`数组元素的类型是`SpanListPair`
         2. `SpanListPair`里维护了两个`SpanList`, 一个是正常的`SpanList`, 一个另有用途, 不做深入研究.
         3. `free_`索引值为i对应的`SpanList`，该`SpanList`的`Span`都持有i 个Pages.
         4. i最大为`MaxNumber`, MaxNumber的值由`kMaxPages`决定
      2. large\_:
         1. 大于`kMaxPages`个Pages(大对象)的内存分配是从`large_`中分配的
   3. **CentralFreeList**:
      1. 我们可以称之为中央缓存，中央缓存被线程共享，从中央缓存`CentralFreeList`获取缓存需要加锁。
      2. `CentralFreeList`里面有个属性`size_class_`，就是`SizeClass`的值，来自于映射表`SizeMap`这个数组的索引值。`CentralFreeList`里的`Span`会做一件事情，按照这个`size_class_`值对应的规则拆解`Span`为多个`Object`，同时这些`Object`构成`FreeList`。
      3. CentralFreeList在TransferCacheManager的哪个属性下维护?
         1. `TransferCacheManager`这个结构里的`freelist_`属性。
   4. **ThreadCache**: 我们可以称之为线程缓存，`TCMalloc`内存分配器的核心所在。`ThreadCache`被每个线程持有，分配内存时不用加锁，性能好。
      1. ThreadCache中有一个list_成员是array结构的, 其中每个element是FreeList.
      2. array中索引为i的FreeList里面都是8B*i大小的结构.
   5. TCMalloc整个分配内存的流程是什么样的?
      1. 我们把TCMalloc中分配的对象分为两类: 小对象和非小对象, 依据是: 要分配的大小在SizeMap维护的映射表中, 就是小对象; 不在, 就是非小对象。
      2. 当给小对象分配内存时：`ThreadCache`的内存不足时，从对应`SizeClass`的`CentralFreeList`获取，如果获取不到，`CentralFreeList`再从`PageHeap`里获取内存。
      3. 当给非小对象分配内存时：`PageHeap.free_`和`PageHeap.large_`里获取。

3. TCMalloc中的Thread-Caching机制是什么？它如何提高内存分配的效率？

   1. 该机制的基本思想是为每个线程维护一个本地缓存，使得线程可以独立进行内存的分配和释放，减少线程之间的竞争。
   2. 无锁操作；
   3. 本地缓存
   4. 减少碎片

4. 如何使用TCMalloc来分配大块内存？TCMalloc的Large-Object分配器是如何工作的？

   1. TCMalloc直接从PageHeap中分配， 回收也是先放到PageHeap中，达到一定界限了统一回收。

5. TCMalloc是如何避免内存碎片的？

   TCMalloc采取了一些策略来尽量减少内存碎片，从而提高内存的利用率。下面是TCMalloc避免内存碎片的一些主要方法：

   1. **Size Class：** TCMalloc使用"size class"的概念，将不同大小的对象分组到相似大小的内存块中。这有助于减少内存碎片，因为当分配小块内存时，它会从相应的size class中获取，而不是使用过大的内存块。
   2. **Memory Span：** TCMalloc使用"memory span"来管理一系列相邻的内存块，这些内存块通常包含若干个相邻的内存页。通过合并相邻的内存块，TCMalloc可以减少碎片化，提高内存的利用率。
   3. **Thread-Caching：** TCMalloc的线程本地缓存（Thread-Caching）机制允许每个线程独立进行内存的分配和释放，减少了全局内存池的争用，有助于减少碎片。

6. 内存碎片对应用程序有何影响？

   1. **浪费内存：** 内存碎片导致未被充分利用的内存空间，从而浪费了系统资源。

   2. **性能下降：** 内存碎片可能导致内存分配请求无法满足，从而触发更频繁的系统调用，导致性能下降。

   3. **程序中断：** 当系统无法满足分配请求时，程序可能会遇到内存分配失败，导致程序中断或崩溃。

   4. **不稳定性：** 内存碎片可能导致系统内存不稳定，增加了应用程序出现未预期问题的可能性。

7. TCMalloc如何处理多线程的并发访问？

   1. Thread-Caching 专属于this_thread, 可达到无锁访问;
   2. Central FreeLists和PageHeap加锁访问;

8. 什么是TCMalloc的锁机制？它如何实现锁的高效性？

   1. TCMalloc中使用了自旋锁（SpinLock）来进行一些必要的同步操作。自旋锁是一种无阻塞锁，线程在尝试获取锁时会一直自旋，不会主动阻塞。这有助于避免线程的上下文切换，提高锁的获取速度。

9. TCMalloc与jemalloc、malloc等其他内存分配器相比，有哪些优势和劣势？

   1. TCMalloc: 优势: 多线程性能, 低碎片化, Google生态; 劣势: 在一些特定场景下可能不如其他分配器的适应性强;
   2. jemalloc: 优势: 内存分配效率, 可拓展性, 调优选项丰富; 劣势: 碎片化;
   3. malloc: 优势: 标准接口, 稳定性; 劣势: 多线程性能, 碎片化。