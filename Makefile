NAME = webserv


CONF_PARSING_PATH 	= srcs/config/
CONF_PARSING_FILES 	= config_file.cpp location_utils.cpp locationBloc.cpp \
					paramFound.cpp utils_config.cpp server_utils.cpp init.cpp \
					exit.cpp cgi.cpp
SRCS_CONF_PARSING	= ${addprefix ${CONF_PARSING_PATH}, ${CONF_PARSING_FILES}}

SRCS_DEBUG_PARSING	= srcs/debug/ft_debug_config.cpp

GNL_PATH 			= srcs/config/get_next_line/
GNL_FILES 			= get_next_line.cpp get_next_line_utils.cpp
SRCS_GNL 			= ${addprefix ${GNL_PATH}, ${GNL_FILES}}

REQUESTS_PATH 		= srcs/requests/
REQUESTS_FILES 		= Request.cpp utils_request.cpp
SRCS_REQUESTS 		= ${addprefix ${REQUESTS_PATH}, ${REQUESTS_FILES}}

CGI_PATH 			= srcs/cgi/
CGI_FILES 			= Cgi.cpp
SRCS_CGI 			= ${addprefix ${CGI_PATH}, ${CGI_FILES}}

WEBSERV_PATH 		= srcs/webserv/
WEBSERV_FILES 		= webserv.cpp Server.cpp poll.cpp
SRCS_WEBSERV 		= ${addprefix ${WEBSERV_PATH}, ${WEBSERV_FILES}}

RESPONSES_PATH 		= srcs/responses/
RESPONSES_FILES 	= Response.cpp responses_utils.cpp error_msg.cpp
SRCS_RESPONSES 		= ${addprefix ${RESPONSES_PATH}, ${RESPONSES_FILES}}

SRCS = main.cpp ${SRCS_CONF_PARSING} ${SRCS_DEBUG_PARSING} ${SRCS_GNL} ${SRCS_REQUESTS} ${SRCS_WEBSERV} ${SRCS_CGI} ${SRCS_RESPONSES} 

OBJECTS = $(SRCS:.cpp=.o)

CXX = @clang++

CXXFLAGS += -Wall -Wextra -Werror -std=c++98
DEBUG += -Wall -Wextra -Werror -std=c++98 -fsanitize=address -g3

all : $(NAME)
	@echo "\033[0;32m✅ webserv is ready\033[0m"

$(NAME) : $(OBJECTS)
	@${CXX} ${CXXFLAGS} -o ${NAME} $(OBJECTS)

debug : $(OBJECTS)
	@${CXX} ${DEBUG} -o ${NAME} $(OBJECTS)

clean : 
	@/bin/rm -f $(OBJECTS)

fclean : clean
	@/bin/rm -f $(NAME)

re : fclean all
