#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include "common.hpp"
#include "Request.hpp"
#include "Config.hpp"
#include "Location.hpp"
#include "CGI.hpp"
#include "flags.hpp"
#include "Client.hpp"

class Response
{
	public:
		Response(Request &req, Config *conf, Client *client);
		//Response(const Response &ref);
		//Response	&operator=(const Response &ref);
		~Response(void);
		void 			set_status(unsigned int s);
		unsigned int	status();
		void			send();
		static	strMap							_mimeTypes;
		static	std::map<unsigned int, str_t>	_codes;
		static	str_t							_error_page[2];
		str_t									set_body_cgi(Request req);
	private:
		void	upload_file(Request &req);
		void	select_location(Request &req);
		void	set_body_ress(Request &req, Config *conf);
		void	set_redir();
		void	set_headers(str_t path);
		void	add_header(str_t key, str_t val);
		void	get_error_page();
		bool	cgi_match(str_t uri);
		str_t	add_head();
		strMap	headers();
		Response(void);

		Client									*_client;
		Config									*_conf;
		str_t									_route;
		FLAGS									_flags;
		Location								*_loc;
		unsigned int							_status;
		int										_fd;
		strMap									_headers;
		
		str_t									_index;
		str_t									_head;
		str_t									_body;
};

#endif