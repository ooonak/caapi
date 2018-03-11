#include "clientutils.hpp"
#include "cautils.hpp"

int main() {
  /*
  x509Conf cnf;
  cnf.commonName = "www.xyz.ibm";
  ClientUtils cu;
  cu.makeCsr(cnf);
  */

  CaUtils cu("/tmp");
  if (!cu.init())
      return 1;

  return 0;
}
