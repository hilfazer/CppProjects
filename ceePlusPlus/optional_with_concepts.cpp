#include <type_traits>


template<typename T>
class optional {
public:
	optional(const T& val) {}
	optional(const optional&) requires(std::is_trivially_constructible_v<T>) = default;
};


struct NotCopyable
{
	int value;

	NotCopyable(const int val)
		: value(val)
	{}

	NotCopyable(const NotCopyable&) = delete;
};

void testOkay()
{
	optional<int> a{2};
	optional<int> b{a};
}

void testNotOkay()
{
	optional<NotCopyable> a{2};
	optional<NotCopyable> b{a};
}
