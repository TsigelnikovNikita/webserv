/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_page_generators.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/18 18:17:41 by imicah            #+#    #+#             */
/*   Updated: 2020/12/21 12:42:53 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"

bytes WebServ::autoindexGenerate(HttpRequest *request, const std::string& path_to_target) {
	bytes			body_response;
	DIR*			directory;
	dirent*			current_file;

	directory = opendir(path_to_target.c_str());
	current_file = readdir(directory);
	current_file = readdir(directory);
	body_response.add("<html><head><title>Index of " + request->getTarget() + "</title></head><body>"
																			  "<h1>Index of " + request->getTarget() +
					  "</h1><hr><pre><a href=\"../\">../</a><br>");
	while ((current_file = readdir(directory)) != nullptr) {
		std::string		file(current_file->d_name);
		if (current_file->d_type == 4)
			file.append("/");
		body_response.add("<a href=\"" + file + "\">" + file + "</a><br>");
	}
	body_response.add("</pre><hr></body></html>");
	return (body_response);
}

bytes	WebServ::generateErrorPage(const std::string& code) {
	return ("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" "
			"content=\"width=device-width, initial-scale=1.0\"><meta http-equiv=\"X-UA-Compatible\" "
			"content=\"ie=edge\"><title>" + code + " " + HttpResponse::message_phrases.at(code) + "</title>"
			"<style>h1, p {text-align: center;}</style></head><body><h1>" + code + " " +
			HttpResponse::message_phrases.at(code) + "</h1><hr><p>WebServ/0.1</p></body></html>");
}
