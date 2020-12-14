/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/21 20:30:05 by nikita            #+#    #+#             */
/*   Updated: 2020/12/09 00:10:01 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() : _autoindex(non_accepted), _root(""), _cgi_pass(""), _proxy_pass(""), _index("index.html") {
	_allow_methods.assign(5, non_accepted);
}

void	Location::add_accepted_method(uint8_t method_index) { _allow_methods[method_index] = accepted; }
void	Location::set_root(const std::string& root) { _root = root; }
void	Location::set_autoindex(bool autoindex) { _autoindex = autoindex; }
void	Location::set_cgi_path(const std::string& cgi_path) { _cgi_pass = cgi_path; }
void	Location::set_index(const std::string& index) { _index = index; }
void	Location::set_location_type(int location_type) { _location_type = location_type; }
void	Location::set_extension(const std::string &extension) {_extension = extension;}

void	Location::erase_accepted_methods() {
	for (int i = 0; i < _allow_methods.size(); ++i)
		_allow_methods[i] = false;
}

const std::vector<bool>&	Location::get_allow_methods() const { return (_allow_methods); }

int						Location::get_location_type() const { return (_location_type); }

const std::string&		Location::get_path() const { return (_path); }
const std::string&		Location::get_root() const { return (_root); }
const std::string&		Location::get_extension() const { return (_extension); }

bool Location::is_allow_method(const std::string& method) const {
	static std::string	methods[NUMBER_METHODS] = {"GET", "HEAD", "POST", "PUT", "DELETE", "OPTIONS"};

	for (int i = 0; i < NUMBER_METHODS; ++i) {
		if (method == methods[i])
			return (_allow_methods[i]);
	}
	return (false);
}

bool Location::is_autoindex() const { return (_autoindex); }

const std::string&	Location::get_index() const { return (_index); }

void Location::set_path(const std::string& path) { _path = path; }

void Location::setProxyPass(const std::string &proxyPass) { _proxy_pass = proxyPass; }
