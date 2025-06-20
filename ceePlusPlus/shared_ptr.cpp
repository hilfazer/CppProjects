#include <memory>


struct Bar {
	char c = '.';
	    // some data that we want to point to
};

struct Foo {
	Foo(int e) {}
	Bar bar;
};

void aliasingConstructor()
{
	Foo foo{2};

	std::shared_ptr<Foo> f = std::make_shared<Foo>(4);
	std::shared_ptr<Bar> specific_data(f, &f->bar);

	f.reset();
	specific_data->c = 'g';
}
