#pragma once

#include "statuses.hpp"

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
