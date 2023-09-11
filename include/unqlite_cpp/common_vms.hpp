//                     _ _ _       _____
//                    | (_) |     /  __ \ _     _
//   _   _ _ __   __ _| |_| |_ ___| /  \/| |_ _| |_     C++ unqlite wrapper
//  | | | | '_ \ / _` | | | __/ _ \ |  |_   _|_   _|    https://github.com/WookeyBiscotti/unqlite_cpp
//  | |_| | | | | (_| | | | ||  __/ \__/\|_|   |_|
//   \__,_|_| |_|\__, |_|_|\__\___|\____/
//                  | |
//                  |_|
//
// Copyright (c) 2023 Alexandr Savchenko (skyswood@gmail.com)
// Distributed under the MIT License (MIT)
#pragma once

#include "db.hpp"

#include <optional>

namespace up {

struct store_record_vm {
	vm vm;
	static std::optional<store_record_vm> make(up::db& db) noexcept {
		auto vmo = db.compile(R"(
		if (!db_exists($collection)) {
			$rc = db_create($collection);
			if (!$rc) {
				return;
			}
		}
		$rc = db_store($collection, $value);
		)");
		if (vmo) {
			return store_record_vm{std::move(*vmo)};
		} else {
			return {};
		}
	}

	bool store(std::string_view collection, const value& value) noexcept {
		vm.bind("collection", collection);
		vm.bind("value", value);
		auto res = false;
		if (vm.exec()) {
			if (auto rc = vm.extract("rc")) {
				res = rc->is_bool() && rc->get_bool();
			}
		}
		vm.reset_vm();
		return res;
	}
};

struct fetch_record_vm {
	vm vm;
	static std::optional<fetch_record_vm> make(up::db& db) noexcept {
		auto vmo = db.compile("$record = db_fetch($collection);"
		                      "if($record == NULL) db_reset_record_cursor($collection);");
		if (vmo) {
			return fetch_record_vm{std::move(*vmo)};
		} else {
			return {};
		}
	}

	std::optional<value> fetch(std::string_view collection) noexcept {
		std::optional<value> res;
		vm.bind("collection", collection);
		if (vm.exec()) {
			if (auto record = vm.extract("record"); !record->is_null()) {
				res = record->make_value();
			}
		}
		vm.reset_vm();
		return res;
	}
};

struct fetch_all_records_vm {
	vm vm;
	static std::optional<fetch_all_records_vm> make(up::db& db) noexcept {
		auto vmo = db.compile("$records = db_fetch_all($collection);");
		if (vmo) {
			return fetch_all_records_vm{std::move(*vmo)};
		} else {
			return {};
		}
	}

	std::optional<vm_value> fetch(std::string_view collection) noexcept {
		std::optional<vm_value> res;
		vm.bind("collection", collection);
		if (vm.exec()) {
			if (auto record = vm.extract("records")) {
				res = std::move(record);
			}
		}
		vm.reset_vm();
		return res;
	}
};

struct fetch_first_record_vm {
	vm vm;
	static std::optional<fetch_first_record_vm> make(up::db& db) noexcept {
		auto vmo = db.compile("db_reset_record_cursor($collection);"
		                      "$records = db_fetch($collection);");
		if (vmo) {
			return fetch_first_record_vm{std::move(*vmo)};
		} else {
			return {};
		}
	}

	std::optional<vm_value> fetch(std::string_view collection) noexcept {
		std::optional<vm_value> res;
		vm.bind("collection", collection);
		if (vm.exec()) {
			if (auto record = vm.extract("records")) {
				res = std::move(record);
			}
		}
		vm.reset_vm();
		return res;
	}
};

struct drop_record_vm {
	vm vm;
	static std::optional<drop_record_vm> make(up::db& db) noexcept {
		auto vmo = db.compile("$rc = db_drop_record($collection, $id);");
		if (vmo) {
			return drop_record_vm{std::move(*vmo)};
		} else {
			return {};
		}
	}

	bool drop(std::string_view collection, std::int64_t id) noexcept {
		vm.bind("collection", collection);
		vm.bind("id", id);
		auto res = false;
		if (vm.exec()) {
			if (auto rc = vm.extract("rc")) {
				res = rc->is_bool() && rc->get_bool();
			}
		}
		vm.reset_vm();
		return res;
	}
};

} // namespace up
