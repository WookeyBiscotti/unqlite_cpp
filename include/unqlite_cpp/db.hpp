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
#include "kv_cursor.hpp"
#include "statuses.hpp"
#include "vm.hpp"

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

#ifdef UNQLITE_CPP_ALLOW_EXCEPTIONS
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

#ifdef UNQLITE_CPP_ALLOW_EXCEPTIONS
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
