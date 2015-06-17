/*
 * ui_skinned_window.c
 * Copyright 2011 John Lindgren
 *
 * This file is part of Audacious.
 *
 * Audacious is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, version 2 or version 3 of the License.
 *
 * Audacious is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * Audacious. If not, see <http://www.gnu.org/licenses/>.
 *
 * The Audacious team does not consider modular code linking to Audacious or
 * using our public API to be a derived work.
 */

#include "window.h"
#include "plugin.h"
#include "skins_cfg.h"

#if 0
void Window::apply_shape ()
{
    gdk_window_shape_combine_region (gtk_widget_get_window (gtk ()),
     m_is_shaded ? m_sshape : m_shape, 0, 0);
}

void Window::realize ()
{
    apply_shape ();
}
#endif

bool Window::button_press (QMouseEvent * event)
{
    /* pass double clicks through; they are handled elsewhere */
    if (event->button () != Qt::LeftButton || event->type () == QEvent::MouseButtonDblClick)
        return false;

    if (m_is_moving)
        return true;

    dock_move_start (m_id, event->globalX (), event->globalY ());
    m_is_moving = true;
    return true;
}

bool Window::button_release (QMouseEvent * event)
{
    if (event->button () != Qt::LeftButton)
        return false;

    m_is_moving = false;
    return true;
}

bool Window::motion (QMouseEvent * event)
{
    if (! m_is_moving)
        return true;

    dock_move (event->globalX (), event->globalY ());
    return true;
}

bool Window::close ()
{
    skins_close ();
    return true;
}

Window::~Window ()
{
    dock_remove_window (m_id);

#if 0
    if (m_shape)
        gdk_region_destroy (m_shape);
    if (m_sshape)
        gdk_region_destroy (m_sshape);
#endif
}

Window::Window (int id, int * x, int * y, int w, int h, bool shaded) :
    m_id (id),
    m_is_shaded (shaded)
{
    w *= config.scale;
    h *= config.scale;

    if (id == WINDOW_MAIN)
        setWindowFlags (Qt::Window | Qt::FramelessWindowHint);
    else
        setWindowFlags (Qt::Dialog | Qt::FramelessWindowHint);

    move (* x, * y);
    add_input (w, h, true, true);
    setFixedSize (w, h);
    setAttribute (Qt::WA_NoSystemBackground);

    m_normal = new QWidget (this);
    m_normal->resize (w, h);
    m_shaded = new QWidget (this);
    m_shaded->resize (w, h);

    if (shaded)
        m_normal->hide ();
    else
        m_shaded->hide ();

    dock_add_window (id, this, x, y, w, h);
}

void Window::resize (int w, int h)
{
    w *= config.scale;
    h *= config.scale;

    Widget::resize (w, h);
    setFixedSize (w, h);

    m_normal->resize (w, h);
    m_shaded->resize (w, h);

    dock_set_size (m_id, w, h);
}

#if 0
void Window::set_shapes (GdkRegion * shape, GdkRegion * sshape)
{
    if (m_shape)
        gdk_region_destroy (m_shape);
    if (m_sshape)
        gdk_region_destroy (m_sshape);

    m_shape = shape;
    m_sshape = sshape;

    if (gtk_widget_get_realized (gtk ()))
        apply_shape ();
}
#endif

void Window::set_shaded (bool shaded)
{
    if (m_is_shaded == shaded)
        return;

    if (shaded)
    {
        m_normal->hide ();
        m_shaded->show ();
    }
    else
    {
        m_shaded->hide ();
        m_normal->show ();
    }

    m_is_shaded = shaded;

#if 0
    if (gtk_widget_get_realized (gtk ()))
        apply_shape ();
#endif
}

void Window::put_widget (bool shaded, Widget * widget, int x, int y)
{
    x *= config.scale;
    y *= config.scale;

    widget->setParent (shaded ? m_shaded : m_normal);
    widget->move (x, y);
}

void Window::move_widget (bool shaded, Widget * widget, int x, int y)
{
    x *= config.scale;
    y *= config.scale;

    widget->move (x, y);
}
