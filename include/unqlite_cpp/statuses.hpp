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
