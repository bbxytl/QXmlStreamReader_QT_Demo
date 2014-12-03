#include "xmlstream.h"
#include <QFile>
#include <QMessageBox>

XmlStream::XmlStream(const QString &fileName)
{
    m_fileName=fileName;
    m_map.insert ("_BEGIN_","BQTable");
    m_map.insert ("BQTable","BQTitle");
    m_map.insert ("BQTitle","BQItem");
    m_map.insert ("BQItem","BQNorm");
    m_map.insert ("BQNorm","_END_");
    m_cur=m_map.value("_BEGIN_");
}

XmlStream::~XmlStream()
{
}

bool XmlStream::readFile()
{
    QFile file(m_fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug()<<QString("打不开文件 : %1").arg (m_fileName);
        return false;
    }
    m_reader.setDevice(&file);
    bool berror=false;
    while (!m_reader.atEnd()) {
        if(m_reader.isStartElement ())
            if(m_reader.name().trimmed()==m_map.value("_BEGIN_")){
                qDebug()<<"读取到： "<<m_reader.name ();
                //文件解析过程中解析出错，跳出解析，进行后继安全处理
                if(!readElement ())berror=true;
                break;
            }
        m_reader.readNext ();
     }
    file.close();
    if (m_reader.hasError()||(berror)) {
        qDebug()<<QString("解析文件  %1  失败").arg (m_fileName);
        return false;
    } else if (file.error() != QFile::NoError||(berror)) {
        qDebug()<<QString("无法读取文件 : %1").arg(m_fileName);
        return false;
     }
    qDebug()<<"文件解析成功 ！";
    return true;
}

bool XmlStream::writerFile(const QString fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        qDebug()<<QString("打不开文件 : %1").arg (m_fileName);
        return false;
    }
    try{
        m_writer.setDevice(&file);
        m_writer.setAutoFormatting(true);
        //文档开始
        m_writer.writeStartDocument();

        //写文档内容
        m_writer.writeStartElement("BQTable");
        m_writer.writeAttribute("att","test");
            m_writer.writeStartElement("BQTitle");
            m_writer.writeCharacters("This is a Test String!");
            m_writer.writeEndElement();
        m_writer.writeEndElement();

        //文档结束
        m_writer.writeEndDocument();
    }catch(...){
        return false;
    }
    return true;
}

bool XmlStream::readElement()
{
    QString curMap=m_cur;
    //判断读取的是否正确
    if(!(m_reader.isStartElement()&&
         m_reader.name().trimmed ()==curMap.trimmed())){
        qDebug()<<QString("标签 %1 读取错误 ！ 所读为： ").arg(curMap)
               <<m_reader.name();
        return false;
    }

    //处理标签的属性
    optAttributes();

    //处理标签的子标签
    m_reader.readNext();
    while(!m_reader.atEnd()){
        if(m_reader.isEndElement()){
            qDebug()<<"已读取完： "<<m_reader.name();
            m_reader.readNext();
            break;
        }
        if(!m_reader.isWhitespace())
            qDebug()<<QString("%1 中读取到： %2").
                      arg(curMap).arg(m_reader.name().toString());

        if(m_reader.isStartElement()){
            //只有当前标签不是最后一个时才进行子标签的处理
            if(m_map.value(m_cur)!="_END_"){
                QString child=(m_map.value(m_cur));
                if(m_reader.name().trimmed()== child){
                    m_cur=child;//取子标签
                    if(!readElement())return false;
                }
                else{
                    qDebug()<<QString("标签 %1 子节点格式错误 ! 所读为： %2").
                              arg(curMap).arg(m_reader.name().toString ());
                    return false;
                }
            }
        }else
            m_reader.readNext();
    }
    return true;
}

void XmlStream::optAttributes()
{
    if(m_reader.attributes ().size ()<=0){
        qDebug()<<"  "<<m_reader.name ()<<" 没有属性！";
        return;
    }
    qDebug()<<"  "<<m_reader.name ()<<" 的属性__Begin";
    foreach (QXmlStreamAttribute var, m_reader.attributes ().toList ()) {
        qDebug()<<"\t"<<var.name ()<<" = "<<var.value ();
    }
    qDebug()<<"  "<<m_reader.name ()<<" 的属性__End";
}
