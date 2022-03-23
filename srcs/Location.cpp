#include "Location.hpp"

/*
*
*/

Location::Location(str_t block)
{
	this->_route = block.substr(0, block.find(" "));
	set_root(search_config(block, "root"));
	set_autoindex(search_config(block, "autoindex"));
	set_index(search_config(block, "index"));
	set_methods(search_config(block, "methods"));
	set_cgi_path(search_config(block, "cgi_path"));
	set_cgi_extension(search_config(block, "cgi_extension"));
	set_upload_pass(search_config(block, "upload_pass"));
	set_upload_path(search_config(block, "upload_path"));
	set_redir(search_config(block, "return"));
	if (_cgi_extension != "" && _cgi_path != "")
		_flags |= LOC_CGI;
}

Location::Location(const Location &ref)
{
	_index = ref._index;
	_root = ref._root;
	_route = ref._route;
	_cgi_path = ref._cgi_path;
	_cgi_extension = ref._cgi_extension;
	_flags = ref._flags;
	_upload_path = ref._upload_path;
	_redir = ref._redir;
}

Location &Location::operator=(const Location &ref)
{
	if (this != &ref)
	{
		//_autoindex = ref._autoindex;
		_index = ref._index;
		_root = ref._root;
		_route = ref._route;
		_cgi_path = ref._cgi_path;
		_cgi_extension = ref._cgi_extension;
		_flags = ref._flags;
		_upload_path = ref._upload_path;
		_redir = ref._redir;
	}
	return (*this);
}

Location::~Location() {}

/*
* Setters
*/

void Location::set_root(str_t line)
{
	if (line == "")
		this->_flags |= LOC_ROOT;
	else
		this->_root = line.substr(line.find(" ") + 1);
}

void Location::set_autoindex(str_t line)
{
	str_t tmp;
	tmp = line.substr(line.find(" ") + 1);
	if (line == "" || tmp == "off");   
	else if (tmp == "on")
		this->_flags |= LOC_AUTO;
	else
		throw str_t("error: bad arguement for autoindex");
}

void Location::set_index(str_t line)
{
	size_t space;

	if (line == "")
		return ;
	space = line.find(" ");
	while (space != str_t::npos)
	{
		if (line.find(" ", space + 1) != str_t::npos)
			this->_index.push_back(line.substr(space + 1, line.find(' ', space + 1) - space - 1));
		else
			this->_index.push_back(line.substr(space + 1));
		space = line.find(' ', space + 1);
		_flags |= LOC_INDEX;
	}
}

void Location::set_methods(str_t line)
{
	size_t space;

	if (line == "")
		return ;
	space = line.find(" ");
	while (space != str_t::npos)
	{
		if (line.find(" ", space + 1) != str_t::npos)
			this->_methods.push_back(line.substr(space + 1, line.find(' ', space + 1) - space - 1));
		else
			this->_methods.push_back(line.substr(space + 1));
		space = line.find(' ', space + 1);
		_flags |= LOC_METHOD;
	}
}

void Location::set_cgi_path(str_t line)
{
   if (line == "")
	   this->_cgi_path = "";
	else
		this->_cgi_path = line.substr(line.find(" ") + 1);
}

void Location::set_cgi_extension(str_t line)
{
   if (line == "")
	   this->_cgi_extension = "";
	else
		this->_cgi_extension = line.substr(line.find(" ") + 1);
}

void Location::set_upload_pass(str_t line)
{
	if (line == "")
		return;
	str_t tmp = line.substr(line.find(" ") + 1);
	if (tmp == "true")
		_flags |= LOC_UPLOAD;
	else if (tmp == "false");
	else
		std::cout << "bad value for upload pass";
}

void Location::set_upload_path(str_t line)
{
	if (line == "" || !(_flags & LOC_UPLOAD))
		return;
	else
	{
		_upload_path = line.substr(line.find(" ") + 1);
		std::cout << _upload_path <<std::endl;
		if (access( _upload_path.c_str(), F_OK ))
			_flags = _flags & ~LOC_UPLOAD; //log: did not find upload directory
	}
}

void Location::set_redir(std::string line)
{
    size_t space_pos;
    size_t space_pos_bis;

    space_pos = 0;
    space_pos_bis = 0;
    if (line == "")
    {
        this->_redir.first = "";
        this->_redir.second = "";
    }
    else
    {
        space_pos = line.find(" ");
        space_pos_bis = line.find(" ", space_pos + 1);
        if (space_pos == std::string::npos || space_pos_bis == std::string::npos ||
            line.find(" ", space_pos_bis + 1) != std::string::npos)
            throw std::string("Error: Wrong number argument for return");
        this->_redir.first = line.substr(space_pos + 1, (space_pos_bis - space_pos - 1));
        this->_redir.second = line.substr(space_pos_bis + 1);
    }
	std::cout << redir().first << "=>" << redir().second << std::endl;	
}

/*
* Getters
*/

std::list<str_t> &Location::index() { return (_index); }

std::list<str_t> &Location::methods() { return (_methods); }

str_t Location::cgi_path() const { return (this->_cgi_path); }

str_t Location::cgi_extension() const { return (this->_cgi_extension); }

str_t Location::root() const { return (this->_root); }

str_t Location::route() const { return (this->_route); }

str_t Location::upload_path() const { return (_upload_path); }

FLAGS	Location::flags() const { return (this->_flags); }

strPair Location::redir() const { return (this->_redir); }

/*
* Member functions
*/

str_t Location::search_config(str_t config, str_t key)
{
	size_t  begin;
	size_t  end;
	size_t  duplicate;
	str_t   line;

	begin = config.find(key, 2);
	if (begin == str_t::npos)
		return ("");
	
	while (config[begin - 2] != ';' && config[begin - 2] != '{' && config[begin - 2] != '}'
			&& begin - 2 != str_t::npos)
	{
		begin = config.find(key, begin + 1);
		if (begin == str_t::npos)
			return ("");
	}

	if (config[begin + key.size()] != ' ')
		throw str_t("error: missing space for " + key);
	
	end = config.find(";", begin);
	duplicate = end;
	
	while ((duplicate = config.find(key, duplicate + 1)) != str_t::npos)
	{
		if (config[duplicate - 2] == ';' || config[duplicate - 2] == '}')
			throw str_t("error: duplicate key: " + key);
	}

	return (line = config.substr(begin, (end - begin)));
}

/*
* Non-member functions
*/

std::ostream& operator<<(std::ostream& os, Location &src)
{
	os << "{" << std::endl;
	// std::list<std::string> index = src.index();

	// for (std::list<std::string>::iterator it = index.begin() ; it != index.end() ; ++it)
	// 	os << "\t\t- " << *it << std::endl;

	os << "\t\t- " << src.redir().first << "=>" << src.redir().second << std::endl;	

	os << "}" << std::endl;
	return (os);
}