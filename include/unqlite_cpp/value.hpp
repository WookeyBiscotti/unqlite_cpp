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

#include <cassert>
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <string_view>
#include <variant>
#include <vector>

namespace up {
class value {
  public:
	using object = std::map<std::string, value>;
	using array = std::vector<value>;

	value(std::nullptr_t) noexcept;

	value() noexcept = default;
	value(bool val) noexcept;
	value(std::int64_t val) noexcept;
	value(double val) noexcept;

	value(const char* val) noexcept;
	value(std::string val) noexcept;
	value(std::string_view val) noexcept;

	value(object val) noexcept;
	value(array val) noexcept;

	value(void* val) noexcept;

	bool is_null() const noexcept;

	bool is_bool() const noexcept;
	bool is_int() const noexcept;
	bool is_double() const noexcept;
	bool is_string() const noexcept;
	bool is_resource() const noexcept;

	bool is_array() const noexcept;
	bool is_object() const noexcept;

	value& operator=(std::nullptr_t) noexcept;

	value& operator=(std::string val) noexcept;
	value& operator=(std::string_view val) noexcept;
	value& operator=(const char* val) noexcept;
	value& operator=(void*) noexcept;

	value& operator=(bool val) noexcept;
	value& operator=(double val) noexcept;
	value& operator=(std::int64_t val) noexcept;

	value& operator[](const std::string& key) noexcept;
	value& operator[](std::size_t idx) noexcept;

	bool get_bool() const noexcept;
	std::int64_t get_int() const noexcept;
	double get_double() const noexcept;
	const std::string& get_string() const noexcept;
	std::string_view get_string_view() const noexcept;
	void* get_resource() const noexcept;

	bool get_bool_or_default(bool d) const noexcept;
	std::int64_t get_int_or_default(std::int64_t i) const noexcept;
	double get_double_or_default(double d) const noexcept;
	std::string get_string_or_default(std::string s) const noexcept;
	std::string_view get_string_view_or_default(std::string_view s) const noexcept;
	void* get_resource_or_default(void* r) const noexcept;

	template<class Fn>
	bool foreach_array(Fn fn) const noexcept;
	template<class Fn>
	bool foreach_array(Fn fn) noexcept;

	template<class Fn>
	bool foreach_if_array(Fn fn) const noexcept;
	template<class Fn>
	bool foreach_if_array(Fn fn) noexcept;

	template<class Fn>
	bool foreach_object(Fn fn) const noexcept;
	template<class Fn>
	bool foreach_object(Fn fn) noexcept;

	bool contains(const std::string& key) const noexcept;
	const value* find(const std::string& key) const noexcept;
	value* find(const std::string& key) noexcept;

	std::size_t size() const noexcept;

	bool operator==(const value& other) const noexcept;
	bool operator!=(const value& other) const noexcept;

#ifdef UNQLITE_CPP_ALLOW_EXCEPTIONS
	const value& at(const std::string& key) const;
	const value& at(std::size_t idx) const;

	value& at(const std::string& key);
	value& at(std::size_t idx);

	bool get_bool_or_throw() const;
	std::int64_t get_int_or_throw() const;
	double get_double_or_throw() const;
	std::string get_string_or_throw() const;
	std::string_view get_string_view_or_throw() const;
	void* get_resource_or_throw() const;
#endif

  private:
	std::variant<std::nullptr_t, bool, std::int64_t, double, std::string, object, array, void*> _obj;
};

inline value& value::operator=(bool val) noexcept {
	_obj = val;
	return *this;
}
inline value& value::operator=(std::nullptr_t) noexcept {
	_obj = nullptr;
	return *this;
}
inline value& value::operator=(std::string val) noexcept {
	_obj = std::move(val);
	return *this;
}
inline value& value::operator=(std::string_view val) noexcept {
	_obj = std::string(val);
	return *this;
}
inline value& value::operator=(const char* val) noexcept {
	_obj = std::string(val);
	return *this;
}
inline value& value::operator=(void* val) noexcept {
	_obj = val;
	return *this;
}
inline value& value::operator=(double val) noexcept {
	_obj = val;
	return *this;
}
inline value& value::operator=(std::int64_t val) noexcept {
	_obj = val;
	return *this;
}
inline value& value::operator[](const std::string& key) noexcept {
	auto v = std::get_if<object>(&_obj);
	if (!v) {
		_obj = object{};
		v = std::get_if<object>(&_obj);
	}

	return (*v)[key];
}

inline value& value::operator[](std::size_t idx) noexcept {
	auto v = std::get_if<array>(&_obj);
	if (!v) {
		_obj = array{};
		v = std::get_if<array>(&_obj);
	}
	if (v->size() < idx + 1) {
		v->resize(idx + 1);
	}

	return (*v)[idx];
}

inline bool value::get_bool() const noexcept {
	return *std::get_if<bool>(&_obj);
}
inline std::int64_t value::get_int() const noexcept {
	return *std::get_if<std::int64_t>(&_obj);
}
inline double value::get_double() const noexcept {
	return *std::get_if<double>(&_obj);
}
inline const std::string& value::get_string() const noexcept {
	return *std::get_if<std::string>(&_obj);
}
inline std::string_view value::get_string_view() const noexcept {
	return get_string();
}

template<class Fn>
bool value::foreach_array(Fn fn) const noexcept {
	const array& a = *std::get_if<array>(&_obj);
	for (std::size_t i = 0; i != a.size(); ++i) {
		if (!fn(i, a[i])) {

			return false;
		}
	}

	return false;
}

template<class Fn>
bool value::foreach_array(Fn fn) noexcept {
	// Effective C++, 3d ed: Avoiding Duplication in const and Non-const Member Functions
	return static_cast<const value*>(this)->foreach_array(fn);
}
template<class Fn>
bool value::foreach_object(Fn fn) const noexcept {
	const object& o = *std::get_if<object>(&_obj);
	for (auto& [key, value] : o) {
		if (!fn(key, value)) {

			return false;
		}
	}

	return false;
}
template<class Fn>
bool value::foreach_if_array(Fn fn) const noexcept {
	if (is_array()) {
		return foreach_array(fn);
	}
	return false;
}
template<class Fn>
bool value::foreach_if_array(Fn fn) noexcept {
	if (is_array()) {
		return foreach_array(fn);
	}
	return false;
}

template<class Fn>
bool value::foreach_object(Fn fn) noexcept {
	// Effective C++, 3d ed: Avoiding Duplication in const and Non-const Member Functions
	return static_cast<const value*>(this)->foreach_object(fn);
}

inline void* value::get_resource() const noexcept {
	return *std::get_if<void*>(&_obj);
}

inline bool value::get_bool_or_default(bool b) const noexcept {
	return is_bool() ? get_bool() : b;
}
inline std::int64_t value::get_int_or_default(std::int64_t i) const noexcept {
	return is_int() ? get_int() : i;
}
inline double value::get_double_or_default(double d) const noexcept {
	return is_double() ? get_double() : d;
}
inline std::string value::get_string_or_default(std::string s) const noexcept {
	return is_string() ? get_string() : s;
}
inline std::string_view value::get_string_view_or_default(std::string_view s) const noexcept {
	return is_string() ? get_string_view() : s;
}
inline void* value::get_resource_or_default(void* r) const noexcept {
	return is_resource() ? get_resource() : r;
}

inline bool value::contains(const std::string& key) const noexcept {
	return std::get_if<object>(&_obj)->count(std::string(key)) != 0;
}
inline const value* value::find(const std::string& key) const noexcept {
	auto* m = std::get_if<object>(&_obj);
	if (auto found = m->find(key); found != m->end()) {
		return &found->second;
	} else {
		return {};
	}
}
inline value* value::find(const std::string& key) noexcept {
	return static_cast<value*>(this)->find(key);
}

inline std::size_t value::size() const noexcept {
	if (auto v = std::get_if<array>(&_obj)) {
		return v->size();
	} else {
		return std::get_if<object>(&_obj)->size();
	}
}

inline bool value::is_null() const noexcept {
	return std::get_if<std::nullptr_t>(&_obj);
}

inline bool value::is_bool() const noexcept {
	return std::get_if<bool>(&_obj);
}
inline bool value::is_int() const noexcept {
	return std::get_if<std::int64_t>(&_obj);
}
inline bool value::is_double() const noexcept {
	return std::get_if<double>(&_obj);
}
inline bool value::is_string() const noexcept {
	return std::get_if<std::string>(&_obj);
}
inline bool value::is_resource() const noexcept {
	return std::get_if<void*>(&_obj);
}

inline bool value::is_array() const noexcept {
	return std::get_if<array>(&_obj);
}
inline bool value::is_object() const noexcept {
	return std::get_if<object>(&_obj);
}

inline value::value(std::nullptr_t) noexcept: _obj(nullptr) {
}

inline value::value(bool val) noexcept: _obj(val) {
}
inline value::value(std::int64_t val) noexcept: _obj(val) {
}
inline value::value(double val) noexcept: _obj(val) {
}

inline value::value(const char* val) noexcept: _obj(std::string(val)) {
}
inline value::value(void* val) noexcept: _obj(val) {
}
inline value::value(std::string val) noexcept: _obj(std::move(val)) {
}
inline value::value(std::string_view val) noexcept: _obj(std::string(val)) {
}

inline value::value(object val) noexcept: _obj(std::move(val)) {
}
inline value::value(array val) noexcept: _obj(std::move(val)) {
}

inline bool value::operator==(const value& other) const noexcept {
	return _obj == other._obj;
}
inline bool value::operator!=(const value& other) const noexcept {
	return !this->operator==(other);
}

#ifdef UNQLITE_CPP_ALLOW_EXCEPTIONS
inline const value& value::at(const std::string& key) const {
	if (!is_object()) {
		throw wrong_type("Value is not an object.");
	}
	auto found = find(key);
	if (found) {
		return *found;
	} else {
		throw out_of_range("Key :`" + key + "' not found.");
	}
}

inline value& value::at(const std::string& key) {
	return const_cast<value&>((static_cast<const value*>(this))->at(key));
}

inline value& value::at(std::size_t idx) {
	return const_cast<value&>((static_cast<const value*>(this))->at(idx));
}
inline const value& value::at(std::size_t idx) const {
	if (!is_array()) {
		throw wrong_type("Value is not an array.");
	}
	if (idx < size()) {
		return (*std::get_if<array>(&_obj))[idx];
	} else {
		throw out_of_range("Index out of range.");
	}
}

inline bool value::get_bool_or_throw() const {
	if (!is_bool()) {
		throw wrong_type("Value is not a bool.");
	}
	return get_bool();
}

inline std::int64_t value::get_int_or_throw() const {
	if (!is_int()) {
		throw wrong_type("Value is not an int.");
	}
	return get_int();
}

inline double value::get_double_or_throw() const {
	if (!is_double()) {
		throw wrong_type("Value is not a double.");
	}
	return get_double();
}

inline std::string value::get_string_or_throw() const {
	if (!is_string()) {
		throw wrong_type("Value is not a string.");
	}
	return get_string();
}

inline std::string_view value::get_string_view_or_throw() const {
	if (!is_string()) {
		throw wrong_type("Value is not a string.");
	}
	return get_string_view();
}

inline void* value::get_resource_or_throw() const {
	if (!is_resource()) {
		throw wrong_type("Value is not a resource.");
	}
	return get_resource();
}
#endif

} // namespace up
