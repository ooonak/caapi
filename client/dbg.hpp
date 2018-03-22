#ifndef __DBG_HPP__
#define __DBG_HPP__

#include <iostream>
#include <string.h>
#include <string>

#ifndef WITH_DEBUG
#define dbg(M)
#else
#define __FILENAME__                                                           \
  (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define dbg(M)                                                                 \
  std::cout << "[ " << __FILENAME__ << " : " << __LINE__ << " " << __func__    \
            << "() ] " << M << std::endl
#endif

#define err(M)                                                                 \
  std::cerr << "[ " << __FILENAME__ << " : " << __LINE__ << " " << __func__    \
            << "() ] ERROR: " << M << std::endl

#endif // __DBG_HPP__
