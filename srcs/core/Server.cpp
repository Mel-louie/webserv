#include "Server.hpp"
#include "utils.hpp"

/*
					.--------------.
					| Constructors |
					'--------------'
*/

Server::Server(){}

Server::Server(Config &conf)
{
	init_listen(conf.ports());
	_conf = &conf;
}

Server::Server(const Server &ref)
{
	_ls = ref._ls;
	_conf = ref._conf;
	_id = ref._id;
}

Server	&Server::operator=(const Server &ref)
{
	if (&ref != this)
	{
		_ls = ref._ls;
		_conf = ref._conf;
		_id = ref._id;
	}
	return (*this);
}

Server::~Server(void)
{
}


/*
					.---------.
					| Getters |
					'---------'
*/

int		Server::id()
{
	return (_id);
}

Config	*Server::conf(void)
{ return (_conf); }


/*
					.----------------.
					| Manage sockets |
					'----------------'
*/

void	Server::close_fd()
{
	for (listen_sockets::iterator it = _ls.begin(); it != _ls.end(); it++)
	{
		close(it->first);
	}
}

bool	Server::is_listen(int fd, int *port)
{
	for (listen_sockets::iterator it = _ls.begin(); it != _ls.end(); it++)
	{
		if (it->first == fd)
		{
			*port = it->second;
			return (true);
		}
	}
	return (false);
}

void	Server::add_to_epoll(int epoll_fd)
{
	event_t	tmp;
	tmp.events = EPOLLIN;
	for (listen_sockets::iterator it = _ls.begin(); it != _ls.end(); it++)
	{
		tmp.data.fd = it->first;
		if (epoll_ctl (epoll_fd, EPOLL_CTL_ADD, it->first, &tmp) == -1) /*intialize interest list*/
			fatal("epoll add listen");
	}
}


/*
					.------.
					| init |
					'------'
*/

void	Server::init_listen(int_v ports)
{
	int first = 1;
	for (int_v::iterator it = ports.begin(); it!= ports.end(); it++)
	{
		sockaddr_in servaddr;
		int			socket_fd;
		servaddr.sin_family = AF_INET;
    	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    	servaddr.sin_port = htons(*it);

		if ((socket_fd = socket(servaddr.sin_family, SOCK_STREAM, 0)) < 0)
       		fatal("could not create socket");

		if ((bind(socket_fd , (sockaddr*) &servaddr, sizeof(servaddr))) < 0)
        	fatal("could not bind");
    
    	if (listen(socket_fd , 10) < 0)
			fatal("can not listen (i am a cisgender man)");

		unlock_socket(socket_fd);
		_ls[socket_fd] = *it;
		if (first && !(first = 0))
			_id = socket_fd;
	}
}