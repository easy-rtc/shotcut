/*
 * Copyright (c) 2013-2018 Meltytech, LLC
 * Author: Dan Dennedy <dan@dennedy.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FILTER_H
#define FILTER_H

#include <QObject>
#include <QString>
#include <QVariant>
#include <QRectF>
#include <MltFilter.h>
#include <MltProducer.h>

#include "qmlmetadata.h"
#include "shotcut_mlt_properties.h"

class AbstractJob;

class QmlFilter : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isNew READ isNew)
    Q_PROPERTY(QString path READ path)
    Q_PROPERTY(QStringList presets READ presets NOTIFY presetsChanged)
    Q_PROPERTY(int in READ in WRITE setIn NOTIFY inChanged)
    Q_PROPERTY(int out READ out WRITE setOut NOTIFY outChanged)
    Q_PROPERTY(int animateIn READ animateIn WRITE setAnimateIn NOTIFY animateInChanged)
    Q_PROPERTY(int animateOut READ animateOut WRITE setAnimateOut NOTIFY animateOutChanged)
    Q_PROPERTY(int duration READ duration NOTIFY durationChanged)

public:
    explicit QmlFilter();
    explicit QmlFilter(Mlt::Filter& mltFilter, const QmlMetadata* metadata, QObject *parent = 0);
    ~QmlFilter();

    bool isNew() const { return m_isNew; }
    void setIsNew(bool isNew) { m_isNew = isNew; }

    Q_INVOKABLE QString get(QString name);
    Q_INVOKABLE double getDouble(QString name);
    Q_INVOKABLE QRectF getRect(QString name);
    Q_INVOKABLE void set(QString name, QString value);
    Q_INVOKABLE void set(QString name, double value);
    Q_INVOKABLE void set(QString name, int value);
    Q_INVOKABLE void set(QString name, double x, double y, double width, double height, double opacity = 1.0);
    QString path() const { return m_path; }
    Q_INVOKABLE void loadPresets();
    QStringList presets() const { return m_presets; }
    /// returns the index of the new preset
    Q_INVOKABLE int  savePreset(const QStringList& propertyNames, const QString& name = QString());
    Q_INVOKABLE void deletePreset(const QString& name);
    Q_INVOKABLE void analyze(bool isAudio = false);
    Q_INVOKABLE static int framesFromTime(const QString& time);
    Q_INVOKABLE static QString timeFromFrames(int frames);
    Q_INVOKABLE void getHash();
    Mlt::Producer& producer() { return m_producer; }
    int in();
    void setIn(int value);
    int out();
    void setOut(int value);
    Mlt::Filter& filter() { return m_filter; }
    int animateIn() { return m_filter.get_int(kShotcutAnimInProperty); }
    void setAnimateIn(int value);
    int animateOut() { return m_filter.get_int(kShotcutAnimOutProperty); }
    void setAnimateOut(int value);
    int duration();

public slots:
    void preset(const QString& name);

signals:
    void presetsChanged();
    void analyzeFinished(bool isSuccess);
    void changed(); /// Use to let UI and VUI QML signal updates to each other.
    void inChanged();
    void outChanged();
    void animateInChanged();
    void animateOutChanged();
    void durationChanged();

private:
    const QmlMetadata* m_metadata;
    Mlt::Filter m_filter;
    Mlt::Producer m_producer;
    QString m_path;
    bool m_isNew;
    QStringList m_presets;
    
    QString objectNameOrService();
};

class AnalyzeDelegate : public QObject
{
    Q_OBJECT
public:
    explicit AnalyzeDelegate(Mlt::Filter& filter);

public slots:
    void onAnalyzeFinished(AbstractJob *job, bool isSuccess);

private:
    Mlt::Filter m_filter;
};

#endif // FILTER_H
