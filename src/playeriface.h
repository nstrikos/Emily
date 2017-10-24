#ifndef PLAYERIFACE_H
#define PLAYERIFACE_H

class QString;

class PlayerIface
{
public:
    virtual void sendIndex(QString index) = 0;
};

#endif // PLAYERIFACE_H
