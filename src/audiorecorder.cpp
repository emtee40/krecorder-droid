#include "audiorecorder.h"

constexpr int MAX_VOLUME = 1000;

AudioRecorder::AudioRecorder(QObject *parent) : QAudioRecorder(parent)
{
    m_audioProbe = new AudioProber(parent);
    m_audioProbe->setSource(this);
    
    QQmlEngine::setObjectOwnership(m_audioProbe, QQmlEngine::CppOwnership);
    
    // once the file is done writing, save recording to model
    connect(this, &QAudioRecorder::stateChanged, this, &AudioRecorder::handleStateChange);
}

void AudioRecorder::handleStateChange(QAudioRecorder::State state)
{
    if (state == QAudioRecorder::StoppedState) {
        if (resetRequest) {
            // reset
            resetRequest = false;
            QFile(actualLocation().fileName()).remove();
            recordingName = "";
            
        } else {
            // rename file to desired file name
            renameCurrentRecording();
            // create recording
            saveRecording();
        }
        
        // clear volumes list
        m_audioProbe->clearVolumesList();
        
    } else if (state == QAudioRecorder::PausedState) {
        cachedDuration = duration();
    }
}


void AudioRecorder::renameCurrentRecording()
{
    if (recordingName != "") {
        
        // determine new file name
        QStringList spl = actualLocation().fileName().split(".");
        QString suffix = spl.size() > 0 ? spl[spl.size()-1] : "";
        QString path = QStandardPaths::writableLocation(QStandardPaths::MusicLocation) + "/" + recordingName;
        QString updatedPath = path + suffix;
        
        // if the file already exists, add a number to the end
        int cur = 1;
        QFileInfo check(path + suffix);
        while (check.exists()) {
            updatedPath = QString("%1-%2.%3").arg(path, QString::number(cur), suffix);
            check = QFileInfo(updatedPath);
            cur++;
        }
        
        QFile(actualLocation().path()).rename(updatedPath);
     
        savedPath = updatedPath;
        recordingName = "";
    } else {
        savedPath = actualLocation().path();
    }
}

void AudioRecorder::saveRecording() 
{
    // get file name from path
    QStringList spl = savedPath.split("/");
    QString fileName = spl[spl.size()-1].split(".")[0];
    
    RecordingModel::inst()->insertRecording(savedPath, fileName, QDateTime::currentDateTime(), cachedDuration / 1000);
}
