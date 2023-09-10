#include <doctest/doctest.h>
#include <unqlite_cpp/unqlite_cpp.hpp>
//
#include <array>
#include <cstdint>
#include <iostream>
#include <limits>
#include <string_view>

TEST_SUITE("up::value") {
	TEST_CASE("init and assign") {
		auto make_different_type_values = [] {
			return std::array<up::value, 7>{up::value(nullptr), up::value(true), up::value(345l), up::value(14e-27),
			    up::value("Test"), up::value(up::value::array{}), up::value(up::value::object{})};
		};

		SUBCASE("null") {
			auto checks = [](const up::value& v) {
				CHECK(v.is_null());

				CHECK_FALSE(v.is_bool());
				CHECK_FALSE(v.is_int());
				CHECK_FALSE(v.is_double());
				CHECK_FALSE(v.is_string());
				CHECK_FALSE(v.is_array());
				CHECK_FALSE(v.is_object());
				CHECK_FALSE(v.is_resource());
			};
			up::value v(nullptr);
			checks(v);

			for (auto& v : make_different_type_values()) {
				v = nullptr;
				checks(v);
			}
		}
		SUBCASE("bool") {
			auto checks = [](const up::value& v) {
				CHECK(v.is_bool());

				CHECK_FALSE(v.is_null());
				CHECK_FALSE(v.is_int());
				CHECK_FALSE(v.is_double());
				CHECK_FALSE(v.is_string());
				CHECK_FALSE(v.is_array());
				CHECK_FALSE(v.is_object());
				CHECK_FALSE(v.is_resource());
			};
			SUBCASE("false") {
				up::value v(false);
				checks(v);
				CHECK(v.get_bool() == false);
			}
			SUBCASE("true") {
				up::value v(true);
				checks(v);
				CHECK(v.get_bool() == true);
			}
			SUBCASE("assign") {
				for (auto& v : make_different_type_values()) {
					v = true;
					checks(v);
				}
			}
		}
		SUBCASE("int") {
			auto checks = [](const up::value& v) {
				CHECK(v.is_int());

				CHECK_FALSE(v.is_null());
				CHECK_FALSE(v.is_bool());
				CHECK_FALSE(v.is_double());
				CHECK_FALSE(v.is_string());
				CHECK_FALSE(v.is_array());
				CHECK_FALSE(v.is_object());
				CHECK_FALSE(v.is_resource());
			};
			SUBCASE("more then 0") {
				up::value v(899l);
				checks(v);
				CHECK(v.get_int() == 899l);
			}
			SUBCASE("less then 0") {
				up::value v(-7623l);
				checks(v);
				CHECK(v.get_int() == -7623l);
			}
			SUBCASE("0") {
				up::value v(0l);
				checks(v);
				CHECK(v.get_int() == 0);
			}
			SUBCASE("assign") {
				for (auto& v : make_different_type_values()) {
					v = 757l;
					checks(v);
				}
			}
		}
		SUBCASE("double") {
			auto checks = [](const up::value& v) {
				CHECK(v.is_double());

				CHECK_FALSE(v.is_null());
				CHECK_FALSE(v.is_bool());
				CHECK_FALSE(v.is_int());
				CHECK_FALSE(v.is_string());
				CHECK_FALSE(v.is_array());
				CHECK_FALSE(v.is_object());
				CHECK_FALSE(v.is_resource());
			};
			SUBCASE("signaling nan") {
				up::value v(std::numeric_limits<double>::signaling_NaN());
				checks(v);
			}
			SUBCASE("nan") {
				up::value v(std::numeric_limits<double>::quiet_NaN());
				checks(v);
			}
			SUBCASE("infinity") {
				up::value v(std::numeric_limits<double>::infinity());
				checks(v);
			}
			SUBCASE("simple") {
				up::value v(123.12312);
				checks(v);
				CHECK(v.get_double() == 123.12312);
			}
			SUBCASE("assign") {
				for (auto& v : make_different_type_values()) {
					v = 543762.01123;
					checks(v);
				}
			}
		}
		SUBCASE("string") {
			auto checks = [](const up::value& v) {
				CHECK(v.is_string());

				CHECK_FALSE(v.is_null());
				CHECK_FALSE(v.is_bool());
				CHECK_FALSE(v.is_int());
				CHECK_FALSE(v.is_double());
				CHECK_FALSE(v.is_array());
				CHECK_FALSE(v.is_object());
				CHECK_FALSE(v.is_resource());
			};
			SUBCASE("string init") {
				up::value v(std::string("qweqwsadsdasds"));
				checks(v);
				CHECK(v.get_string() == "qweqwsadsdasds");
			}
			SUBCASE("const char* init") {
				up::value v("qweasd asdqwe");
				checks(v);
				CHECK(v.get_string_view() == "qweasd asdqwe");
			}
			SUBCASE("std::string_vew init") {
				up::value v(std::string_view("czczxfewrfq 123123 "));
				checks(v);
				CHECK(v.get_string_view() == "czczxfewrfq 123123 ");
			}
			SUBCASE("assign") {
				for (auto& v : make_different_type_values()) {
					v = "123as";
					checks(v);
				}
			}
		}
		SUBCASE("resource") {
			auto checks = [](const up::value& v) {
				CHECK(v.is_resource());

				CHECK_FALSE(v.is_null());
				CHECK_FALSE(v.is_bool());
				CHECK_FALSE(v.is_int());
				CHECK_FALSE(v.is_double());
				CHECK_FALSE(v.is_string());
				CHECK_FALSE(v.is_array());
				CHECK_FALSE(v.is_object());
			};
			SUBCASE("init with (void*)[some ptr]") {
				up::value v(static_cast<void*>(&v));
				checks(v);
				CHECK(v.get_resource() == static_cast<void*>(&v));
			}
			SUBCASE("assign") {
				for (auto& v : make_different_type_values()) {
					v = static_cast<void*>(&v);
					checks(v);
				}
			}
		}
		SUBCASE("array") {
			auto checks = [](const up::value& v) {
				CHECK(v.is_array());

				CHECK_FALSE(v.is_null());
				CHECK_FALSE(v.is_bool());
				CHECK_FALSE(v.is_int());
				CHECK_FALSE(v.is_double());
				CHECK_FALSE(v.is_string());
				CHECK_FALSE(v.is_resource());
				CHECK_FALSE(v.is_object());
			};
			SUBCASE("empty array") {
				up::value v(up::value::array{});
				checks(v);
				CHECK(v.size() == 0);
			}
			SUBCASE("non empty array") {
				up::value v(up::value::array{nullptr, true, 434l, 65456.123, &v, "TEST", up::value::array{1l, 2l},
				    up::value::object{{"qwe", 1l}}});
				checks(v);
				CHECK(v.size() == 8);
				CHECK(v[0].is_null());
				CHECK(v[1].is_bool());
				CHECK(v[1].get_bool() == true);
				CHECK(v[2].is_int());
				CHECK(v[2].get_int() == 434l);
				CHECK(v[3].is_double());
				CHECK(v[3].get_double() == 65456.123);
				CHECK(v[4].is_resource());
				CHECK(v[4].get_resource() == &v);
				CHECK(v[5].is_string());
				CHECK(v[5].get_string() == "TEST");
				CHECK(v[6].is_array());
				CHECK(v[6].size() == 2);
				CHECK(v[7].is_object());
				CHECK(v[7].size() == 1);
			}
			SUBCASE("assign") {
				for (auto& v : make_different_type_values()) {
					v = up::value::array{};
					checks(v);
				}
			}
		}
		SUBCASE("object") {
			auto checks = [](const up::value& v) {
				CHECK(v.is_object());

				CHECK_FALSE(v.is_null());
				CHECK_FALSE(v.is_bool());
				CHECK_FALSE(v.is_int());
				CHECK_FALSE(v.is_double());
				CHECK_FALSE(v.is_string());
				CHECK_FALSE(v.is_resource());
				CHECK_FALSE(v.is_array());
			};
			SUBCASE("empty object") {
				up::value v(up::value::object{});
				checks(v);
			}
			SUBCASE("non empty object") {
				up::value v(up::value::object{{{"nullptr", nullptr}, {"true", true}, {"434l", 434l},
				    {"65456.123", 65456.123}, {"&v", &v}, {"TEST", "TEST"}, {"array", up::value::array{"qwe", "asd"}},
				    {"object", up::value::object{{"ert", 12l}}}}});
				checks(v);
				CHECK(v.size() == 8);
				CHECK(v["nullptr"].is_null());
				CHECK(v.contains("nullptr"));
				CHECK(v["true"].is_bool());
				CHECK(v["true"].get_bool() == true);
				CHECK(v.contains("true"));
				CHECK(v["434l"].is_int());
				CHECK(v["434l"].get_int() == 434l);
				CHECK(v.contains("434l"));
				CHECK(v["65456.123"].is_double());
				CHECK(v["65456.123"].get_double() == 65456.123);
				CHECK(v.contains("65456.123"));
				CHECK(v["&v"].is_resource());
				CHECK(v["&v"].get_resource() == &v);
				CHECK(v.contains("&v"));
				CHECK(v["TEST"].is_string());
				CHECK(v["TEST"].get_string() == "TEST");
				CHECK(v.contains("TEST"));
				CHECK(v["array"].is_array());
				CHECK(v["array"].size() == 2);
				CHECK(v.contains("array"));
				CHECK(v["object"].is_object());
				CHECK(v["object"].size() == 1);
				CHECK(v.contains("object"));

				CHECK(v.find("object").has_value());
				CHECK(v.find("object")->find("ert"));
				CHECK_FALSE(v.find("non exist object").has_value());
			}
			SUBCASE("assign") {
				for (auto& v : make_different_type_values()) {
					v = up::value::object{};
					checks(v);
				}
			}
		}
	}
	TEST_CASE("compare") {
		SUBCASE("null") {
			CHECK(up::value(nullptr) == up::value(nullptr));
		}
		SUBCASE("int") {
			CHECK(up::value(123l) == up::value(123l));
			CHECK(up::value(123l) != up::value(234l));
		}
		SUBCASE("double") {
			CHECK(up::value(0.0) == up::value(0.0));
			CHECK(up::value(0.0) != up::value(234.3));
		}
		SUBCASE("string") {
			CHECK(up::value("string1") == up::value("string1"));
			CHECK(up::value("string1") != up::value("string2"));
		}
		SUBCASE("resource") {
			CHECK(up::value(reinterpret_cast<void*>(123)) == up::value(reinterpret_cast<void*>(123)));
			CHECK(up::value(reinterpret_cast<void*>(123)) != up::value(reinterpret_cast<void*>(321)));
		}
		SUBCASE("array") {
			auto make_array = [] {
				return up::value::array{"val1", true, 3l, up::value::object{{"submember", "folded_data"}}};
			};
			up::value v1 = make_array();
			up::value v2 = make_array();
			CHECK(v1 == v2);

			v1[4] = nullptr;
			CHECK(v1 != v2);
		}
		SUBCASE("object") {
			auto make_object = [] {
				return up::value::object{{"member1", "simple_data"},
				    {"member2", up::value::object{{"submember", "folded_data"}}}};
			};
			up::value v1 = make_object();
			up::value v2 = make_object();
			CHECK(v1 == v2);

			v2["member3"] = 1234l;
			CHECK(v1 != v2);
		}
	}
}
