/*
 * Every class that needs to receive wav files from a DownloadManager
 * must implement this interface
 * DownloadManager receiver will call this function whenever it receives
 * a wav file
*/

#ifndef DOWNLOADMANAGERIFACE_H
#define DOWNLOADMANAGERIFACE_H

class QBuffer;
class QString;

class DownloadManagerIface
{
public:
    virtual void handleWav(QBuffer* buffer, QString index) = 0;
};

#endif // DOWNLOADMANAGERIFACE_H
