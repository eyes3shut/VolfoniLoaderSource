#include <QtGui>
#include <QList>

#include "decrypt.h"


Decrypt::Decrypt()
    : QDialog()
{

    C[0]='a';
    C[1]='b';
    C[2]='c';
    C[3]='d';
    C[4]='e';
    C[5]='f';
    C[6]='g';
    C[7]='h';
    C[8]='i';
    C[9]='j';
    C[10]='k';
    C[11]='l';
    C[12]='m';
    C[13]='n';
    C[14]='o';
    C[15]='p';
    C[16]='q';
    C[17]='r';
    C[18]='s';
    C[19]='t';
    C[20]='u';
    C[21]='v';
    C[22]='w';
    C[23]='x';
    C[24]='y';
    C[25]='z';
    C[26]='A';
    C[27]='B';
    C[28]='C';
    C[29]='D';
    C[30]='E';
    C[31]='F';
    C[32]='G';
    C[33]='H';
    C[34]='I';
    C[35]='J';
    C[36]='K';
    C[37]='L';
    C[38]='M';
    C[39]='N';
    C[40]='O';
    C[41]='P';
    C[42]='Q';
    C[43]='R';
    C[44]='S';
    C[45]='T';
    C[46]='U';
    C[47]='V';
    C[48]='W';
    C[49]='X';
    C[50]='Y';
    C[51]='Z';
    C[52]='0';
    C[53]='1';
    C[54]='2';
    C[55]='3';
    C[56]='4';
    C[57]='5';
    C[58]='6';
    C[59]='7';
    C[60]='8';
    C[61]='9';
    C[62]=':';
    C[63]=',';
    C[64]=';';
    C[65]='.';
    C[66]='!';
    C[67]='?';
    C[68]='/';
    C[69]='é';
    C[70]='è';
    C[71]='à';
    C[72]='#';
    C[73]='\"';
    C[74]='&';
    C[75]='%';
    C[76]='*';
    C[77]='(';
    C[78]=')';
    C[79]='[';
    C[80]=']';
    C[81]='=';
    C[82]='+';
    C[83]='-';
    C[84]='{';
    C[85]='}';
    C[86]='ù';
    C[87]='$';
    C[88]='Ü';
    C[89]='ç';
    C[90]='£';
    C[91]='¤';
    C[92]='_';
    C[93]='@';
    C[94]='\'';
    C[95]='\\';
    C[96]='î';
    C[97]='ê';
    C[98]='ë';
    C[99]='ï';
    C[100]='â';
    C[101]='ä';
    C[102]='ô';
    C[103]='ö';
    C[104]='û';
    C[105]='ü';
    C[106]='§';
    C[107]='µ';
    C[108]='~';
    C[109]='|';
    C[110]='^';
    C[111]='`';
    C[112]='¨';


    I[0]=0;
    I[1]=6;
    I[2]=7;
    I[3]=8;
    I[4]=9;
    I[5]=10;
    I[6]=11;
    I[7]=12;
    I[8]=5;
    I[9]=1;
    I[10]=2;
    I[11]=3;
    I[12]=4;
    I[13]=20;
    I[14]=19;
    I[15]=18;
    I[16]=17;
    I[17]=16;
    I[18]=15;
    I[19]=14;
    I[20]=13;
    I[21]=21;
    I[22]=22;
    I[23]=23;
    I[24]=24;
    I[25]=25;
    I[26]=26;
    I[27]=27;
    I[28]=28;
    I[29]=40;
    I[30]=39;
    I[31]=38;
    I[32]=37;
    I[33]=36;
    I[34]=35;
    I[35]=34;
    I[36]=33;
    I[37]=32;
    I[38]=31;
    I[39]=62;
    I[40]=30;
    I[41]=29;
    I[42]=50;
    I[43]=49;
    I[44]=47;
    I[45]=48;
    I[46]=46;
    I[47]=45;
    I[48]=44;
    I[49]=43;
    I[50]=42;
    I[51]=41;
    I[52]=51;
    I[53]=52;
    I[54]=53;
    I[55]=54;
    I[56]=55;
    I[57]=56;
    I[58]=60;
    I[59]=59;
    I[60]=58;
    I[61]=57;
    I[62]=63;
    I[63]=61;
    I[64]=70;
    I[65]=69;
    I[66]=68;
    I[67]=67;
    I[68]=66;
    I[69]=65;
    I[70]=64;
    I[71]=73;
    I[72]=72;
    I[73]=71;
    I[74]=92;
    I[75]=91;
    I[76]=90;
    I[77]=89;
    I[78]=88;
    I[79]=87;
    I[80]=86;
    I[81]=85;
    I[82]=84;
    I[83]=83;
    I[84]=82;
    I[85]=81;
    I[86]=80;
    I[87]=79;
    I[88]=78;
    I[89]=77;
    I[90]=76;
    I[91]=75;
    I[92]=74;
    I[93]=94;
    I[94]=93;
    I[95]=95;
    I[96]=96;
    I[97]=105;
    I[98]=104;
    I[99]=103;
    I[100]=102;
    I[101]=101;
    I[102]=100;
    I[103]=99;
    I[104]=98;
    I[105]=97;
    I[106]=106;
    I[107]=107;
    I[108]=108;
    I[109]=109;
    I[110]=110;
    I[111]=111;
    I[112]=112;

    nDiff = 113;

}


void Decrypt::decryption(QString filenameFrom, QString filenameTo, QString key){
    //taille de la clé
    int nChKey = key.size();
    // tableau avec les valeurs ascii en boucle de la clé
    int keyValue[MaxCharacterPerLine];

    for(int i = 0; i<MaxCharacterPerLine;i++){
        int n = 0;
        if(i>=nChKey){
            int modulo = i/nChKey;
            n = i -(modulo*nChKey);
        }
        else n = i;
        QChar ch = key.at(n).toLatin1();
        keyValue[i] = CtoI(ch);
    }

    QFile fichier(filenameFrom);
    fichier.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream flux(&fichier);
    flux.setCodec("ISO 8859-1");
    QString ligne;
    QString nouveauTexte = "";
    int nLine = 0;
    while(! flux.atEnd())
    {

        ligne = flux.readLine();
        //traitement de la ligne

        QString nouvelleLigne = "";
        for(int i = 0; i<ligne.size();i++){
            QChar ch = ligne.at(i).toLatin1();
            if(QString(QChar(ligne.at(i)))==" ")nouvelleLigne.append(" ");
            else if(QString(QChar(ligne.at(i)))=="\n")nouvelleLigne.append("\n");
            else if(QString(QChar(ligne.at(i)))=="\t")nouvelleLigne.append("\t");
            else if(QString(QChar(ligne.at(i)))=="\0")nouvelleLigne.append("\0");
            else if(QString(QChar(ligne.at(i)))=="€")nouvelleLigne.append("€");
            else {
                int chValue = CtoI(ch);
                if(keyValue[i] && 1){// ascii de la clé impair
                    chValue -= keyValue[i];
                }else{//ascii de la clé pair
                    chValue += keyValue[i];
                }
                if(chValue>=nDiff)chValue-=nDiff;
                if(chValue<0)chValue+=nDiff;
                nouvelleLigne.append(QString(ItoC(chValue)));
            }
        }


        nouveauTexte.append(nouvelleLigne + "\n");
        nLine++;
    }

    fichier.close();

    QFile fichier2(filenameTo);
    bool ok = fichier2.open(QIODevice::WriteOnly  | QIODevice::Text);
    if (ok == false)QMessageBox::warning(this, "Error", "Impossible to save the decrypted file. \nMaybe your destination file is in read only mode.");
    QTextStream flux2(&fichier2);
    flux2.setCodec("ISO 8859-1");
    flux2 <<nouveauTexte;
    fichier2.close();

}

QString Decrypt::decryptLine(QString ligne,QString key){
    //taille de la clé
    int nChKey = key.size();
    // tableau avec les valeurs ascii en boucle de la clé
    int keyValue[MaxCharacterPerLine];

    for(int i = 0; i<MaxCharacterPerLine;i++){
        int n = 0;
        if(i>=nChKey){
            int modulo = i/nChKey;
            n = i -(modulo*nChKey);
        }
        else n = i;
        QChar ch = key.at(n).toLatin1();
        keyValue[i] = CtoI(ch);
    }

    QString nouvelleLigne = "";
    //traitement de la ligne
    for(int i = 0; i<ligne.size();i++){
        QChar ch = ligne.at(i).toLatin1();
        if(QString(QChar(ligne.at(i)))==" ")nouvelleLigne.append(" ");
        else if(QString(QChar(ligne.at(i)))=="\n")nouvelleLigne.append("\n");
        else if(QString(QChar(ligne.at(i)))=="\t")nouvelleLigne.append("\t");
        else if(QString(QChar(ligne.at(i)))=="\0")nouvelleLigne.append("\0");
        else if(QString(QChar(ligne.at(i)))=="€")nouvelleLigne.append("€");
        else {
            int chValue = CtoI(ch);
            if(keyValue[i] && 1){// ascii de la clé impair
                chValue -= keyValue[i];
            }else{//ascii de la clé pair
                chValue += keyValue[i];
            }
            if(chValue>=nDiff)chValue-=nDiff;
            if(chValue<0)chValue+=nDiff;
            nouvelleLigne.append(QString(ItoC(chValue)));
        }
    }
    return nouvelleLigne;
}

QChar Decrypt::ItoC(int i){
    int index = 0;
    for(int j = 0; j<nDiff; j++){
        if(i == I[j]){
            index = j;
            break;
        }
    }
    return C[index];
}

int Decrypt::CtoI(QChar c){
    int index = 0;
    for(int i = 0; i<nDiff; i++){
        if(c == C[i]){
            index = i;
            break;
        }
    }
    return I[index];
}




