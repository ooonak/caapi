#include "client.hpp"

int main() {
  Client *client = new Client("http://localhost:8080/sign");
  client->makeCert();

  return 0;
}
