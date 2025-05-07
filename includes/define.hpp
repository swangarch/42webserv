#ifndef DEFINE_HPP
#define DEFINE_HPP


#ifdef DEBUG_SHOW
# define DEBUG true
# else
# define DEBUG false
#endif

#define MAX_EVENTS 1024

#define READ_BUFFER_SIZE 16384
#define WRITE_BUFFER_SIZE 16384

#define SOCK_FD 0
#define CLIENT_FD 1
#define EP_FD 2
#define CGI_ERR_FD 3
#define PIPE_FD_WRITE 4
#define PIPE_FD_READ 5

#define NOT_END 0
#define READ_FINISH 1
#define CLIENT_CLOSE 2
#define READ_ERR 3
#define OTHER_ERR 4
#define PARSE_ERR 5
#define CGI_READ_FINISH 6

#define READ_HEAD_FINISH 1
#define READ_HEAD_NOT_END 0

#define CGI_TIMEOUT 5
#define REQ_TIMEOUT 30

#define T_INFO 0
#define T_REQ 1
#define T_RESP 2
#define T_ERR 3

#define NOT_PROCESSED 0
#define PROCESSED 1
#define WAIT_CGI 2

# define MAX_BODY_LENGTH 200000000

# define MAX_SHOW_LENGTH 45

# ifndef COLOR
#  define COLOR

const std::string RED = "\001\033[1;31m\002";
const std::string YELLOW = "\001\033[1;33m\002";
const std::string BLUE = "\001\033[1;34m\002";
const std::string GREEN = "\001\033[1;32m\002";
const std::string COLOR_END = "\001\033[0m\002";
const std::string WHITE_B = "\001\033[1;37m\002";

# endif

extern int sigRecieved;

typedef struct     s_ip
{
    std::string    ip;
    std::string    port;
} t_ip;

#endif