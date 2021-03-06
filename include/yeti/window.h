//===-- yeti/window.h -----------------------------------*- mode: C++11 -*-===//
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

#ifndef _YETI_WINDOW_H_
#define _YETI_WINDOW_H_

#include "yeti/core.h"

namespace yeti {

/// ...
class YETI_PUBLIC Window {
 YETI_DISALLOW_COPYING(Window)

 private:
  Window();
  ~Window();

 public:
  struct Description {
    /// ...
    const char *title;

    /// ...
    struct {
      /// ...
      u32 width;
      /// ...
      u32 height;
    } dimensions;

    /// ...
    bool resizeable;

    /// ...
    bool closable;
  };

 public:
  struct Event {
    enum Type {
      CLOSED = 1,
    };

    Type type;
  };

  typedef void (*EventHandler)(Window *window, const Event &event, void *ctx);

 public:
  // TODO(mtwilliams): Document this interface.
  static Window *open(const Window::Description &desc);

  void update(EventHandler event_handler,
              void *event_handler_ctx = NULL);

  void close();

  // TODO(mtwilliams): Document this interface.
  void show();
  void hide();

  // TODO(mtwilliams): Document this interface.
  void minimize();
  void maximize();

  void restore();

  // TODO(mtwilliams): Provide a platform agnostic interface for setting the
  // window's cursor, including the ability to hide the cursor.
  void cursor();
  void set_cursor();

  // TODO(mtwilliams): Document this interface.
  bool clip();
  bool unclip();

 public:
  // TODO(mtwilliams): Document this interface.
  void title(char title[256]) const;
  void set_title(const char *new_title);

  // TODO(mtwilliams): Document this interface.
  void dimensions(u32 *width, u32 *height) const;
  void resize(u32 new_width, u32 new_height);

  // TODO(mtwilliams): Document this interface.
  bool closable() const;
  void set_closable(bool closable);

  // TODO(mtwilliams): Document this interface.
  bool keyboard_focus() const;
  bool set_keyboard_focus(bool new_keyboard_focus);

  // TODO(mtwilliams): Document this interface.
  bool mouse_focus() const;
  bool set_mouse_focus(bool new_mouse_focus);

 public:
  // TODO(mtwilliams): Document this interface.
  uintptr_t to_native_hndl() const;

 private:
  uintptr_t internal_hndl_;
  uintptr_t native_hndl_;
  bool is_resizeable_;
  bool is_closable_;
};

} // yeti

#endif // _YETI_WINDOW_H_
