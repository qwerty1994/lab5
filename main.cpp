#include <cassert>
#include <sstream>
#include <stdexcept>

#include "LinkedList.h"

int main()
{
// См. примечание внизу, почему все тело main() заключено в блок { }.
{
	// В конструкции используется т. н. оператор запятая. В С++ справедливо
	// (a, b, c) == (c), то есть результат списка выражений через запятую ---
	// это результат последнего выражения. Таким образом вычисляется выражение
	// во внутренних скобках. Его результат --- результат последнего, то есть
	// условия, которое необходимо проверить. При срабатывании assert()
	// включает в сообщение об ошибке все выражение во внутренних скобках,
	// включая первое (строку). Это позволяет напечатать описание проверки,
	// которая выполняется. Сравните:
	//
	//	size() == 4   <--- как понять, зачем это проверялось?
	//
	//	("pop_front() should decrease size", xs.size() == 4)   <--,
	//                                                           /
	//      При чтении понятно: проверялась работа pop_front() -'
	//
	assert( ("error message", 2*2 == 4) );


    LinkedList empty;
	assert(("size() for empty list should be 0", empty.size() == 0));


	LinkedList xs { 1, 2, 3, 4, 5 };
	assert(("size() for {1,2,3,4,5} should be 5", xs.size() == 5));


	assert(("value_at(0) for {1,2,3,4,5} should be 1", xs.value_at(0) == 1));
	assert(("value_at(1) for {1,2,3,4,5} should be 2", xs.value_at(1) == 2));
	assert(("value_at(2) for {1,2,3,4,5} should be 3", xs.value_at(2) == 3));
	assert(("value_at(3) for {1,2,3,4,5} should be 4", xs.value_at(3) == 4));
	assert(("value_at(4) for {1,2,3,4,5} should be 5", xs.value_at(4) == 5));


	std::ostringstream buffer;

	buffer.str(" ");
	buffer << empty;
	assert(("{} as string", buffer.str() == "[]"));

	buffer.str("");
	buffer << xs;
	assert(("{1,2,3,4,5} as string", buffer.str() == "[1, 2, 3, 4, 5]"));

	xs.push_back(6);
	assert(("push_back() should increase size", xs.size() == 6));
	assert(("push_back() should push back", xs.value_at(5) == 6));


	xs.pop_back();
	assert(("pop_back() should decrease size", xs.size() == 5));


	xs.push_front(0);
	assert(("push_front() should increase size", xs.size() == 6));
	assert(("push_front() should push front", xs.value_at(0) == 0));


	xs.pop_front();
	assert(("pop_front() should decrease size", xs.size() == 5));


	try {
		xs.value_at(666);
		// Если предыдущая строка приведет к возбуждению исключения
		// (как и должно быть), следующая строка не выполнится.
		// Если же она выполнится, assert() всегда сработает,
		// потому что его условие всегда ложно (false).
		assert(("value_at(666) should raise exception", false));
	}
	catch (const std::out_of_range&) {
	}
	catch (...) {
		assert(("value_at(666) should raise `out_of_range'", false));
	}


	assert(("an object should be equal to itself", xs == xs));

	assert(("even an empty object should be equal to itself", empty == empty));

	LinkedList as { 1, 2, 3 };
	LinkedList bs { 1, 2, 3 };
	assert(("identical lists should be equal", as == bs));


	auto ys = xs;
	assert(("copy construction should produce identical objects", xs == ys));

	ys.push_back(7);
	assert(("push_back() to a copy should not change original", xs != ys));

	ys.push_front(-1);
	assert(("push_front() to a copy should not change original", xs != ys));

	ys.value_at(2) = 42;
	assert(("modification of a copy elements should not change original elements",
			xs.value_at(2) != ys.value_at(2)));
	ys.value_at(2) = 3;

	xs.value_at(2) = 42;
	assert(("modification of original elements should not change copy elements",
			xs.value_at(2) != ys.value_at(2)));
	xs.value_at(2) = 3;

	LinkedList zs;
	zs = xs;
	assert(("copy assignxment should produce identical objects", xs == zs));

	zs.push_back(7);
	assert(("push_back() to a copy should not change original", xs != zs));

	zs.push_front(-1);
	assert(("push_front() to a copy should not change original", xs != zs));

	zs.value_at(2) = 42;
	assert(("modification of a copy elements should not change original elements",
			xs.value_at(2) != zs.value_at(2)));
	zs.value_at(2) = 3;

	xs.value_at(2) = 42;
	assert(("modification of original elements should not change copy elements",
			xs.value_at(2) != zs.value_at(2)));
	xs.value_at(2) = 3;


	auto function_that_returns_an_object = [&xs]() -> LinkedList
	{
		return xs;
	};
	LinkedList ws = function_that_returns_an_object();
	assert(("move construction should work", ws == xs));

	auto vaz = xs;
	vaz = vaz;
	assert(("self-assignment should not corrupt an object", vaz == xs));

	auto us = xs;
	us.insert_before(1, 11);
	assert(("insert_before() should increase size() by 1", us.size() == xs.size() + 1));
	assert(("insert_before(1, 11) should insert 11 before element #1",
	        us.value_at(0) == 1 &&
	        us.value_at(1) == 11 &&
	        us.value_at(2) == 2));

	us.remove_at(1);
	assert(("remove_at(1) should return `us' back to `xs'", us == xs));

	us.insert_after(4, 55);
	assert(("insert_after() should increase size() by 1", us.size() == xs.size() + 1));
	assert(("insert_after(4, 55) should insert 55 after element #4",
	        us.value_at(4) == 5 &&
	        us.value_at(5) == 55));

	us.remove_at(5);
	assert(("remove_at(5) should return `us' back to `xs'", us == xs));

	try {
		auto ts = xs;
		xs.insert_after(666, 666);
		assert(("insert_after(666, 666) should raise exception", false));
	}
	catch (const std::out_of_range&) {
	}
	catch (...) {
		assert(("insert_after(666, 666) should raise `out_of_range'", false));
	}

	try {
		auto ts = xs;
		xs.insert_before(666, 666);
		assert(("insert_before(666, 666) should raise exception", false));
	}
	catch (const std::out_of_range&) {
	}
	catch (...) {
		assert(("insert_before(666, 666) should raise `out_of_range'", false));
	}

	try {
		auto ts = xs;
		xs.remove_at(666);
		assert(("remove_at(666) should raise exception", false));
	}
	catch (const std::out_of_range&) {
	}
	catch (...) {
		assert(("remove_at(666) should raise `out_of_range'", false));
	}
}
// В этой точке будут вызваны деструкторы объектов, созданных внутри блока.
// Если в них есть критические ошибки, программа завершится аварийно.
// Деструкторы бы были вызваны и в конце main(), но в этом случае ошибка
// выполнения может оказаться скрыта.

}
