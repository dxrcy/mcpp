#include "connection.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/un.h>
#include <unistd.h>

#include <sstream>
#include <stdexcept>

namespace mcpp {
SocketConnection::SocketConnection() {
  // Using std libs only to avoid dependency on socket lib
  _socket_handle = socket(AF_UNIX, SOCK_STREAM, 0);
  if (_socket_handle == -1) {
    throw std::runtime_error("Failed to create socket.");
  }

  sockaddr_un server_addr{};
  server_addr.sun_family = AF_UNIX;
  strcpy(server_addr.sun_path, "/tmp/elci-proxy");

  if (connect(_socket_handle, reinterpret_cast<struct sockaddr*>(&server_addr),
              sizeof(server_addr)) < 0) {
    throw std::runtime_error("Failed to connect to the server. Check if the server is running.");
  }
}

void SocketConnection::send(const std::string& data_string) {
  _last_sent = data_string;
  ssize_t result = write(_socket_handle, data_string.c_str(), data_string.length());
  if (result < 0) {
    throw std::runtime_error("Failed to send data.");
  }
}

std::string SocketConnection::recv() const {
  std::stringstream response_stream;
  char buffer[BUFFER_SIZE];

  ssize_t bytes_read;
  do {
    bytes_read = read(_socket_handle, buffer, sizeof(buffer));
    if (bytes_read < 0) {
      throw std::runtime_error("Failed to receive data.");
    }

    response_stream.write(buffer, bytes_read);
  } while (buffer[bytes_read - 1] != '\n');

  std::string response = response_stream.str();

  // Remove trailing \n
  if (!response.empty() && response[response.length() - 1] == '\n') {
    response.pop_back();
  }

  if (response == FAIL_RESPONSE) {
    std::string error_msg = "Server failed to execute command: ";
    error_msg += _last_sent;
    throw std::runtime_error(error_msg);
  }
  return response;
}
} // namespace mcpp
