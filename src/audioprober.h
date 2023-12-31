/*
 * SPDX-FileCopyrightText: 2020 Jonah Brüchert <jbb@kaidan.im>
 * SPDX-FileCopyrightText: 2020-2021 Devin Lin <devin@kde.org>
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#pragma once

#include <QAudioProbe>
#include <QObject>
#include <QTimer>
#include <QVariant>
#include <QAudioRecorder>
#include <QMediaPlayer>
#include <QDebug>

class AudioProber : public QAudioProbe
{
    Q_OBJECT
    Q_PROPERTY(QVariantList volumesList READ volumesList NOTIFY volumesListChanged)
    Q_PROPERTY(int animationIndex READ animationIndex NOTIFY animationIndexChanged)
    Q_PROPERTY(int maxVolumes READ maxVolumes WRITE setMaxVolumes NOTIFY maxVolumesChanged)

public:
    AudioProber(QObject *parent = nullptr);
    AudioProber(QObject *parent, QAudioRecorder *source);
    AudioProber(QObject *parent, QMediaPlayer *source);

    void process(QAudioBuffer buffer);
    void processVolumeBar();

    QVariantList volumesList() const;

    int maxVolumes();
    void setMaxVolumes(int m);

    int animationIndex();

    void clearVolumesList();

private:
    void handleRecorderState(QAudioRecorder::State state);
    void handlePlayerState(QMediaPlayer::State state);
    
    int m_audioSum = 0;  //
    int m_audioLen = 0; // used for calculating the value of one volume bar from many
    int m_animationIndex = 0; // which index rectangle is being expanded
    int m_maxVolumes = 100; // based on width of screen

    QVariantList m_volumesList;

    QTimer *volumeBarTimer;
    QAudioRecorder *m_recorderSource;
    QMediaPlayer *m_playerSource;
    
Q_SIGNALS:
    void volumesListAdded(int volume);
    void volumesListChanged();
    void animationIndexChanged();
    void maxVolumesChanged();
    void volumesListCleared();
};
