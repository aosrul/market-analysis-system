/*
 * Класс реализующий чтение .hst файлов, использование массивов данных.
 *
 * Hst файлы хранят в себе данные в бинарном виде.
 * При прочтении образуют следующую структуру данных:
 *
 * "2015-05-16T08:50:00Z09:00","119.405000","119.425000","119.403000","119.424000","37"
 * "2015-05-16T08:51:00Z09:00","119.423000","119.448000","119.411000","119.446000","36"
 * ...
 */

#ifndef HSTREADER_H
#define HSTREADER_H

#include <QObject>
#include <QVector>

#pragma pack(push,1)        // Выравнивание структуры в памяти по 1 байту.
typedef struct HeaderBytes     // Total 148 bytes
{
    quint32  Version;        // database version - 400 or 401 = 4 bytes
    QChar Copyright[64];    // copyright info = 64 bytes
    QChar Symbol[12];       // symbol name = 12 bytes
    quint32  Period;         // symbol timeframe	= 4 bytes
    quint32  Digits;         // the amount of digits after decimal point	= 4 bytes
    quint32  TimeSign;       // timesign of the database creation = 4 bytes
    quint32  LastSync;       // the last synchronization time = 4 bytes
    quint32  Unused[13];     // to be used in future	= 52 bytes
} HeaderBytes;

typedef struct HistoryBytes    // Total 60 bytes (version 401)
{
    qint64 Time;            // bar start time = 8 bytes
    double Open;            // open price = 8 bytes
    double High;            // highest price = 8 bytes
    double Low;             // lowest price = 8 bytes
    double Close;           // close price = 8 bytes
    qint64 Volume;          // tick count = 8 bytes
    qint32 Spread;          // spread = 4 bytes
    qint64 RealVolume;      // real volume = 8 bytes
} HistoryBytes;
/*
typedef struct     // Total 44 bytes (version 400)
{
    qint32 Time;            // bar start time = 4 bytes
    double Open;            // open price = 8 bytes
    double Low;             // lowest price = 8 bytes
    double High;            // highest price = 8 bytes
    double Close;           // close price = 8 bytes
    double Volume;          // tick count = 8 bytes
} HistoryBytes400;*/
#pragma pack(pop)

class HstReader : public QObject
{
    Q_OBJECT

public:
    explicit HstReader(QObject *parent = 0);
    HstReader(QString fName);
    ~HstReader();

private:
    HeaderBytes header;
    QVector<HistoryBytes*> historyVector;
    //QVector<HistoryBytes400> history400Vector;

    uint historySize;
    int historyVersion;

    QString fileName;

    uint *positionHst;


signals:

public slots:
    bool readFromFile();
    //bool readFromFile(int histSize);
    //bool readFromFile(QString fName);
    //bool readFromFile(uint startPosition, uint stopPosition);

    void setFileName(QString fName);
    QString getFileName() const;

    //bool readHeader();
    //bool readHistory(uint *pos);
    uint getHistorySize() const;
    HeaderBytes *getHeaderStruct();
    QString getHeaderString() const;

    QVector<HistoryBytes*> getHistoryVector() const;
    QString getHistoryString(int numberPosition) const;
    // getHistoryOpenNNVector
};

#endif // HSTREADER_H
