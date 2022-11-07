#include "conection.h"

//LINKS REFERENCIAS:
//https://www.man7.org/linux/man-pages/man2/socket.2.html
//https://man7.org/linux/man-pages/man7/socket.7.html
//https://www.man7.org/linux/man-pages/man3/setsockopt.3p.html
//https://man7.org/linux/man-pages/man2/setsockopt.2.html
//https://linux.die.net/man/3/htons
//https://linux.die.net/man/3/inet_aton
//https://www.howtogeek.com/225487/what-is-the-difference-between-127.0.0.1-and-0.0.0.0/
//https://www.man7.org/linux/man-pages/man2/bind.2.html
//https://www.man7.org/linux/man-pages/man2/accept.2.html

int setup_server(char* IP, int port) {
  int server_socket, client_socket, addr_size;
  struct sockaddr_in server_addr;

  check((server_socket = socket(AF_INET, SOCK_STREAM, 0)), "Error al crear el socket");

  // Se configura el socket a gusto (recomiendo fuertemente el REUSEPORT!)
  int opt = 1;
  int ret = setsockopt(server_socket, SOL_SOCKET, SO_REUSEPORT, &opt, sizeof(opt));

  // Se guardan el puerto e IP en la estructura antes definida
  // memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  inet_aton(IP, &server_addr.sin_addr);
  server_addr.sin_port = htons(port);

  check(bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)), "Error al hacer el bind");
  check(listen(server_socket, 1), "Error al hacer el listen");

  return server_socket;
}

int accept_new_connection(int server_socket) {
  int client_socket;
  struct sockaddr_in client_addr;
  int addr_size = sizeof(struct sockaddr_in);

  check((client_socket = accept(server_socket, (struct sockaddr *)&client_addr, (socklen_t*)&addr_size)), "Error al aceptar la conexión");

  return client_socket;
}

int check(int exp, const char *msg) {
  if (exp == -1) {
    perror(msg);
    exit(1);
  }
  return exp;
}
