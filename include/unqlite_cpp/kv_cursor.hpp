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

#include <unqlite.h>

#include <algorithm>
#include <cassert>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace up {

enum class kv_cursor_match_direction {
	EXACT,    // Default search method supported by all key/value storage subsystem.
	          // An exact match is performed. If the record exists, the cursor is
	          // left pointing to it.
	MATCH_LE, // The cursor is left pointing to the largest key in the database
	          // that is smaller than (pKey/nKey). If the database contains no
	          // keys smaller than (pKey/nKey), the cursor is left at EOF. This
	          // option have sense only if the underlying key/value storage
	          // subsystem support range search (i.e: B+Tree, R+Tree, etc.).
	          // Otherwise this option is ignored and an exact match is performed.
	MATCH_GE, // The cursor is left pointing to the smallest key in the database
	          // that is larger than (pKey/nKey). If the database contains no keys
	          // larger than (pKey/nKey), the cursor is left at EOF. This option
	          // have sense only if the underlying key/value storage subsystem
	          // support range search (i.e: B+Tree, R+Tree, etc.). Otherwise this
	          // option is ignored and an exact match is performed.
};

class kv_cursor {
	friend class db;

  public:
	kv_cursor() = delete;
	~kv_cursor();

	kv_cursor(kv_cursor&& other) noexcept;

	kv_cursor& operator=(kv_cursor&& other) noexcept;
	kv_cursor& operator=(const kv_cursor& other) = delete;

	bool seek(std::string_view key, kv_cursor_match_direction direction = kv_cursor_match_direction::EXACT,
	    kv_cursor_op_status* status = nullptr) noexcept;

	bool first(kv_cursor_op_status* status = nullptr) noexcept;
	bool last(kv_cursor_op_status* status = nullptr) noexcept;

	bool valid() const noexcept;

	bool next(kv_cursor_op_status* status = nullptr) noexcept;
	bool prev(kv_cursor_op_status* status = nullptr) noexcept;

	std::optional<std::string> key(kv_cursor_op_status* status = nullptr) const noexcept;
	template<class Fn>
	bool key_callback(Fn fn, kv_cursor_op_status* status = nullptr) const noexcept;

	std::optional<std::string> data_string(kv_cursor_op_status* status = nullptr) const noexcept;
	std::optional<std::vector<unsigned char>> data_vector(kv_cursor_op_status* status = nullptr) const noexcept;
	template<class Fn>
	bool data_callback(Fn fn, kv_cursor_op_status* status = nullptr) const noexcept;

	bool remove_entry(kv_cursor_op_status* status = nullptr) noexcept;

	bool reset_cursor(kv_cursor_op_status* status = nullptr) noexcept;

#ifdef UNQLITEPP_ALLOW_EXCEPTIONS
	kv_cursor& seek_or_throw(std::string_view key,
	    kv_cursor_match_direction direction = kv_cursor_match_direction::EXACT);

	kv_cursor& first_or_throw();
	kv_cursor& last_or_throw();

	kv_cursor& next_or_throw();
	kv_cursor& prev_or_throw();

	template<class Fn>
	bool key_callback_or_throw(Fn fn) const;
	std::string key_or_throw() const;

	template<class Fn>
	bool data_callback_or_throw(Fn fn) const;
	std::string data_string_or_throw() const;
	std::vector<unsigned char> data_vector_or_throw() const;

	kv_cursor& remove_entry_or_throw();

	kv_cursor& reset_cursor_or_throw();
#endif

  private:
	kv_cursor(unqlite* db, unqlite_kv_cursor* cursor) noexcept: _db(db), _cursor(cursor) {
	}

	bool process_op_error(int rc, kv_cursor_op_status* status) const noexcept;

#ifdef UNQLITEPP_ALLOW_EXCEPTIONS
	template<class... Args>
	void throw_if_error(bool (kv_cursor::*op)(Args... args, kv_cursor_op_status* status) noexcept, Args... args);

	template<class Fn>
	bool throw_if_error_in_callback(Fn fn,
	    int (*op)(unqlite_kv_cursor* pCursor, int (*xConsumer)(const void*, unsigned int, void*), void* pUserData));
#endif

  private:
	unqlite* _db;
	unqlite_kv_cursor* _cursor;
};

inline bool kv_cursor::process_op_error(int rc, kv_cursor_op_status* out_status) const noexcept {
	kv_cursor_op_status status;
	if (rc == UNQLITE_OK) {
		status = kv_cursor_op_status::OK;
	} else if (rc == UNQLITE_EMPTY) {
		status = kv_cursor_op_status::EMPTY;
	} else if (rc == UNQLITE_NOTIMPLEMENTED) {
		status = kv_cursor_op_status::NOTIMPLEMENTED;
	} else if (rc == UNQLITE_EOF) {
		status = kv_cursor_op_status::EOF_;
	} else if (rc == UNQLITE_ABORT) {
		status = kv_cursor_op_status::ABORT;
	} else /*if (rc == UNQLITE_CORRUPT)*/ {
		status = kv_cursor_op_status::CORRUPT;
	}

	if (out_status) {
		*out_status = status;
	}

	return status == kv_cursor_op_status::OK;
}

inline kv_cursor::~kv_cursor() {
	assert(!!_db == !!_cursor);
	if (_cursor) {
		unqlite_kv_cursor_release(_db, _cursor);
	}
}

inline kv_cursor::kv_cursor(kv_cursor&& other) noexcept {
	_cursor = other._cursor;
	_db = other._db;
	other._db = nullptr;
	other._cursor = nullptr;
}

inline kv_cursor& kv_cursor::operator=(kv_cursor&& other) noexcept {
	std::swap(_cursor, other._cursor);
	std::swap(_db, other._db);

	return *this;
}

inline bool kv_cursor::seek(std::string_view key, kv_cursor_match_direction direction,
    kv_cursor_op_status* status) noexcept {
	int unq_dir;
	if (direction == kv_cursor_match_direction::EXACT) {
		unq_dir = UNQLITE_CURSOR_MATCH_EXACT;
	} else if (direction == kv_cursor_match_direction::MATCH_LE) {
		unq_dir = UNQLITE_CURSOR_MATCH_LE;
	} else {
		unq_dir = UNQLITE_CURSOR_MATCH_GE;
	}
	return process_op_error(unqlite_kv_cursor_seek(_cursor, key.data(), key.size(), unq_dir), status);
}

inline bool kv_cursor::first(kv_cursor_op_status* status) noexcept {
	return process_op_error(unqlite_kv_cursor_first_entry(_cursor), status);
}

inline bool kv_cursor::last(kv_cursor_op_status* status) noexcept {
	return process_op_error(unqlite_kv_cursor_last_entry(_cursor), status);
}

inline bool kv_cursor::valid() const noexcept {
	return unqlite_kv_cursor_valid_entry(_cursor) == 1;
}

inline bool kv_cursor::next(kv_cursor_op_status* status) noexcept {
	return process_op_error(unqlite_kv_cursor_next_entry(_cursor), status);
}
inline bool kv_cursor::prev(kv_cursor_op_status* status) noexcept {
	return process_op_error(unqlite_kv_cursor_prev_entry(_cursor), status);
}

inline bool kv_cursor::remove_entry(kv_cursor_op_status* status) noexcept {
	return process_op_error(unqlite_kv_cursor_delete_entry(_cursor), status);
}

inline bool kv_cursor::reset_cursor(kv_cursor_op_status* status) noexcept {
	return process_op_error(unqlite_kv_cursor_reset(_cursor), status);
}

template<class Fn>
bool kv_cursor::key_callback(Fn fn, kv_cursor_op_status* status) const noexcept {
	return process_op_error(unqlite_kv_cursor_key_callback(
	                            _cursor,
	                            [](const void* data, unsigned int size, void* rfn) noexcept -> int {
		                            auto fn = *reinterpret_cast<Fn*>(rfn);
		                            return fn(data, static_cast<std::size_t>(size)) ? UNQLITE_OK : UNQLITE_ABORT;
	                            },
	                            &fn),
	    status);
}
inline std::optional<std::string> kv_cursor::key(kv_cursor_op_status* status) const noexcept {
	std::string res;
	if (key_callback(
	        [&](const void* data, std::size_t size) {
		        res.resize(size);
		        auto begin = static_cast<const char*>(data);
		        res.assign(begin, begin + size);
		        return true;
	        },
	        status)) {
		return res;
	} else {
		return {};
	}
}

template<class Fn>
bool kv_cursor::data_callback(Fn fn, kv_cursor_op_status* status) const noexcept {
	return process_op_error(unqlite_kv_cursor_data_callback(
	                            _cursor,
	                            [](const void* data, unsigned int size, void* rfn) noexcept -> int {
		                            auto fn = *reinterpret_cast<Fn*>(rfn);
		                            return fn(data, static_cast<std::size_t>(size)) ? UNQLITE_OK : UNQLITE_ABORT;
	                            },
	                            &fn),
	    status);
}
inline std::optional<std::string> kv_cursor::data_string(kv_cursor_op_status* status) const noexcept {
	std::string res;
	if (data_callback(
	        [&](const void* data, std::size_t size) {
		        res.resize(size);
		        auto begin = static_cast<const char*>(data);
		        res.assign(begin, begin + size);
		        return true;
	        },
	        status)) {
		return res;
	} else {
		return {};
	}
}
inline std::optional<std::vector<unsigned char>> kv_cursor::data_vector(kv_cursor_op_status* status) const noexcept {
	std::vector<unsigned char> res;
	if (data_callback(
	        [&](const void* data, std::size_t size) {
		        res.resize(size);
		        auto begin = static_cast<const unsigned char*>(data);
		        res.assign(begin, begin + size);
		        return true;
	        },
	        status)) {
		return res;
	} else {
		return {};
	}
}

#ifdef UNQLITEPP_ALLOW_EXCEPTIONS
template<class... Args>
void kv_cursor::throw_if_error(bool (kv_cursor::*op)(Args... args, kv_cursor_op_status* status) noexcept,
    Args... args) {
	kv_cursor_op_status status;
	if ((this->*op)(args..., &status)) {
		return;
	} else {
		throw exception_with_status(status);
	}
}

template<class Fn>
bool kv_cursor::throw_if_error_in_callback(Fn fn,
    int (*op)(unqlite_kv_cursor* pCursor, int (*xConsumer)(const void*, unsigned int, void*), void* pUserData)) {
	kv_cursor_op_status status;
	std::string_view error_text;
	std::exception_ptr exception;
	auto user_data = std::make_pair(&fn, &exception);
	using user_data_type = decltype(user_data);
	auto rc = process_op_error(op(
	                               _cursor,
	                               [](const void* data, unsigned int size, void* ud) noexcept -> int {
		                               user_data_type user_data = *reinterpret_cast<user_data_type*>(ud);
		                               try {
			                               return (*user_data.first)(data, static_cast<std::size_t>(size)) ?
			                                          UNQLITE_OK :
			                                          UNQLITE_ABORT;
		                               } catch (...) {
			                               *(user_data.second) = std::current_exception();
			                               return UNQLITE_ABORT;
		                               }
	                               },
	                               &user_data),
	    &status, &error_text);

	if (rc) {
		return true;
	} else {
		if (*user_data.second) {
			std::rethrow_exception(*user_data.second);
		} else {
			if (status == kv_cursor_op_status::ABORT) {
				return false;
			} else {
				throw up::exception_with_status(status, error_text);
			}
		}
	}
}

inline kv_cursor& kv_cursor::seek_or_throw(std::string_view key, kv_cursor_match_direction direction) {
	throw_if_error(&kv_cursor::seek, key, direction);
	return *this;
}
inline kv_cursor& kv_cursor::first_or_throw() {
	throw_if_error(&kv_cursor::first);
	return *this;
}
inline kv_cursor& kv_cursor::last_or_throw() {
	throw_if_error(&kv_cursor::last);
	return *this;
}
inline kv_cursor& kv_cursor::next_or_throw() {
	throw_if_error(&kv_cursor::next);
	return *this;
}
inline kv_cursor& kv_cursor::prev_or_throw() {
	throw_if_error(&kv_cursor::prev);
	return *this;
}
inline std::string kv_cursor::key_or_throw() const {
	kv_cursor_op_status status;
	std::optional<std::string> key_o;
	if (auto key_o = key(&status)) {
		return std::move(*key_o);
	} else {
		throw exception_with_status(status);
	}
}
template<class Fn>
bool kv_cursor::key_callback_or_throw(Fn fn) const {
	return throw_if_error_in_callback(fn, unqlite_kv_cursor_key_callback);
}
inline std::string kv_cursor::data_string_or_throw() const {
	kv_cursor_op_status status;
	std::optional<std::string> key_o;
	if (auto key_o = data_string(&status)) {
		return std::move(*key_o);
	} else {
		throw exception_with_status(status);
	}
}
inline std::vector<unsigned char> kv_cursor::data_vector_or_throw() const {
	kv_cursor_op_status status;
	std::optional<std::vector<unsigned char>> data_o;
	if (auto key_o = data_vector(&status)) {
		return std::move(*data_o);
	} else {
		throw exception_with_status(status);
	}
}
template<class Fn>
bool kv_cursor::data_callback_or_throw(Fn fn) const {
	return throw_if_error_in_callback(fn, unqlite_kv_cursor_key_callback);
}
inline kv_cursor& kv_cursor::remove_entry_or_throw() {
	throw_if_error(&kv_cursor::remove_entry);
	return *this;
}
inline kv_cursor& kv_cursor::reset_cursor_or_throw() {
	throw_if_error(&kv_cursor::reset_cursor);
	return *this;
}
#endif

} // namespace up
