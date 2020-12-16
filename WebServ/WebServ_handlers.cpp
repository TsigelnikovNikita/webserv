/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   WebServ_handlers.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: imicah <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/12 08:06:21 by imicah            #+#    #+#             */
/*   Updated: 2020/12/16 22:46:01 by imicah           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "WebServ.hpp"
#include <iostream>

void	WebServ::ReadRequest(Client *client) {
	char	buff[512];
	int 	bytes;
	static int	w;

	ft_memset(buff, 0, 512);
	bytes = recv(client->GetSocket(), buff, 512, MSG_TRUNC);
	client->AddToBuffer(buff);
	if (client->GetBuffer().rfind("\r\n\r\n") != std::string::npos)
		client->NextStage();
}


bool WebServ::_CheckError(Response& response, Client *client, const VirtualServer& virtual_server, struct stat& buff,
																					  std::string& path_to_target) {
	const Location&			location = virtual_server.GetLocation(client->GetRequest());
	const Request&			request = client->GetRequest();
	int 					fd;

	if (stat(path_to_target.c_str(), &buff) == -1) {
		response.SetStatusCode("404");
		return (true);
	}
	if (S_ISDIR(buff.st_mode) && request.GetTarget().back() != '/') {
		response.SetStatusCode("301");
		response.AddHeader("Location", "http://" + virtual_server.GetIp() + ":" + virtual_server.GetPort() + request.GetTarget() + "/");
		return (true);
	}
	else if (!location.IsAllowMethod(request.GetMethod())) {
		response.SetStatusCode("405");
		return (true);
	}
	if (S_ISDIR(buff.st_mode) && (fd = open((path_to_target + location.GetIndex()).c_str(), O_RDONLY)) != -1) {
		path_to_target.append(location.GetIndex());
		stat(path_to_target.c_str(), &buff);
		close(fd);
	}
	else if (S_ISDIR(buff.st_mode) && !location.GetAutoindex()) {
		response.SetStatusCode("403");
		return (true);
	}
	return (false);
}

std::string WebServ::_GenerateErrorPage(const std::string& code) const {
	return ("<!DOCTYPE html><html lang=\"en\"><head><meta charset=\"UTF-8\"><meta name=\"viewport\" "
		 	"content=\"width=device-width, initial-scale=1.0\"><meta http-equiv=\"X-UA-Compatible\" "
			"content=\"ie=edge\"><title>Error Page</title><style>h1 {text-align: center;}</style></head><body>"
   			"<h1>" + code + "</h1></body></html>");
}

void WebServ::_SetErrorPage(Response& response, const Location& location, const VirtualServer& virtual_server,
							const Request& request) {
	std::string		path_to_target;

	try {
		path_to_target.append(location.GetPath() + virtual_server.GetErrorPage(response.GetStatusCode()));
		response.AddHeader("Location", "http://" + virtual_server.GetIp() + ":" + virtual_server.GetPort() + path_to_target);
		response.SetStatusCode("302");
	}
	catch (std::exception&) {
		response.SetBody(_GenerateErrorPage(response.GetStatusCode()));
	}
}

void	WebServ::GenerateResponse(Client *client) {
	const VirtualServer&	virtual_server = _GetVirtualServer(client);
	const Location&			location = virtual_server.GetLocation(client->GetRequest());
	const Request&			request = client->GetRequest();
	std::string				path_to_target = _GetPathToTarget(request, location);
	Response				response;
	struct stat				buff;

	if (_CheckError(response, client, virtual_server, buff, path_to_target))
		_SetErrorPage(response, location, virtual_server, request);
	else
		std::cout << "KU";

	client->SetResponse(response);
	client->GetResponse().GenerateResponse();
	client->NextStage();
}

void	WebServ::SendResponse(Client* client) {
	Response&	response = client->GetResponse();

	response.SendResponse(client->GetSocket());
	if (response.GetBuffer().empty()) {
		if (response.GetHeader("Connection") == "close")
			client->SetStage(close_connection_);
		else
			client->SetStage(read_request_);

		client->ClearResponse();
		client->ClearRequest();
		client->ClearBuffer();
	}
}

void	WebServ::CloseConnection(Client* client) {
	close(client->GetSocket());
}
