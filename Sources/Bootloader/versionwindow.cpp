#include "versionwindow.h"
#include <QFile>
#include <QVBoxLayout>
#include <QHBoxLayout>

VersionWindow::VersionWindow(QWidget *parent, QString trigram, QString firmware, QString customerNumber, QString pathResource) :
    QDialog(parent,Qt::WindowCloseButtonHint),
    _trigram(trigram),
    _firmware(firmware),
    _customerNumber(customerNumber),
    _pathResource(pathResource)
{
    errorOccured = false;
    connect(&qnam, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(slotAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
    connect(&qnam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));

    setWindowTitle("Select Firmare");
    wait  = new QTimer();
    connect(wait, SIGNAL(timeout()), this, SLOT(StopWait()));

    m_Label = new QLabel("Available firmware:");
    m_versionsCombo = new QComboBox();
    m_OK = new QPushButton("OK");
    m_OK->setFixedWidth(70);
    m_CANCEL = new QPushButton("CANCEL");
    m_CANCEL->setFixedWidth(70);
    m_LabelDiff = new QLabel("");
    m_LabelDiff->setAlignment(Qt::AlignCenter);
    QLabel *m_Label2 = new QLabel("Current firmware: "+_firmware);

    m_versionsCombo->setMinimumHeight(30);
    connect(m_versionsCombo, SIGNAL(activated(int)), this, SLOT(showDiff(int)));

    QHBoxLayout *layoutBouton = new QHBoxLayout;
    layoutBouton->addSpacing(50);
    layoutBouton->addWidget(m_OK);
    layoutBouton->addWidget(m_CANCEL);
    QVBoxLayout *layoutPrincipal = new QVBoxLayout;
    layoutPrincipal->addWidget(m_Label);
    layoutPrincipal->addWidget(m_versionsCombo);
    layoutPrincipal->addSpacing(15);
    QHBoxLayout *layoutPrincipal2 = new QHBoxLayout;
    layoutPrincipal2->addLayout(layoutPrincipal);
    layoutPrincipal2->addWidget(m_LabelDiff);
    QVBoxLayout *layoutPrincipal3 = new QVBoxLayout;
    layoutPrincipal3->addWidget(m_Label2);
    layoutPrincipal3->addLayout(layoutPrincipal2);
    layoutPrincipal3->addLayout(layoutBouton);
    setLayout(layoutPrincipal3);

    resize(200,50);

    //////////////////////////////////////////////////////////////////////////////////////////

#ifdef Q_WS_MAC
    QString macChange =_pathResource;
    macChange.append("versioning.xml");
    file = new QFile(macChange);
#else
    file = new QFile("versioning.xml");
#endif
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Unable to open versioning information");
        QMessageBox::critical(this,"Error","\nUnable to acces the information about firmware.\n\nPlease try again later");
    }

    QString fileName = "versioning.xml";
#ifdef Q_WS_MAC
    fileName =_pathResource;
    fileName.append("versioning.xml");
#endif
    QDomDocument settings(fileName);

    QString errorMsg;
    if(!settings.setContent(file, &errorMsg)){ //handle error (print error message on pane for instance)
        std::cout << "error: couldn't read " << fileName.toStdString().c_str()  << " , error message: " << errorMsg.toStdString().c_str() << std::endl;
        file->close();
    }
    file->close();

    QString tempoNoms[100];
    //parse the settings tree and restore state
    QDomElement root = settings.documentElement();
    if(_trigram!=""){// si le trigram est lisible
        QDomElement a = root.firstChildElement(_trigram);
        QDomElement b = a.firstChildElement("new-version");

        nAvailableUpdates = 0;

        // exemple de nom de version : IR_IR V01.17.01
        while(!b.isNull()){// balaye les versions disponibles sur la base de données
            QDomElement c = b.firstChildElement("name");
            m_versionsCombo->addItem(c.text());
            tempoNoms[nAvailableUpdates]=c.text();
            c = b.firstChildElement("filename");
            fileNameUpdate[nAvailableUpdates]=_trigram + "/" + c.text();
            //QString urlTempo = "http://www.volfoni.com/VolfoniLoader/Serveur/"+_trigram + "/" + c.text();
            QString urlTempo = "http://vl.volfoni.com/Serveur/"+_trigram + "/" + c.text();
            urlUpdate[nAvailableUpdates] = tr(urlTempo.toStdString().c_str());
            nAvailableUpdates++;

            b = b.nextSiblingElement("new-version");
        }

        file->close();
    }
    else{// on les liste toutes
        QString allTrigram[100];
        allTrigram[0] = "AHU";
        allTrigram[1] = "AME";
        allTrigram[2] = "SCC";
        allTrigram[3] = "SCP";
        allTrigram[4] = "EDG";
        nAvailableUpdates = 0;
        for(int i = 0; i<5; i++){
            QDomElement a = root.firstChildElement(allTrigram[i]);
            QDomElement b = a.firstChildElement("new-version");
            // exemple de nom de version : IR_IR V01.17.01
            while(!b.isNull()){// balaye les versions disponibles sur la base de données
                QDomElement c = b.firstChildElement("name");
                m_versionsCombo->addItem(c.text());
                tempoNoms[nAvailableUpdates]=c.text();
                c = b.firstChildElement("filename");
                fileNameUpdate[nAvailableUpdates]=allTrigram[i] + "/" + c.text();
                //QString urlTempo = "http://www.volfoni.com/VolfoniLoader/Serveur/"+allTrigram[i] + "/" + c.text();
                QString urlTempo = "http://vl.volfoni.com/Serveur/"+allTrigram[i] + "/" + c.text();
                urlUpdate[nAvailableUpdates] = tr(urlTempo.toStdString().c_str());
                nAvailableUpdates++;


                b = b.nextSiblingElement("new-version");
            }

            file->close();
        }
    }
    for(int j=0; j<nAvailableUpdates;j++){
        if(_firmware == tempoNoms[j]){
            m_versionsCombo->setCurrentIndex(j);
            m_LabelDiff->setText("No change detected");
            break;
        }
    }
    /////////////////////////////////////////////////////////////////////////////////////

    connect(m_CANCEL, SIGNAL(clicked()), this, SLOT(RemoveAndQuit()));
    connect(m_OK, SIGNAL(clicked()), this, SLOT(selectFile()));
    connect(this, SIGNAL(Set_version(QString)), parent, SLOT(Set_version_p(QString)));
}

void VersionWindow::showDiff(int a){
    m_LabelDiff->setText("");
    if(_firmware == m_versionsCombo->currentText()){
        m_LabelDiff->setText("No change detected");
        return;
    }
    QStringList list = m_versionsCombo->currentText().split(".");
    if(list.size()!=3)return;
    QString tempoCustoNumber = list.at(0);

    QStringList list2 = _firmware.split(".");
    if(list2.size()!=3)return;
    QString currentCustoNumber = list2.at(0);
    if(tempoCustoNumber == currentCustoNumber){// si meme type de numero de client
        if(QString("%1%2").arg(list.at(1)).arg(list.at(2)).toInt()> QString("%1%2").arg(list2.at(1)).arg(list2.at(2)).toInt())
            m_LabelDiff->setText("Changes from current firmware:\n"+changeWithPrevious(_firmware,m_versionsCombo->currentText()));
        else m_LabelDiff->setText("Description:\n"+getDescription(m_versionsCombo->currentText()));
    }
    else m_LabelDiff->setText("Description:\n"+getDescription(m_versionsCombo->currentText()));

}

QString VersionWindow::changeWithPrevious(QString current, QString lastOne){//renvoi les differences entres 2 versions

    QString changes = "";
    QStringList list3 = lastOne.split(".");
    if(list3.size()!=3)return "";
    int toStop = QString("%1%2").arg(list3.at(1)).arg(list3.at(2)).toInt();


#ifdef Q_WS_MAC
    QString macChange =_pathResource;
    macChange.append("versioning.xml");
    QFile *file = new QFile(macChange);
#else
    QFile *file = new QFile("versioning.xml");
#endif
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return "";
    }
    QString fileName = "versioning.xml";
#ifdef Q_WS_MAC
    fileName =_pathResource;
    fileName.append("versioning.xml");
#endif
    QDomDocument settings(fileName);

    QString errorMsg;
    if(!settings.setContent(file, &errorMsg)){ //handle error (print error message on pane for instance)
        std::cout << "error: couldn't read " << fileName.toStdString().c_str()  << " , error message: " << errorMsg.toStdString().c_str() << std::endl;
        file->close();
    }
    file->close();

    QStringList list = QString(current).split(".");//découpe le nom de la version actuelle selon les "."

    int nCurrent = QString("%1%2").arg(list.at(1)).arg(list.at(2)).toInt(); // int de la version courante

    //parse the settings tree and restore state
    QDomElement root = settings.documentElement();
    QDomElement a = root.firstChildElement(_trigram);
    QDomElement b = a.firstChildElement("new-version");

    while(!b.isNull()){// balaye les versions disponibles sur la base de données
        QDomElement c = b.firstChildElement("name");
        QString _temp = c.text();
        // exemple de nom de version : IR_IR_V01.17.02
        QStringList _list = QString(_temp).split(".");//découpe le nom de la version selon les "_V"

        int nVersion = QString("%1%2").arg(_list.at(1)).arg(_list.at(2)).toInt();
        if(nVersion>nCurrent && nVersion<=toStop){// version superieure a l'actuelle -> enregistrer les modifs
            c = b.firstChildElement("modification");
            while(!c.isNull()){
                changes.append("- ");
                changes.append(c.text());
                changes.append("\n");
                c =c.nextSiblingElement("modification");
            }
        }
        b = b.nextSiblingElement("new-version");
    }

    file->close();

    return changes;
}

QString VersionWindow::getDescription(QString current){//renvoi la description d'un firmware

    QString changes = "";

#ifdef Q_WS_MAC
    QString macChange =_pathResource;
    macChange.append("versioning.xml");
    QFile *file = new QFile(macChange);
#else
    QFile *file = new QFile("versioning.xml");
#endif
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return "";
    }
    QString fileName = "versioning.xml";
#ifdef Q_WS_MAC
    fileName =_pathResource;
    fileName.append("versioning.xml");
#endif
    QDomDocument settings(fileName);

    QString errorMsg;
    if(!settings.setContent(file, &errorMsg)){ //handle error (print error message on pane for instance)
        std::cout << "error: couldn't read " << fileName.toStdString().c_str()  << " , error message: " << errorMsg.toStdString().c_str() << std::endl;
        file->close();
    }
    file->close();

    //parse the settings tree and restore state
    QDomElement root = settings.documentElement();
    QDomElement a = root.firstChildElement(_trigram);
    QDomElement b = a.firstChildElement("new-version");

    while(!b.isNull()){// balaye les versions disponibles sur la base de données
        QDomElement c = b.firstChildElement("name");
        if(c.text() == current){
            c = b.firstChildElement("description");
            changes = c.text();
        }
        b = b.nextSiblingElement("new-version");
    }
    file->close();

    return changes;
}

void VersionWindow::StopWait(){ //si pas de reponse de téléchargement

  wait->stop();
  QApplication::restoreOverrideCursor();
  QMessageBox::critical(this,"Reading time exceeded","Unable to download firmware. \nTry it again later.\n\n");
  errorOccured = true;
}
void VersionWindow::selectFile()
{
    errorOccured = false;
    QApplication::setOverrideCursor(Qt::WaitCursor);
    wait->start(5000);
    QStringList list = fileNameUpdate[m_versionsCombo->currentIndex()].split("/");

    QString fileName = list.at(list.size()-1);
#ifdef Q_WS_MAC
    fileName =_pathResource;
    fileName.append(list.at(list.size()-1));
#endif
    if (QFile::exists(fileName)) {
        QFile::remove(fileName);
    }

    fileFirmware = new QFile(fileName);
    if (!fileFirmware->open(QIODevice::WriteOnly)) {
        qWarning("Unable to save the firmware file");
        errorOccured = true;
        delete fileFirmware;
        fileFirmware = 0;
        return;
    }

    // schedule the request
    httpRequestAborted = false;

    startRequest(urlUpdate[m_versionsCombo->currentIndex()]);

}

void VersionWindow::RemoveAndQuit()
{
    file->remove();
    this->close();
}

void VersionWindow::startRequest(QUrl _url)
{
    qWarning("send get to http serveur");
    reply = qnam.get(QNetworkRequest(_url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
}
void VersionWindow::httpFinished()
{
    wait->stop();
    if (httpRequestAborted) {
        if (fileFirmware) {
            fileFirmware->close();
            fileFirmware->remove();
            delete fileFirmware;
            fileFirmware = 0;
        }
        reply->deleteLater();
        return;
    }

    fileFirmware->flush();
    fileFirmware->close();

    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()) {
        fileFirmware->remove();
        QMessageBox::information(this, tr("HTTP"),
                                 tr("Download failed: %1.")
                                 .arg(reply->errorString()));
        errorOccured = true;
        return;
    } else if (!redirectionTarget.isNull()) {
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        if (QMessageBox::question(this, tr("HTTP"),
                                  tr("Redirect to %1 ?").arg(newUrl.toString()),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            url = newUrl;
            reply->deleteLater();
            fileFirmware->open(QIODevice::WriteOnly);
            fileFirmware->resize(0);
            startRequest(url);
            return;
        }
    }
    else
    {
        qWarning("Informations downloaded from server.");
    }

    reply->deleteLater();
    reply = 0;
    delete fileFirmware;
    fileFirmware = 0;

    QApplication::restoreOverrideCursor();

    QString filename = fileNameUpdate[m_versionsCombo->currentIndex()];
    if(!errorOccured)emit Set_version(filename);
    if(!errorOccured)RemoveAndQuit();
}

void VersionWindow::slotAuthenticationRequired(QNetworkReply*,QAuthenticator *authenticator)
{
    authenticator->setUser("VolfoniLoader");
    authenticator->setPassword("vol123");
}

void VersionWindow::sslErrors(QNetworkReply*,const QList<QSslError> &errors)
{
    QString errorString;
    foreach (const QSslError &error, errors) {
        if (!errorString.isEmpty())
            errorString += ", ";
        errorString += error.errorString();
    }

    if (QMessageBox::warning(this, tr("HTTP"),
                             tr("One or more SSL errors has occurred: %1").arg(errorString),
                             QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore) {
        errorOccured = true;
        reply->ignoreSslErrors();
    }
}

void VersionWindow::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    if (fileFirmware)
        fileFirmware->write(reply->readAll());
}
