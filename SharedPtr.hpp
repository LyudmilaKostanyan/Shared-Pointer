#pragma once
#include <iostream>
#include <atomic>
#include <utility>

struct ControlBlock {
    std::atomic<std::size_t> strong_count{1};
};

template<typename T>
class SharedPtr {
private:
    T*              ptr;
    ControlBlock*   ctrl;

    void release() {
        if (ctrl && ctrl->strong_count.fetch_sub(1) == 1) {
            delete ptr;
            delete ctrl;
        }
    }

public:
    SharedPtr() : ptr(nullptr), ctrl(nullptr) {}

    explicit SharedPtr(T* p) : ptr(p), ctrl(p ? new ControlBlock{} : nullptr) {}

    SharedPtr(SharedPtr const& other) : ptr(other.ptr), ctrl(other.ctrl)
    {
        if (ctrl)
            ctrl->strong_count.fetch_add(1, std::memory_order_relaxed);
    }

    SharedPtr& operator=(SharedPtr const& other) {
        if (this != &other) {
            release();
            ptr  = other.ptr;
            ctrl = other.ctrl;
            if (ctrl)
                ctrl->strong_count.fetch_add(1, std::memory_order_relaxed);
        }
        return *this;
    }

    ~SharedPtr() {
        release();
    }

    void reset(T* p = nullptr) {
        release();
        if (p) {
            ptr  = p;
            ctrl = new ControlBlock{};
        } else {
            ptr  = nullptr;
            ctrl = nullptr;
        }
    }

    void swap(SharedPtr& other) {
        std::swap(ptr,  other.ptr);
        std::swap(ctrl, other.ctrl);
    }

    T* get() const { return ptr; }

    T&   operator*()  const { return *ptr; }
    T*   operator->() const { return ptr; }
    
    T&   operator[](std::size_t i)       { return ptr[i]; }
    const T& operator[](std::size_t i) const { return ptr[i]; }

    std::size_t use_count() const {
        return ctrl ? ctrl->strong_count.load(std::memory_order_relaxed) : 0;
    }

    bool unique() const {
        return use_count() == 1;
    }
};