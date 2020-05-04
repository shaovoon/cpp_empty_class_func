#include <iostream>

class Same
{};

class Different
{};

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
    void Swap(short n, Same)
    {

    }
	void Swap(short& n, Different)
	{
        short upper = (n & 0xff) << 8;
		short lower = (n & 0xff00) >> 8;
        n = upper | lower;
	}

};

int main()
{
	std::cout << "sizeof(Same) is " << sizeof(Same) << std::endl;
	std::cout << "sizeof(Different) is " << sizeof(Different) << std::endl;

	std::cout << std::endl;

	std::cout << "Create PacketWriter<Same> to write a short int" << std::endl;
    PacketWriter<Same> w;
    w.Write(256);

	std::cout << std::endl;

	std::cout << "Create PacketWriter<Different> to write a short int" << std::endl;
	PacketWriter<Different> w2;
	w2.Write(256);

	return 0;
}
