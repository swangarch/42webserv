#include "../includes/webserv.hpp"

int sigRecieved = 0;

int main(int ac, char **av) {
    if (ac == 1 || ac == 2) {
        try {
                log::loginfo(T_INFO, "Webserv starts, reading config.");
                std::string     config_path("config/default.conf");
                if (ac == 2) {
                    config_path = av[1];
                }
                ConfigManager   config_info;
                config_info.creatConfig(config_path);
                std::vector<std::vector<ServerInfo> >    server_grp;
                ConfigManager::handleMultiServer(config_info.getServerSet(), server_grp);

                log::loginfo(T_INFO, "Config OK, launching.");
                webserv server(server_grp);
                server.runWebserv();
        }
        catch (std::exception& e) {
            log::loginfo(T_ERR, e.what());
        }
        log::loginfo(T_INFO, "Webserv is closed.");
    } else {
        log::loginfo(T_ERR, "Usage: " + std::string(av[0]) + " [configure file]");
    }
    return 0;
}