#ifndef GSTARLOCALHEADER_H
#define GSTARLOCALHEADER_H

#include <QtCore/QtGlobal>

#define MAX_CHUNK_SIZE 0x0000FFFFLL

class GStarChunkHeader
{
public:
    enum Protocols
    {
        ConnectionJoined = 1,
        GetAllSocketIds,
        ConnectionLeft,
        PostChunkData
    };

public:
    GStarChunkHeader() : pd(0) { }
    ~GStarChunkHeader() { }

public:
    char *data() const { return const_cast<char *>(reinterpret_cast<const char *>(&pd)); }
    const char *constData() const { return reinterpret_cast<const char *>(&pd); }
    qint64 size() const { return 6; }

    bool isValid() const { return pd != 0; }
    void reset() { pd = 0; }

    qint64 senderId() const { return ((pd >> 0x20) & 0x0000ffff); }
    qint64 targetId() const { return ((pd >> 0x20) & 0x0000ffff); }

    qint64 protocol() const { return (pd & 0x0000000fll) >> 0x00; }
    qint64 chunkSize() const { return (pd & 0xfffff000ll) >> 0x0c; }
    qint64 chunkIndex() const { return (pd & 0x00000ff0ll) >> 0x04; }

    void set(qint64 targetId, qint64 protocol, qint64 chunkSize, qint64 chunkIndex)
    {
        pd = ((targetId & 0x0000ffffll) << 0x20) | ((protocol & 0x0000000fll) << 0x00)
                | ((chunkSize & 0x000000ffll) << 0x0c) | ((chunkIndex & 0x000000ffll) << 0x04);
    }

    void dumpInfo() const { qDebug("GStarChunkHeader: pd: %016llx", pd); }

private:
    quint64 pd;
};

#endif // GSTARLOCALHEADER_H
