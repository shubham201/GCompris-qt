/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtCore/qmath.h>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtCore/QLocale>

#include <QSettings>
#include <QStandardPaths>
#include "ApplicationSettings.h"
#include <QDebug>

#define GC_DEFAULT_LOCALE "en_US.UTF-8"

static const QString GENERAL_GROUP_KEY = "General";
static const QString FULLSCREEN_KEY = "fullscreen";
static const QString ENABLE_AUDIO_KEY = "enableSounds";
static const QString ENABLE_EFFECTS_KEY = "enableEffects";

ApplicationSettings::ApplicationSettings(QObject *parent): QObject(parent),
     m_config(QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation) + "/gcompris/GCompris.conf", QSettings::IniFormat)

{
    m_config.beginGroup(GENERAL_GROUP_KEY);
    // Default values if file does not exist
    if(!m_config.contains(FULLSCREEN_KEY)) {
        m_config.setValue(ENABLE_EFFECTS_KEY, true);

#if defined(Q_OS_ANDROID) || defined(Q_OS_IOS) || defined(Q_OS_BLACKBERRY)
        m_config.setValue(FULLSCREEN_KEY, true);
#else
        m_config.setValue(FULLSCREEN_KEY, false);
#endif

        m_config.setValue(ENABLE_AUDIO_KEY, true);
         // Todo get locale, if "C", put default locale
        m_config.setValue("locale", GC_DEFAULT_LOCALE);
        m_config.sync();
    }

    m_isEffectEnabled = m_config.value(ENABLE_EFFECTS_KEY).toBool();
    m_isFullscreen = m_config.value(FULLSCREEN_KEY).toBool();
    m_isAudioEnabled = m_config.value(ENABLE_AUDIO_KEY).toBool();
    m_locale = m_config.value("locale").toString();

    m_config.endGroup();
    connect(this, SIGNAL(audioEnabledChanged()), this, SLOT(notifyAudioEnabledChanged()));
    connect(this, SIGNAL(fullscreenChanged()), this, SLOT(notifyFullscreenChanged()));
    connect(this, SIGNAL(localeChanged()), this, SLOT(notifyLocaleChanged()));
}

void ApplicationSettings::notifyAudioEnabledChanged()
{
    // Save in config
    m_config.beginGroup(GENERAL_GROUP_KEY);
    m_config.setValue(ENABLE_AUDIO_KEY, m_isAudioEnabled);
    m_config.endGroup();
    qDebug() << "notifyAudio: " << m_isAudioEnabled;
    m_config.sync();
}

void ApplicationSettings::notifyLocaleChanged()
{
    // Save in config
    m_config.beginGroup(GENERAL_GROUP_KEY);
    m_config.setValue("locale", m_locale);
    m_config.endGroup();
    qDebug() << "new locale: " << m_locale;
    m_config.sync();
}

void ApplicationSettings::notifyFullscreenChanged()
{
    // Save in config
    m_config.beginGroup(GENERAL_GROUP_KEY);
    m_config.setValue(FULLSCREEN_KEY, m_isFullscreen);
    m_config.endGroup();
    qDebug() << "fullscreen set to: " << m_isFullscreen;
    m_config.sync();
}
