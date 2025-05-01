#include <iostream>
#include <vector>

template <typename T>
class CircularBuffer
{
public:
    explicit CircularBuffer(size_t capacity)
    {
        if (capacity <= 0) {
            throw std::invalid_argument("Capacity must be greater than 0");
        }
        buffer_.resize(capacity);
        capacity_ = capacity;
        head_ = tail_ = size_ = 0;
    }

    ~CircularBuffer() = default;

    [[nodiscard]] bool is_empty() const
    {
        return size_ == 0;
    }

    [[nodiscard]] bool is_full() const
    {
        return size_ == capacity_;
    }

    [[nodiscard]] size_t size() const
    {
        return size_;
    }

    void push_back(T item)
    {
        if (is_full())
        {
            head_ = (head_ + 1) % capacity_;
        }
        else
        {
            size_++;
        }
        buffer_[tail_] = item;
        tail_ = (tail_ + 1) % capacity_;
    }

    void push_front(T item)
    {
        if (is_full())
        {
            tail_ = (tail_ - 1 + capacity_) % capacity_;
        }
        else
        {
            size_++;
        }
        head_ = (head_ - 1 + capacity_) % capacity_;
        buffer_[head_] = item;
    }

    T pop_back()
    {
        if (is_empty())
        {
            throw std::runtime_error("Circular queue is empty");
        }
        tail_ = (tail_ - 1 + capacity_) % capacity_;
        size_--;
        return buffer_[tail_];
    }

    T pop_front()
    {
        if (is_empty())
        {
            throw std::runtime_error("Circular queue is empty");
        }
        T t{buffer_[head_]};
        head_ = (head_ + 1) % capacity_;
        size_--;
        return t;
    }

    void clear()
    {
        size_ = 0;
        head_ = 0;
        tail_ = 0;
    }

    template<typename Callback>
    void iterate(Callback cb)
    {
        for (size_t i = head_, num = 0; num < size_; i = (i + 1) % capacity_, num++)
        {
            cb(buffer_[i]);
        }
    }

private:
    std::vector<T> buffer_;
    size_t capacity_;
    size_t head_;
    size_t tail_;
    size_t size_;
};

int main()
{
    CircularBuffer<int> cb(3);
    cb.push_back(10);
    cb.push_back(20);
    cb.push_back(30);
    std::cout << "cb.size(): " << cb.size() << std::endl;
    cb.iterate([](int item){std::cout << item << std::endl;});

    cb.push_back(40);
    cb.push_front(50);
    std::cout << "cb.size(): " << cb.size() << std::endl;
    cb.iterate([](int item){std::cout << item << std::endl;});

    cb.pop_back();
    cb.pop_front();
    std::cout << "cb.size(): " << cb.size() << std::endl;
    cb.iterate([](int item){std::cout << item << std::endl;});
}
