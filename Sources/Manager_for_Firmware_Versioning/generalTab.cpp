#include <QtGui>
#include <QList>

#include "generalTab.h"
#include "login.h"

GeneralTab::GeneralTab(QString trigram, MainWindow *mW, QString trigrams[100], int nDevices)
    : QDialog(),
      _trigram(trigram),
      _mW(mW),
      fieldsOK(false)
{

    projectOpen = false;
    _nDevices = nDevices;
    for(int i = 0; i<_nDevices;i++)_trigrams[i]= trigrams[i];

    connect(&qnam, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(slotAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
    connect(&qnam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));

    versioningFile =  new QLineEdit;
    versioningFile->setReadOnly(true);
    versioningFile->setEnabled(false);
    workingDirectory = new QLineEdit;
    workingDirectory->setReadOnly(true);
    workingDirectory->setEnabled(false);

    QLabel *l_versioningFile =  new QLabel("Versioning file:");
    QLabel *l_workingDirectory = new QLabel("Working directory (root folder):");


    m_browseFile = new QPushButton("Browse");
    connect(m_browseFile, SIGNAL(clicked()), this, SLOT(browseFile()));

    m_ftp = new QPushButton("Get it from FTP\nserver");
    connect(m_ftp, SIGNAL(clicked()), this, SLOT(ftpMode()));

    create = new QPushButton("Create a new\nversioning directory");
    create->setFixedWidth(150);
    connect(create, SIGNAL(clicked()), this, SLOT(browseFolder()));

    QFrame *frame1 = new QFrame;
    frame1->setFrameStyle(QFrame::VLine);
    frame1->setLineWidth(10);
    frame1->setMidLineWidth(3);
    frame1->setPalette(QColor(255,255,255));

    QGridLayout *grid = new QGridLayout;
    grid->addWidget(l_versioningFile,0,0);
    grid->addWidget(versioningFile,0,1);
    grid->addWidget(l_workingDirectory,1,0);
    grid->addWidget(workingDirectory,1,1);

    QLabel *label1 = new QLabel("Or");
    QLabel *label2 = new QLabel("Or");

    QHBoxLayout *right = new QHBoxLayout;
    right->addWidget(m_browseFile);
    right->addWidget(label1);
    right->addWidget(create);
    right->addWidget(label2);
    right->addWidget(m_ftp);

    QGroupBox *groupRight = new QGroupBox("Choices");
    groupRight->setLayout(right);

    QHBoxLayout *layout_top = new QHBoxLayout;
    layout_top->addLayout(grid);
    layout_top->addSpacing(10);
    layout_top->addWidget(groupRight);

    QLabel *label_picture = new QLabel("");
    label_picture->setAlignment(Qt::AlignCenter);
    label_picture->setPixmap(QPixmap(":/MainWindow/ressources/explications.png"));
    label_picture->setStyleSheet("background: white");
    label_picture->setFixedSize(750,270);

    layout_principal = new QVBoxLayout(this);
    layout_principal->addLayout(layout_top);
    //layout_principal->addWidget(apply,0,Qt::AlignCenter);
    layout_principal->addWidget(label_picture,0,Qt::AlignLeft);

    setLayout(layout_principal);
    setMinimumHeight(230);

}

int GeneralTab::checkAlreadyOpen(){
    if(projectOpen==true){
        if (QMessageBox::question(this, tr("Warning"),
                                  tr("A project is already open!\nDo you want to close it and open the new one?"),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            _mW->resetTabs();
            projectOpen = false;
            versioningFile->setText("");
            workingDirectory->setText("");
            return 1;

        }else return 0;
    }else return 1;
}

void GeneralTab::ftpMode(){
    if(checkAlreadyOpen()==0)return;
    QString fileName = QFileDialog::getExistingDirectory(this,tr("Select a working directory for local work"),"./../../../../", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(fileName.isEmpty()){
      //the user did not choose a  file
      return;
    }
    fileName.replace("\\","/");
    fileName.append("/");
    workingDirectory->setText(fileName);
    versioningFile->setText(fileName+"versioning.xml");
    Login log_in(this, true);
    log_in.setGT(this);
    log_in.exec();

}

void GeneralTab::logUser(QString m_url, QString m_login, QString m_pwd){
    _url = m_url;
    _login = m_login;
    _pwd = m_pwd;
    errorOccured = false;

    progress = new Progress(this, 0);
    progress->setValueTitle("Progress to download files");
    progress->setClosable(false);
    progress->setValueProgress(0);
    progress->setValueLabel("Start Downloading");
    progress->setGT(this);
    progress->exec();


}

void GeneralTab::browseFile(){
    if(checkAlreadyOpen()==0)return;
    QString fileName = QFileDialog::getOpenFileName(this,tr("Choose versioning file"),"./../../../../", tr("XML files (versioning.xml)"));
    if(fileName.isEmpty()){
        //the user did not choose a  file
        return;
    }
    versioningFile->setText(fileName);

    QStringList list = QString(fileName).split("/");//découpe le chemin selon les "/"
    QString pathFolder = "";
    if(list.size()>0)for(int i = 0; i<list.size()-1;i++)pathFolder.append(list.at(i)+"/");
    workingDirectory->setText(pathFolder);
    applyValues();
}

void GeneralTab::browseFolder(){
    if(checkAlreadyOpen()==0)return;
    QString fileName = QFileDialog::getExistingDirectory(this,tr("Select a working directory"),"./../../../../", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(fileName.isEmpty()){
      //the user did not choose a  file
      return;
    }
    fileName.replace("\\","/");
    fileName.append("/");
    workingDirectory->setText(fileName);
    versioningFile->setText(fileName+"versioning.xml");
    applyValues();
}


QString GeneralTab::getFilename(){
    return versioningFile->text();
}

QString GeneralTab::getFolder(){
    return workingDirectory->text();
}

void GeneralTab::applyValues(){

    projectOpen = true;
    if(workingDirectory->text()!="" && versioningFile->text()!=""){
        workingDirectory->setEnabled(false);
        versioningFile->setEnabled(false);
        fieldsOK = true;
        //create->setEnabled(false);
        //m_browseFile->setEnabled(false);
        //m_ftp->setEnabled(false);
        _mW->setTabs();
    }else {
        fieldsOK = false;
        QMessageBox::warning(this,"Warning", "Something is not set");
    }
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions de connexion a internet
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GeneralTab::CheckUpdates(){//Verify if a new version is available
    QString tempo = QString("%1versioning.xml").arg(_url);
    url = tr(tempo.toStdString().c_str());

    QFileInfo fileInfo(url.path());
    QString fileName = workingDirectory->text() + fileInfo.fileName();


    if (QFile::exists(fileName)) {
        QFile::remove(fileName);
    }

    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly)) {
        qWarning("Unable to save the file versioning.xml");
        delete file;
        file = 0;
        QMessageBox::warning(this,"Error", "An error occured, please try again");
        return;
    }

    // schedule the request
    httpRequestAborted = false;
    startRequest(url);

}

void GeneralTab::startRequest(QUrl url)
{
    qWarning("send get to http serveur");
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
}

void GeneralTab::httpFinished()
{
    if(errorOccured ==true){QMessageBox::warning(this, tr("Error"), tr("Errors occured or download canceled, please try again."));return;}
    if (httpRequestAborted) {
        if (file) {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        reply->deleteLater();
        qWarning("Error http request aborted");
        errorOccured = true;
        return;
    }

    file->flush();
    file->close();

    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()) {
        file->remove();
        qWarning("Download failed");
    } else if (!redirectionTarget.isNull()) {
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        if (QMessageBox::question(this, tr("HTTP"),
                                  tr("Redirect to %1 ?").arg(newUrl.toString()),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            url = newUrl;
            reply->deleteLater();
            file->open(QIODevice::WriteOnly);
            file->resize(0);
            startRequest(url);
            return;
        }else errorOccured = true;
    }
    else
    {
        qWarning("Informations downloaded from server.");
    }

    reply->deleteLater();
    reply = 0;
    delete file;
    file = 0;

    progress->setValueProgress(15);
    progress->setValueLabel("Versioning.xml file downloaded");
    DownloadVersions();

}

void GeneralTab::httpReadyRead()
{
    if(errorOccured ==true){QMessageBox::warning(this, tr("Error"), tr("Errors occured or download canceled, please try again."));return;}
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    if (file)
        file->write(reply->readAll());
}

void GeneralTab::slotAuthenticationRequired(QNetworkReply*,QAuthenticator *authenticator)
{
    authenticator->setUser(_login);
    authenticator->setPassword(_pwd);
}

void GeneralTab::sslErrors(QNetworkReply*,const QList<QSslError> &errors)
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
        reply->ignoreSslErrors();
    }
}

void GeneralTab::DownloadVersions(){
    QString fileName = workingDirectory->text()+"versioning.xml";
    file = new QFile(fileName);
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Unable to open versioning information");
        QMessageBox::critical(this,"Error","\nUnable to open local copy of versioning.xml.");
    }

    QDomDocument settings(fileName);

    QString errorMsg;
    if(!settings.setContent(file, &errorMsg)){ //handle error (print error message on pane for instance)
        std::cout << "error: couldn't read " << fileName.toStdString().c_str()  << " , error message: " << errorMsg.toStdString().c_str() << std::endl;
        file->close();
    }
    file->close();

    //parse the settings tree and restore state
    QDomElement root = settings.documentElement();
    nFound = 0;
    for(int i =0; i<_nDevices;i++){
        QDomElement a = root.firstChildElement(_trigrams[i]);
        QDomElement bb = a.firstChildElement("new-version");
        while(!bb.isNull()){// balaye les versions disponibles
            bb = bb.nextSiblingElement("new-version");
            nFound++;// pour compter le nombre de fichiers a telecharger et adapter la progress bar
        }
    }

    space = (100-15)/nFound;
    int count = 0;

    for(int i =0; i<_nDevices;i++){
        QDomElement a = root.firstChildElement(_trigrams[i]);
        QDomElement b = a.firstChildElement("new-version");

        while(!b.isNull()){// balaye les versions disponibles

            QDomElement c = b.firstChildElement("filename");
            shortfileNameToDownload[count] = c.text();
            fileNameToDownload[count] = _url+_trigrams[i]+"/"+c.text();
            fileNameToStore[count] = workingDirectory->text() +_trigrams[i]+"/"+c.text();
            folderToStore[count] = workingDirectory->text() +_trigrams[i];

            b = b.nextSiblingElement("new-version");

            count++;
        }
    }
    if(nFound>0){
        currentN = 0;
        url = tr(fileNameToDownload[currentN].toStdString().c_str());

        //QFileInfo fileInfo(url.path());
        if(QDir(folderToStore[currentN]).exists()==false)QDir().mkdir(folderToStore[currentN]);

        QString fileName = fileNameToStore[currentN];

        if (QFile::exists(fileName)) {
            QFile::remove(fileName);
        }

        file = new QFile(fileName);
        if (!file->open(QIODevice::WriteOnly)) {
            qWarning("Unable to save a file");
            errorOccured = true;
            delete file;
            file = 0;
            return;
        }

        // schedule the request
        httpRequestAborted = false;

        startRequest2(fileNameToDownload[currentN]);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////
// fonctions pour un chemin de fichier, allégé sans message d'erreurs
void GeneralTab::startRequest2(QUrl url)
{
    qWarning("send get to http serveur");
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished2()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead2()));
}

void GeneralTab::httpFinished2()
{
    if(errorOccured ==true){QMessageBox::warning(this, tr("Error"), tr("Errors occured or download canceled, please try again."));return;}
    progress->setValueProgress(15+(currentN+1)*space);
    progress->setValueLabel("Download "+shortfileNameToDownload[currentN]+"...");
    if(currentN==nFound-1){
        progress->setValueProgress(100);
        progress->setValueLabel("Download finished");
    }
    if (httpRequestAborted) {
        if (file) {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        reply->deleteLater();
        qWarning("Error http request aborted");
        errorOccured = true;
        return;
    }

    file->flush();
    file->close();

    QVariant redirectionTarget = reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    if (reply->error()) {
        file->remove();
        qWarning("Download failed");
        QMessageBox::warning(this, tr("Error"),
                                     tr("A download failed"));
    } else if (!redirectionTarget.isNull()) {
        QUrl newUrl = url.resolved(redirectionTarget.toUrl());
        if (QMessageBox::question(this, tr("HTTP"),
                                  tr("Redirect to %1 ?").arg(newUrl.toString()),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            url = newUrl;
            reply->deleteLater();
            file->open(QIODevice::WriteOnly);
            file->resize(0);
            startRequest2(url);
            return;
        }else errorOccured = true;
    }
    else
    {
        qWarning("Informations downloaded from server.");
    }

    reply->deleteLater();
    reply = 0;
    delete file;
    file = 0;

    currentN++;

    if(errorOccured == true){
        QMessageBox::warning(this, tr("Error"), tr("Errors occured, please try again."));
        return;
    }
    if(currentN<nFound){
        url = tr(fileNameToDownload[currentN].toStdString().c_str());

        if(QDir(folderToStore[currentN]).exists()==false)QDir().mkdir(folderToStore[currentN]);

        QString fileName = fileNameToStore[currentN];

        if (QFile::exists(fileName)) {
            QFile::remove(fileName);
        }

        file = new QFile(fileName);
        if (!file->open(QIODevice::WriteOnly)) {
            qWarning("Unable to save a file");
            errorOccured = true;
            delete file;
            file = 0;
            return;
        }

        // schedule the request
        httpRequestAborted = false;

        startRequest2(fileNameToDownload[currentN]);
    }else{// fin du telechargement
        progress->setClosable(true);
        progress->close();
        if(errorOccured == true)QMessageBox::warning(this, tr("Error"), tr("Errors occured, please try again."));
        else {
            QMessageBox::about(this, tr("Notify"), tr("Download finished"));
            applyValues();
            /*if (QMessageBox::question(this, tr("Open versioning file"),
                                      tr("Do you want to open versioning file and edit firmware?"),
                                      QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                applyValues();
            }*/
        }

    }

}

void GeneralTab::setCancelDownload(){

    errorOccured = true;
}

void GeneralTab::httpReadyRead2()
{
    if(errorOccured ==true){QMessageBox::warning(this, tr("Error"), tr("Errors occured or download canceled, please try again."));return;}
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    if (file)
        file->write(reply->readAll());
}

