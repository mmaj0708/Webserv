

#include "../../includes/common.hpp"

// CONSTRUCT
Cgi::Cgi(Request req, s_location loc, s_serverConfig validVH, std::string pathFile)
: _req(req), _loc(loc)
{
	std::string tmpBuf;
	int i = 0;
	
	_args = new char*[3];
	_args[0] = strdup(this->_loc.scriptName.c_str());
	_args[1] = strdup(pathFile.c_str());
	_args[2] = NULL;

	_env = new char*[16];
	// CONTENT_LENGTH
	if (this->_req.getMethod() == "GET")
		_env[i++] = strdup("CONTENT_LENGTH=0");
	else if (this->_req.getMethod() == "POST")
	{
		std::stringstream intToString;
		intToString << this->_req.getBody().size();
		tmpBuf = std::string("CONTENT_LENGTH=") + intToString.str();
		_env[i++] = strdup(tmpBuf.c_str());
	}
	// CONTENT_TYPE
	if (this->_req.getMethod() == "POST")
	{
		tmpBuf = "CONTENT_TYPE=" + this->_req.getSpecificHeaderValue("CONTENT-TYPE");
		_env[i++] = strdup(tmpBuf.c_str());
	}
	// GATEWAY_INTERFACE
	_env[i++] = strdup("GATEWAY_INTERFACE=CGI/1.1");
	// PATH_INFO
	tmpBuf = "PATH_INFO=" + pathFile;
	_env[i++] = strdup(tmpBuf.c_str());
	// PATH_TRANSLATED
	tmpBuf = "PATH_TRANSLATED=" + pathFile;
	_env[i++] = strdup(tmpBuf.c_str());
	// REDIRECT_STATUS
	_env[i++] = strdup("REDIRECT_STATUS=200");
	// REQUEST_METHOD
	if (this->_req.getMethod() == "GET")
		_env[i++] = strdup("REQUEST_METHOD=GET");
	else if (this->_req.getMethod() == "POST")
		_env[i++] = strdup("REQUEST_METHOD=POST");
	// REQUEST_URI
	tmpBuf = "REQUEST_URI=" + pathFile;
	_env[i++] = strdup(tmpBuf.c_str());
	// SERVER_NAME
	_env[i++] = strdup("SERVER_NAME=servername");
	// SERVER_PORT
	std::stringstream portToString;
	portToString << validVH.port;
	tmpBuf = "SERVER_PORT=" + portToString.str();
	_env[i++] = strdup(tmpBuf.c_str());
	// SERVER_PROTOCOL
	_env[i++] = strdup("SERVER_PROTOCOL=HTTP/1.1");
	// SERVER_SOFTWARE
	_env[i++] = strdup("SERVER_SOFTWARE=Webnul");
	// QUERY_STRING
	tmpBuf = "QUERY_STRING=" + this->_req.getQuery();
	_env[i++] = strdup(tmpBuf.c_str());
	// REMOTE_ADDR
	tmpBuf = "REMOTE_ADDR=" + validVH.ip;
	_env[i++] = strdup(tmpBuf.c_str());
	// DIR_PATH
	tmpBuf = "DIR_PATH=" + this->_loc.root;
	_env[i++] = strdup(tmpBuf.c_str());
	_env[i++] = NULL;
}

// DESTRUCT
Cgi::~Cgi()
{
	int i = 0;
	while (_args[i])
	{
		free(_args[i]);
		i++;
	}
	delete [] _args;

	i = 0;
	while (_env[i])
	{
		free(_env[i]);
		i++;
	}
	delete [] _env;
}

// MEMBER FCTs
std::string Cgi::execCgi()
{
	int fdOut[2];
	int fdIN[2];
	std::string msgbody;
	
	if (pipe(fdOut) < 0 || pipe(fdIN) < 0)
		std::cout << "pipe failed in executeCGI method" << std::endl;

	pid_t pid = fork();
	
	if (!pid){ // in child
		
		// stdout is now a copy of fdOut[1] and in case post method, stdin is a copy of fdIn[0]
		dup2(fdOut[1], STDOUT_FILENO);
		close(fdOut[0]);
		close(fdOut[1]);

		dup2(fdIN[0], STDIN_FILENO);
		close(fdIN[0]);
		close(fdIN[1]);

		// change the repo into where the program is
		chdir(this->_loc.root.c_str());
	
		if (execve(this->_args[0], this->_args, this->_env) < 0){
			exit(-1);
		}
	}
	else if (pid > 0){ // in parent
		
		close(fdOut[1]);
		if (this->_req.getMethod() == "POST"){
			if (write(fdIN[1], this->_req.getBody().c_str(), this->_req.getBody().size()) < 0)
				std::cout << "write failed in executeCGI method" << std::endl;
		}
		else
			if (write(fdIN[1], _getBuffFile.c_str(), _getBuffFile.size()) < 0)
				std::cout << "write failed in executeCGI method" << std::endl;
		close(fdIN[1]);
		close(fdIN[0]);
		
        // Checking if execve correctly worked
        int status = 0;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status) && WEXITSTATUS(status) == -1)
			std::cerr << "error: execve failed in executeCGI method" << std::endl;
		char buf[CGI_PIPE_BUFFER_SIZE + 1] = {0};
		while (read(fdOut[0], buf, CGI_PIPE_BUFFER_SIZE) > 0)
		{
			msgbody += buf;
			memset(buf, 0, CGI_PIPE_BUFFER_SIZE + 1);
		}
		msgbody += buf;

		close(fdOut[0]);
	}
	else{
		close(fdOut[1]);
		close(fdOut[0]);
		if (this->_req.getMethod() == "POST")
		{
			close(fdIN[0]);
			close(fdIN[1]);
		}
		std::cout << "fork failed executeCGI method" << std::endl;
	}
	return msgbody;
}
