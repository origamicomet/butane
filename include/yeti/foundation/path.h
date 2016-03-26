//===-- yeti/foundation/path.h ----------------------------*- mode: C++ -*-===//
//
//                 _____               _     _   _
//                |   __|___ _ _ ___ _| |___| |_|_|___ ___
//                |   __| . | | |   | . | .'|  _| | . |   |
//                |__|  |___|___|_|_|___|__,|_| |_|___|_|_|
//
//       This file is distributed under the terms described in LICENSE.
//
//===----------------------------------------------------------------------===//
//
// TODO(mtwilliams): Document the purpose of this file.
//
//===----------------------------------------------------------------------===//

#ifndef _YETI_FOUNDATION_PATH_H_
#define _YETI_FOUNDATION_PATH_H_

#include "yeti/config.h"
#include "yeti/linkage.h"

#include "yeti/foundation/types.h"
#include "yeti/foundation/support.h"
#include "yeti/foundation/assert.h"

namespace yeti {
namespace foundation {

namespace path {

/// Preferred path seperator for the current platform.
extern YETI_PUBLIC char seperator();

/// Returns the extension of the @path.
extern YETI_PUBLIC const char *extension(const char *path);

} // fs

} // foundation
} // yeti

#endif // _YETI_FOUNDATION_PATH_H_
