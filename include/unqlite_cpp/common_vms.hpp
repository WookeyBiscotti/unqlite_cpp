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

class vm_common {
  public:
	up::db& db;
	vm vm;

  protected:
	vm_common(up::db& db, up::vm vm): db(db), vm(std::move(vm)) {}
};

class vm_store_record {
  public:
	up::db& db;
	vm vm;

	static std::optional<vm_store_record> make(up::db& db) noexcept {
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
			return vm_store_record{db, std::move(*vmo)};
		} else {
			return {};
		}
	}

	vm_store_record(up::db& db):
	    vm_store_record(db, [&]() {
		    auto vm = make(db);
		    if (vm) {
			    return std::move(vm->vm);
		    } else {
			    throw up::exception("Can't make vm");
		    }
	    }()) {}

	bool store(std::string_view collection, const value& value) noexcept {
		vm.bind("collection", collection);
		vm.bind("value", value);
		auto res = false;
		if (vm.exec()) {
			if (auto rc = vm.extract("rc")) {
				res = rc->is_bool() && rc->get_bool();
			}
		}
		db.commit();
		vm.reset_vm();
		return res;
	}

	void store_or_throw(std::string_view collection, const value& value) {
		auto rc = store(collection, value);
		if (!rc) {
			throw up::exception("Can't store data");
		}
	}

  private:
	vm_store_record(up::db& db, up::vm vm): db(db), vm(std::move(vm)) {}
};

class vm_fetch_record {
  public:
	vm vm;

	static std::optional<vm_fetch_record> make(up::db& db) noexcept {
		auto vmo = db.compile("$record = db_fetch($collection);"
		                      "if($record == NULL) db_reset_record_cursor($collection);");
		if (vmo) {
			return vm_fetch_record(std::move(*vmo));
		} else {
			return {};
		}
	}

	vm_fetch_record(up::db& db):
	    vm_fetch_record([&]() {
		    auto vm = make(db);
		    if (vm) {
			    return std::move(vm->vm);
		    } else {
			    throw up::exception("Can't make vm");
		    }
	    }()) {}

	std::optional<vm_value> fetch(std::string_view collection) noexcept {
		std::optional<vm_value> res;
		vm.bind("collection", collection);
		if (vm.exec()) {
			if (auto record = vm.extract("record"); !record->is_null()) {
				res = std::move(record);
			}
		}
		vm.reset_vm();
		return res;
	}

	std::optional<value> fetch_value(std::string_view collection) noexcept {
		return this->fetch(collection)->make_value();
	}

	vm_value fetch_or_throw(std::string_view collection) {
		auto rc = fetch(collection);
		if (!rc) {
			throw up::exception("Can't store data");
		} else {
			return std::move(*rc);
		}
	}

	value fetch_value_or_throw(std::string_view collection) { return fetch_or_throw(collection).make_value(); }

  private:
	vm_fetch_record(up::vm vm): vm(std::move(vm)) {}
};

class vm_fetch_all_records {
  public:
	vm vm;
	static std::optional<vm_fetch_all_records> make(up::db& db) noexcept {
		auto vmo = db.compile("$records = db_fetch_all($collection);");
		if (vmo) {
			return vm_fetch_all_records(std::move(*vmo));
		} else {
			return {};
		}
	}

	vm_fetch_all_records(up::db& db):
	    vm_fetch_all_records([&]() {
		    auto vm = make(db);
		    if (vm) {
			    return std::move(vm->vm);
		    } else {
			    throw up::exception("Can't make vm");
		    }
	    }()) {}

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

	std::optional<value> fetch_value(std::string_view collection) noexcept {
		return this->fetch(collection)->make_value();
	}

	vm_value fetch_or_throw(std::string_view collection) {
		auto rc = fetch(collection);
		if (!rc) {
			throw up::exception("Can't fetch data");
		} else {
			return std::move(*rc);
		}
	}

	value fetch_value_or_throw(std::string_view collection) { return fetch_or_throw(collection).make_value(); }

  private:
	vm_fetch_all_records(up::vm vm): vm(std::move(vm)) {}
};

class vm_fetch_first_record {
  public:
	vm vm;
	static std::optional<vm_fetch_first_record> make(up::db& db) noexcept {
		auto vmo = db.compile("db_reset_record_cursor($collection);"
		                      "$records = db_fetch($collection);");
		if (vmo) {
			return vm_fetch_first_record(std::move(*vmo));
		} else {
			return {};
		}
	}

	vm_fetch_first_record(up::db& db):
	    vm_fetch_first_record([&]() {
		    auto vm = make(db);
		    if (vm) {
			    return std::move(vm->vm);
		    } else {
			    throw up::exception("Can't make vm");
		    }
	    }()) {}

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

	std::optional<value> fetch_value(std::string_view collection) noexcept {
		return this->fetch(collection)->make_value();
	}

	vm_value fetch_or_throw(std::string_view collection) {
		auto rc = fetch(collection);
		if (!rc) {
			throw up::exception("Can't fetch data");
		} else {
			return std::move(*rc);
		}
	}

	value fetch_value_or_throw(std::string_view collection) { return fetch_or_throw(collection).make_value(); }

  private:
	vm_fetch_first_record(up::vm vm): vm(std::move(vm)) {}
};

class vm_drop_record {
  public:
	up::db& db;
	vm vm;
	static std::optional<vm_drop_record> make(up::db& db) noexcept {
		auto vmo = db.compile("$rc = db_drop_record($collection, $id);");
		if (vmo) {
			return vm_drop_record{db, std::move(*vmo)};
		} else {
			return {};
		}
	}

	vm_drop_record(up::db& db):
	    vm_drop_record(db, [&]() {
		    auto vm = make(db);
		    if (vm) {
			    return std::move(vm->vm);
		    } else {
			    throw up::exception("Can't make vm");
		    }
	    }()) {}

	bool drop(std::string_view collection, std::int64_t id) noexcept {
		vm.bind("collection", collection);
		vm.bind("id", id);
		auto res = false;
		if (vm.exec()) {
			if (auto rc = vm.extract("rc")) {
				res = rc->is_bool() && rc->get_bool();
			}
		}
		db.commit();
		vm.reset_vm();
		return res;
	}

	void drop_or_throw(std::string_view collection, std::int64_t id) {
		auto rc = drop(collection, id);
		if (!rc) {
			throw up::exception("Can't drop data");
		}
	}

  private:
	vm_drop_record(up::db& db, up::vm vm): db(db), vm(std::move(vm)) {}
};

} // namespace up
