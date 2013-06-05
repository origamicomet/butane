// This file is part of Butane. See README.md and LICENSE.md for details.
// Copyright (c) 2012 Michael Williams <devbug@bitbyte.ca>

#ifndef _BUTANE_RENDER_CONTEXT_H_
#define _BUTANE_RENDER_CONTEXT_H_

#include <butane/butane.h>
#include <butane/math.h>

namespace butane {
  class SwapChain;
  class RenderTarget;
  class DepthStencilTarget;
  class BUTANE_EXPORT RenderContext final {
    __foundation_trait(RenderContext, non_copyable);

    public:
      struct Command {
        public:
          typedef uint64_t Key;

        public:
          Command()
            : key(0), offset(0)
          {}

        public:
          FOUNDATION_INLINE bool operator==( const Command& cmd ) const
          { return (key == cmd.key); }

          FOUNDATION_INLINE bool operator!=( const Command& cmd ) const
          { return (key != cmd.key); }

          FOUNDATION_INLINE bool operator>( const Command& cmd ) const
          { return (key > cmd.key); }

          FOUNDATION_INLINE bool operator<( const Command& cmd ) const
          { return (key < cmd.key); }

          FOUNDATION_INLINE bool operator>=( const Command& cmd ) const
          { return (key >= cmd.key); }

          FOUNDATION_INLINE bool operator<=( const Command& cmd ) const
          { return (key <= cmd.key); }

        public:
          Key key;
          uintptr_t offset;
      };

    public:
      RenderContext();
      ~RenderContext();

    public:
      void clear(
        const Command::Key key,
        RenderTarget* render_target,
        const Vec4f& to_color );

      void clear(
        const Command::Key key,
        DepthStencilTarget* depth_stencil_target,
        const float to_depth,
        const uint32_t to_stencil );

      void bind_render_and_depth_stencil_views(
        const Command::Key key,
        size_t num_of_render_targets,
        RenderTarget** render_targets,
        DepthStencilTarget* depth_stencil_target );

      void present(
        const Command::Key key,
        SwapChain* swap_chain );

    private:
      void command(
        const Command::Key key,
        const void* cmd,
        size_t cmd_len );

    public:
      FOUNDATION_INLINE const Array<Command>& commands() const
      { return _commands; }

      FOUNDATION_INLINE size_t num_of_commands() const
      { return max(_commands_offset, (size_t)1); }

      FOUNDATION_INLINE const void* stream() const
      { return _stream.raw(); }

    private:
      Array<Command> _commands;
      size_t _commands_offset;
      Array<uint8_t> _stream;
      size_t _stream_offset;
  };
} // butane

#endif // _BUTANE_RENDER_CONTEXT_H_
