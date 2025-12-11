#include <QtGui>
#include <QList>

#include "editTab.h"


EditTab::EditTab(QString trigram, MainWindow *mW, QString fileNameVersioning, QString folder, QString pathResource)
    : QDialog(),
      _trigram(trigram),
      _mW(mW),
      _fileNameVersioning(fileNameVersioning),
      _folder(folder),
      KEY_Decrypt("volfoni123"),
      debutVersion(":102000"),
      _pathResource(pathResource)
{

    encrypt = new Encrypt();
    decrypt = new Decrypt();

    noError = true;

    layout_updates = new QVBoxLayout;
    signalMapperDeleteUpdate = new QSignalMapper;
    connect(signalMapperDeleteUpdate, SIGNAL(mapped(int)), this, SLOT(deleteUpdate(int)));

    signalMapperDelModif = new QSignalMapper;
    connect(signalMapperDelModif, SIGNAL(mapped(QString)), this, SLOT(delModification(QString)));

    signalMapperAddModif = new QSignalMapper;
    connect(signalMapperAddModif, SIGNAL(mapped(QString)), this, SLOT(addModification(QString)));

    signalMapperBrowseUpdate = new QSignalMapper;
    connect(signalMapperBrowseUpdate, SIGNAL(mapped(int)), this, SLOT(browseUpdt(int)));

    signalMapperEncryptUpdate = new QSignalMapper;
    connect(signalMapperEncryptUpdate, SIGNAL(mapped(int)), this, SLOT(encryptUpdt(int)));

    signalMapperDecryptUpdate = new QSignalMapper;
    connect(signalMapperDecryptUpdate, SIGNAL(mapped(int)), this, SLOT(decryptUpdt(int)));

    signalMapperCRC_Update = new QSignalMapper;
    connect(signalMapperCRC_Update, SIGNAL(mapped(int)), this, SLOT(CRC_Updt(int)));

    nUpdate = 0;

    QLabel *l_filter = new QLabel("Filter:");
    l_filter->setFixedWidth(60);

    comboFilter = new QComboBox;
    comboFilter->addItem("All");
    comboFilter->setFixedWidth(150);
    connect(comboFilter, SIGNAL(activated(int)), this, SLOT(validFilter(int)));

    QHBoxLayout *layout_filter = new QHBoxLayout;
    layout_filter->addWidget(l_filter);
    layout_filter->addWidget(comboFilter,0,Qt::AlignLeft);

    QFrame *frame = new QFrame;
    frame->setFrameStyle(QFrame::HLine | QFrame::Raised);
    frame->setLineWidth(0);
    frame->setMidLineWidth(3);

     if(QFile::exists(_fileNameVersioning)){
        QFile file(_fileNameVersioning);
        QDomDocument settings(_fileNameVersioning);

        //handle non-existent file case (print debug message and return)
        if (!file.open(QIODevice::ReadOnly)){
            std::cout << "Error while reading frontEnd configuration file" << std::endl;
            return;
        }
        QString errorMsg;
        if(!settings.setContent(&file, &errorMsg)){ //handle error (print error message on pane for instance)
            std::cout << "error: couldn't read " << _fileNameVersioning.toStdString().c_str()  << " , error message: " << errorMsg.toStdString().c_str() << std::endl;
            file.close();
            return;
        }
        file.close();


        QDomElement root = settings.documentElement();
        QDomElement a = root.firstChildElement(_trigram);
        QDomElement b = a.firstChildElement("new-version");
        int nFound = 0;

        while(!b.isNull()){
            fileNameValue[nFound] = "";
            newfileNameValue[nFound] = "";
            nameValue[nFound] = "";
            descriptionValue[nFound] = "";
            dateValue[nFound] = "";
            CRCValue[nFound] = "";
            QDomElement c = b.firstChildElement("filename");
            fileNameValue[nFound] = c.text();
            c = b.firstChildElement("name");
            nameValue[nFound] = c.text();
            c = b.firstChildElement("date");
            dateValue[nFound] = c.text();
            c = b.firstChildElement("description");
            descriptionValue[nFound] = c.text();
            c = b.firstChildElement("CRC");
            CRCValue[nFound] = c.text();
            CRCValue[nFound] = CRCValue[nFound].toUpper();

            c = b.firstChildElement("modification");
            nModifFound[nFound] = 0;
            while(!c.isNull()){
                modifValue[nFound][nModifFound[nFound]]= c.text();
                nModifFound[nFound]++;
                c =c.nextSiblingElement("modification");
            }

            nFound++;
            b = b.nextSiblingElement("new-version");
        }
        //////////////////////////////////////////////////////////////////////////////////////////
        // tri par type de version
        bool ok = true;
        for(int i =0; i<nFound; i++){
            if(nameValue[i].contains("_V")==false || nameValue[i].contains(".")==false)ok = false;
            QStringList list = QString(nameValue[i]).split(".");
            if(list.size()<3)ok = false;
        }
        if(ok == true){// pas de tri si les noms de versions ne sont pas bons
            // variable temporaires
            QString t_fileNameValue[1000];
            QString t_newfileNameValue[1000];
            QString t_nameValue[1000];
            QString t_descriptionValue[1000];
            QString t_dateValue[1000];
            QString t_CRCValue[1000];
            QString t_modifValue[1000][100];
            int t_nModifFound[1000];

            for(int i =0; i<nFound; i++){
                QStringList list = QString(nameValue[i]).split(".");
                QString tempo = list.at(0);
                bool okNew = true;
                for(int j = 0; j<i; j++){
                    if(t_nameValue[j].contains(tempo)==true){// verif si la version est deja dans le tableau ou non
                        okNew=false;
                        break;
                    }
                }
                if(okNew==true){// nouvelle version, on l'ajoute à la fin
                    t_fileNameValue[i] = fileNameValue[i];
                    t_newfileNameValue[i] = newfileNameValue[i];
                    t_nameValue[i] = nameValue[i];
                    t_descriptionValue[i] = descriptionValue[i];
                    t_dateValue[i] = dateValue[i];
                    t_CRCValue[i] = CRCValue[i];
                    t_nModifFound[i] = nModifFound[i];
                    for(int k = 0; k<nModifFound[i];k++)t_modifValue[i][k] = modifValue[i][k];
                }
                else{// version existante, on les regroupe et on les classe dans l'ordre

                    // on l'ajoute a la fin aussi
                    t_fileNameValue[i] = fileNameValue[i];
                    t_newfileNameValue[i] = newfileNameValue[i];
                    t_nameValue[i] = nameValue[i];
                    t_descriptionValue[i] = descriptionValue[i];
                    t_dateValue[i] = dateValue[i];
                    t_CRCValue[i] = CRCValue[i];
                    t_nModifFound[i] = nModifFound[i];
                    for(int k = 0; k<nModifFound[i];k++)t_modifValue[i][k] = modifValue[i][k];

                    // nom de versions dans l'ordre
                    for(int d = 0; d<i;d++){
                        if(t_nameValue[d].contains(tempo)==true){

                            QString tt_fileNameValue = t_fileNameValue[i];
                            QString tt_newfileNameValue = t_newfileNameValue[i];
                            QString tt_nameValue = t_nameValue[i];
                            QString tt_descriptionValue = t_descriptionValue[i];
                            QString tt_dateValue = t_dateValue[i];
                            QString tt_CRCValue = t_CRCValue[i];
                            QString tt_modifValue[100];
                            int tt_nModifFound = t_nModifFound[i];
                            for(int h = 0; h<tt_nModifFound;h++)tt_modifValue[h]=t_modifValue[i][h];

                            for(int gg = i; gg>d+1;gg--){
                                t_fileNameValue[gg] = t_fileNameValue[gg-1];
                                t_newfileNameValue[gg] = t_newfileNameValue[gg-1];
                                t_nameValue[gg] = t_nameValue[gg-1];
                                t_descriptionValue[gg] = t_descriptionValue[gg-1];
                                t_dateValue[gg] = t_dateValue[gg-1];
                                t_CRCValue[gg] = t_CRCValue[gg-1];
                                t_nModifFound[gg] = t_nModifFound[gg-1];
                                for(int h = 0; h<t_nModifFound[gg-1];h++)t_modifValue[gg][h]=t_modifValue[gg-1][h];
                            }

                            t_fileNameValue[d+1] = tt_fileNameValue;
                            t_newfileNameValue[d+1] = tt_newfileNameValue;
                            t_nameValue[d+1] = tt_nameValue;
                            t_descriptionValue[d+1] = tt_descriptionValue;
                            t_dateValue[d+1] = tt_dateValue;
                            t_CRCValue[d+1] = tt_CRCValue;
                            t_nModifFound[d+1] = tt_nModifFound;
                            for(int h = 0; h<tt_nModifFound;h++)t_modifValue[d+1][h]=tt_modifValue[h];

                            break;

                        }
                    }


                    // numero de version dans l'ordre
                    int repeat = 0;
                    int tempoN = 0;
                    for(int d = 0; d<=i;d++){
                        if(t_nameValue[d].contains(tempo)==true){
                            repeat++; // on a repeat fois cette version
                            //std::cout<<t_nameValue[d].toStdString()<<"   "<<d<<std::endl;
                        }
                    }
                    for(int e = 0; e<repeat;e++){// on repete l'operation repeat fois pour etre sur qu'on a bien tout mis dans l'ordre
                        int next = 0;
                        for(int d = 0; d<i;d++){
                            if(t_nameValue[d].contains(tempo)==true && t_nameValue[d+1].contains(tempo)==true){

                                QStringList list2 = QString(t_nameValue[d]).split(".");
                                QString ttt = QString("%1%2").arg(list2.at(1)).arg(list2.at(2));
                                tempoN = ttt.toInt(); // numéro de la version lu

                                QStringList list3 = QString(t_nameValue[d+1]).split(".");
                                QString ttt2 = QString("%1%2").arg(list3.at(1)).arg(list3.at(2));
                                next = ttt2.toInt(); // numéro de la version lu, indice +1

                                if(next>tempoN){// on echange leur place
                                    QString tt_fileNameValue = t_fileNameValue[d];
                                    QString tt_newfileNameValue = t_newfileNameValue[d];
                                    QString tt_nameValue = t_nameValue[d];
                                    QString tt_descriptionValue = t_descriptionValue[d];
                                    QString tt_dateValue = t_dateValue[d];
                                    QString tt_CRCValue = t_CRCValue[d];
                                    QString tt_modifValue[100];
                                    int tt_nModifFound = t_nModifFound[d];
                                    for(int h = 0; h<tt_nModifFound;h++)tt_modifValue[h]=t_modifValue[d][h];

                                    t_fileNameValue[d] = t_fileNameValue[d+1];
                                    t_newfileNameValue[d] = t_newfileNameValue[d+1];
                                    t_nameValue[d] = t_nameValue[d+1];
                                    t_descriptionValue[d] = t_descriptionValue[d+1];
                                    t_dateValue[d] = t_dateValue[d+1];
                                    t_CRCValue[d] = t_CRCValue[d+1];
                                    t_nModifFound[d] = t_nModifFound[d+1];
                                    for(int h = 0; h<t_nModifFound[d+1];h++)t_modifValue[d][h]=t_modifValue[d+1][h];

                                    t_fileNameValue[d+1] = tt_fileNameValue;
                                    t_newfileNameValue[d+1] = tt_newfileNameValue;
                                    t_nameValue[d+1] = tt_nameValue;
                                    t_descriptionValue[d+1] = tt_descriptionValue;
                                    t_dateValue[d+1] = tt_dateValue;
                                    t_CRCValue[d+1] = tt_CRCValue;
                                    t_nModifFound[d+1] = tt_nModifFound;
                                    for(int h = 0; h<tt_nModifFound;h++)t_modifValue[d+1][h]=tt_modifValue[h];

                                    break;
                                }
                            }
                        }
                    }
                }
            }

            for(int i = 0; i <nFound; i++){// du - au plus recent
                fileNameValue[i] = t_fileNameValue[nFound -1 -i];
                newfileNameValue[i] = t_newfileNameValue[nFound -1 -i];
                nameValue[i] = t_nameValue[nFound -1 -i];
                descriptionValue[i] = t_descriptionValue[nFound -1 -i];
                dateValue[i] = t_dateValue[nFound -1 -i];
                CRCValue[i] = t_CRCValue[nFound -1 -i];
                nModifFound[i] = t_nModifFound[nFound -1 -i];
                for(int k = 0; k<nModifFound[i];k++)modifValue[i][k] = t_modifValue[nFound -1 -i][k];
            }
        }

        //////////////////////////////////////////////////////////////////////////////////////////
        for(int i = 0; i <nFound; i++){
            addUpdateFunction(true);
            nUpdate++;
        }
        updateFilter();
    }

    addUpdateButton = new QPushButton(tr("Add an update"));
    addUpdateButton->setCursor(Qt::PointingHandCursor);
    addUpdateButton->setFixedWidth(150);
    addUpdateButton->setIcon(QIcon(":/MainWindow/ressources/ajout.png"));
    connect(addUpdateButton, SIGNAL(clicked()), this, SLOT(addUpdate()));

    QVBoxLayout *layout_bis = new QVBoxLayout;
    layout_bis->addLayout(layout_updates);
    layout_bis->addSpacing(180);

    areaScroll = new QWidget();
    areaScroll->setLayout(layout_bis);

    Scroll =  new QScrollArea;
    Scroll->setWidgetResizable(true);
    Scroll->setWidget(areaScroll);

    layout_principal = new QVBoxLayout;
    layout_principal->addLayout(layout_filter);
    layout_principal->addSpacing(10);
    layout_principal->addWidget(frame);
    layout_principal->addWidget(Scroll);
    layout_principal->addWidget(addUpdateButton);

    setLayout(layout_principal);
    setMinimumHeight(270);

}

void EditTab::NametextChanged(const QString & texte){

    if(texte.contains(".")==true)
    {
        updateFilter();
    }
    // update des couleurs
    for(int i = 0; i<nUpdate;i++){
        if(name[i]->text().contains(".")){
        QStringList tempo = QString(name[i]->text()).split(".");
        QString tempo2 = tempo.at(0);
        QString text = "";
        int somme = 0;
        for(int j =0; j<tempo2.size();j++){text.append(QString(QChar(tempo2.at(j)))+"\n");somme+=tempo2.at(j).toAscii();}
        somme = somme*somme*somme;
        text.replace("_", " ");
        l_version[i]->setText(text);
        int r = somme%251;
        int g = somme%201;
        int b = somme%187;
        colorLabel[i]= QColor(r,g,b,120);
        l_version[i]->setPalette(colorLabel[i]);
        }else {
            l_version[i]->setPalette(QColor(0,0,0,120));
            l_version[i]->setText("");
        }
    }
}

void EditTab::updateFilter(){
    comboFilter->clear();
    comboFilter->addItem("All");
    nVersionDiff = 0;
    for(int i = 0; i<nUpdate;i++){
        if(name[i]->text().contains(".")==true){

            QStringList list = QString(name[i]->text()).split(".");
            QString tempo = list.at(0);

            bool newV = true;
            for(int i = 0; i <nVersionDiff; i++){
                if(tempo == diffVersions[i]){
                    newV = false;
                    break;
                }
            }
            if(newV==true){
                diffVersions[nVersionDiff]=tempo;
                nVersionDiff++;
            }
        }
    }
    for(int i = 0; i<nVersionDiff;i++){// suprrime si il existe plus
        bool existV = false;
        for(int j = 0; j<nUpdate;j++){
            if(name[j]->text().contains(diffVersions[i])==true){
                existV = true;
                break;            }
        }
        if(existV == false){
            if(i!=nVersionDiff-1)for(int j = i; j<nVersionDiff;j++)diffVersions[j] = diffVersions[j+1];
            nVersionDiff--;
        }
    }
    for(int i = 0; i<nVersionDiff;i++)comboFilter->addItem(diffVersions[i]);
}

void EditTab::validFilter(int a){
    if(comboFilter->currentIndex()!=0){
        for(int i = 0; i<nUpdate;i++){
            if(name[i]->text().contains(".")==true){
                QStringList list = QString(name[i]->text()).split(".");
                if(list.at(0)==comboFilter->currentText()){
                    if(GroupBoxUpdate[i]->isVisible()==false)GroupBoxUpdate[i]->show();
                }
                else if(GroupBoxUpdate[i]->isVisible()==true)GroupBoxUpdate[i]->hide();
            }
        }
    }else for(int i = 0; i<nUpdate;i++)if(GroupBoxUpdate[i]->isVisible()==false)GroupBoxUpdate[i]->show();
}

void EditTab::deleteUpdate(int deleted){

    for(int i = deleted; i<nUpdate-1; i++){
        fileName[i]->setText(fileName[i+1]->text());
        newfileName[i]->setText(newfileName[i+1]->text());
        name[i]->setText(name[i+1]->text());
        description[i]->setText(description[i+1]->text());
        date[i]->setText(date[i+1]->text());
        CRC[i]->setText(CRC[i+1]->text());
        l_version[i]->setText(l_version[i+1]->text());
        l_version[i]->setPalette(l_version[i+1]->palette());
        for(int j = 0; j<100;j++){
            if(delModif[i+1][j]->isVisible()==true && delModif[i][j]->isVisible()==false)delModif[i][j]->setVisible(true);
            else if(delModif[i+1][j]->isVisible()==true && delModif[i][j]->isVisible()==true);
            else delModif[i][j]->setVisible(false);

            if(addModif[i+1][j]->isVisible()==true && addModif[i][j]->isVisible()==false)addModif[i][j]->setVisible(true);
            else if(addModif[i+1][j]->isVisible()==true && addModif[i][j]->isVisible()==true);
            else addModif[i][j]->setVisible(false);

            if(l_modif[i+1][j]->isVisible()==true && l_modif[i][j]->isVisible()==false)l_modif[i][j]->setVisible(true);
            else if(l_modif[i+1][j]->isVisible()==true && l_modif[i][j]->isVisible()==true);
            else l_modif[i][j]->setVisible(false);

            if(modif[i+1][j]->isVisible()==true && modif[i][j]->isVisible()==false)modif[i][j]->setVisible(true);
            else if(modif[i+1][j]->isVisible()==true && modif[i][j]->isVisible()==true);
            else modif[i][j]->setVisible(false);
        }

        for(int j = 0; j<nModifFound[i+1];j++){
            modif[i][j]->setText(modif[i+1][j]->text());
        }

    }

    QLayoutItem *child;
    child = layout_updates->takeAt(nUpdate-1);

    delete child->widget();
    delete child;

    nUpdate--;
}

void EditTab::delModification(QString values){
    QStringList list = QString(values).split("x");
    int n_Update = list.at(0).toInt();
    int n_Modif = list.at(1).toInt();

    for(int j = n_Modif; j<nModifFound[n_Update]-1;j++){
        modif[n_Update][j]->setText(modif[n_Update][j+1]->text());
    }
    if(n_Modif==nModifFound[n_Update]-1){
        if(addModif[n_Update][nModifFound[n_Update]-2]->isVisible()==false)addModif[n_Update][nModifFound[n_Update]-2]->setVisible(true);

        l_modif[n_Update][nModifFound[n_Update]-1]->setVisible(false);
        modif[n_Update][nModifFound[n_Update]-1]->setVisible(false);
        delModif[n_Update][nModifFound[n_Update]-1]->setVisible(false);
        addModif[n_Update][nModifFound[n_Update]-1]->setVisible(false);

    }
    else for(int j = n_Modif; j<nModifFound[n_Update];j++){
        if(delModif[n_Update][j+1]->isVisible()==true && delModif[n_Update][j]->isVisible()==false)delModif[n_Update][j]->setVisible(true);
        else if(delModif[n_Update][j+1]->isVisible()==true && delModif[n_Update][j]->isVisible()==true);
        else delModif[n_Update][j]->setVisible(false);

        if(addModif[n_Update][j+1]->isVisible()==true && addModif[n_Update][j]->isVisible()==false)addModif[n_Update][j]->setVisible(true);
        else if(addModif[n_Update][j+1]->isVisible()==true && addModif[n_Update][j]->isVisible()==true);
        else addModif[n_Update][j]->setVisible(false);

        if(l_modif[n_Update][j+1]->isVisible()==true && l_modif[n_Update][j]->isVisible()==false)l_modif[n_Update][j]->setVisible(true);
        else if(l_modif[n_Update][j+1]->isVisible()==true && l_modif[n_Update][j]->isVisible()==true);
        else l_modif[n_Update][j]->setVisible(false);

        if(modif[n_Update][j+1]->isVisible()==true && modif[n_Update][j]->isVisible()==false)modif[n_Update][j]->setVisible(true);
        else if(modif[n_Update][j+1]->isVisible()==true && modif[n_Update][j]->isVisible()==true);
        else modif[n_Update][j]->setVisible(false);
    }
    nModifFound[n_Update]--;
    if(nModifFound[n_Update]==1){
        if(delModif[n_Update][0]->isVisible()==true)delModif[n_Update][0]->setVisible(false);
        if(addModif[n_Update][0]->isVisible()==false)addModif[n_Update][0]->setVisible(true);
    }
}

void EditTab::addModification(QString values){

    QStringList list = QString(values).split("x");
    int n_Update = list.at(0).toInt();
    int n_Modif = list.at(1).toInt();
    addModif[n_Update][n_Modif+1]->setVisible(true);
    l_modif[n_Update][n_Modif+1]->setVisible(true);
    modif[n_Update][n_Modif+1]->setVisible(true);
    modif[n_Update][n_Modif+1]->setText("");
    delModif[n_Update][n_Modif+1]->setVisible(true);

    addModif[n_Update][n_Modif]->setVisible(false);
    delModif[n_Update][n_Modif]->setVisible(true);

    nModifFound[n_Update]++;
}

void EditTab::browseUpdt(int a){
    QString fileNameU = QFileDialog::getOpenFileName(this,tr("Choose update file"),_folder + "/" +_trigram, tr("updates files (*.hex)"));
    if(fileNameU.isEmpty()){
      //the user did not choose a  file
      return;
    }

    QStringList list = QString(fileNameU).split("/");//découpe le chemin selon les "/"
    if(list.size()>0)fileName[a]->setText(list.at(list.size()-1));
    if(QFile::exists(_folder + "/" + _trigram +"/"+ fileName[a]->text())==false){
        if (QMessageBox::question(this, tr("Notify"),
                                  tr("The file '%1' doesn't exist in %2 directory, do you want to copy it inside?\n"
                                     "It is required to send root folder to server.").arg(list.at(list.size()-1)).arg(_trigram),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            if(QDir(_folder+"/"+_trigram).exists()==false)QDir().mkdir(_folder+"/"+_trigram);
            QFile::copy(fileNameU,_folder+"/"+_trigram+"/" + list.at(list.size()-1));
        }
    }
}

void EditTab::encryptUpdt(int a){
    if(fileName[a]->text()!=""){
        QString fileNameU = QString("%1%2/%3").arg(_folder).arg(_trigram).arg(fileName[a]->text());
        if(QFile::exists(fileNameU)==false){
            QMessageBox::warning(this,"Warning","File not exists.");
            return;
        }
        bool dejaEncrypte = false;

        QString cheminFichier = _folder + "/" + _trigram +"/"+ fileName[a]->text();
        cheminFichier.replace("//","/");
        QString cryptedDebutVersion = encrypt->encryptLine(debutVersion,KEY_Decrypt);
        QFile fichier(cheminFichier);
        fichier.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream flux(&fichier);
        QString ligne;
        while(! flux.atEnd())
        {
            ligne = flux.readLine();
            //traitement de la ligne
            if(ligne.startsWith(debutVersion)){// fichier non encrypté, rien a faire
                dejaEncrypte = false;
                break;
            }
            if(ligne.startsWith(cryptedDebutVersion)){// fichier encrypté
                dejaEncrypte = true;
                break;
            }
        }
        fichier.close();

        if(dejaEncrypte == false){

            // copy temporaire


            QStringList list = QString(cheminFichier).split("/");

            // sauvegarde de l'original
            QString fileCopy = "";
            for(int i = 0; i<list.size()-1;i++)fileCopy.append(list.at(i)+"/");
            QString tempo = list.at(list.size()-1);
            if(!tempo.contains("enc.hex"))fileCopy.append(tempo.replace(".hex","_encrypted.hex"));
            else fileCopy.append(tempo.replace("enc.hex","_encrypted_enc.hex"));

            encrypt->encryption(cheminFichier,fileCopy, KEY_Decrypt);

            QMessageBox::about(this,"Notify","Finished.");

            list = QString(fileCopy).split("/");//découpe le chemin selon les "/"
            if(list.size()>0){
                fileName[a]->setText(list.at(list.size()-1));
                newfileName[a]->setText(list.at(list.size()-1));
            }

        }else QMessageBox::about(this,"Warning","File already encrypted.");

    }else QMessageBox::about(this,"Warning","Filename is not set.");
}

void EditTab::decryptUpdt(int a){
    if(fileName[a]->text()!=""){
        QString fileNameU = QString("%1%2/%3").arg(_folder).arg(_trigram).arg(fileName[a]->text());
        if(QFile::exists(fileNameU)==false){
            QMessageBox::warning(this,"Warning","File not exists.");
            return;
        }
        bool dejaDecrypte = false;

        QString cheminFichier = _folder + "/" + _trigram +"/"+ fileName[a]->text();
        cheminFichier.replace("//","/");
        QString cryptedDebutVersion = encrypt->encryptLine(debutVersion,KEY_Decrypt);
        QFile fichier(cheminFichier);
        fichier.open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream flux(&fichier);
        QString ligne;
        while(! flux.atEnd())
        {
            ligne = flux.readLine();
            //traitement de la ligne
            if(ligne.startsWith(debutVersion)){// fichier non encrypté, rien a faire
                dejaDecrypte = true;
                break;
            }
            if(ligne.startsWith(cryptedDebutVersion)){// fichier encrypté
                dejaDecrypte = false;
                break;
            }
        }
        fichier.close();

        if(dejaDecrypte == false){

            // copy temporaire
            QStringList list = QString(cheminFichier).split("/");

            // sauvegarde de l'original
            QString fileCopy = "";
            for(int i = 0; i<list.size()-1;i++)fileCopy.append(list.at(i)+"/");
            QString tempo = list.at(list.size()-1);
            if(!tempo.contains("enc.hex"))fileCopy.append(tempo.replace(".hex","_decrypted.hex"));
            else fileCopy.append(tempo.replace("enc.hex","_decrypted_enc.hex"));

            decrypt->decryption(cheminFichier,fileCopy, KEY_Decrypt);

            QMessageBox::about(this,"Notify","Finished.");

            list = QString(fileCopy).split("/");//découpe le chemin selon les "/"
            if(list.size()>0){
                fileName[a]->setText(list.at(list.size()-1));
                newfileName[a]->setText(list.at(list.size()-1));
            }
        }else QMessageBox::about(this,"Warning","File already decrypted.");

    }else QMessageBox::about(this,"Warning","Filename is not set.");
}

void EditTab::CRC_Updt(int a){
    if(fileName[a]->text()!=""){
        QString fileNameU = QString("%1%2/%3").arg(_folder).arg(_trigram).arg(fileName[a]->text());
        if(QFile::exists(fileNameU)==false){
            std::cout<<fileNameU.toStdString()<<" pas bon"<<std::endl;
            QMessageBox::warning(this,"Warning","File not exists.");
            return;
        }
        CRC[a]->setText(getCRC32(fileNameU));
    }else QMessageBox::about(this,"Warning","Filename is not set.");
}

void EditTab::addUpdate(){
    addUpdateFunction(false);
    nUpdate++;

    focusBottom();
}

void EditTab::focusBottom(){

    Scroll->verticalScrollBar()->setSliderPosition(Scroll->verticalScrollBar()->maximum());
}

void EditTab::addUpdateFunction(bool init){

    QLabel *l_fileName = new QLabel("Filename:");
    fileName[nUpdate] = new QLineEdit("");
    browseUpdate[nUpdate] = new QPushButton("Browse");
    browseUpdate[nUpdate]->setFixedWidth(70);
    connect(browseUpdate[nUpdate], SIGNAL(clicked()), signalMapperBrowseUpdate, SLOT(map()));
    signalMapperBrowseUpdate->setMapping(browseUpdate[nUpdate],nUpdate);

    encryptUpdate[nUpdate] = new QPushButton("Encrypt file");
    encryptUpdate[nUpdate]->setFixedWidth(100);
    connect(encryptUpdate[nUpdate], SIGNAL(clicked()), signalMapperEncryptUpdate, SLOT(map()));
    signalMapperEncryptUpdate->setMapping(encryptUpdate[nUpdate],nUpdate);

    decryptUpdate[nUpdate] = new QPushButton("Decrypt file");
    decryptUpdate[nUpdate]->setFixedWidth(100);
    connect(decryptUpdate[nUpdate], SIGNAL(clicked()), signalMapperDecryptUpdate, SLOT(map()));
    signalMapperDecryptUpdate->setMapping(decryptUpdate[nUpdate],nUpdate);

    QLabel *l_newfileName = new QLabel("New filename (optional):");
    newfileName[nUpdate] = new QLineEdit("");

    QLabel *l_name = new QLabel("Firmware Revision:");
    QLabel *l_nameH = new QLabel("?");
    QImage imageHelp(":/MainWindow/ressources/help.png");
    l_nameH->setPixmap(QPixmap::fromImage(imageHelp));
    l_nameH->setToolTip(tr("'name'_V'xx'.'xx'.'xx'\n"
                           "'name' is the name of firmware and 'xx' are two digits numbers\n"
                           "'name'_V'xx' is the most important thing and\n"
                           "equal to \"client number\"\n"
                           "Example: IR_RF_V01.16.02"));
    name[nUpdate] = new QLineEdit("");
    connect(name[nUpdate], SIGNAL(textChanged(const QString &)), this, SLOT(NametextChanged(const QString &)));

    QLabel *l_description = new QLabel("Description:");
    description[nUpdate] = new QLineEdit("");

    QLabel *l_date = new QLabel("Date:");
    date[nUpdate] = new QLineEdit("");

    QLabel *l_CRC = new QLabel("CRC32:");
    CRC[nUpdate] = new QLineEdit("");
    CRC_Update[nUpdate] = new QPushButton("Evaluate");
    CRC_Update[nUpdate]->setFixedWidth(70);
    connect(CRC_Update[nUpdate], SIGNAL(clicked()), signalMapperCRC_Update, SLOT(map()));
    signalMapperCRC_Update->setMapping(CRC_Update[nUpdate],nUpdate);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////
    // partie modifications
    GridModif[nUpdate] = new QGridLayout;
    for(int i = 0; i <100;i++){
        QString title = QString("Modification %1:").arg(QString::number(i+1));
        l_modif[nUpdate][i] = new QLabel(title);
        GridModif[nUpdate]->addWidget(l_modif[nUpdate][i],i,0);
        modif[nUpdate][i] = new QLineEdit("");
        GridModif[nUpdate]->addWidget(modif[nUpdate][i],i,1);

        delModif[nUpdate][i] = new QPushButton("");
        delModif[nUpdate][i]->setCursor(Qt::PointingHandCursor);
        delModif[nUpdate][i]->setFixedWidth(25);
        delModif[nUpdate][i]->setIcon(QIcon(":/MainWindow/ressources/delete.png"));
        GridModif[nUpdate]->addWidget(delModif[nUpdate][i],i,2);

        connect(delModif[nUpdate][i], SIGNAL(clicked()), signalMapperDelModif, SLOT(map()));
        QString coord = QString("%1x%2").arg(QString::number(nUpdate)).arg(QString::number(i));
        signalMapperDelModif->setMapping(delModif[nUpdate][i],coord);

        addModif[nUpdate][i] = new QPushButton("");
        addModif[nUpdate][i]->setCursor(Qt::PointingHandCursor);
        addModif[nUpdate][i]->setFixedWidth(25);
        addModif[nUpdate][i]->setIcon(QIcon(":/MainWindow/ressources/ajout.png"));
        GridModif[nUpdate]->addWidget(addModif[nUpdate][i],i,3);

        connect(addModif[nUpdate][i], SIGNAL(clicked()), signalMapperAddModif, SLOT(map()));
        QString coord2 = QString("%1x%2").arg(QString::number(nUpdate)).arg(QString::number(i));
        signalMapperAddModif->setMapping(addModif[nUpdate][i],coord2);
    }
    if(init == false){// cas ajout par bouton
        nModifFound[nUpdate]=1;
        for(int i = 1; i <100;i++){// on caches toutes les modifs possibles
            modif[nUpdate][i]->hide();
            l_modif[nUpdate][i]->hide();
            addModif[nUpdate][i]->hide();
        }
        for(int i = 0; i <100;i++){
            delModif[nUpdate][i]->hide();
        }
        // ajout de la date actuelle dans le champ date
        QDate dateToday=QDate::currentDate();
        date[nUpdate]->setText(dateToday.toString("dd/MM/yyyy"));
    }
    else {// cas ajout initial
        for(int i = 0; i <nModifFound[nUpdate];i++){
            modif[nUpdate][i]->setText(modifValue[nUpdate][i]);
            if(i<nModifFound[nUpdate]-1){
                addModif[nUpdate][i]->hide();
            }
            if(/*i==nModifFound[nUpdate]-1*/nModifFound[nUpdate]<=1){
                delModif[nUpdate][i]->hide();
            }
        }
        for(int i = nModifFound[nUpdate]; i <100;i++){
            modif[nUpdate][i]->hide();
            l_modif[nUpdate][i]->hide();
            addModif[nUpdate][i]->hide();
            delModif[nUpdate][i]->hide();
        }
    }
    //////////////////////////////////////////////////////////////////////////////////////////////////////////
    // partie champs normaux
    GridUpdate[nUpdate] = new QGridLayout;
    GridUpdate[nUpdate]->addWidget(l_fileName,0,0);
    GridUpdate[nUpdate]->addWidget(fileName[nUpdate],0,2);
    GridUpdate[nUpdate]->addWidget(browseUpdate[nUpdate],0,3);
    GridUpdate[nUpdate]->addWidget(encryptUpdate[nUpdate],0,4);
    GridUpdate[nUpdate]->addWidget(decryptUpdate[nUpdate],0,5);
    GridUpdate[nUpdate]->addWidget(l_newfileName,1,0);
    GridUpdate[nUpdate]->addWidget(newfileName[nUpdate],1,2);
    GridUpdate[nUpdate]->addWidget(l_name,2,0);
    GridUpdate[nUpdate]->addWidget(l_nameH,2,1);
    GridUpdate[nUpdate]->addWidget(name[nUpdate],2,2);
    GridUpdate[nUpdate]->addWidget(l_description,3,0);
    GridUpdate[nUpdate]->addWidget(description[nUpdate],3,2);
    GridUpdate[nUpdate]->addWidget(l_date,4,0);
    GridUpdate[nUpdate]->addWidget(date[nUpdate],4,2);
    GridUpdate[nUpdate]->addWidget(l_CRC,5,0);
    GridUpdate[nUpdate]->addWidget(CRC[nUpdate],5,2);
    GridUpdate[nUpdate]->addWidget(CRC_Update[nUpdate],5,3);

    deleteThisUpdate[nUpdate] = new QPushButton(tr("Delete this update"));
    deleteThisUpdate[nUpdate]->setCursor(Qt::PointingHandCursor);
    deleteThisUpdate[nUpdate]->setFixedWidth(150);
    deleteThisUpdate[nUpdate]->setIcon(QIcon(":/MainWindow/ressources/delete.png"));
    connect(deleteThisUpdate[nUpdate], SIGNAL(clicked()), signalMapperDeleteUpdate, SLOT(map()));
    signalMapperDeleteUpdate->setMapping(deleteThisUpdate[nUpdate],nUpdate);

    l_version[nUpdate] = new QLabel("");
    l_version[nUpdate]->setFixedWidth(30);
    l_version[nUpdate]->setAutoFillBackground(true);
    l_version[nUpdate]->setAlignment(Qt::AlignCenter);
    l_version[nUpdate]->setPalette(QColor(0,0,0));
    colorLabel[nUpdate] = QColor(0,0,0,120);

    if(init==true){
        fileName[nUpdate]->setText(fileNameValue[nUpdate]);
        if(QFile::exists(_folder + "/" + _trigram +"/"+ fileNameValue[nUpdate])==false){
            QString errorMessage = QString("Impossible to find the file:\n%1\nin '%2' folder.").arg(fileNameValue[nUpdate]).arg(_trigram);
            QMessageBox::warning(this,"Warning",errorMessage);
        }
        newfileName[nUpdate]->setText(fileNameValue[nUpdate]);
        name[nUpdate]->setText(nameValue[nUpdate]);
        description[nUpdate]->setText(descriptionValue[nUpdate]);
        date[nUpdate]->setText(dateValue[nUpdate]);
        CRC[nUpdate]->setText(CRCValue[nUpdate]);
        if(nameValue[nUpdate].contains(".")){
        QStringList tempo = QString(nameValue[nUpdate]).split(".");
        QString tempo2 = tempo.at(0);
        QString text = "";
        int somme = 0;
        for(int j =0; j<tempo2.size();j++){text.append(QString(QChar(tempo2.at(j)))+"\n");somme+=tempo2.at(j).toAscii();}
        somme = somme*somme*somme;
        text.replace("_", " ");
        l_version[nUpdate]->setText(text);
        int r = somme%251;
        int g = somme%201;
        int b = somme%187;
        colorLabel[nUpdate]= QColor(r,g,b,120);
        l_version[nUpdate]->setPalette(colorLabel[nUpdate]);
        }else{
            l_version[nUpdate]->setText("");
            colorLabel[nUpdate] = QColor(0,0,0,120);
            l_version[nUpdate]->setPalette(QColor(0,0,0,120));
        }
    }

    QFrame *frame = new QFrame;
    frame->setFrameStyle(QFrame::HLine | QFrame::Raised);
    frame->setLineWidth(0);
    frame->setMidLineWidth(3);
    frame->setPalette(QColor(255,0,255));

    grids[nUpdate] = new QVBoxLayout;
    grids[nUpdate]->addLayout(GridUpdate[nUpdate]);
    grids[nUpdate]->addLayout(GridModif[nUpdate]);
    grids[nUpdate]->addWidget(deleteThisUpdate[nUpdate],0,Qt::AlignCenter);
    grids[nUpdate]->addWidget(frame);


    QHBoxLayout *layoutGroupBox = new QHBoxLayout;
    layoutGroupBox->addWidget(l_version[nUpdate]);
    layoutGroupBox->addSpacing(20);
    layoutGroupBox->addLayout(grids[nUpdate]);

    GroupBoxUpdate[nUpdate] = new QGroupBox("Update");
    GroupBoxUpdate[nUpdate]->setLayout(layoutGroupBox);

    layout_updates->addWidget(GroupBoxUpdate[nUpdate]);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// info pour la table view
QString EditTab::getFileName(){
    QString text = "";
    for(int i = 0 ;i <nUpdate;i++){
        QString filename = "";
        if((fileName[i]->text()!= newfileName[i]->text()) && newfileName[i]->text()!="" && newfileName[i]->text().endsWith(".hex")==true)filename = newfileName[i]->text();
        else filename = fileName[i]->text();
        text.append(filename);
        if(i!=nUpdate-1)text.append("**");
    }
    return text;
}

QString EditTab::getName(){
    QString text = "";
    for(int i = 0 ;i <nUpdate;i++){
        text.append(name[i]->text());
        if(i!=nUpdate-1)text.append("**");
    }
    return text;
}

QString EditTab::getDescription(){
    QString text = "";
    for(int i = 0 ;i <nUpdate;i++){
        text.append(description[i]->text());
        if(i!=nUpdate-1)text.append("**");
    }
    return text;
}

QString EditTab::getDate(){
    QString text = "";
    for(int i = 0 ;i <nUpdate;i++){
        text.append(date[i]->text());
        if(i!=nUpdate-1)text.append("**");
    }
    return text;
}

QString EditTab::getCRC(){
    QString text = "";
    for(int i = 0 ;i <nUpdate;i++){
        text.append(CRC[i]->text());
        if(i!=nUpdate-1)text.append("**");
    }
    return text;
}

QString EditTab::getModifs(){
    QString text = "";
    for(int i = 0 ;i <nUpdate;i++){
        for(int j = 0; j<nModifFound[i];j++){
            text.append(modif[i][j]->text());
            if(j!=nModifFound[i]-1)text.append(" / ");
        }
        if(i!=nUpdate-1)text.append("**");
    }
    return text;
}

int EditTab::getNUpdate(){
    return nUpdate;
}

int EditTab::getNVersionDiff(){
    return nVersionDiff;
}

QString EditTab::getNVersionDiffValue(){
    QString text = "";
    for(int i = 0; i<nVersionDiff;i++){
        text.append(diffVersions[i]);
        if(i!=nVersionDiff-1)text.append("**");
    }
    return text;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// partie crc32

#define BUFSIZE 8192

const quint32 crc32_tab[256] =
{
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
    0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
    0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
    0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
    0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
    0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
    0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
    0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
    0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
    0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
    0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
    0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
    0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
    0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
    0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
    0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
    0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
    0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
    0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
    0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
    0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
    0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
    0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
    0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
    0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
    0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
    0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
    0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
    0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
    0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
    0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
    0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
    0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
    0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
    0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
    0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
    0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
    0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
    0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
    0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
    0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
    0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
    0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
    0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
    0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
    0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
    0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
    0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
    0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
    0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
    0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
    0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
    0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
    0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

QString EditTab::getCRC32(QString filePath)
{
    ///////////////////////////////
    // decryptage si besoin      //
    ///////////////////////////////
    QString cryptedDebutVersion = encrypt->encryptLine(debutVersion,KEY_Decrypt);

    QFile fichier(filePath);
    fichier.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream flux(&fichier);
    QString ligne;
    while(! flux.atEnd())// si fichier crypté version simple
    {
        ligne = flux.readLine();
        //traitement de la ligne
        if(ligne.startsWith(debutVersion)){// cas fichier normal pas crypté
            break;
        }
        if(ligne.startsWith(cryptedDebutVersion)){// cas fichier crypté avec une clé
            QString tempo = "";
#ifdef Q_WS_WIN32
            if(filePath.endsWith("enc.hex"))tempo = "tempo_enc.hex";
            else tempo = "tempo.hex";
#else
            if(filePath.endsWith("enc.hex")){
                tempo =_pathResource;
                tempo.append("tempo_enc.hex");
            }
            else {
                tempo =_pathResource;
                tempo.append("tempo.hex");
            }
#endif
            decrypt->decryption(filePath, tempo, KEY_Decrypt);
            filePath = tempo;
            break;
        }
    }
    // fin du decryptage ou non.


    QFile file(filePath);
    char buf[BUFSIZE];
    quint32 crc32 = 0;
    qint64 i = 0, n = 0;

    if(!file.open(QIODevice::ReadOnly))
    {
        return "";
    }

    crc32 = 0xffffffff;

    while((n = file.read(buf, 1)) > 0)
    {
        for(i = 0; i < n; i++)
        {
            crc32 = (crc32 >> 8) ^ crc32_tab[(crc32 & 0xff) ^ buf[i]];
        }
        //qDebug() << n;
    }

    crc32 ^= 0xffffffff;

    file.close();
    QString macChange = "tempo.hex";
#ifdef Q_WS_MAC
    macChange =_pathResource;
    macChange.append("tempo.hex");
#endif
    if(QFile::exists(macChange))QFile::remove(macChange);

    macChange = "tempo_enc.hex";
#ifdef Q_WS_MAC
    macChange =_pathResource;
    macChange.append("tempo_enc.hex");
#endif
    if(QFile::exists(macChange))QFile::remove(macChange);

    QString myString(QByteArray::number(crc32, 16));

    myString = myString.toUpper();

    return myString;

    return myString;
}

bool EditTab::getSucces(){
    return  noError;
}

void EditTab::setSucces(){
    noError = true;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
QDomElement EditTab::save(){

    QDomDocument settings("products_versions");
    QDomElement root = settings.createElement(_trigram);
    settings.appendChild(root);

    for(int i = 0; i<nUpdate;i++){
        if((fileName[i]->text()!= newfileName[i]->text() && newfileName[i]->text()!="") && newfileName[i]->text().endsWith(".hex")==true){
            bool ok = QFile::copy(_folder+"/"+_trigram+"/" + fileName[i]->text(),_folder+"/"+ _trigram+"/" + newfileName[i]->text());
            if(ok == true){
                ok = QFile::remove(_folder+"/"+ _trigram+"/" + fileName[i]->text());
                fileName[i]->setText(newfileName[i]->text());
                if(ok == false){
                    QString message = QString("An error occured when renaming file from:\n%1\nto\n%2\n\nInitial filename is still existing.").arg(fileName[i]->text()).arg(newfileName[i]->text());
                    QMessageBox::warning(this,"Error",message);
                }
            }else{
                QString message = QString("Impossible to rename file from:\n%1\nto\n%2\n\nMaybe the destination file is write protected.").arg(fileName[i]->text()).arg(newfileName[i]->text());
                QMessageBox::warning(this,"Error",message);

            }
            if(ok == false)noError = false;
        }
        if(QFile::exists(_folder + "/" + _trigram +"/"+ fileName[i]->text())==false){
            QString errorMessage = QString("Impossible to find the file:\n%1\nin '%2' folder.").arg(fileName[i]->text()).arg(_trigram);
            QMessageBox::warning(this,"Warning",errorMessage);
            noError = false;
        }
        if(name[i]->text().contains("_V")==false || name[i]->text().contains(".")==false){
            QString errorMessage = QString("Syntax error for an update name:\n%1\nin %2 tab").arg(name[i]->text()).arg(_mW->nameDevices[_mW->CurrentTabIndex()-1]);
            QMessageBox::warning(this,"Warning",errorMessage);
            noError = false;
        }

        QDomElement _version = settings.createElement("new-version");
        root.appendChild(_version);

        QDomElement _filename = settings.createElement("filename");
        _version.appendChild(_filename);
        QDomText filename_ = settings.createTextNode(fileName[i]->text());
        _filename.appendChild(filename_);

        QDomElement _name = settings.createElement("name");
        _version.appendChild(_name);
        QDomText name_ = settings.createTextNode(name[i]->text());
        _name.appendChild(name_);

        QDomElement _description = settings.createElement("description");
        _version.appendChild(_description);
        QDomText description_ = settings.createTextNode(description[i]->text());
        _description.appendChild(description_);

        QDomElement _date = settings.createElement("date");
        _version.appendChild(_date);
        QDomText date_ = settings.createTextNode(date[i]->text());
        _date.appendChild(date_);

        QDomElement _CRC = settings.createElement("CRC");
        _version.appendChild(_CRC);
        QDomText CRC_ = settings.createTextNode(CRC[i]->text());
        _CRC.appendChild(CRC_);

        for(int j = 0; j<nModifFound[i];j++){
            QDomElement _modif = settings.createElement("modification");
            _version.appendChild(_modif);
            QDomText modif_ = settings.createTextNode(modif[i][j]->text());
            _modif.appendChild(modif_);
        }
    }

    return root;

}
