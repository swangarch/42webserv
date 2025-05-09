# Webserv

A lightweight, fully custom HTTP/1.1 web server written in **C++98** for the **42 cursus**, built and tested on **Linux systems**.

This project aims to implement core functionalities of a HTTP server such as **Nginx**, with support for multiple hosts, static and dynamic content (via **CGI**), and configurable routing through a custom configuration file.

---

see demo
https://youtu.be/3btqIa93FJ4

## 🌐 Features

- **HTTP/1.1 compliant**
  - Persistent connections (keep-alive by default)
  - Chunked transfer encoding support
- **IO Multiplexing** with `epoll`
- Supports **GET**, **POST**, **HEAD**, and **DELETE** methods
- **CGI** support:
  - Dynamically execute scripts (e.g. Python, PHP, etc.)
  - Configurable CGI extensions and interpreters
  - Proper handling of CGI timeouts, status codes, and stderr output
- **Nginx-like configuration file syntax**
  - Define multiple server blocks
  - Fine-grained control via location blocks
- Support for:
  - Custom **error pages**
  - **Autoindex** directory listings
  - **Alias** and **redirection**
  - **File upload** (with configurable upload paths)
  - Per-location **client body size limits**
- **Cookies** via CGI scripts
- **Timeouts** for client requests and CGI execution
- **Multiple virtual hosts**:
  - Based on `ip:port` and/or `server_name`
  - Shared sockets for different server blocks on the same port
- **Minimal process model**:
  - Single main process handles all connections
  - Forked child processes only for CGI execution

---

## ⚙️ Usage

### Compilation

bash
make        # Build in normal mode
make debug  # Build with verbose debug output

./webserv                     # Use default config (config/default.conf)
./webserv path/to/file.conf  # Use custom configuration
Then open your browser and go to http://localhost:8080 or http://localhost:8081.

🧾 Example Configuration

see config/default.conf.

Note: Always use absolute paths for root, alias, and CGI interpreters.

📁 Project Structure

A test suite is provided with:

A bash script for automated testing of routes, methods, and behaviors

A test web page to http method, upload files, execute CGI scripts and cookies.

You can modify the config/default.conf to point to the test site directory for quick validation.

🔍 Implementation Details
Single-threaded event loop using epoll: scalable and efficient

Each request is parsed and dispatched according to the config file

CGI child processes are spawned only when needed

CGI output is captured via pipes, including error streams

Graceful handling of:

Request timeouts

CGI timeouts and crashes

Malformed requests

Configuration errors

🛠️ Dependencies
No external dependencies

Fully written in C++98

Designed for Linux

📜 License
This project is part of the 42 school curriculum. Educational use only. Write your own server.
