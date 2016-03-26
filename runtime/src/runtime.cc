//===-- yeti/runtime.cc -----------------------------------*- mode: C++ -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//

#include "yeti.h"
#include "yeti/runtime/standard_application.h"

#include <stdlib.h>
#include <stdio.h>

// TODO(mtwilliams): Install assertion handler.
// TODO(mtwilliams): Install error handler.

int main(int argc, const char *argv[]) {
  ::setlocale(LC_ALL, "en_US.UTF-8");

  // Let the games begin.
  yeti::runtime::StandardApplication app;
  app.run();

  return EXIT_FAILURE;
}
