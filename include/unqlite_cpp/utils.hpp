
#pragma once

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
