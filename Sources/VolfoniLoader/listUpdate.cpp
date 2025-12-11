#include "listUpdate.h"



ListUpdate::ListUpdate(QWidget* parent,MainWindow *mW, QString currentVersion, QString trigrammeProduct, QString customerNumber, QString user, QString langfile)
    : QDialog(parent,Qt::WindowMaximizeButtonHint | Qt::WindowCloseButtonHint),
  _mW(mW),
  _currentVersion(currentVersion),
  _trigrammeProduct(trigrammeProduct),
  _customerNumber(customerNumber),
  _user(user),
  _langfile(langfile)
{
    this->setWindowIcon(QIcon(":/MainWindow/ressources/volfo160.ico"));
    this->setWindowTitle(tr("Available updates"));
    this->resize(700,470);
    indexSelected = 0;

    m_apply = new QPushButton(tr("Select this update"));
    m_apply->setCursor(Qt::PointingHandCursor);
    m_apply->setFixedWidth(150);
    m_apply->setEnabled(false); // pas possible de cliquer tant qu'on a pas sélectionné de mise à jour
    connect(m_apply, SIGNAL(clicked()), this, SLOT(save()));

    m_cancel = new QPushButton(tr("Cancel"));
    m_cancel->setCursor(Qt::PointingHandCursor);
    m_cancel->setFixedWidth(100);
    connect(m_cancel, SIGNAL(clicked()), this, SLOT(close()));

    layout_buttons = new QHBoxLayout;
    layout_buttons->addWidget(m_apply,0,Qt::AlignRight);
    layout_buttons->addWidget(m_cancel);

    wait  = new QTimer();
    connect(wait, SIGNAL(timeout()), this, SLOT(StopWait()));
    QApplication::setOverrideCursor(Qt::WaitCursor);
    wait->start(5000);// enleve le sablier au bout de 5 secondes (utile si bug de lecture car connexion trop lente ou qui bug)

    connect(&qnam, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(slotAuthenticationRequired(QNetworkReply*,QAuthenticator*)));
    connect(&qnam, SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>)), this, SLOT(sslErrors(QNetworkReply*,QList<QSslError>)));

    CheckUpdates();

}

ListUpdate::~ListUpdate()
{

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions d'affichage
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ListUpdate::drawTableUpdate(){
    // ecriture du tableau
    model = new QStandardItemModel(nAvailableUpdates,3);
    table = new QTableView();

    for (int row = 0; row < nAvailableUpdates; ++row) {
      QStandardItem *item1 = new QStandardItem();
      item1->setText(nameUpdate[row]);
      item1->setTextAlignment(Qt::AlignCenter);
      model->setItem(row,0,item1);

      QStandardItem *item2 = new QStandardItem();
      item2->setText(descriptionUpdate[row]);
      item2->setTextAlignment(Qt::AlignCenter);
      model->setItem(row,1,item2);

      QStandardItem *item3 = new QStandardItem();
      item3->setText(dateUpdate[row]);
      item3->setTextAlignment(Qt::AlignCenter);
      model->setItem(row,2,item3);

      QStandardItem *item4 = new QStandardItem();
      item4->setText(changesUpdate[row]);
      item4->setTextAlignment(Qt::AlignLeft);
      model->setItem(row,3,item4);

    }

    QStringList entetes;
    entetes << tr("Updates name") <<tr("Description") << tr("Date of publication") << tr("Changes with previous version");
    model->setHorizontalHeaderLabels(entetes);
    table->setModel(model);
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    table->horizontalHeader()->setStretchLastSection(true); // dernière colonne adapté a longueur du texte
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);// cases non modifiables
    table->setSelectionBehavior(QAbstractItemView::SelectRows);// focus (fond bleu) sur toute une ligne et non juste la case sous la souris
    connect(table, SIGNAL(clicked(QModelIndex)), this, SLOT(editFonction(QModelIndex)));

    if(_trigrammeProduct.size()!=3){// c'est bien un trigramme
      model->clear();
      model->setHorizontalHeaderLabels(entetes);
    }
    QApplication::restoreOverrideCursor();// fin du sablier, tout s'est bien passé
    wait->stop();// fin de la tempo de securité pour le temps de lecture des infos

    QVBoxLayout *layout_table = new QVBoxLayout;
    layout_table->addWidget(table);

    areaScroll = new QWidget();
    areaScroll->setLayout(layout_table);

    Scroll =  new QScrollArea;
    Scroll->setWidgetResizable(true);
    Scroll->setWidget(areaScroll);

    selectedUpdate = new QLineEdit();
    selectedUpdate->setReadOnly(true);
    connect(selectedUpdate, SIGNAL(textChanged(const QString &)), this, SLOT(enableApply(const QString &)));
    l_selectedUpdate =  new QLabel(tr("Selected update:"));
    l_selectedUpdate->setFixedWidth(100);

    layout_selectedUpdate = new QHBoxLayout;
    layout_selectedUpdate->addWidget(l_selectedUpdate);
    layout_selectedUpdate->addWidget(selectedUpdate);

    QString labelInstalled = QString(tr("Current installed version: ")+"%1").arg(_currentVersion);
    l_currentUpdate =  new QLabel(labelInstalled);

    m_browse = new QPushButton(tr("Browse"));
    m_browse->setFixedWidth(100);
    if(_user == "advanced")m_browse->hide();
    connect(m_browse, SIGNAL(clicked()), this, SLOT(browseUpdate()));


    layout_principal = new QVBoxLayout;
    layout_principal->addWidget(Scroll);
    layout_principal->addWidget(m_browse,0,Qt::AlignLeft);
    layout_principal->addWidget(l_currentUpdate);
    layout_principal->addLayout(layout_selectedUpdate);
    layout_principal->addLayout(layout_buttons);

    setLayout(layout_principal);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions de slots
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ListUpdate::save(){

    QString temp = QString(tr("Are you sure to install %1 version?")).arg(selectedUpdate->text());
    QMessageBox::StandardButton button =QMessageBox::question(this,
                      tr("Install version"),
                      temp,
                      QMessageBox::Yes | QMessageBox::No);
    if (button == QMessageBox::Yes){
        close();
        if(selectedUpdate->text().endsWith(".hex")== true)_mW->requestUpdateFirmware(selectedUpdate->text(),"",true);
        else _mW->requestUpdateFirmware(fileNameUpdate[indexSelected],nameUpdate[indexSelected],false);

    }
}

void ListUpdate::StopWait(){ //si pas de reponse de téléchargement

  QApplication::restoreOverrideCursor();
  wait->stop();
  QMessageBox::critical(this,"Reading time exceeded","Unable to read versioning information, maybe your network is too slow or non-existent. \nTry it again later.\n\n");
  ListUpdate::close();
}

void ListUpdate::browseUpdate(){
    QString fileName = QFileDialog::getOpenFileName(this,
                     tr("Choose an update file"),
                     "./",
                     tr("update files (*.hex)"));
    if(fileName.isEmpty()){
      return;
    }
    selectedUpdate->setText(fileName);


}

void ListUpdate::editFonction(QModelIndex idx){ // reccupere le numero de la ligne actuellement selectionné
    if (!idx.isValid())return;
    indexSelected = idx.row();
    selectedUpdate->setText(nameUpdate[idx.row()]);
}

void ListUpdate::enableApply(const QString & text){ // si le nom de la mise à jour selectionné est non nul, alors on peut passer à la suite
    if(text!="")m_apply->setEnabled(true);
    else m_apply->setEnabled(false);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions de connexion a internet
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ListUpdate::CheckUpdates(){//Verify if a new version is available
    //url = tr("http://www.volfoni.com/VolfoniLoader/Serveur/versioning.xml");
    url = tr("http://vl.volfoni.com/Serveur/versioning.xml");
    QFileInfo fileInfo(url.path());
    QString fileName = fileInfo.fileName();
#ifdef Q_WS_MAC
    QStringList listM = QString(_langfile).split("/");
    fileName ="";
    for(int i = 0; i<listM.size()-1;i++)fileName.append(listM.at(i)+"/");
    fileName.append("versioning.xml");
#endif
    if (QFile::exists(fileName)) {
        QFile::remove(fileName);
    }

    file = new QFile(fileName);
    if (!file->open(QIODevice::WriteOnly)) {
        qWarning("Unable to save the file versioning.xml");
        delete file;
        file = 0;
        return;
    }

    // schedule the request
    httpRequestAborted = false;
    startRequest(url);

}

void ListUpdate::startRequest(QUrl url)
{
    qWarning("send get to http serveur");
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),
            this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()),
            this, SLOT(httpReadyRead()));
}

void ListUpdate::httpFinished()
{
    if (httpRequestAborted) {
        if (file) {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        reply->deleteLater();
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
        if (QMessageBox::question(this, "HTTP",
                                  tr("Redirect to %1 ?").arg(newUrl.toString()),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            url = newUrl;
            reply->deleteLater();
            file->open(QIODevice::WriteOnly);
            file->resize(0);
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
    delete file;
    file = 0;

    GetVersions();

}

void ListUpdate::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply
    if (file)
        file->write(reply->readAll());
    else
        qWarning("File Write Error !");
}

void ListUpdate::slotAuthenticationRequired(QNetworkReply*,QAuthenticator *authenticator)
{
    authenticator->setUser("VolfoniLoader");
    authenticator->setPassword("vol123");
}

void ListUpdate::sslErrors(QNetworkReply*,const QList<QSslError> &errors)
{
    QString errorString;
    foreach (const QSslError &error, errors) {
        if (!errorString.isEmpty())
            errorString += ", ";
        errorString += error.errorString();
    }

    if (QMessageBox::warning(this, "HTTP",
                             tr("One or more SSL errors has occurred: %1").arg(errorString),
                             QMessageBox::Ignore | QMessageBox::Abort) == QMessageBox::Ignore) {
        reply->ignoreSslErrors();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Fonctions pour récupérer des fichiers en ligne
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void ListUpdate::GetVersions(){

    QString macChange ="versioning.xml";
#ifdef Q_WS_WIN32
    file = new QFile(macChange);
#endif
#ifdef Q_WS_MAC
    QStringList listM = QString(_langfile).split("/");
    macChange ="";
    for(int i = 0; i<listM.size()-1;i++)macChange.append(listM.at(i)+"/");
    macChange.append("versioning.xml");
    file = new QFile(macChange);
#endif
    if (!file->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qWarning("Unable to open versioning information");
        QMessageBox::critical(this,tr("Error"),tr("\nUnable to acces the information about updates.\n\nPlease try again later"));
    }

    QString fileName = macChange;

    QDomDocument settings(fileName);

    QString errorMsg;
    if(!settings.setContent(file, &errorMsg)){ //handle error (print error message on pane for instance)
      std::cout << "error: couldn't read " << fileName.toStdString().c_str()  << " , error message: " << errorMsg.toStdString().c_str() << std::endl;
      file->close();
    }
    file->close();

    //parse the settings tree and restore state
    QDomElement root = settings.documentElement();
    QDomElement a = root.firstChildElement(_trigrammeProduct);
    QDomElement b = a.firstChildElement("new-version");

    nAvailableUpdates = 0;

    // exemple de nom de version : IR_IR V01.17.01
    QStringList list = QString(_currentVersion).split(".");//découpe le nom de la version selon les "V"
    QString allowedUpdate = "";
    if(list.size()>1)allowedUpdate = list.at(0); // allowedUpdate = IR_IR V01
    if(allowedUpdate ==""){
        wait->stop();
        QMessageBox::about(this,tr("Reading version error"),tr("\nMaybe last update failed."));
    }

    while(!b.isNull()){// balaye les versions disponibles sur la base de données
      QDomElement c = b.firstChildElement("name");
      bool allowedUpdateValue = false;
      QString _temp = c.text();

      // exemple de nom de version : IR_IR V01.17.01
      QStringList _list = QString(_temp).split(".");//découpe le nom de la version selon les "."
      QString _allowedUpdate = "";
      if(_list.size()>1)_allowedUpdate = _list.at(0); // allowedUpdate = IR_IR V01

      if(_allowedUpdate == _customerNumber)allowedUpdateValue = true;
      else allowedUpdateValue = false;

      if(_user == "advanced" && allowedUpdateValue==true){ // si il y a le même numéro de modification majeure alors on l'affiche
        nameUpdate[nAvailableUpdates] = c.text();
        c = b.firstChildElement("filename");
        fileNameUpdate[nAvailableUpdates] = c.text();
        c = b.firstChildElement("description");
        descriptionUpdate[nAvailableUpdates] = c.text();
        c = b.firstChildElement("date");
        dateUpdate[nAvailableUpdates] = c.text();
        c = b.firstChildElement("modification");

        int n = 0;
        while(!c.isNull()){
          if(n!=0)changesUpdate[nAvailableUpdates].append("/ ");
          n++;
          changesUpdate[nAvailableUpdates].append(c.text());
          changesUpdate[nAvailableUpdates].append(" ");
          c =c.nextSiblingElement("modification");
        }
        if(fileNameUpdate[nAvailableUpdates]!="")nAvailableUpdates++; // test si la mise a jour n'est pas nul (bug de fichier xml)
      }
      else if(_user == "admin"){
          nameUpdate[nAvailableUpdates] = c.text();
          c = b.firstChildElement("filename");
          fileNameUpdate[nAvailableUpdates] = c.text();
          c = b.firstChildElement("description");
          descriptionUpdate[nAvailableUpdates] = c.text();
          c = b.firstChildElement("date");
          dateUpdate[nAvailableUpdates] = c.text();
          c = b.firstChildElement("modification");

          int n = 0;
          while(!c.isNull()){
            if(n!=0)changesUpdate[nAvailableUpdates].append("/ ");
            n++;
            changesUpdate[nAvailableUpdates].append(c.text());
            changesUpdate[nAvailableUpdates].append(" ");
            c =c.nextSiblingElement("modification");
          }
          if(fileNameUpdate[nAvailableUpdates]!="")nAvailableUpdates++; // test si la mise a jour n'est pas nul (bug de fichier xml)


      }

      b = b.nextSiblingElement("new-version");
    }

    file->close();
    file->remove();

    // securité pour ne pas depasser la taille max des tableaux QString (200 lignes)
    if(nAvailableUpdates>199){
        QMessageBox::warning(this,
                     tr("Bug"),
                     tr("Sorry, an error occurred when reading database"));

        ListUpdate::close();
    }
    else if(nAvailableUpdates==0 && _user == "advanced"){ // si N = 0, erreur de lecture, essayer de recommencer pour voir si le bug re apparait
                                    // ou bien bug d'installation de la mise à jour, le numero est illisible
                                    // ou bien 0 mise a jour disponible pour ce produit sur le serveur
        QMessageBox::warning(this,tr("Problem"), tr("No available update for this product"));

    }
    //else if(_trigrammeProduct.size()>3)QMessageBox::warning(this,tr("Problem"), tr("probleme, voir numero client, pas de numero de serie lisible"));

    drawTableUpdate();

}
