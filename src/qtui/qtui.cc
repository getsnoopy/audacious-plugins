/*
 * qtui.cc
 * Copyright 2014 Michał Lipski
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions, and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions, and the following disclaimer in the documentation
 *    provided with the distribution.
 *
 * This software is provided "as is" and without any warranty, express or
 * implied. In no event shall the authors be liable for any damages arising from
 * the use of this software.
 */

#include <QApplication>

#include <libaudcore/i18n.h>
#include <libaudcore/plugin.h>
#include <libaudcore/plugins.h>

#include "main_window.h"

static MainWindow * window;

static bool init ()
{
    return true;
}

static void cleanup ()
{

}

static void run ()
{
    int dummy_argc = 0;
    QApplication qapp (dummy_argc, 0);

    window = new MainWindow;
    window->show ();

    qapp.exec ();

    delete window;
}

static void show (bool show)
{

}

static void quit ()
{
    qApp->quit();
}

#define AUD_PLUGIN_NAME     N_("Qt Interface")
#define AUD_PLUGIN_INIT     init
#define AUD_PLUGIN_CLEANUP  cleanup
#define AUD_IFACE_RUN       run

#define AUD_IFACE_SHOW      show
#define AUD_IFACE_QUIT      quit

#define AUD_DECLARE_IFACE
#include <libaudcore/plugin-declare.h>
