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
// Distributed under the MIT License (MIT) (See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT)

#pragma once

// #include "common_vms.hpp"
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


// #include "db.hpp"
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


// #include "exception.hpp"
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


// #include "statuses.hpp"
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


#include <string_view>

namespace up {

template<class S>
struct status_to_string_view {};

enum class db_make_status {
	OK,
	NOMEM, // Out of memory (An unlikely scenario).
};
template<>
struct status_to_string_view<db_make_status> {
	static constexpr std::string_view value = "db_make_status";
};

enum class db_make_kv_cursor_status {
	OK,
	CORRUPT,
	ABORT,
	NOTIMPLEMENTED,
	NOMEM, // Out of memory (An unlikely scenario).
};
template<>
struct status_to_string_view<db_make_kv_cursor_status> {
	static constexpr std::string_view value = "db_make_kv_cursor_status";
};

enum class db_compilation_status {
	OK,
	COMPILE_ERR, // is returned when compile-time errors occur. That is, the caller must fix its erroneous Jx9 code and
	             // call the compile interface again.
	VM_ERR, // is returned when something goes wrong during initialization of the virtual machine (i.e. Out of memory).
	        // But remember, this is a very unlikely scenario on modern hardware even on modern embedded system.
	IOERR,  // is returned when IO errors occurs (i.e. permission error, nonexistant file, etc.) or the underlying VFS
	        // does not implement the xMmap() method which is not the case when the built-in VFS is used.
};
template<>
struct status_to_string_view<db_compilation_status> {
	static constexpr std::string_view value = "db_compilation_status";
};

enum class db_transaction_status {
	OK,        // is returned on success. Any other return value indicates failure such as:
	READ_ONLY, // Read-only database.
	BUSY,      // Another thread or process have an exclusive lock on the database. In this case, the caller should wait
	           // until the lock holder relinquish it.
	IOERR,     // OS specific error.
	NOMEM,     // Out of memory (Unlikely scenario).
	ABORT,     // Another thread have released the database handle.
};
template<>
struct status_to_string_view<db_transaction_status> {
	static constexpr std::string_view value = "db_transaction_status";
};

enum class db_kv_read_status {
	OK,       // is returned on success. Any other return value indicates failure such as:
	NOTFOUND, // Nonexistent record.
	BUSY,     // Another thread or process have an exclusive lock on the database. In this case, the caller should wait
	          // until the lock holder relinquish it.
	IOERR,    // OS specific error.
	NOMEM,    // Out of memory (An unlikely scenario).
	ABORT,    // Client consumer callback request an operation abort.
};
template<>
struct status_to_string_view<db_kv_read_status> {
	static constexpr std::string_view value = "db_kv_read_status";
};

enum class db_kv_write_status {
	OK,        // is returned on success. Any other return value indicates failure such as:
	BUSY,      // Another thread or process have a reserved or exclusive lock on the database. In which case, the caller
	           // should wait until the lock holder relinquish it.
	READ_ONLY, // Read-only Key/Value storage engine.
	NOTIMPLEMENTED, // The underlying KV storage engine does not implement the xReplace() method.
	PERM,           // Permission error.
	LIMIT,          // Journal file record limit reached (An unlikely scenario).
	IOERR, // OS specific error. This error is considered harmful and you should perform an immediate rollback via
	       // unqlite_rollback().
	NOMEM, // Out of memory (Unlikely scenario). This error is considered harmful and you should perform an immediate
	       // rollback via unqlite_rollback().
};
template<>
struct status_to_string_view<db_kv_write_status> {
	static constexpr std::string_view value = "db_kv_write_status";
};

enum class kv_cursor_op_status {
	OK,
	EMPTY,
	CORRUPT,
	NOTIMPLEMENTED,
	EOF_,
	ABORT,
};
template<>
struct status_to_string_view<kv_cursor_op_status> {
	static constexpr std::string_view value = "kv_cursor_op_status";
};

enum class vm_execute_status {
	OK,
	VM_CURRUPTED, /* Stale vm */
	ABORT,        /* Another thread have released this instance */
	LOCKED,       /* Locked VM */
};
template<>
struct status_to_string_view<vm_execute_status> {
	static constexpr std::string_view value = "vm_execute_status";
};

enum class vm_extract_status {
	OK,
	NOTFOUND,
};
template<>
struct status_to_string_view<vm_extract_status> {
	static constexpr std::string_view value = "vm_extract_status";
};

enum class vm_bind_status {
	OK,
	ERROR,
};
template<>
struct status_to_string_view<vm_bind_status> {
	static constexpr std::string_view value = "vm_bind_status";
};

enum class vm_exec_status {
	OK,
	ERROR,
};
template<>
struct status_to_string_view<vm_exec_status> {
	static constexpr std::string_view value = "vm_exec_status";
};

} // namespace up


#ifdef UNQLITEPP_ALLOW_EXCEPTIONS

#include <stdexcept>
#include <string>
#include <string_view>

namespace up {

class exception: public std::runtime_error {
  public:
	exception(const std::string& msg): std::runtime_error(msg) {}
};

class out_of_range: public exception {
  public:
	out_of_range(const std::string& msg): exception(msg) {}
};

class wrong_type: public exception {
  public:
	wrong_type(const std::string& msg): exception(msg) {}
};

template<class S>
class exception_with_status: public exception {
  public:
	exception_with_status(S status, std::string_view text = ""):
	    exception(std::string(status_to_string_view<S>::value) + ": " +
	              std::to_string(static_cast<std::size_t>(status)) + ". " + std::string(text)),
	    _status(status) {}

	S status() const noexcept { return _status; }

  private:
	S _status;
};

} // namespace up

#endif

// #include "kv_cursor.hpp"
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


// #include "exception.hpp"

// #include "statuses.hpp"


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

// #include "statuses.hpp"

// #include "vm.hpp"
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


// #include "exception.hpp"

// #include "statuses.hpp"

// #include "vm_value.hpp"
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


// #include "exception.hpp"

// #include "value.hpp"
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


// #include "exception.hpp"


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
	std::string get_string() const noexcept;
	std::string_view get_string_view() const noexcept;
	void* get_resource() const noexcept;

	template<class Fn>
	bool foreach_array(Fn fn) const noexcept;
	template<class Fn>
	bool foreach_array(Fn fn) noexcept;

	template<class Fn>
	bool foreach_object(Fn fn) const noexcept;
	template<class Fn>
	bool foreach_object(Fn fn) noexcept;

	bool contains(const std::string& key) const noexcept;
	std::optional<value> find(const std::string& key) const noexcept;

	std::size_t size() const noexcept;

	bool operator==(const value& other) const noexcept;
	bool operator!=(const value& other) const noexcept;

#ifdef UNQLITEPP_ALLOW_EXCEPTIONS
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
inline std::string value::get_string() const noexcept {
	return *std::get_if<std::string>(&_obj);
}
inline std::string_view value::get_string_view() const noexcept {
	return *std::get_if<std::string>(&_obj);
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
bool value::foreach_object(Fn fn) noexcept {
	// Effective C++, 3d ed: Avoiding Duplication in const and Non-const Member Functions
	return static_cast<const value*>(this)->foreach_object(fn);
}

inline void* value::get_resource() const noexcept {
	return *std::get_if<void*>(&_obj);
}

inline bool value::contains(const std::string& key) const noexcept {
	return std::get_if<object>(&_obj)->count(std::string(key)) != 0;
}
inline std::optional<value> value::find(const std::string& key) const noexcept {
	auto* m = std::get_if<object>(&_obj);
	if (auto found = m->find(key); found != m->end()) {
		return found->second;
	} else {
		return {};
	}
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

#ifdef UNQLITEPP_ALLOW_EXCEPTIONS
inline const value& value::at(const std::string& key) const {
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


#include <unqlite.h>

#include <optional>

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

#ifdef UNQLITEPP_ALLOW_EXCEPTIONS
	vm_value extract_or_throw(const std::string& var_name);

	void bind_or_throw(const std::string& var_name, const vm_value& var);
	void bind_or_throw(const std::string& var_name, const value& var);

	void exec_or_throw();

	void reset_vm_or_throw();
#endif

  private:
	vm(unqlite_vm* vm) noexcept: _vm(vm) {}

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
	return bind(var_name, vm_value(make_vm_value(value)));
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

#ifdef UNQLITEPP_ALLOW_EXCEPTIONS
inline vm_value vm::extract_or_throw(const std::string& var_name) {
	auto v = extract(var_name);
	if (v) {
		return std::move(*v);
	} else {
		throw exception_with_status(vm_extract_status::NOTFOUND);
	}
}

inline void vm::bind_or_throw(const std::string& var_name, const vm_value& var) {
	if (!bind(var_name, var)) {
		throw exception_with_status(vm_bind_status::ERROR);
	}
}

inline void vm::bind_or_throw(const std::string& var_name, const value& var) {
	if (!bind(var_name, var)) {
		throw exception_with_status(vm_bind_status::ERROR);
	}
}

inline void vm::exec_or_throw() {
	vm_execute_status status;
	if (!exec(&status)) {
		throw exception_with_status(status);
	}
}

inline void vm::reset_vm_or_throw() {
	if (!reset_vm()) {
		throw exception_with_status(vm_exec_status::ERROR);
	}
}

#endif

} // namespace up


#include <unqlite.h>

#include <cassert>
#include <cstdint>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>

namespace up {

// Analog UNQLITE_XXX
enum db_mode : unsigned int {
	OPEN_READONLY = 0x00000001,        /* Read only mode. Ok for [unqlite_open] */
	OPEN_READWRITE = 0x00000002,       /* Ok for [unqlite_open] */
	OPEN_CREATE = 0x00000004,          /* Ok for [unqlite_open] */
	OPEN_NOMUTEX = 0x00000020,         /* Ok for [unqlite_open] */
	OPEN_OMIT_JOURNALING = 0x00000040, /* Omit journaling for this database. Ok for [unqlite_open] */
	OPEN_IN_MEMORY = 0x00000080,       /* An in memory database. Ok for [unqlite_open]*/
	OPEN_MMAP = 0x00000100,            /* Obtain a memory view of the whole file. Ok for [unqlite_open] */
	OPEN_EXCLUSIVE = 0x00000008,       /* VFS only */
	OPEN_TEMP_DB = 0x00000010,         /* VFS only */
};

class db {
  public:
	db() = delete;
	db(db&& other) noexcept;

	~db() noexcept;

	static std::optional<db> make(const std::string& filename, unsigned int mode = OPEN_CREATE,
	    db_make_status* status = nullptr) noexcept;

	// Config
	bool disable_auto_commit() noexcept;

	// jx9 API
	std::optional<vm> compile(std::string_view code, db_compilation_status* status = nullptr,
	    std::string_view* error_text = nullptr) noexcept;
	std::optional<vm> compile_file(const std::string& filepath, db_compilation_status* status = nullptr,
	    std::string_view* error_text = nullptr) noexcept;

	// Key/Value API
	bool store(std::string_view key, std::string_view data, db_kv_write_status* status = nullptr,
	    std::string_view* error_text = nullptr) noexcept;
	bool store(std::string_view key, const void* data, std::size_t size, db_kv_write_status* status = nullptr,
	    std::string_view* error_text = nullptr) noexcept;
	bool append(std::string_view key, const void* data, std::size_t size, db_kv_write_status* status = nullptr,
	    std::string_view* error_text = nullptr) noexcept;
	bool append(std::string_view key, std::string_view data, db_kv_write_status* status = nullptr,
	    std::string_view* error_text = nullptr) noexcept;
	bool remove(std::string_view key, db_kv_write_status* status = nullptr,
	    std::string_view* error_text = nullptr) noexcept;

	template<class Fn>
	bool fetch_callback(std::string_view key, Fn fn, db_kv_read_status* status = nullptr,
	    std::string_view* error_text = nullptr) const noexcept;
	std::optional<std::string> fetch_string(std::string_view key, db_kv_read_status* status = nullptr,
	    std::string_view* error_text = nullptr) const noexcept;
	std::optional<std::vector<unsigned char>> fetch_vector(std::string_view key, db_kv_read_status* status = nullptr,
	    std::string_view* error_text = nullptr) const noexcept;

	// Cursor API
	std::optional<kv_cursor> make_kv_cursor(db_make_kv_cursor_status* status = nullptr) noexcept;

	// Transaction API
	bool begin(db_transaction_status* status = nullptr, std::string_view* error_text = nullptr) noexcept;
	bool rollback(db_transaction_status* status = nullptr, std::string_view* error_text = nullptr) noexcept;
	bool commit(db_transaction_status* status = nullptr, std::string_view* error_text = nullptr) noexcept;

#ifdef UNQLITEPP_ALLOW_EXCEPTIONS
	db(const std::string& filename, unsigned int mode = OPEN_CREATE);

	vm compile_or_throw(std::string_view code);
	vm compile_file_or_throw(const std::string& filepath);

	void store_or_throw(std::string_view key, std::string_view data);
	void store_or_throw(std::string_view key, const void* data, std::size_t size);
	void append_or_throw(std::string_view key, const void* data, std::size_t size);
	void append_or_throw(std::string_view key, std::string_view data);
	void remove_or_throw(std::string_view key);

	template<class Fn>
	bool fetch_callback_or_throw(std::string_view key, Fn fn) const;
	std::optional<std::string> fetch_string_or_throw(std::string_view key) const;
	std::optional<std::vector<unsigned char>> fetch_vector_or_throw(std::string_view key) const;

	kv_cursor make_kv_cursor_or_throw();
#endif
  private:
	db(unqlite* _db) noexcept;

	bool process_transaction_rc(int rc, db_transaction_status* status_out, std::string_view* error_text) const noexcept;
	bool process_vm_compile_rc(int rc, db_compilation_status* status_out, std::string_view* error_text) const noexcept;
	bool process_write_rc(int rc, db_kv_write_status* status_out, std::string_view* error_text) const noexcept;
	bool process_read_rc(int rc, db_kv_read_status* status_out, std::string_view* error_text) const noexcept;

	vm return_vm_or_throw(std::optional<vm> vm, db_compilation_status status, std::string_view error_text);

  private:
	unqlite* _db;
};

inline db::db(db&& other) noexcept {
	_db = other._db;
	other._db = nullptr;
}

inline db::db(unqlite* db) noexcept: _db(db) {
}

inline std::optional<db> db::make(const std::string& filename, unsigned int mode, db_make_status* status) noexcept {
	unqlite* db;
	auto res = unqlite_open(&db, filename.c_str(), mode);
	if (res == UNQLITE_OK) {
		if (status) {
			*status = db_make_status::OK;
		}

		return up::db(db);
	}

	if (status) {
		*status = db_make_status::NOMEM;
	}

	return {};
}

inline db::~db() noexcept {
	if (_db) {
		unqlite_close(_db);
	}
}

inline bool db::disable_auto_commit() noexcept {
	return unqlite_config(_db, UNQLITE_CONFIG_DISABLE_AUTO_COMMIT) == UNQLITE_OK;
}

inline std::optional<vm> db::compile(std::string_view code, db_compilation_status* status,
    std::string_view* error_text) noexcept {
	unqlite_vm* v;
	return process_vm_compile_rc(unqlite_compile(_db, code.data(), code.size(), &v), status, error_text) ?
	           std::optional<vm>(vm(v)) :
	           std::nullopt;
}

inline std::optional<vm> db::compile_file(const std::string& filepath, db_compilation_status* status,
    std::string_view* error_text) noexcept {
	unqlite_vm* v;
	return process_vm_compile_rc(unqlite_compile_file(_db, filepath.c_str(), &v), status, error_text) ?
	           std::optional<vm>(vm(v)) :
	           std::nullopt;
}

inline bool db::process_transaction_rc(int rc, db_transaction_status* status_out,
    std::string_view* error_text) const noexcept {
	db_transaction_status status;
	if (rc == UNQLITE_OK) {
		if (status_out) {
			*status_out = db_transaction_status::OK;
		}

		return true;
	} else if (rc == UNQLITE_NOMEM) {
		status = db_transaction_status::NOMEM;
	} else if (rc == UNQLITE_READ_ONLY) {
		status = db_transaction_status::READ_ONLY;
	} else if (rc == UNQLITE_BUSY) {
		status = db_transaction_status::BUSY;
	} else if (rc == UNQLITE_IOERR) {
		status = db_transaction_status::IOERR;
	} else /*if (rc == UNQLITE_ABORT)*/ {
		status = db_transaction_status::ABORT;
	}
	if (status_out) {
		*status_out = status;
	}
	if (error_text) {
		const char* error_log;
		int len;
		auto rc = unqlite_config(_db, UNQLITE_CONFIG_ERR_LOG, &error_log, &len);
		if (rc == UNQLITE_OK) {
			*error_text = std::string_view(error_log, len);
		} else {
			*error_text = "Db is currupted. Can't fill `error_text`.";
		}
	}

	return false;
}
inline bool db::process_write_rc(int rc, db_kv_write_status* status_out, std::string_view* error_text) const noexcept {
	db_kv_write_status status;
	if (rc == UNQLITE_OK) {
		if (status_out) {
			*status_out = db_kv_write_status::OK;
		}

		return true;
	} else if (rc == UNQLITE_BUSY) {
		status = db_kv_write_status::BUSY;
	} else if (rc == UNQLITE_READ_ONLY) {
		status = db_kv_write_status::READ_ONLY;
	} else if (rc == UNQLITE_NOTIMPLEMENTED) {
		status = db_kv_write_status::NOTIMPLEMENTED;
	} else if (rc == UNQLITE_PERM) {
		status = db_kv_write_status::PERM;
	} else if (rc == UNQLITE_LIMIT) {
		status = db_kv_write_status::LIMIT;
	} else if (rc == UNQLITE_IOERR) {
		status = db_kv_write_status::IOERR;
	} else /*if (rc == UNQLITE_NOMEM)*/ {
		status = db_kv_write_status::NOMEM;
	}
	if (status_out) {
		*status_out = status;
	}
	if (error_text) {
		const char* error_log;
		int len;
		auto rc = unqlite_config(_db, UNQLITE_CONFIG_ERR_LOG, &error_log, &len);
		if (rc == UNQLITE_OK) {
			*error_text = std::string_view(error_log, len);
		} else {
			*error_text = "Db is currupted. Can't fill `error_text`.";
		}
	}

	return false;
}
inline bool db::process_read_rc(int rc, db_kv_read_status* status_out, std::string_view* error_text) const noexcept {
	db_kv_read_status status;
	if (rc == UNQLITE_OK) {
		if (status_out) {
			*status_out = db_kv_read_status::OK;
		}

		return true;
	} else if (rc == UNQLITE_NOTFOUND) {
		status = db_kv_read_status::NOTFOUND;
	} else if (rc == UNQLITE_BUSY) {
		status = db_kv_read_status::BUSY;
	} else if (rc == UNQLITE_IOERR) {
		status = db_kv_read_status::IOERR;
	} else if (rc == UNQLITE_NOMEM) {
		status = db_kv_read_status::NOMEM;
	} else /*(rc == UNQLITE_ABORT)*/ {
		status = db_kv_read_status::ABORT;
	}
	if (status_out) {
		*status_out = status;
	}
	if (error_text) {
		const char* error_log;
		int len;
		auto rc = unqlite_config(_db, UNQLITE_CONFIG_ERR_LOG, &error_log, &len);
		if (rc == UNQLITE_OK) {
			*error_text = std::string_view(error_log, len);
		} else {
			*error_text = "Db is currupted. Can't fill `error_text`.";
		}
	}

	return false;
}
inline bool db::process_vm_compile_rc(int rc, db_compilation_status* status_out,
    std::string_view* error_text) const noexcept {
	db_compilation_status status;
	if (rc == UNQLITE_OK) {
		if (status_out) {
			*status_out = db_compilation_status::OK;
		}

		return true;
	} else if (rc == UNQLITE_COMPILE_ERR) {
		status = db_compilation_status::COMPILE_ERR;
	} else if (rc == UNQLITE_IOERR) {
		status = db_compilation_status::IOERR;
	} else /*if(rc == UNQLITE_VM_ERR)*/ {
		status = db_compilation_status::VM_ERR;
	}
	if (error_text) {
		const char* error_log;
		int len;
		auto rc = unqlite_config(_db, UNQLITE_CONFIG_JX9_ERR_LOG, &error_log, &len);
		if (rc == UNQLITE_OK) {
			*error_text = std::string_view(error_log, len);
		} else {
			*error_text = "Db is currupted. Can't fill `error_text`.";
		}
	}

	return false;
}

inline bool db::begin(db_transaction_status* status, std::string_view* error_text) noexcept {
	auto rc = unqlite_begin(_db);

	return process_transaction_rc(rc, status, error_text);
}
inline bool db::rollback(db_transaction_status* status, std::string_view* error_text) noexcept {
	auto rc = unqlite_rollback(_db);

	return process_transaction_rc(rc, status, error_text);
}
inline bool db::commit(db_transaction_status* status, std::string_view* error_text) noexcept {
	auto rc = unqlite_commit(_db);

	return process_transaction_rc(rc, status, error_text);
}

inline bool db::store(std::string_view key, std::string_view data, db_kv_write_status* status,
    std::string_view* error_text) noexcept {
	return store(key, data.data(), data.size(), status, error_text);
}
inline bool db::store(std::string_view key, const void* data, std::size_t size, db_kv_write_status* status,
    std::string_view* error_text) noexcept {
	auto rc = unqlite_kv_store(_db, key.data(), key.length(), data, size);

	return process_write_rc(rc, status, error_text);
}
inline bool db::append(std::string_view key, const void* data, std::size_t size, db_kv_write_status* status,
    std::string_view* error_text) noexcept {
	auto rc = unqlite_kv_append(_db, key.data(), key.length(), data, size);

	return process_write_rc(rc, status, error_text);
}
inline bool db::append(std::string_view key, std::string_view data, db_kv_write_status* status,
    std::string_view* error_text) noexcept {
	auto rc = unqlite_kv_append(_db, key.data(), key.length(), data.data(), data.size());

	return process_write_rc(rc, status, error_text);
}
inline bool db::remove(std::string_view key, db_kv_write_status* status, std::string_view* error_text) noexcept {
	auto rc = unqlite_kv_delete(_db, key.data(), key.length());

	return process_write_rc(rc, status, error_text);
}

template<class Fn>
bool db::fetch_callback(std::string_view key, Fn fn, db_kv_read_status* status,
    std::string_view* error_text) const noexcept {
	return process_read_rc(unqlite_kv_fetch_callback(
	                           _db, key.data(), key.size(),
	                           [](const void* data, unsigned int size, void* rfn) noexcept -> int {
		                           auto fn = *reinterpret_cast<Fn*>(rfn);
		                           return fn(data, static_cast<std::size_t>(size)) ? UNQLITE_OK : UNQLITE_ABORT;
	                           },
	                           &fn),
	    status, error_text);
}
inline std::optional<std::string> db::fetch_string(std::string_view key, db_kv_read_status* status,
    std::string_view* error_text) const noexcept {
	std::string res;
	if (fetch_callback(key, [&res](const void* data, std::size_t size) {
		    res.resize(size);
		    auto begin = static_cast<const char*>(data);
		    res.assign(begin, begin + size);
		    return true;
	    })) {
		return res;
	} else {
		return {};
	}
}
inline std::optional<std::vector<unsigned char>> db::fetch_vector(std::string_view key, db_kv_read_status* status,
    std::string_view* error_text) const noexcept {
	std::vector<unsigned char> res;
	if (fetch_callback(key, [&res](const void* data, std::size_t size) {
		    res.resize(size);
		    auto begin = static_cast<const unsigned char*>(data);
		    res.assign(begin, begin + size);

		    return true;
	    })) {
		return res;
	} else {
		return {};
	}
}

inline std::optional<kv_cursor> db::make_kv_cursor(db_make_kv_cursor_status* status) noexcept {
	unqlite_kv_cursor* cursor;
	auto rc = unqlite_kv_cursor_init(_db, &cursor);
	if (rc == UNQLITE_OK) {
		return kv_cursor(_db, cursor);
	} else {
		if (status) {
			if (rc == UNQLITE_ABORT) {
				*status = db_make_kv_cursor_status::ABORT;
			} else if (rc == UNQLITE_NOTIMPLEMENTED) {
				*status = db_make_kv_cursor_status::NOTIMPLEMENTED;
			} else if (rc == UNQLITE_NOMEM) {
				*status = db_make_kv_cursor_status::NOMEM;
			} else /*if (rc == UNQLITE_CORRUPT)*/ {
				*status = db_make_kv_cursor_status::CORRUPT;
			}
		}
		return {};
	}
}

#ifdef UNQLITEPP_ALLOW_EXCEPTIONS
inline db::db(const std::string& filename, unsigned int mode) {
	unqlite* db;
	auto res = unqlite_open(&db, filename.c_str(), mode);
	if (res == UNQLITE_OK) {
		_db = db;
		return;
	}
	throw up::exception_with_status<db_make_status>(db_make_status::NOMEM);
}

inline vm db::return_vm_or_throw(std::optional<vm> vm, db_compilation_status status, std::string_view error_text) {
	if (vm) {
		return std::move(*vm);
	} else {
		throw up::exception_with_status(status, error_text);
	}
}

inline vm db::compile_or_throw(std::string_view code) {
	db_compilation_status status;
	std::string_view text;
	return return_vm_or_throw(db::compile(code, &status, &text), status, text);
}

inline vm db::compile_file_or_throw(const std::string& filepath) {
	db_compilation_status status;
	std::string_view text;
	return return_vm_or_throw(db::compile_file(filepath, &status, &text), status, text);
}

inline void db::store_or_throw(std::string_view key, std::string_view data) {
	db_kv_write_status status;
	std::string_view text;
	if (store(key, data, &status, &text)) {
		throw up::exception_with_status(status, text);
	}
}
inline void db::store_or_throw(std::string_view key, const void* data, std::size_t size) {
	db_kv_write_status status;
	std::string_view text;
	if (store(key, data, size, &status, &text)) {
		throw up::exception_with_status(status, text);
	}
}
inline void db::append_or_throw(std::string_view key, const void* data, std::size_t size) {
	db_kv_write_status status;
	std::string_view text;
	if (append(key, data, size, &status, &text)) {
		throw up::exception_with_status(status, text);
	}
}
inline void db::append_or_throw(std::string_view key, std::string_view data) {
	db_kv_write_status status;
	std::string_view text;
	if (append(key, data, &status, &text)) {
		throw up::exception_with_status(status, text);
	}
}
inline void db::remove_or_throw(std::string_view key) {
	db_kv_write_status status;
	std::string_view text;
	if (remove(key, &status, &text)) {
		throw up::exception_with_status(status, text);
	}
}

template<class Fn>
bool db::fetch_callback_or_throw(std::string_view key, Fn fn) const {
	db_kv_read_status status;
	std::string_view error_text;
	std::exception_ptr exception;
	auto user_data = std::make_pair(&fn, &exception);
	using user_data_type = decltype(user_data);
	auto rc = process_read_rc(unqlite_kv_fetch_callback(
	                              _db, key.data(), key.size(),
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
		}
		if (status == db_kv_read_status::ABORT || status == db_kv_read_status::NOTFOUND) {
			return false;
		} else {
			throw up::exception_with_status(status, error_text);
		}
	}
}

inline std::optional<std::string> db::fetch_string_or_throw(std::string_view key) const {
	std::string res;
	if (fetch_callback_or_throw(key, [&res](const void* data, std::size_t size) {
		    res.resize(size);
		    auto begin = static_cast<const char*>(data);
		    res.assign(begin, begin + size);
		    return true;
	    })) {
		return res;
	} else {
		return {};
	}
}

inline std::optional<std::vector<unsigned char>> db::fetch_vector_or_throw(std::string_view key) const {
	std::vector<unsigned char> res;
	if (fetch_callback_or_throw(key, [&res](const void* data, std::size_t size) {
		    res.resize(size);
		    auto begin = static_cast<const unsigned char*>(data);
		    res.assign(begin, begin + size);

		    return true;
	    })) {
		return res;
	} else {
		return {};
	}
}

inline kv_cursor db::make_kv_cursor_or_throw() {
	db_make_kv_cursor_status status;
	auto cursor = make_kv_cursor(&status);
	if (cursor) {
		return std::move(*cursor);
	} else {
		throw up::exception_with_status(status);
	}
}

#endif

} // namespace up


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

	std::optional<vm_value> fetch(std::string_view collection) noexcept {
		std::optional<vm_value> res;
		vm.bind("collection", collection);
		if (vm.exec()) {
			if (auto record = vm.extract("record")) {
				res = std::move(record);
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

// #include "db.hpp"

// #include "exception.hpp"

// #include "kv_cursor.hpp"

// #include "statuses.hpp"

// #include "utils.hpp"
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


#include <sstream>

namespace up {

namespace detail {

// TODO: rewrite
template<class V>
void to_json_string(const V& value, std::stringstream& s, std::size_t folding, std::size_t spaces_per_fold) noexcept {
	if (value.is_bool()) {
		s << value.get_bool();
	} else if (value.is_int()) {
		s << value.get_int();
	} else if (value.is_double()) {
		s << value.get_double();
	} else if (value.is_string()) {
		s << "\"" << value.get_string() << "\"";
	} else if (value.is_resource()) {
		s << value.get_resource();
	} else if (value.is_array()) {
		if (value.size()) {
			s << (spaces_per_fold ? "[\n" : "[");
			value.foreach_array([&](std::size_t, const V& e) {
				s << std::string((folding + 1) * 2 * spaces_per_fold, ' ');
				to_json_string(e, s, folding + 1, spaces_per_fold);
				s << (spaces_per_fold ? ",\n" : ", ");

				return true;
			});
			s.seekp(-2, std::ios_base::cur);
			if (spaces_per_fold) {
				s << "\n" << std::string((folding * 2) * spaces_per_fold, ' ');
			}
			s << "]";
		} else {
			s << "[]";
		}
	} else if (value.is_object()) {
		if (value.size()) {
			s << (spaces_per_fold ? "{\n" : "{");
			value.foreach_object([&](const std::string& key, const V& e) {
				s << std::string((folding + 1) * 2 * spaces_per_fold, ' ');
				s << "\"" << key << "\": ";
				to_json_string(e, s, folding + 1, spaces_per_fold);
				s << (spaces_per_fold ? ",\n" : ", ");

				return true;
			});
			s.seekp(-2, std::ios_base::cur);
			if (spaces_per_fold) {
				s << "\n" << std::string((folding * 2) * spaces_per_fold, ' ');
			}
			s << "}";
		} else {
			s << "{}";
		}
	} else {
		s << "null";
	}
}
} // namespace detail

template<class V>
std::string to_json_string(const V& value, std::size_t folding = 2) {
	std::stringstream ss;
	detail::to_json_string(value, ss, 0, folding);
	return ss.str();
}
} // namespace up

// #include "value.hpp"

// #include "vm.hpp"

// #include "vm_value.hpp"

