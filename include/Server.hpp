#ifndef SERVER_HPP
# define SERVER_HPP

#include "common.hpp"
#include "Client.hpp"
#include "Config.hpp"

class Server
{
	public:
		Server();
		Server(Config &conf);
		Server(const Server &ref);
		Server	&operator=(const Server &ref);
		~Server(void);

		//listen_sockets	listen_sock();
		int				id();

		bool	is_connected(int fd);
		bool	is_listen(int fd, int *port);
		void	add_to_epoll(int epoll_fd);
		void	init_listen(int_v ports);
		void	close_fd();
		Config	*conf();
	private:
		Config				*_conf;
		listen_sockets		_ls;
		int					_id;
};

#endif