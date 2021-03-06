#ifndef LOCATION_HPP
# define LOCATION_HPP

# include "../include/common.hpp"
# include <string>

class Location
{
	public:
		Location(str_t block);
		Location(const Location &rhs);
		Location &operator=(const Location &ref);
		~Location();

		str_t 				autoindex() const;
		std::list<str_t>	&index();
		FLAGS				&methods();
		str_t				root() const;
		str_t				cgi_path() const;
		str_t				cgi_extension() const ;
		str_t				route() const ;
		str_t				upload_path() const ;
		FLAGS				flags() const;
		Redir				redir() const;
		std::list<str_t>	&allowed_methods();

		str_t search_config(str_t config, str_t key);

	private:
		std::list<str_t>	_index;
		str_t				_root;
		str_t				_route;
		str_t				_cgi_path;
		str_t				_cgi_extension;
		str_t				_upload_path;
		FLAGS				_methods;
		Redir				_redir;
		FLAGS				_flags;
		
		void set_autoindex(str_t line);
		void set_index(str_t line);
		void set_methods(str_t line);
		void add_method(str_t str);
		void set_cgi_path(str_t line);
		void set_cgi_extension(str_t line);
		void set_root(str_t line);
		void set_upload_pass(str_t line);
		void set_upload_path(str_t line);
		void set_redir(str_t line);
		Location();
};

#endif