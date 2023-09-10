#include <doctest/doctest.h>

#include <unqlite_cpp/unqlite_cpp.hpp>

#include <unordered_map>

TEST_CASE("kv_cursors") {
	auto db = up::db::make("test", up::db_mode::OPEN_IN_MEMORY);

	std::unordered_map<std::string, std::string> generated_data;

	for (int i = 0; i != 100; ++i) {
		auto key = "key_" + std::to_string(i);
		auto data = "data_" + std::to_string(i) + std::to_string(i);
		db->store(key, data);
		generated_data[key] = data;
	}

	auto cursor = db->make_kv_cursor();
	REQUIRE(cursor);

	SUBCASE("next") {
		while (cursor->valid()) {
			auto key = cursor->key();
			REQUIRE(key);

			auto data = cursor->data_string();
			REQUIRE(data);

			CHECK(generated_data.count(*key) != 0);
			generated_data.erase(*key);

			CHECK(cursor->next());
		}

		CAPTURE(generated_data.size());
		CHECK(generated_data.empty());
	}
	SUBCASE("prev") {
		CHECK(cursor->valid());

		cursor->prev();

		CHECK_FALSE(cursor->valid());
	}
	SUBCASE("first") {
		auto records_count = 0;
		auto first_key = *cursor->key();
		while (cursor->valid()) {
			records_count++;
			cursor->next();
		}
		CHECK(records_count == generated_data.size());

		CHECK_FALSE(cursor->valid());

		cursor->first();
		CHECK(cursor->valid());
		CHECK(cursor->key() == first_key);
	}
	SUBCASE("reset") {
		auto records_count = 0;
		auto first_key = *cursor->key();
		while (cursor->valid()) {
			records_count++;
			cursor->next();
		}
		CHECK(records_count == generated_data.size());

		CHECK_FALSE(cursor->valid());

		cursor->reset_cursor();
		CHECK(cursor->valid());
		CHECK(cursor->key() == first_key);
	}
	SUBCASE("last") {
		auto cursor2 = db->make_kv_cursor();
		while (cursor2->valid()) {
			cursor2->next();
			if (cursor2->valid()) {
				cursor->next();
			}
		}

		CHECK_FALSE(cursor2->valid());
		CHECK(cursor->valid());

		auto last_key = *cursor->key();

		cursor2->last();
		CHECK(*cursor2->key() == last_key);
	}
	SUBCASE("seek") {
		for (const auto& kv : generated_data) {
			CHECK(cursor->seek(kv.first));
		}
		for (const auto& kv : generated_data) {
			CHECK_FALSE(cursor->seek(kv.first + "_not_existed"));
		}
	}
	SUBCASE("remove") {
		for (const auto& kv : generated_data) {
			CHECK(cursor->seek(kv.first));
			CHECK(cursor->remove_entry());
			CHECK_FALSE(cursor->seek(kv.first));
		}
	}
}
