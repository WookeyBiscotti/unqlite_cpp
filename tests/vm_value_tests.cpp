#include <doctest/doctest.h>
#include <unqlite_cpp/unqlite_cpp.hpp>

TEST_CASE("vm_value") {
	auto db = up::db::make("test", up::db_mode::OPEN_IN_MEMORY);
	REQUIRE(db);

	auto vm = db->compile("$value = {int: 123, bool: TRUE, null2: NULL, string: 'string', double: 3.1415, "
	                      "array:[1,2,3,4], object: {a1:'qwe', b2:321}};");
	REQUIRE(vm);

	REQUIRE(vm->exec());

	auto value = vm->extract("value");
	REQUIRE(value);

	auto val_int = value->find("int");
	CHECK(val_int);
	CHECK(val_int->is_int());
	CHECK(val_int->get_int() == 123);

	auto val_bool = value->find("bool");
	CHECK(val_bool);
	CHECK(val_bool->is_bool());
	CHECK(val_bool->get_bool() == true);

	auto val_null = value->find("null2");
	CHECK(val_null);
	CHECK(val_null->is_null());

	auto val_double = value->find("double");
	CHECK(val_double);
	CHECK(val_double->is_double());
	CHECK(val_double->get_double() == 3.1415);

	auto val_string = value->find("string");
	CHECK(val_string);
	CHECK(val_string->is_string());
	CHECK(val_string->get_string() == "string");

	// Currently unqlite have bug in array indexing
	// SUBCASE("array") {
	// 	auto val_array = value->find("array");
	// 	CHECK(val_array);
	// 	CHECK(val_array->is_array());
	// 	CHECK(val_array->size() == 4);
	// 	CHECK((*val_array)[0].get_int() == 1);
	// 	CHECK((*val_array)[1].get_int() == 2);
	// 	CHECK((*val_array)[2].get_int() == 3);
	// 	CHECK((*val_array)[3].get_int() == 4);
	// }

	auto val_object = value->find("object");
	CHECK(val_object);
	CHECK(val_object->is_object());
	CHECK((*val_object)["a1"].get_string() == "qwe");
	CHECK((*val_object)["b2"].get_int() == 321);
}
