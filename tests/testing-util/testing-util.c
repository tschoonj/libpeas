/*
 * testing-util.c
 * This file is part of libpeas
 *
 * Copyright (C) 2011 - Garrett Regier
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU Library General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdlib.h>

#include <glib.h>
#include <girepository.h>

#include "libpeas/peas-engine-priv.h"

#include "testing-util.h"

static PeasEngine *engine = NULL;
static GLogFunc default_log_func;

/* These are warning that just have to happen for testing
 * purposes and as such we don't want to abort on them.
 *
 * Would be nice if we could assert that they were...
 */
static const gchar *allowed_patterns[] = {
  "*does-not-exist*",
  "*lib*loader.so*cannot open shared object file: No such file or directory*",
  "*Could not find 'IAge' in *info-missing-iage.plugin*",
  "*Could not find 'Module' in *info-missing-module.plugin*",
  "*Could not find 'Name' in *info-missing-name.plugin*",
  "*Error loading *info-missing-iage.plugin*",
  "*Error loading *info-missing-module.plugin*",
  "*Error loading *info-missing-name.plugin*",
  "*Could not find loader 'disabled'*",
  "*Could not find loader 'invalid'*",
  "*Bad plugin file: *invalid.plugin*",
  "*Error loading *invalid.plugin*"
};

static void
log_handler (const gchar    *log_domain,
             GLogLevelFlags  log_level,
             const gchar    *message,
             gpointer        user_data)
{
  gint i;

  if ((log_level & G_LOG_LEVEL_DEBUG) != 0 ||
      (log_level & G_LOG_LEVEL_INFO) != 0 ||
      (log_level & G_LOG_LEVEL_MESSAGE) != 0)
    {
      default_log_func (log_domain, log_level, message, user_data);
      return;
    }

  if ((log_level & G_LOG_LEVEL_CRITICAL) != 0 ||
      (log_level & G_LOG_LEVEL_ERROR) != 0)
    {
      goto out;
    }

  for (i = 0; i < G_N_ELEMENTS (allowed_patterns); ++i)
    {
      if (g_pattern_match_simple (allowed_patterns[i], message))
        return;
    }

out:

  default_log_func (log_domain, log_level, message, user_data);

  /* Make sure we abort for warnings */
  abort ();
}

void
testing_util_init (void)
{
  GError *error = NULL;
  static gboolean initialized = FALSE;

  if (initialized)
    return;

  /* Don't always abort on warnings */
  g_log_set_always_fatal (G_LOG_LEVEL_CRITICAL);

  default_log_func = g_log_set_default_handler (log_handler, NULL);

  g_irepository_prepend_search_path (BUILDDIR "/libpeas");

  g_setenv ("PEAS_PLUGIN_LOADERS_DIR", BUILDDIR "/loaders", TRUE);

  g_irepository_require (g_irepository_get_default (), "Peas", "1.0", 0, &error);
  g_assert_no_error (error);

  initialized = TRUE;
}

PeasEngine *
testing_util_engine_new (void)
{
  if (engine != NULL)
    return engine;

  testing_util_init ();

  /* Must be after requiring typelibs */
  engine = peas_engine_new ();

  g_object_add_weak_pointer (G_OBJECT (engine), (gpointer *) &engine);

  peas_engine_add_search_path (engine, BUILDDIR "/tests/plugins",
                                       SRCDIR   "/tests/plugins");

  return engine;
}

void
testing_util_engine_free (PeasEngine *engine_)
{
  /* Incase a test needs to free the engine */
  if (engine != NULL)
    {
      g_object_unref (engine_);

      /* Make sure that at the end of every test the engine is freed */
      g_assert (engine == NULL);
    }
}

int
testing_util_run_tests (void)
{
  int retval;

  retval = g_test_run ();

  /* Cannot call this with atexit() because
   * gcov does not register that it was called.
   */
  peas_engine_shutdown ();

  return retval;
}