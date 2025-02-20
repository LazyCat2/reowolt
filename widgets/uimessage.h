#ifndef UIMESSAGE_H
#define UIMESSAGE_H

#include <QHBoxLayout>

#include "../client/api_structs.h"
#include "../client/cache.h"
#include "../downloader.h"

class UiMessage : public QHBoxLayout
{
public:
    UiMessage(message_id, Cache*, Downloader*);
    UiMessage(Message*);
};

#endif // UIMESSAGE_H
