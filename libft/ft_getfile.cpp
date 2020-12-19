/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_getfile.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nikita <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/06 02:11:18 by nikita            #+#    #+#             */
/*   Updated: 2020/12/19 11:11:37 by nikita           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "libft.hpp"

std::string		ft_getfile(const char *path) {
	const int 			fd = open(path, O_RDONLY);
	std::string			result;
	int					bytes;
	char				buff[2049];

	while ((bytes = read(fd, &buff, 2048)) > 0) {
		buff[bytes] = 0;
		result.append(buff);
	}
	close(fd);
	return (result);
}
