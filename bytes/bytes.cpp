/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bytes.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/19 11:41:09 by imicah            #+#    #+#             */
/*   Updated: 2020/12/22 17:26:03 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "bytes.hpp"

void			memcpy(const void* src, void* dst, size_t size) {
	unsigned char*	b_src = (unsigned char*)src;
	unsigned char*	b_dst = (unsigned char*)dst;

	unsigned int*	w_src = (unsigned int*)b_src;
	unsigned int*	w_dst = (unsigned int*)b_dst;

	while (size >= 4) {
		*w_dst++ = *w_src++;
		size -= 4;
	}

	if (size) {
		b_dst = (unsigned char*)w_dst;
		b_src = (unsigned char*)w_src;
		while (size > 0)
		{
			*b_dst++ = *b_src++;
			size--;
		}
	}
}

void			bytes::add(const char* string, size_t i) {
	char*	temp_buff;

	if (!size_) {
		delete [] buffer;
		buffer = bytedup(string, i);
		size_ = i;
	}
	else {
		temp_buff = buffer;
		buffer = new char[size_ + i + 1];
		memcpy(temp_buff, buffer, size_);
		memcpy(string, buffer + size_, i);
		size_ += i;
		buffer[size_] = 0;
		delete []temp_buff;
	}
}

void			bytes::clear() {
	delete []buffer;
	size_ = 0;
	buffer = new char[1];
}

void			bytes::erase(size_t n) {
	if (n >= size_)
		clear();
	else {
		char*	temp_buff = buffer;

		buffer = new char[size_ - n + 1];
		memcpy(temp_buff + n, buffer, size_ - n);
		size_ -= n;
		delete[] temp_buff;
	}
}

bytes&		bytes::operator=(const bytes& string) {
	delete []buffer;
	buffer = bytedup(string, string.size_);
	size_ = string.size_;
	return (*this);
}

size_t		bytes::find(const char* needle) const {
	for (int i = 0; i < size_; ++i) {
		if (buffer[i] == *needle) {
			int 	j;
			for (j = 0; needle[j]; ++j)
				if (buffer[i + j] != needle[j]) break;
			if (!needle[j])	return (i);
		}
	}
	return (-1);
}

size_t		bytes::rfind(const char* needle) const {
	int i = size_;

	i--;
	for (; i >= 0; --i) {
		if (buffer[i] == *needle) {
			int 	j;
			for (j = 0; needle[j]; ++j)
				if (buffer[i + j] != needle[j]) break;
			if (!needle[j])	return (i);
		}
	}
	return (-1);
}

bytes	bytes::substr(size_t i) {
	bytes	result;

	i = (i < size_) ? i : size_;
	result.buffer = bytedup(*this, i);
	result.size_ = i;
	return (result);
}

char*	bytes::bytedup(const char* src, size_t size) {
	char*			dst = new char[size + 1];

	memcpy(src, dst, size);
	dst[size] = 0;
	return (dst);
}

void	bytes::rtrim(size_t n) {
	char*	temp_buff = buffer;

	size_ -= n;
	buffer = new char[size_ + 1];
	buffer = bytedup(temp_buff, size_);
	delete []temp_buff;
}
