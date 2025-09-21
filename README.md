# C++ Empty Class and Function

## Introduction

More than 10 years ago, when I first debugged my [Boost](https://www.boost.org/) code, I stepped into empty class and empty function. At that time, I was wondering what empty class and function devoid of any code is good for. In this tip, I am going to use a simple endian swap example to demonstrate the use of empty class and function in templates.

First, we define two&nbsp;classes, `Same` and `Different` to indicate same or different endianness. Why can&#39;t we use `true` or `false`? You ask. `true` and `false` are of the same boolean type. As a template type, they are the same, as far as the C++ compiler is concerned.

```Cpp
class Same
{};

class Different
{};
```

What is the size of an empty class?

```Cpp
std::cout << "sizeof(Same) is " << sizeof(Same) << std::endl;
std::cout << "sizeof(Different) is " << sizeof(Different) << std::endl;
```

Visual C++, G++ and Clang all give the size of one. Why size of one? Because C++ memory allocator has trouble instantiating zero-sized object.

```
sizeof(Same) is 1
sizeof(Different) is 1
```

Next, we have our dummy `PacketWriter` which takes in one template type, `SameEndianType` and has one `Write` function that takes a `short` integer and swap it if the `SameEndianType` is `Different`. We have 2 `Swap` functions where there is nothing to be done for same endianness. C++ compiler will optimize away the call to empty `Swap()` in release build. There is no need to give a name to the second parameter of `Swap()` because it is not going to be used inside `Swap()` anyway.

```Cpp
// Dummy PacketWriter
template<typename SameEndianType>
class PacketWriter
{
    typedef SameEndianType endian_type;
public:
    void Write(short n)
    {
        std::cout << "Before swapping, n=" << n << std::endl;
        Swap(n, endian_type());
        std::cout << "After swapping, n=" << n << std::endl;
    }
private:
    void Swap(short& n, Same)
    {
    }
    void Swap(short& n, Different)
    {
        short upper = (n & 0xff) << 8;
        short lower = (n & 0xff00) >> 8;
        n = upper | lower;
    }
};
```

The code below tests two `PacketWriter` objects of `Same` and `Different` endian.

```Cpp
std::cout << "Create PacketWriter<Same> to write a short int" << std::endl;
PacketWriter<Same> w;
w.Write(256);

std::cout << "Create PacketWriter<Different> to write a short int" << std::endl;
PacketWriter<Different> w2;
w2.Write(256);
```

The output of the test program is as expected: `PacketWriter<Same>` do not swap the `short` integer whereas `PacketWriter<Different>` do actually swap it. The upside of this templated endianness is endianness check before swap is eliminated at runtime but the downside is that you cannot change the behaviour dynamically during runtime.

```
Create PacketWriter<Same> to write a short int
Before swapping, n=256
After swapping, n=256

Create PacketWriter<Different> to write a short int
Before swapping, n=256
After swapping, n=1
```

That&#39;s all for today! Hope you like this tip!

## History

* 4<sup>th</sup> May, 2020: Initial version

