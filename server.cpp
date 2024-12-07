#include <bits/stdc++.h>
#ifdef _WIN32
  #include <winsock2.h>
  #include <windows.h>
  #pragma comment(lib, "Ws2_32.lib")
#else
  #include <unistd.h>
  #include <sys/types.h>
  #include <sys/socket.h>
  #include <arpa/inet.h>
  #include <netdb.h>
  #include <errno.h> 
#endif

using namespace std;

int main() {
  cout << unitbuf; 
  cerr << unitbuf;

  #ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
      cerr << "WSAStartup failed\n";
      return 1;
    }
  #endif

  int server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    cerr << "Failed to create server socket\n";
    return 1;
  }
  
  int reuse = 1;
  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse)) < 0) {
    cerr << "setsockopt failed: " << strerror(errno) << "\n";
    return 1;
  }
  
  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(4221);
  
  if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) != 0) {
    cerr << "Failed to bind to port 4221: " << strerror(errno) << "\n";
    return 1;
  }
  
  if (listen(server_fd, 5) != 0) {
    cerr << "listen failed: " << strerror(errno) << "\n";
    return 1;
  }
  
  struct sockaddr_in client_addr;
  socklen_t client_addr_len = sizeof(client_addr);
  
  while (true) {
    cout << "Waiting for a client to connect...\n";
    int client_fd = accept(server_fd, (struct sockaddr *) &client_addr, &client_addr_len);
    if (client_fd < 0) {
      cerr << "accept failed: " << strerror(errno) << "\n";
      continue;
    }

    cout << "Client connected\n";

    // 🔥 Read the client's request
    char buffer[1024] = {0};
    int bytes_received = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_received <= 0) {
      cerr << "Failed to read request\n";
      close(client_fd);
      continue;
    }

    // 🔍 Extract the request line (the first line of the request)
    string request(buffer);
    size_t first_line_end = request.find("\r\n");
    string request_line = request.substr(0, first_line_end);
    cout << "Request Line: " << request_line << endl;

    // 🔍 Split the request line by spaces
    vector<string> tokens;
    stringstream ss(request_line);
    string token;
    while (ss >> token) tokens.push_back(token);

    string path = (tokens.size() > 1) ? tokens[1] : "";

    cout << "Extracted path: " << path << endl;

    // ✅ Check the path and send the appropriate response
    const char *response;
    if (path == "/") {
      response = "HTTP/1.1 200 OK\r\n\r\n";
    } else {
      response = "HTTP/1.1 404 Not Found\r\n\r\n";
    }

    send(client_fd, response, strlen(response), 0);

    #ifdef _WIN32
      closesocket(client_fd);
    #else
      close(client_fd);
    #endif
  }

  #ifdef _WIN32
    closesocket(server_fd);
    WSACleanup();
  #else
    close(server_fd);
  #endif

  return 0;
}
