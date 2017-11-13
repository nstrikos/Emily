/*
 * Explanation: Reading the comments from top to the bottom, will give you
 * idea of how the text is converted to wav
*/

#include "downloadmanagerimpl.h"
#include "constants.h"
#include <QBuffer>

DownloadManagerImpl::DownloadManagerImpl()
{
    m_wavReceiver = NULL;
    m_reply = NULL;
    m_file = NULL;
    m_actualBuffers = 0;
    m_delBuffers = 0;
    m_downloading = false;
    m_voice = herculesVoice;
}

void DownloadManagerImpl::setWavReceiver(DownloadManagerIface *wavReceiver)
{
    m_wavReceiver = wavReceiver;
}

void DownloadManagerImpl::textToSpeech(QString text, QString index)
{
    //This is the entry point to the algorithm
    //When new text with a new index is coming,
    //we add them to m_textList and m_indexList
    //and we call processLists()
    m_textList << text;
    m_indexList << index;
    processLists();
}

void DownloadManagerImpl::processLists()
{
    //This function is called when new text arrives
    //or when the processing of a text has finished

    //If no text is being processed
    //we take the first text and index and we process them

    //If text is being process, this function will get called
    //when the processing is finished
    if (!m_downloading)
    {
        if (!m_textList.isEmpty())
        {
            QString text = m_textList.takeFirst();
            QString index = m_indexList.takeFirst();
            performTextToWav(text, index);
        }
    }
}

void DownloadManagerImpl::performTextToWav(QString text, QString index)
{
    //we proceed only if text is not being processed
    if (!m_downloading)
    {
        //we inform the object that text is being processed
        //so incoming texts will have to wait
        m_downloading = true;

        //we store the current index, so we will be able to send to nvda
        m_index = index;

        //we create a new buffer to write wav
        m_file = new QBuffer();
        m_actualBuffers++;

        //we make sure buffer is writable
        if (!m_file->open(QIODevice::WriteOnly))
        {
            delete m_file;
            m_file = NULL;
            return;
        }

        //We start writing the command we will send to http server
        QString part1 = "http://localhost:59125/process?INPUT_TEXT=";
        QString part2 = "&INPUT_TYPE=TEXT&OUTPUT_TYPE=AUDIO&AUDIO=AU_FILE&";
        QString part3 = m_voice;

        QString command = part1 + text + part2 + part3;

        //Let the object know that we have not cancelled the request
        m_httpRequestAborted = false;

        //schedule the request
        startRequest(command);
    }
}

void DownloadManagerImpl::startRequest(QUrl url)
{
    //Send the request to http server
    m_reply = m_qnam.get(QNetworkRequest(url));

    //When the http server has new data, httpReadyRead() gets called
    connect(m_reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));

    //When the request has finished, httpFinished() gets called
    connect(m_reply, SIGNAL(finished()),  this, SLOT(httpFinished()));
}

void DownloadManagerImpl::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    if (m_file)
        m_file->write(m_reply->readAll());
}

void DownloadManagerImpl::httpFinished()
{
    //When the server has sent all data
    //we delete m_reply
    m_reply->deleteLater();
    m_reply = NULL;

    //If the process was cancelled, we stop here
    if (m_httpRequestAborted) {
        return;
    }

    //otherwise we call finishRequest()
    finishRequest();
}

void DownloadManagerImpl::finishRequest()
{
    //When the request is over
    //we let the object know that we dont download anymore
    m_downloading = false;

    //And we send the m_file buffer and m_index to the receiver
    if (m_wavReceiver != NULL)
        m_wavReceiver->handleWav(m_file, m_index);

    //Finally we call processLists to start over
    processLists();
}

void DownloadManagerImpl::cancelDownload()
{
    //When we need to cancel the process
    //First we clear the lists, so the object has no more text to process
    clearLists();

    //And then we send cancel to the http server
    if ( m_reply != NULL)
    {
        m_downloading = false;
        m_httpRequestAborted = true;
        m_reply->abort();
        if (m_file != NULL)
        {
            delete m_file;
            m_file = NULL;
            m_delBuffers++;
        }
    }
}

void DownloadManagerImpl::clearLists()
{
    m_textList.clear();
    m_indexList.clear();
}

void DownloadManagerImpl::setVoice(QString voice)
{
    if (voice == herculesVoiceDisplay)
        m_voice = herculesVoice;
    else if (voice == emilyVoiceDisplay)
        m_voice = emilyVoice;
    else if (voice == coriVoiceDisplay)
        m_voice = coriVoice;
    else if (voice == spikeVoiceDisplay)
        m_voice = spikeVoice;
    else if (voice == rmsVoiceDisplay)
        m_voice = rmsVoice;
    else if (voice == prudenceVoiceDisplay)
        m_voice = prudenceVoice;
    else if (voice == poppyVoiceDisplay)
        m_voice = poppyVoice;
    else if (voice == obadiahVoiceDisplay)
        m_voice = obadiahVoice;
    else if (voice == sltVoiceDisplay)
        m_voice = sltVoice;
    else if (voice == bdlVoiceDisplay)
        m_voice = bdlVoice;
    else if (voice == pavoqueVoiceDisplay)
        m_voice = pavoqueVoice;
    else if (voice == pierreVoiceDisplay)
        m_voice = pierreVoice;
    else if (voice == luciaVoiceDisplay)
        m_voice = luciaVoice;
    else if (voice == turkishVoiceDisplay)
        m_voice = turkishVoice;
    else if (voice == teluguVoiceDisplay)
        m_voice = teluguVoice;
    else
    {
        m_voice = herculesVoice;
    }
}

DownloadManagerImpl::~DownloadManagerImpl()
{
    qDebug() << "DownloadManagerImpl destructor called";
//    cancelDownload();
//    if (m_file != NULL)
//    {
//        delete m_file;
//        m_file = NULL;
//        qDebug() << "DownloadmanagerImpl: deleting buffer";
//    }
    qDebug() << "DownloaManagerImpl: Number of total buffers created: " << m_actualBuffers;
    qDebug() << "DownloaManagerImpl: Number of buffers deleted : " << m_delBuffers;
    qDebug() << "DownloaManagerImpl: Number of actual buffers created: " << m_actualBuffers - m_delBuffers;
    qDebug() << "DownloadManagerImpl destructor completed";
}
