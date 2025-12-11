#include "proglcd.h"

ProgLCD::ProgLCD(QWidget *parent) : QDialog(parent)
{
    //Layout boutons
    ouvrir = new QPushButton("Open");
    sauver = new QPushButton("Save");
    appliquer = new QPushButton("Apply");
    quitter = new QPushButton("Cancel");
    QHBoxLayout *layoutBoutons = new QHBoxLayout;
    layoutBoutons->addWidget(ouvrir);
//    layoutBoutons->addWidget(sauver);
    layoutBoutons->addSpacing(100);
    layoutBoutons->addWidget(appliquer);
    layoutBoutons->addWidget(quitter);

    //Fenetre complete
    QVBoxLayout *layoutMain = new QVBoxLayout;
    nomFichier = new QTextEdit;
    nomFichier->setFixedSize(500,30);
    contenu = new QTextEdit;
    contenu->setFixedSize(500,500);
    comBar = new QProgressBar();
    layoutMain->addWidget(nomFichier);
//    layoutMain->addWidget(contenu);
    layoutMain->addWidget(comBar);
    layoutMain->addLayout(layoutBoutons);
    setLayout(layoutMain);
    setWindowTitle("Glasses Time Settings");

    //Connections SLOT
    connect(quitter, SIGNAL(clicked()), this, SLOT(accept()));
    connect(appliquer, SIGNAL(clicked()), this, SLOT(compil()));
    connect(contenu, SIGNAL(textChanged()), this, SLOT(colorer()));
    connect(sauver, SIGNAL(clicked()), this, SLOT(sauvegarder()));
    connect(ouvrir, SIGNAL(clicked()), this, SLOT(ouvrirFichier()));

    //variables par défaut
    varInter = 1000;
    comBar->setValue(0);
    varPERIOD = 16666;

    //Watchdog pour attente réponse des lunettes
    waitAck  = new QTimer();
    connect(waitAck, SIGNAL(timeout()), this, SLOT(StopWaitAck()));

}

ProgLCD::~ProgLCD()
{
}

void ProgLCD::sauvegarder()
{
    QString filename = QFileDialog::getSaveFileName(NULL, tr("Save File"), QDir::rootPath(), "Text (*.txt)" );
    QFile fichier(filename);
    fichier.open(QIODevice::WriteOnly);
    QTextStream flux(&fichier);
    flux << contenu->toPlainText();
    fichier.close();
}

void ProgLCD::ouvrirFichier()
{
    QString filename = QFileDialog::getOpenFileName(NULL, tr("Open File"), QDir::rootPath(), "Text (*.txt)" );
    nomFichier->setText(filename);
//    QFile fichier(filename);
//    fichier.open(QIODevice::ReadOnly);
//    QTextStream flux(&fichier);
//    contenu->setText(flux.readAll());
//    fichier.close();
}

void ProgLCD::colorer()
{
    highlighter = new Highlighter(contenu->document());
}

void ProgLCD::compil()
{
    qDebug() << "Recuperation dernière version du fichier: " << nomFichier->toPlainText();
    QFile fichier(nomFichier->toPlainText());
    fichier.open(QIODevice::ReadOnly);
    QTextStream str(&fichier);

//    QString text = contenu->toPlainText();
//    QTextStream str(&text, QIODevice::ReadOnly);
    QString line, idGlass, cmdLCD;
    QStringList idList, timesList, statesList, interList;
    int i,count,timing;
    unsigned char data[64];

    //initialisation variables
    for(i=0;i<8;i++) {varTiming[i] = 0; varCdeLcd[i] = 0; varCdeLcd[i+8] = 0;}
    varInter = 1000;
    varUP = 0;
    varDOWN = 0;
    varDelay = 0;
    varDiode = 0;
    varEEPROM = 0;

    for(int n=0; !str.atEnd(); n++){
        line = str.readLine();
        if(line.startsWith("ID:")) {
            qDebug() << line;
            idList = line.split(" ");
            if (idList.size() > 1){
                idGlass = idList.value(1);
                if(idGlass == "0") idGlass="";
            }
            else idGlass = "";
        }
        else if(line.startsWith("TIMES:")) {
            qDebug() << line;
            timesList = line.split(" ");
        }
        else if(line.startsWith("STATES:")) {
            qDebug() << line;
            statesList = line.split(" ");
        }
        else if(line.startsWith("INTER:")) {
            qDebug() << line;
            interList = line.split(" ");
            varInter = interList.at(1).toInt() / 2;
        }
        else if(line.startsWith("UP:")) {
            qDebug() << line;
            interList = line.split(" ");
            varUP = interList.at(1).toInt();
        }
        else if(line.startsWith("DOWN:")) {
            qDebug() << line;
            interList = line.split(" ");
            varDOWN = interList.at(1).toInt();
        }
        else if(line.startsWith("DELAY:")) {
            qDebug() << line;
            interList = line.split(" ");
            varDelay = interList.at(1).toInt();
        }
        else if(line.startsWith("DIODE:")) {
            qDebug() << line;
            interList = line.split(" ");
            varDiode = interList.at(1).toInt();
        }
        else if(line.startsWith("EESAVE:")) {
            qDebug() << line;
            interList = line.split(" ");
            varEEPROM = interList.at(1).toInt();
        }
        else if(line.startsWith("PERIOD:")) {
            qDebug() << line;
            interList = line.split(" ");
            varPERIOD = interList.at(1).toInt();
        }
    }

    if( (timesList.size() != statesList.size()) || (timesList.size() <= 2) ) {
        QMessageBox::critical(this,"Error", "Times and/or states are not valid");
        return;
    }

    count=0;

    for(i=2; i<=timesList.size(); i++) {

        if (i<timesList.size()) timing = timesList.at(i).toInt() - timesList.at(i-1).toInt();
        else timing = varPERIOD - timesList.at(i-1).toInt();
        cmdLCD = statesList.at(i-1);

        qDebug() << "Timing:" << timing << "cmdLCD:" << cmdLCD;

        timing = timing/2;
        varTiming[count] = timing;

        //GAUCHE
        if (cmdLCD.left(1)=="0") {
            varCdeLcd[count] = HBx_Close_Plus;
            varCdeLcd[count+8] = HBx_Close_Minus;
        }
        else if (cmdLCD.left(1)=="P") {
            varCdeLcd[count] = HBx_Prepa;
            varCdeLcd[count+8] = HBx_Prepa;
        }
        else if (cmdLCD.left(1)=="1") {
            varCdeLcd[count] = HBx_Open_Plus;
            varCdeLcd[count+8] = HBx_Open_Minus;
        }

        //DROITE
        if (cmdLCD.right(1)=="0") {
            varCdeLcd[count] += HAx_Close_Plus;
            varCdeLcd[count+8] += HAx_Close_Minus;
        }
        else if (cmdLCD.right(1)=="P") {
            varCdeLcd[count] += HAx_Prepa;
            varCdeLcd[count+8] += HAx_Prepa;
        }
        else if (cmdLCD.right(1)=="1") {
            varCdeLcd[count] += HAx_Open_Plus;
            varCdeLcd[count+8] += HAx_Open_Minus;
        }

        count++;
    }

    /***************** V03.08.02 ********************
    for(i=1; i<timesList.size(); i++) {

        if(i>=2) timing = timesList.at(i).toInt() - timesList.at(i-1).toInt();
        else timing = timesList.at(i).toInt();
        timing = timing / 2;

        if (statesList.at(i) == "00"){  //les 2 verres ferm<E9>s
            if( (i==timesList.size()-1) && (varTiming[7]!=0) )
                //si les 2 verres doivent <EA>tre ouverts au d<E9>but de la synchro (varTiming[7]!=0)
                //alors la derniere commande doit etre "2 verres ferm<E9>s"
                //il faut donc lui soustraire la dur<E9>e de la phase de transition
                varTiming[count] = timing - varInter;
            else
                varTiming[count] = timing;
            varCdeLcd[count] = HBx_Close_Plus + HAx_Close_Plus;
            varCdeLcd[count+8] = HBx_Close_Minus + HAx_Close_Minus;
         }
        else if (statesList.at(i) == "10"){  //Gauche ouvert / Droit Ferm<E9>
            if (i==1) {
                //Si les verres doivent <EA>tres ouverts au d<E9>but de la synchro,
                //alors le dernier timing doit correspondre <E0> la dur<E9>e de la phase de transition
                varTiming[7] = varInter;
                varCdeLcd[7] = HBx_Prepa + HAx_Close_Plus;
                varCdeLcd[15]= HBx_Prepa + HAx_Close_Minus;
                varTiming[0] = timing;
                varCdeLcd[0] = HBx_Open_Plus + HAx_Close_Minus;
                varCdeLcd[8] = HBx_Open_Minus + HAx_Close_Plus;
            }
            else {
                //Sinon on ins<E8>re une phase avec la dur<E9>e de la phase de transition
                //Et on retire cette dur<E9>e <E0> la phase pr<E9>c<E9>dente
                varTiming[count] = varInter;
                varTiming[count-1] -= varInter;
                varCdeLcd[count] = HBx_Prepa + HAx_Close_Plus;
                varCdeLcd[count+8]= HBx_Prepa + HAx_Close_Minus;
                varTiming[++count] = timing;
                varCdeLcd[count] = HBx_Open_Plus + HAx_Close_Plus;
                varCdeLcd[count+8]= HBx_Open_Minus + HAx_Close_Minus;
            }
        }
        else if (statesList.at(i) == "01"){  //Gauche ferm<E9> / Droit ouvert
            if (i==1) {
                //Si les verres doivent <EA>tres ouverts au d<E9>but de la synchro,
                //alors le dernier timing doit correspondre <E0> la dur<E9>e de la phase de transition
                varTiming[7] = varInter;
                varCdeLcd[7] = HAx_Prepa + HBx_Close_Plus;
                varCdeLcd[15]= HAx_Prepa + HBx_Close_Minus;
                varTiming[0] = timing;
                varCdeLcd[0] = HAx_Open_Plus + HBx_Close_Minus;
                varCdeLcd[8] = HAx_Open_Minus + HBx_Close_Plus;
            }
            else {
                //Sinon on ins<E8>re une phase avec la dur<E9>e de la phase de transition
                //Et on retire cette dur<E9>e <E0> la phase pr<E9>c<E9>dente
                varTiming[count] = varInter;
                varTiming[count-1] -= varInter;
                varCdeLcd[count] = HAx_Prepa + HBx_Close_Plus;
                varCdeLcd[count+8]= HAx_Prepa + HBx_Close_Minus;
                varTiming[++count] = timing;
                varCdeLcd[count] = HAx_Open_Plus + HBx_Close_Plus;
                varCdeLcd[count+8]= HAx_Open_Minus + HBx_Close_Minus;
            }
        }
        else {
            //2 verres ouverts
            if (i==1) {
                //Si les verres doivent <EA>tres ouverts au d<E9>but de la synchro,
                //alors le dernier timing doit correspondre <E0> la dur<E9>e de la phase de transition
                varTiming[7] = varInter;
                varCdeLcd[7] = HBx_Prepa + HAx_Prepa;
                varCdeLcd[15]= HBx_Prepa + HAx_Prepa;
                varTiming[0] = timing;
                varCdeLcd[0] = HBx_Open_Plus + HAx_Open_Plus;
                varCdeLcd[8] = HBx_Open_Minus + HAx_Open_Minus;
            }
            else {
                //Sinon on ins<E8>re une phase avec la dur<E9>e de la phase de transition
                //Et on retire cette dur<E9>e <E0> la phase pr<E9>c<E9>dente
                varTiming[count] = varInter;
                varTiming[count-1] -= varInter;
                varCdeLcd[count] = HBx_Prepa + HAx_Prepa;
                varCdeLcd[count+8]= HBx_Prepa + HAx_Prepa;
                varTiming[++count] = timing;
                varCdeLcd[count] = HBx_Open_Plus + HAx_Open_Plus;
                varCdeLcd[count+8]= HBx_Open_Minus + HAx_Open_Minus;
            }
        }
        count++;
    }
    ***************************************************/

    qDebug() << "varTiming:" << varTiming[0] << varTiming[1] << varTiming[2] << varTiming[3] << varTiming[4] << varTiming[5] << varTiming[6] << varTiming[7];
    qDebug() << "varCdeLcd:" << varCdeLcd[0] << varCdeLcd[1] << varCdeLcd[2] << varCdeLcd[3] << varCdeLcd[4] << varCdeLcd[5] << varCdeLcd[6] << varCdeLcd[7];
    qDebug() << "varCdeLcd2:" << varCdeLcd[8] << varCdeLcd[9] << varCdeLcd[10] << varCdeLcd[11] << varCdeLcd[12] << varCdeLcd[13] << varCdeLcd[14] << varCdeLcd[15];

    //Verification frequence
    count=0;
    for(i=0; i<8; i++) count += varTiming[i];
    if ( (count<8320) || (count>8350) ) {
            if ( QMessageBox::question(this,"Warning",
                                       "Timings are not based on a 60Hz synchronization, continue anyway ?",
                                       QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
                return;
    }

    //mise en forme des data et envoi
    for(i=0; i<8; i++){
         data[i] = (unsigned char) idGlass[i].toLatin1();
         data[8+ 2*i] = (unsigned char) varTiming[i];
         data[8+ 2*i + 1] = (unsigned char) (varTiming[i] / 256);
         data[8+ 16+i] = (unsigned char) varCdeLcd[i];
         data[8+ 24+i] = (unsigned char) varCdeLcd[i+8];
    }
    data[40] = (unsigned char) varUP;
    data[41] = (unsigned char) (varUP/256);
    data[42] = (unsigned char) varDOWN;
    data[43] = (unsigned char) (varDOWN/256);
    data[44] = (unsigned char) varDelay;
    data[45] = (unsigned char) (varDelay/256);
    data[46] = (unsigned char) varDiode;
    data[47] = (unsigned char) varEEPROM;

    comBar->setValue(20);
    emit apply_clicked(1, data);
    waitAck->start(20000);// si pas de reponse en moins de 20s -> erreur de connexion

    fichier.close();
}

void ProgLCD::finished(int data)
{
    comBar->setValue(data);
    if (data==100){
        waitAck->stop();
        QMessageBox::information(this, "Information", "Glasses are updated");
    }
}

void ProgLCD::StopWaitAck()
{
    waitAck->stop();
    comBar->setValue(0);
    QMessageBox::information(this, "Information", "Impossible to connect to glasses");
}
