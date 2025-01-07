#pragma once
#include <algorithm> // for std::copy
#include <stdexcept> // for std::out_of_range

template <class T>
class vector {
public:
    // 数据
    T* data;
    // 大小
    int Size;
    // 容量
    int capacity;

    // 构造函数
    vector() : Size(0), capacity(1), data(new T[1]) {}

    // 析构函数
    ~vector() {
        delete[] data;
    }
	//含参构造函数(初始化大小,元素)
	vector(int n, T t) : Size(n), capacity(n), data(new T[n]) {
		for (int i = 0; i < n; i++) {
			data[i] = t;
		}
	}
    // 拷贝构造函数
    vector(const vector& v) : Size(v.Size), capacity(v.capacity), data(new T[v.capacity]) {
        std::copy(v.data, v.data + v.Size, data);
    }

    // 赋值运算符
    vector& operator=(const vector& v) {
        if (this != &v) {
            T* new_data = new T[v.capacity];
            std::copy(v.data, v.data + v.Size, new_data);
            delete[] data;
            data = new_data;
            Size = v.Size;
            capacity = v.capacity;
        }
        return *this;
    }

    // 添加元素
    void push_back(T t) {
        if (Size == capacity) {
            resize(capacity * 2);
        }
        data[Size++] = t; // 添加元素
    }

    // 删除元素
	void pop_back() // 删除最后一个元素
    {
        if (Size > 0) Size--;
    }

    // 返回大小
    int size() const {
        return Size;
    }

    // 返回元素（非 const 版本）
    T& operator[](int index) {
        if (index < 0 || index >= Size) {
			throw std::out_of_range("Index out of range");//抛出异常
        }
        return data[index];
    }

    // 返回元素（const 版本）
    const T& operator[](int index) const
    {
        if (index < 0 || index >= Size) {
			throw std::out_of_range("Index out of range");//抛出异常
        }
        return data[index];
    }
    //判断是否为空
	bool empty() const
	{
		return Size == 0;
	}
	void clear() {
		Size = 0;
	}
    // 迭代器类
    class iterator {
    public:
        T* ptr;

        iterator(T* p) : ptr(p) {}

        T& operator*() const { return *ptr; }
        T* operator->() const { return ptr; }

        iterator& operator++() { ++ptr; return *this; }
        iterator operator++(int) { iterator tmp = *this; ++ptr; return tmp; }

        iterator& operator--() { --ptr; return *this; }
        iterator operator--(int) { iterator tmp = *this; --ptr; return tmp; }

        bool operator==(const iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const iterator& other) const { return ptr != other.ptr; }
    };

    // 常量迭代器类
    class const_iterator {
    public:
        const T* ptr;

        const_iterator(const T* p) : ptr(p) {}

        const T& operator*() const { return *ptr; }
        const T* operator->() const { return ptr; }

        const_iterator& operator++() { ++ptr; return *this; }
        const_iterator operator++(int) { const_iterator tmp = *this; ++ptr; return tmp; }

        const_iterator& operator--() { --ptr; return *this; }
        const_iterator operator--(int) { const_iterator tmp = *this; --ptr; return tmp; }

        bool operator==(const const_iterator& other) const { return ptr == other.ptr; }
        bool operator!=(const const_iterator& other) const { return ptr != other.ptr; }
    };

    // 返回迭代器的开始
    iterator begin() { return iterator(data); }

    // 返回迭代器的结束
    iterator end() { return iterator(data + Size); }

    // 返回常量迭代器的开始
    const_iterator begin() const { return const_iterator(data); }

    // 返回常量迭代器的结束
    const_iterator end() const { return const_iterator(data + Size); }

    // 调整容量
    void resize(int new_capacity) {
		T* new_data = new T[new_capacity];//新建一个数组
		std::copy(data, data + Size, new_data);//复制数据
		delete[] data;//删除原来的数组
        data = new_data;
        capacity = new_capacity;
    }
};

