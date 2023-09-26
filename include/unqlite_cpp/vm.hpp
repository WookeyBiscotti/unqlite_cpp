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

#include "exception.hpp"
#include "statuses.hpp"
#include "utils.hpp"
#include "vm_value.hpp"

#include <unqlite.h>

#include <iostream>
#include <optional>

static int VmOutputConsumer(const void* pOutput, unsigned int nOutLen, void* pUserData /* Unused */) {
	std::cout << std::string_view(static_cast<const char*>(pOutput), nOutLen);
	/* All done, data was redirected to STDOUT */
	return UNQLITE_OK;
}

namespace up {

class vm {
	friend class db;

  public:
	vm() = delete;
	vm(vm&& other) noexcept;

	vm& operator=(vm&& other) noexcept;

	~vm() noexcept;

	// nullopt if variable not declared
	std::optional<vm_value> extract(const std::string& var_name) noexcept;

	// nullopt if errors happens.
	// The unqlite does not produce clear errors; one error can indicate different
	// reasons, so this function does not produce a status so as not to confuse
	// the user.
	bool bind(const std::string& var_name, const vm_value& var) noexcept;
	bool bind(const std::string& var_name, const value& var) noexcept;

	bool exec(vm_execute_status* status = nullptr) noexcept;

	// return false if vm is currupted
	bool reset_vm() noexcept;

	vm_value make_vm_value(const value& value) noexcept;

#ifdef UNQLITE_CPP_ALLOW_EXCEPTIONS
	vm_value extract_or_throw(const std::string& var_name);

	vm& bind_or_throw(const std::string& var_name, const vm_value& var);
	vm& bind_or_throw(const std::string& var_name, const value& var);

	vm& exec_or_throw();

	vm& reset_vm_or_throw();
#endif

  private:
	vm(unqlite_vm* vm) noexcept: _vm(vm) { unqlite_vm_config(_vm, UNQLITE_VM_CONFIG_OUTPUT, VmOutputConsumer, 0); }

  private:
	unqlite_vm* _vm;
};

inline vm::vm(vm&& other) noexcept {
	_vm = other._vm;
	other._vm = nullptr;
}

inline std::optional<vm_value> vm::extract(const std::string& var_name) noexcept {
	auto var = unqlite_vm_extract_variable(_vm, var_name.c_str());
	if (var) {

		return vm_value(var);
	} else {

		return {};
	}
}

inline bool vm::bind(const std::string& var_name, const vm_value& var) noexcept {
	return unqlite_vm_config(_vm, UNQLITE_VM_CONFIG_CREATE_VAR, var_name.c_str(), var.impl()) == UNQLITE_OK;
}

inline bool vm::bind(const std::string& var_name, const value& value) noexcept {
	return bind(var_name, make_vm_value(value));
}

inline vm_value vm::make_vm_value(const value& value) noexcept {
	unqlite_value* raw_value;
	if (value.is_bool()) {
		raw_value = unqlite_vm_new_scalar(_vm);
		unqlite_value_bool(raw_value, value.get_bool());
	} else if (value.is_int()) {
		raw_value = unqlite_vm_new_scalar(_vm);
		unqlite_value_int64(raw_value, value.get_int());
	} else if (value.is_double()) {
		raw_value = unqlite_vm_new_scalar(_vm);
		unqlite_value_double(raw_value, value.get_double());
	} else if (value.is_string()) {
		raw_value = unqlite_vm_new_scalar(_vm);
		auto sv = value.get_string_view();
		unqlite_value_string(raw_value, sv.data(), sv.size());
	} else if (value.is_array()) {
		raw_value = unqlite_vm_new_array(_vm);
		value.foreach_array([&](std::size_t i, const class value& val) {
			auto vm_val = make_vm_value(val);
			auto idx = make_vm_value(static_cast<std::int64_t>(i));
			unqlite_array_add_elem(raw_value, idx.impl(), vm_val.impl());

			return true;
		});
	} else if (value.is_object()) {
		raw_value = unqlite_vm_new_array(_vm);
		value.foreach_object([&](const std::string& key, const class value& val) {
			auto vm_val = make_vm_value(val);
			unqlite_array_add_strkey_elem(raw_value, key.c_str(), vm_val.impl());

			return true;
		});
	} else if (value.is_resource()) {
		raw_value = unqlite_vm_new_array(_vm);
		auto rc = value.get_resource();
		unqlite_value_resource(raw_value, rc);
	} else {
		raw_value = unqlite_vm_new_scalar(_vm);
		unqlite_value_null(raw_value);
	}

	return vm_value(raw_value);
}

inline bool vm::exec(vm_execute_status* status) noexcept {
	auto res = unqlite_vm_exec(_vm);
	if (status) {
		if (res == UNQLITE_OK) {
			*status = vm_execute_status::OK;
		} else if (res == UNQLITE_ABORT) {
			*status = vm_execute_status::ABORT;
		} else if (res == UNQLITE_LOCKED) {
			*status = vm_execute_status::LOCKED;
		} else {
			*status = vm_execute_status::VM_CURRUPTED;
		}
	}

	return res == UNQLITE_OK;
}

inline bool vm::reset_vm() noexcept {
	return unqlite_vm_reset(_vm) == UNQLITE_OK;
}

inline vm::~vm() noexcept {
	if (_vm) {
		unqlite_vm_release(_vm);
	}
}

inline vm& vm::operator=(vm&& other) noexcept {
	std::swap(_vm, other._vm);

	return *this;
}

#ifdef UNQLITE_CPP_ALLOW_EXCEPTIONS
inline vm_value vm::extract_or_throw(const std::string& var_name) {
	auto v = extract(var_name);
	if (v) {
		return std::move(*v);
	} else {
		throw exception_with_status(vm_extract_status::NOTFOUND);
	}
}

inline vm& vm::bind_or_throw(const std::string& var_name, const vm_value& var) {
	if (!bind(var_name, var)) {
		throw exception_with_status(vm_bind_status::ERROR);
	}

	return *this;
}

inline vm& vm::bind_or_throw(const std::string& var_name, const value& var) {
	if (!bind(var_name, var)) {
		throw exception_with_status(vm_bind_status::ERROR);
	}

	return *this;
}

inline vm& vm::exec_or_throw() {
	vm_execute_status status;
	if (!exec(&status)) {
		throw exception_with_status(status);
	}

	return *this;
}

inline vm& vm::reset_vm_or_throw() {
	if (!reset_vm()) {
		throw exception_with_status(vm_exec_status::ERROR);
	}

	return *this;
}

#endif

} // namespace up
