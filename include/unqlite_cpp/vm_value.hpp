
#pragma once

#include "exception.hpp"
#include "value.hpp"

#include <unqlite.h>

#include <cstdint>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>

namespace up {

class vm_value;
class vm_value {
	friend class vm;

  public:
	vm_value() = delete;
	vm_value(vm_value&& other) noexcept;

	vm_value& operator=(vm_value&& other) noexcept;

	~vm_value() noexcept;

	bool is_bool() const noexcept;
	bool is_int() const noexcept;
	bool is_double() const noexcept;
	bool is_string() const noexcept;

	bool is_array() const noexcept;
	bool is_object() const noexcept;
	bool is_resource() const noexcept;

	bool is_null() const noexcept;

	bool get_bool() const noexcept;
	double get_double() const noexcept;
	std::int64_t get_int() const noexcept;
	std::string get_string() const noexcept;
	std::string_view get_string_view() const noexcept;
	void* get_resource() const noexcept;

	vm_value operator[](std::string_view key) const noexcept;
	vm_value operator[](std::size_t idx) const noexcept;

	bool contains(std::string_view key) const noexcept;
	std::optional<vm_value> find(std::string_view key) const noexcept;

	std::size_t size() const noexcept;

	template<class Fn>
	bool foreach_object(Fn fn) const noexcept;
	template<class Fn>
	bool foreach_array(Fn fn) const noexcept;

	value make_value() const noexcept;

#ifdef UNQLITEPP_ALLOW_EXCEPTIONS
	const vm_value at(const std::string& key) const;
	vm_value at(const std::string& key);

	const vm_value at(std::size_t idx) const;
	vm_value at(std::size_t idx);

	bool get_bool_or_throw() const;
	std::int64_t get_int_or_throw() const;
	double get_double_or_throw() const;
	std::string get_string_or_throw() const;
	std::string_view get_string_view_or_throw() const;
	void* get_resource_or_throw() const;
#endif

  private:
	vm_value(unqlite_value* v, bool owns = true) noexcept: _v(v) {}

	unqlite_value* impl() const noexcept { return _v; }

  private:
	bool _owns;
	unqlite_value* _v;
};

inline vm_value::vm_value(vm_value&& other) noexcept {
	_owns = other._owns;
	_v = other._v;

	other._owns = false;
	other._v = nullptr;
}

inline vm_value& vm_value::operator=(vm_value&& other) noexcept {
	std::swap(_owns, other._owns);
	std::swap(_v, other._v);

	return *this;
}

inline vm_value::~vm_value() noexcept {
	if (_owns && _v) {
		unqlite_value_release(_v);
	}
}

inline bool vm_value::is_bool() const noexcept {
	return unqlite_value_is_bool(_v);
}
inline bool vm_value::is_int() const noexcept {
	return unqlite_value_is_int(_v);
}
inline bool vm_value::is_double() const noexcept {
	return unqlite_value_is_float(_v);
}
inline bool vm_value::is_string() const noexcept {
	return unqlite_value_is_string(_v);
}

inline bool vm_value::is_array() const noexcept {
	return !unqlite_value_is_json_object(_v) && unqlite_value_is_json_array(_v);
}
inline bool vm_value::is_object() const noexcept {
	return unqlite_value_is_json_object(_v);
}
inline bool vm_value::is_resource() const noexcept {
	return unqlite_value_is_resource(_v);
}
inline bool vm_value::is_null() const noexcept {
	return unqlite_value_is_null(_v);
}

inline bool vm_value::get_bool() const noexcept {
	return unqlite_value_to_bool(_v);
}
inline std::int64_t vm_value::get_int() const noexcept {
	return unqlite_value_to_int64(_v);
}
inline double vm_value::get_double() const noexcept {
	return unqlite_value_to_double(_v);
}
inline std::string vm_value::get_string() const noexcept {
	int size;
	auto str = unqlite_value_to_string(_v, &size);

	return std::string(str, size);
}
inline std::string_view vm_value::get_string_view() const noexcept {
	int size;
	auto str = unqlite_value_to_string(_v, &size);

	return std::string_view(str, size);
}
inline void* vm_value::get_resource() const noexcept {
	return unqlite_value_to_resource(_v);
}

inline bool vm_value::contains(std::string_view key) const noexcept {
	auto res = unqlite_array_fetch(_v, key.data(), key.size());
	if (!res) {

		return false;
	} else {
		unqlite_value_release(res);

		return true;
	}
}

inline std::optional<vm_value> vm_value::find(std::string_view key) const noexcept {
	auto res = unqlite_array_fetch(_v, key.data(), key.size());
	if (!res) {

		return {};
	} else {
		return vm_value(res);
	}
}

inline vm_value vm_value::operator[](std::string_view key) const noexcept {
	return *find(key);
}

inline vm_value vm_value::operator[](std::size_t idx) const noexcept {
	// auto res = unqlite_array_fetch_by_index(_v, static_cast<std::int64_t>(idx));
	// return vm_value(res);
	return *find(std::to_string(idx));
}

inline std::size_t vm_value::size() const noexcept {
	return static_cast<std::size_t>(unqlite_array_count(_v));
}

template<class Fn>
bool vm_value::foreach_object(Fn fn) const noexcept {
	auto res = unqlite_array_walk(
	    _v,
	    [](unqlite_value* key, unqlite_value* value, void* fn_p) -> int {
		    auto& fn = *static_cast<Fn*>(fn_p);

		    int size;
		    auto key_str = unqlite_value_to_string(key, &size);
		    vm_value v(value, false);

		    if (fn(key_str, v)) {

			    return UNQLITE_OK;
		    } else {

			    return UNQLITE_ABORT;
		    }
	    },
	    &fn);

	return res == UNQLITE_OK;
}

template<class Fn>
bool vm_value::foreach_array(Fn fn) const noexcept {
	auto res = unqlite_array_walk(
	    _v,
	    [](unqlite_value* key, unqlite_value* value, void* fn_p) -> int {
		    auto& fn = *static_cast<Fn*>(fn_p);

		    std::int64_t idx = unqlite_value_to_int64(key);
		    vm_value v(value, false);

		    if (fn(idx, v)) {
			    return UNQLITE_OK;
		    } else {
			    return UNQLITE_ABORT;
		    }
	    },
	    const_cast<void*>(static_cast<const void*>(&fn)));

	return res == UNQLITE_OK;
}

inline value vm_value::make_value() const noexcept {
	value value;
	if (is_bool()) {
		value = get_bool();
	} else if (is_int()) {
		value = get_int();
	} else if (is_double()) {
		value = get_double();
	} else if (is_string()) {
		value = get_string();
	} else if (is_array()) {
		value = value::array{};
		if (size() != 0) {
			// Accessing to non existing index causes array resizing to (index + 1) size
			value[size() - 1];
			foreach_array([&](std::size_t i, const class vm_value& val) {
				value[i] = val.make_value();

				return true;
			});
		}
	} else if (is_object()) {
		foreach_object([&](const std::string& key, const class vm_value& val) {
			value[key] = val.make_value();

			return true;
		});
	} else if (is_resource()) {
		value = get_resource();
	}

	return value;
}

#ifdef UNQLITEPP_ALLOW_EXCEPTIONS
inline const vm_value vm_value::at(const std::string& key) const {
	if (!is_object()) {
		throw wrong_type("Value is not an object.");
	}
	auto found = find(key);
	if (found) {
		return std::move(*found);
	} else {
		throw out_of_range("Key :`" + key + "' not found.");
	}
}
inline vm_value vm_value::at(const std::string& key) {
	return (const_cast<vm_value*>(this))->at(key);
}

inline const vm_value vm_value::at(std::size_t idx) const {
	if (!is_array()) {
		throw wrong_type("Value is not an array.");
	}
	if (idx < size()) {
		return *find(std::to_string(idx));
	} else {
		throw out_of_range("Index out of range.");
	}
}

inline vm_value vm_value::at(std::size_t idx) {
	return (const_cast<vm_value*>(this))->at(idx);
}

inline bool vm_value::get_bool_or_throw() const {
	if (!is_bool()) {
		throw wrong_type("Value is not a bool.");
	}
	return get_bool();
}

inline std::int64_t vm_value::get_int_or_throw() const {
	if (!is_int()) {
		throw wrong_type("Value is not an int.");
	}
	return get_int();
}

inline double vm_value::get_double_or_throw() const {
	if (!is_double()) {
		throw wrong_type("Value is not a double.");
	}
	return get_double();
}

inline std::string vm_value::get_string_or_throw() const {
	if (!is_string()) {
		throw wrong_type("Value is not a string.");
	}
	return get_string();
}

inline std::string_view vm_value::get_string_view_or_throw() const {
	if (!is_string()) {
		throw wrong_type("Value is not a string.");
	}
	return get_string_view();
}

inline void* vm_value::get_resource_or_throw() const {
	if (!is_resource()) {
		throw wrong_type("Value is not a resource.");
	}
	return get_resource();
}

#endif

} // namespace up
