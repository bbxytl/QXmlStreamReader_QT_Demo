#ifndef XMLSTREAM_H
#define XMLSTREAM_H

#include <QString>
#include <QStringList>
#include <QMap>
#include <QDebug>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

class XmlStream
{
public:
    XmlStream(const QString &fileName);
    ~XmlStream();

public:
    bool readFile();   //从文件读取XML
    bool writerFile(const QString fileName); //写入XML

private:
    bool readElement();
    void optAttributes();


private:
    QString m_fileName; //文件名
    QXmlStreamReader m_reader;
    QXmlStreamWriter m_writer;
    QMap<QString,QString> m_map;//标签层次关系图
    QString m_cur;    //当前处理的标签

};

#endif // XMLSTREAM_H
