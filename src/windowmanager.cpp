/* Copyright (C) 2012 John Brooks <john.brooks@dereferenced.net>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "windowmanager.h"
#include "dbusadaptor.h"
#include <QGuiApplication>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QQuickView>
#include <QDBusConnection>
#include <QDebug>
#include <QQuickView>

static WindowManager *wmInstance = 0;

WindowManager *WindowManager::instance(QObject *rootObject, QQuickView *view)
{
    if (!wmInstance)
        wmInstance = new WindowManager(rootObject, qApp, view);
    return wmInstance;
}

WindowManager::WindowManager(QObject *rootObject, QObject *parent, QQuickView *view)
    : QObject(parent)
{
    mRootObject = rootObject;
    pView = view;

    new DBusAdaptor(this);
    QDBusConnection::sessionBus().registerService("org.nemomobile.qmlmessages");
    if (!QDBusConnection::sessionBus().registerObject("/", this)) {
        qWarning() << "Cannot register DBus object!";
    }
}

WindowManager::~WindowManager()
{

}

void WindowManager::showGroupsWindow()
{
    QMetaObject::invokeMethod(mRootObject, "showGroupsList");
    setActiveWindow();
}

void WindowManager::showConversation(const QString &localUid, const QString &remoteUid, unsigned type)
{
    Q_UNUSED(type);
    QMetaObject::invokeMethod(mRootObject, "showConversation", Q_ARG(QVariant, localUid), Q_ARG(QVariant, remoteUid));
    setActiveWindow();
}

void WindowManager::setActiveWindow()
{
    pView->raise();
    pView->showFullScreen();
}

