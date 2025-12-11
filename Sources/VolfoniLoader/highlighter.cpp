#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
     : QSyntaxHighlighter(parent)
 {
 }

void Highlighter::highlightBlock(const QString &text)
 {

    QTextCharFormat motConnu;
    motConnu.setForeground(Qt::red);
    motConnu.setFontWeight(QFont::Bold);

    for (int i = 0; i < text.length(); ++i) {
        if (text.mid(i, 1) == "#") {
            setFormat(i, text.length() - i, Qt::darkGreen);
            break;
        }
        else if (text.mid(i, 3) == "ID:") setFormat(i, 3, motConnu);
        else if (text.mid(i, 6) == "NEWID:") setFormat(i, 6, motConnu);
        else if (text.mid(i, 7) == "EESAVE:") setFormat(i, 7, motConnu);
        else if (text.mid(i, 6) == "INTER:") setFormat(i, 6, motConnu);
        else if (text.mid(i, 6) == "TIMES:") setFormat(i, 6, motConnu);
        else if (text.mid(i, 7) == "STATES:") setFormat(i, 7, motConnu);
        else if (text.mid(i, 3) == "UP:") setFormat(i, 3, motConnu);
        else if (text.mid(i, 5) == "DOWN:") setFormat(i, 5, motConnu);
        else if (text.mid(i, 6) == "DELAY:") setFormat(i, 6, motConnu);
        else if (text.mid(i, 6) == "DIODE:") setFormat(i, 6, motConnu);
        else if (text.mid(i, 3) == "/ID") setFormat(i, 3, motConnu);
        else if (text.at(i).isNumber()) setFormat(i, 1, Qt::darkBlue);
    }
 }
