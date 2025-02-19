/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HttpRequest.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/22 15:25:50 by imicah            #+#    #+#             */
/*   Updated: 2020/12/24 17:14:59 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HttpRequest.hpp"

const std::string		HttpRequest::methods[4] = {"GET", "HEAD", "POST", "PUT"};

void	HttpRequest::clear() {
	HttpObject::clear();
	method.clear();
	target.clear();
	chunk_size = -1;
	stage = 0;
}

bytes	HttpRequest::getRequestData() {
	size_t		i = buffer.find("\r\n") + 2;
	bytes		result = buffer.substr(i);

	buffer.erase(i);
	return (result);
}

void	HttpRequest::addDataToRequest(char* data, size_t size) {
	addToBuffer(data, size);
	while (!getBuffer().empty()) {
		switch (getStage()) {
			case parsing_first_line:
				if (getBuffer().find("\r\n") != (size_t)-1)
					parsingFirstLine(getRequestData().c_str());
				else
					return;
			case parsing_headers:
				if (getBuffer().find("\r\n") == 0)
					endOfHeaders();
				else if (getBuffer().find("\r\n") != (size_t)-1)
					parseHeader(getRequestData().c_str());
				else
					return;
				break;
			case parsing_body:
				if (findHeader("content-length"))
					parsingBodyByContentLength();
				else if (findHeader("transfer-encoding"))
					parsingBodyByChunked();
				return;
		}
	}
}

void	HttpRequest::parsingBodyByChunked() {
	while (!getBuffer().empty()) {
		if (getBuffer().find("\r\n") != (size_t)-1) {
			if (chunk_size == -1)
				chunk_size = ft_atoi_hex(getRequestData().c_str());
			else {
				if (chunk_size == 0) {
					setStage(completed);
					buffer.clear();
				}
				else {
					addToBody(getRequestData(), chunk_size);
					chunk_size = -1;
				}
			}
		}
		else
			return;
	}
}

void	HttpRequest::parsingBodyByContentLength() {
	const size_t&	content_length = (size_t)ft_atoi(getHeader("content-length").c_str());

	addToBody(getBuffer());
	buffer.clear();
	if (getBody().size() >= content_length) {
		if (getBody().size() > content_length)
			trimBody(getBody().size() - content_length);
		setStage(completed);
	}
}

void	HttpRequest::parsingFirstLine(std::string line_request) {
	if (std::count(line_request.begin(), line_request.end(), ' ') != 2)
		throw std::string("400");

	std::string		element;
	for (int i = 0; i < 3; ++i) {
		element = line_request.substr(0, line_request.find(' '));
		if (i == 0) {
			if (isValidMethod(element)) setMethod(element);
			else throw std::string("400");
		}
		else if (i == 1)
			setTarget(element);
		else if (element != "HTTP/1.1\r\n" && element != "HTTP/1.0\r\n")
			throw std::string("400");
		line_request.erase(0, line_request.find(' ') + 1);
	}
	setStage(parsing_headers);
}

void	HttpRequest::parseHeader(const std::string& line) {
	size_t		position;
	std::string key;
	std::string value;

	if (std::count(line.begin(), line.end(), ':') < 1)
		throw std::string("400");
	position = line.find(':');
	key = line.substr(0, position);
	position += (line[position + 1] == ' ') ? 2 : 1;
	value = line.substr(position, line.size() - 2 - position);
	std::for_each(key.begin(), key.end(), ft_tolower);
	addHeader(key, value);
}

bool	HttpRequest::isValidMethod(const std::string& method_) {
	for (size_t i = 0; i < 4; ++i)
		if (methods[i] == method_) return (true);
	return (false);
}

void	HttpRequest::endOfHeaders() {
	buffer.erase(2);
	if (!findHeader("host"))
		throw std::string("400");
	else if (getMethod() == "PUT" || getMethod() == "POST") {
		if (!findHeader("content-length") && !findHeader("transfer-encoding"))
			throw std::string("411");
		else if (findHeader("transfer-encoding") && getHeader("transfer-encoding") != "chunked")
			throw std::string("501");
		else
			setStage(parsing_body);
	}
	else
		setStage(completed);
}
