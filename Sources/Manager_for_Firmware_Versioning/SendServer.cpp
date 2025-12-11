#include <QtGui>


#include "SendServer.h"



SendServer::SendServer(QWidget* parent,MainWindow *mW,QString rootFolder,QString m_url, QString m_login, QString m_pwd, QString trigrams[100], int nDevices)
    : QDialog(parent,Qt::WindowCloseButtonHint),
      _mW(mW),
      _rootFolder(rootFolder),
      _url(m_url),
      _login(m_login),
      _pwd(m_pwd)

{

    _nDevices = nDevices;
    for(int i = 0; i<_nDevices;i++)_trigrams[i]= trigrams[i];

    progress = new Progress(this, 1);
    progress->setValueTitle("Progress to upload files");
    progress->setClosable(false);
    progress->setValueProgress(0);
    progress->setValueLabel("Start Uploading");
    progress->setSS(this);
    progress->exec();

}
void SendServer::initialisation(){

    _ftp = new QFtp();
    connect(_ftp, SIGNAL(commandFinished(int, bool)),this, SLOT(ftpCommandFinished(int, bool)));
    connect(_ftp, SIGNAL(listInfo(const QUrlInfo &) ), this, SLOT(listinfo(const QUrlInfo &) ));

    _ftp->connectToHost(_url,21);

    _ftp->login(_login,_pwd);

    _ftp->cd("Serveur");
    /////////////////////////////////////////////////////////////////////////////////
    // liste les fichiers a envoyer (chemin de source et url de destination)
    QString fileName = _rootFolder + "versioning.xml";
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
    nFound = 1; //le premier est versionning.xml
    for(int i =0; i<_nDevices;i++){
        QDomElement a = root.firstChildElement(_trigrams[i]);
        QDomElement bb = a.firstChildElement("new-version");
        while(!bb.isNull()){// balaye les versions disponibles
            bb = bb.nextSiblingElement("new-version");
            nFound++;// pour compter le nombre de fichiers a uploader et adapter la progress bar
        }
    }
    std::cout<<"n file to upload = "<<nFound<<std::endl;
    space = 100/(nFound+1);
    int count = 0;
    fileNameToSend[count] = fileName;
    fileNameToUpload[count] = "versionning.xml";
    folderToUpload[count] = "";
    count++;

    for(int i =0; i<_nDevices;i++){
        QDomElement a = root.firstChildElement(_trigrams[i]);
        QDomElement b = a.firstChildElement("new-version");

        while(!b.isNull()){// balaye les versions disponibles

            QDomElement c = b.firstChildElement("filename");
            shortfileNameToSend[count] = c.text();
            fileNameToSend[count] = _rootFolder+_trigrams[i]+"/"+c.text();
            fileNameToUpload[count] = c.text();
            folderToUpload[count] = _trigrams[i];

            b = b.nextSiblingElement("new-version");

            count++;
        }
    }

    startUpload();
}

void SendServer::startUpload(){
    progress->setValueProgress(0);
    for(int i =0;i<1000;i++)fileOpened[i]=false;
    nUploaded = 0;
    errorOccured = false;
    progress->setValueLabel("Start uploading...");


    _file[nUploaded] = new QFile(fileNameToSend[nUploaded]); // Je récupère le chemin du fichier sur lequel j'ai cliqué
    if (_file[nUploaded]->open(QIODevice::ReadOnly)) // Je vérifie si je peux l'ouvrir
    {
        fileOpened[nUploaded]=true;
        QByteArray data = _file[nUploaded]->readAll();
        _file[nUploaded]->close();
        progress->setValueLabel("Upload versioning.xml...");
        _ftp->put(data, "versioning.xml",QFtp::Ascii); // J'envoie le fichier

    }else {
        if(fileOpened[nUploaded]==false){errorOccured = true; std::cout<<"probleme de fichier "<<fileNameToSend[nUploaded].toStdString()<<std::endl;}
    }

    ////////////////////////////////////////////////////////////////////////////////////



}
void SendServer::listinfo(const QUrlInfo &a){
    _list.append(a.name());
}


void SendServer::ftpCommandFinished(int id , bool error){


    std::cout<<"ID fini = "<<id<<std::endl;
    if(error==true)std::cout<<"erreur"<<std::endl;

    //if(id == currentID){
        if(_ftp->currentCommand() == QFtp::Mkdir){
            if(!error){
                std::cout<<"dossier créé"<<std::endl;
                _file[nUploaded] = new QFile(fileNameToSend[nUploaded]); // Je récupère le chemin du fichier sur lequel j'ai cliqué
                if (_file[nUploaded]->open(QIODevice::ReadOnly)) // Je vérifie si je peux l'ouvrir
                {
                    std::cout<<"envoi fichier et dossier créé"<<std::endl;
                    fileOpened[nUploaded]=true;
                    progress->setValueLabel("Upload "+shortfileNameToSend[nUploaded]+ "...");
                    QByteArray data = _file[nUploaded]->readAll();
                    _file[nUploaded]->close();
                    QString destination = QString("%1/%2").arg(folderToUpload[nUploaded]).arg(fileNameToUpload[nUploaded]);
                    _ftp->put(data, destination,QFtp::Ascii); // J'envoie le fichier

                }else {
                    if(fileOpened[nUploaded]==false){errorOccured = true;std::cout<<"probleme de fichier "<<fileNameToSend[nUploaded].toStdString()<<std::endl;}
                }
            }else {errorOccured = true; std::cout<<"probleme avec mkdir "<<fileNameToSend[nUploaded].toStdString()<<std::endl;}

        }
        else if(_ftp->currentCommand() == QFtp::List){
            if(!error){
                std::cout<<"fini check folder"<<std::endl;
                bool mkdirFolder = true;
                foldertoCheck = folderToUpload[nUploaded];
                for(int i = 0; i<_list.size();i++){
                    std::cout<<_list.at(i).toStdString()<<std::endl;
                    if(_list.at(i)==foldertoCheck){mkdirFolder= false;std::cout<<"dossier deja existant"<<std::endl;break;}
                }
                if(mkdirFolder == true){
                    std::cout<<"je crée le dossier"<<std::endl;
                    progress->setValueLabel("Create "+foldertoCheck+ " folder on server");
                    _ftp->mkdir(foldertoCheck);

                }else{

                    _file[nUploaded] = new QFile(fileNameToSend[nUploaded]); // Je récupère le chemin du fichier sur lequel j'ai cliqué
                    if (_file[nUploaded]->open(QIODevice::ReadOnly)) // Je vérifie si je peux l'ouvrir
                    {
                        std::cout<<"envoi fichier sans creer dossier"<<std::endl;
                        fileOpened[nUploaded]=true;
                        QByteArray data = _file[nUploaded]->readAll();
                        _file[nUploaded]->close();
                        progress->setValueLabel("Upload "+shortfileNameToSend[nUploaded]+ "...");
                        QString destination = QString("%1/%2").arg(folderToUpload[nUploaded]).arg(fileNameToUpload[nUploaded]);
                        _ftp->put(data, destination,QFtp::Ascii); // J'envoie le fichier

                    }else {
                        if(fileOpened[nUploaded]==false){errorOccured = true; std::cout<<"probleme de fichier "<<fileNameToSend[nUploaded].toStdString()<<std::endl;}
                    }
                }
            }else {errorOccured = true; std::cout<<"probleme avec list "<<fileNameToSend[nUploaded].toStdString()<<std::endl;}
        }
        else if(_ftp->currentCommand() == QFtp::Put){
            if(!error){
                progress->setValueProgress((nUploaded+1)*space);
                if(nUploaded==nFound-1)progress->setValueProgress(100);
                nUploaded++;

                std::cout<<"fini upload fichier"<<std::endl;
                if(nUploaded<nFound){
                    _list.clear();
                    _ftp->list() ;
                }else{
                    // fini

                    progress->setValueLabel("Upload finished");
                    QString message = "Upload Finished!\n";
                    if(errorOccured == true)message.append("But error(s) occured during process, check that all is fine on server.");
                    QMessageBox::about(this,"Notify", message);
                    progress->setClosable(true);

                    if(errorOccured == true){
                        QMessageBox::StandardButton button =
                                QMessageBox::question(this,
                                                      tr("Retry"),
                                                      tr("Do you want to retry to upload files?"),
                                                      QMessageBox::Yes | QMessageBox::No);
                        if (button == QMessageBox::Yes){
                            progress->setClosable(false);
                            startUpload();
                        }else progress->close();

                    }else progress->close();
                }
            }else {errorOccured = true; std::cout<<"probleme avec put "<<fileNameToSend[nUploaded].toStdString()<<std::endl;}
        }
    //}
}



SendServer::~SendServer()
{

}

