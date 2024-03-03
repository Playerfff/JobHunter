# week16

## 进度

### 2.25-2.26

1. 完成<一致性协议.md>;
2. 了解paxos, cab, gossip协议;
3. 复习raft协议.

### 2.27

qt学习:

1. QDragEvent机制
2. qt事件机制, eventfilter等;(eventfilter->子类event->子类具体event->父类event)
3. qt鼠标事件, 按键事件和定时器事件.

### 2.28 - 3.3

1. 解读raft源码, 了解raft机制.

# Raft源码阅读

1. 基本模块作用:
    1. config: Config结构体主要用于配置参数的传递，在创建raft实例时需要的参数会通过Config实例传递进去。
    2. entry:  每条消息中可以携带多条Entry记录，每条Entry记录对应一个独立的操作。记录的本地Log的基本单位也是Entry记录。在etcd 中还有一个WAL日志的概念，这两者并非完全等价，所以需要注意一下，避免两者混淆。
    3. message: 在raft模块的实现中，Message是所有消息的抽象，包括了各种类型消息所需要的字段.
    4. inflights: 主要功能是记录当前节点己发出但未收到响应的MsgApp消息
    4. raft: 在etcd-raft模块中，raft结构体是其核心数据结构，在结构体raft中封装了当前节点所有的核心数据。结构体raft实现了Raft协议中最核心的内容， 它也是整个rft模块的核心.
    5. storage: MemoryStorage 是raft模块为Storage 接口提供的一个实现，从名字也能看出，MemoryStorage在内存中维护上述状态信息（hardState字段）、快照数据（snapshot宇段）及所有的Entry记录（ents 字段，［］raftpb.Entry类型〕，在MemoryStorage.ents字段中维护了快照数据之后的所有Entry记录。
    6. unstable: unstable 使用内存数组维护其中所有的Entry记录，对于Leader节点而言，它维护了客户端请求对应的Entry记录；对于Follower节点而言，它维护的是从Leader节点复制来的Entry记录。无论是Leader节点还是Follower节点，对于刚刚接收到的Entry记录首先都会被存储在unstable中。然后按照Raft协议将unstable中缓存的这些Entry记录交给上层模块进行处理，上层模块会将这些Entry记录发送到集群其他节点或进行保存（写入Storage中）。之后，上层模块会调用Advance（）方法通知底层的raft模块将unstable 中对应的Entry记录删除（因为己经保存到了Storage中）。正因为unstable中保存的Entry记录并未进行持久化，可能会因节点故障而意外丢失，所以被称为unstable。
    7. raft_log: Raft 协议中日志复制部分的核心就是在集群中各个节点之间完成日志的复制，因此在raft模块的实现中使用raftLog结构来管理节点上的日志， 它依赖于前面介绍的Storage接口和unstable结构体。 
    8. raft_node: 结构体raft实现了Raft协议中最核心的内容， 它也是整个rft模块的核心， 但是它并没有实现网络传输、 持久化存储（注意与存储Entry记录的raftLog进行区分〉等功能，也没有对外提供简单易用的APi.在raft模块中，结构体Node表示集群中的一个节点，它是在结构体raft之上的一层封装，对外提供相对简单的API接口。

2. raft选举机制:
    1. 当节点的raft实例创建完成之后，当前节点处于Follower状态。
    2. 集群启动一段时间之后，会有一个Follower节点的选举计时器超时， 此时就会创建MsgHup消息（其中Term为0）并调用raft.Step（）方法。首先它会通过tickElection()创建MsgHup消息并将其交给raft.Step（）方法进行处理； raft.Step（）方法会将当前Follower节点切换成PreCandidate状态， 然后创建MsgPreVote类型的消息， 最后将该消息追加到raft.msgs字段中， 等待上层模块将其发迭出去。
    3. 当集群中其他节点（此时集群中其他节点都处于Follower状态〉收到MsgPreVote（其Term字段值为1 ）消息之后， 经过网络层处理及相关验证之后， 最终也会调用raft.Step（）方法进行处理。raft.Step（）方法首先检测该MsgPreVote消息是否为Leader节点迁移时发出的消息及其他合法性检测，决定当前节点是否参与此次选举；之后当前节点会根据自身的状态决定是否将其选票投给MsgPreVote消息的发送节点。
    4. 当PreCandidate状态节点收到半数以上的投票时，会通过rcampaign（）方法发起正式选举，其中会通过raft.becomeCandidate（）方法将当前节点切换成Candidate状态，井向剩余其他节点发送MsgVot巳消息.
3. 日志复制:
    1. raft.handleAppendEntries（）方法首先会检测MsgApp消息中携带的Entry记录是否合法，然后将这些Entry记录追加到raftLog中，最后创建相应的MsgAppResp消息。
    2. 再回到Leader节点，当其收到集群中其他Follower节点发迭的MsgAppResp响应消息之后，也是交由raft.Step（）方法进行处理的，其中会调用raft.step 字段指向的StepLeader() 函数进行处理的。当Leader节点收到MsgAppResp消息时，会通过inflights.freeTo()方法将指定消息及其之前的消息全部清空，释放inflights 空间，让后面的消息继续发送。

启动: main -> kv::RaftNode::main -> kv::RaftNode::ctor -> 配置网络库, WAL, snap