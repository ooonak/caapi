#include "clientutils.hpp"

int main() {
  x509Conf cnf;
  cnf.commonName = "www.xyz.ibm";

  ClientUtils cu;
  cu.makeCsr(cnf);

  return 0;
}
