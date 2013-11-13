/* ========================================================================== */
/*                                                                            */
/* This file is part of Butane.                                               */
/*                                                                            */
/* Author(s):                                                                 */
/*                                                                            */
/*   Michael Williams <devbug@bitbyte.ca>                                     */
/*                                                                            */
/* See LICENSE.md for licensing information.                                  */
/*                                                                            */
/* ========================================================================== */
/*! @file include/butane/application.h
     ...                                                                      */
/* ========================================================================== */

#ifndef _BUTANE_APPLICATION_H_
#define _BUTANE_APPLICATION_H_

#include <butane/config.h>
#include <butane/foundation.h>

/* ========================================================================== */
/*  Application:                                                              */
/*   * C (butane_application_t);                                              */
/*   * C++ (butane::Application).                                             */
/* ========================================================================== */

/* ========================================================================== */
/*  C (butane_application_t):                                                 */
/* ========================================================================== */

#ifdef __cplusplus
extern "C" {
#endif

/*! ... */
typedef void (*butane_application_update_fn)(
  struct butane_application *application,
  const float delta_time);

/*! ... */
typedef void (*butane_application_render_fn)(
  const struct butane_application *application);

/*! ... */
typedef struct butane_application {
  butane_application_update_fn update;
  butane_application_render_fn render;
} butane_application_t;

/* ========================================================================== */

/*! ... */
extern BUTANE_API void butane_application_update(
  butane_application_t *app,
  const float delta_time);

/*! ... */
extern BUTANE_API void butane_application_render(
  const butane_application_t *app);

#ifdef __cplusplus
}
#endif

/* ========================================================================== */
/*  C++ (butane::Application):                                                */
/* ========================================================================== */

#ifdef __cplusplus
namespace butane {
  /*! @copydoc butane_application_t */
  class BUTANE_API Application {
    private:
      Application(const Application &);
      Application& operator= (const Application &);
    protected:
      Application();
      virtual ~Application();
    public:
      /*! @copydoc butane_application_t::update */
      virtual void update(const float delta_time);
      /*! @copydoc butane_application_t::render */
      virtual void render() const;
    private:
      static Application *recover_(::butane_application_t *app);
      static const Application *recover_(const ::butane_application_t *app);
    private:
      static void update_(::butane_application_t *app, const float delta_time);
      static void render_(const ::butane_application_t *app);
    private:
      ::butane_application_t _;
      Application *this_;
  };
} /* butane */
#endif

/* ========================================================================== */

#endif /* _BUTANE_APPLICATION_H_ */
