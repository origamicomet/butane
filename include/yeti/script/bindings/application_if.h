//===-- yeti/script/bindings/application_if.h -----------*- mode: C++11 -*-===//
//
//                             __ __     _   _
//                            |  |  |___| |_|_|
//                            |_   _| -_|  _| |
//                              |_| |___|_| |_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_SCRIPT_BINDINGS_APPLICATION_IF_H_
#define _YETI_SCRIPT_BINDINGS_APPLICATION_IF_H_

#include "yeti/script/binding.h"

namespace yeti {

// See `yeti/application.h`.
class Application;

namespace application_if {

/// \internal
extern YETI_PRIVATE Application *instance(lua_State *L);

/// ...
extern YETI_PUBLIC void expose(Script *script, Application *app);

} // application_if

} // yeti

#endif // _YETI_SCRIPT_BINDINGS_APPLICATION_IF_H_