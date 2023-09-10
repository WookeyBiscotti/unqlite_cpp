#include <doctest/doctest.h>

#include <unqlite_cpp/unqlite_cpp.hpp>

TEST_SUITE("up::db") {
	TEST_CASE("ctor") {
		SUBCASE("mode(OPEN_CREATE)") {
			up::db_make_status status;
			auto db = up::db::make("test", up::db_mode::OPEN_CREATE, &status);
			CHECK(db);
		}
		SUBCASE("mode(OPEN_IN_MEMORY)") {
			up::db_make_status status;
			auto db = up::db::make("test", up::db_mode::OPEN_IN_MEMORY, &status);
			CHECK(db);
		}
		SUBCASE("mode(OPEN_MMAP)") {
			up::db_make_status status;
			auto db = up::db::make("test", up::db_mode::OPEN_MMAP, &status);
			CHECK(db);
		}
		SUBCASE("mode(OPEN_READONLY)") {
			up::db_make_status status;
			auto db = up::db::make("test", up::db_mode::OPEN_READONLY, &status);
			CHECK(db);
		}
		SUBCASE("mode(OPEN_READWRITE)") {
			up::db_make_status status;
			auto db = up::db::make("test", up::db_mode::OPEN_READWRITE, &status);
			CHECK(db);
		}
		SUBCASE("mode(OPEN_NOMUTEX)") {
			up::db_make_status status;
			auto db = up::db::make("test", up::db_mode::OPEN_NOMUTEX, &status);
			CHECK(db);
		}
		SUBCASE("mode(OPEN_OMIT_JOURNALING)") {
			up::db_make_status status;
			auto db = up::db::make("test", up::db_mode::OPEN_OMIT_JOURNALING, &status);
			CHECK(db);
		}
	}
	TEST_CASE("store/load document via jx9") {
		auto db = up::db::make("test", up::db_mode::OPEN_IN_MEMORY);
		REQUIRE(db);

		REQUIRE(db->compile("db_create('test_collection');")->exec());
		REQUIRE(db->compile("db_store('test_collection',"
		                    "{int_data: 12, string_data:'data', array_data:[0,1,2,3]});")
		            ->exec());

		auto vm = db->compile("$rec = db_fetch('test_collection');");
		REQUIRE(vm);
		REQUIRE(vm->exec());

		auto rec = vm->extract("rec");
		REQUIRE(rec);
		REQUIRE(rec->find("int_data")->get_int() == 12);
		REQUIRE(rec->find("string_data")->get_string_view() == "data");
		REQUIRE(rec->find("array_data")->make_value() == up::value::array{0l, 1l, 2l, 3l});
		REQUIRE(rec->find("__id")->is_int());

		REQUIRE(rec->size() == 4);
	}
	TEST_CASE("store/load document via value") {
		auto db = up::db::make("test", up::db_mode::OPEN_IN_MEMORY);
		REQUIRE(db);

		REQUIRE(db->compile("db_create('test_collection');")->exec());

		auto store_vm = db->compile("db_store('test_collection', $value);");
		REQUIRE(store_vm);
		store_vm->bind("value", up::value::object{{"int_data", 12l}, {"string_data", "data"},
		                            {"array_data", up::value::array{0l, 1l, 2l, 3l}}});
		REQUIRE(store_vm->exec());

		auto fetch_vm = db->compile("$rec = db_fetch('test_collection');");
		REQUIRE(fetch_vm);
		REQUIRE(fetch_vm->exec());

		auto rec = fetch_vm->extract("rec");
		REQUIRE(rec);
		REQUIRE(rec->find("int_data")->get_int() == 12);
		REQUIRE(rec->find("string_data")->get_string_view() == "data");
		REQUIRE(rec->find("array_data")->make_value() == up::value::array{0l, 1l, 2l, 3l});
		REQUIRE(rec->find("__id")->is_int());

		REQUIRE(rec->size() == 4);
	}
	TEST_CASE("kv store") {
		SUBCASE("store") {
			auto db = up::db::make("test", up::db_mode::OPEN_IN_MEMORY);
			REQUIRE(db);

			for (int i = 0; i != 100000; ++i) {
				CHECK(db->store("key_" + std::to_string(i), "data_" + std::to_string(i)));
			}
			for (int i = 0; i != 100000; ++i) {
				CHECK(*db->fetch_string("key_" + std::to_string(i)) == "data_" + std::to_string(i));
			}
		}
		SUBCASE("append") {
			auto db = up::db::make("test", up::db_mode::OPEN_IN_MEMORY);
			REQUIRE(db);

			for (int i = 0; i != 100000; ++i) {
				CHECK(db->store("key_" + std::to_string(i), "data_" + std::to_string(i)));
				CHECK(db->append("key_" + std::to_string(i), "data_" + std::to_string(i)));
			}
			for (int i = 0; i != 100000; ++i) {
				CHECK(*db->fetch_string("key_" + std::to_string(i)) ==
				      "data_" + std::to_string(i) + "data_" + std::to_string(i));
			}
		}
		SUBCASE("remove") {
			auto db = up::db::make("test", up::db_mode::OPEN_IN_MEMORY);
			REQUIRE(db);

			for (int i = 0; i != 100000; ++i) {
				db->store("key_" + std::to_string(i), "data_" + std::to_string(i));
			}
			for (int i = 0; i < 100000; i += 2) {
				CHECK(db->remove("key_" + std::to_string(i)));
			}
			for (int i = 0; i < 100000; ++i) {
				const auto key = "key_" + std::to_string(i);
				if (i % 2 == 0) {
					CHECK_FALSE(db->fetch_string(key));
				} else {
					CHECK(db->fetch_string(key));
				}
			}
		}
	}
}
