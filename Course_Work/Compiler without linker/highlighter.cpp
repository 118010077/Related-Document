#include "highlighter.h"


/*
 * Constructor
 */
Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(QColor(102,153,0));
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
                    << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
                    << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
                    << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
                    << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
                    << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
                    << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
                    << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
                    << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
                    << "\\bvoid\\b" << "\\bvolatile\\b" << "\\bbool\\b"<<"\\busing\\b"<<"\\bconstexpr\\b"
                    <<"\\bsizeof\\b"<<"\\bif\\b"<<"\\bfor\\b"<<"\\bforeach\\b"<<"\\bwhile\\b"<<"\\bdo\\b"<<"\\bcase\\b"
                    <<"\\bbreak\\b"<<"\\bcontinue\\b"<<"\\btemplate\\b"<<"\\bdelete\\b"<<"\\bnew\\b"
                    <<"\\bdefault\\b"<<"\\btry\\b"<<"\\breturn\\b"<<"\\bthrow\\b"<<"\\bcatch\\b"<<"\\bgoto\\b"<<"\\belse\\b"
                    <<"\\bextren\\b"<<"\\bthis\\b"<<"\\bswitch\\b"<<"\\binclude\\b"<<"\\bdefine\\b";
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }
    /*
     * Rules for Basic definitions
     */
    //Class
    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("(?<=class\\s)\\w*");
    rule.format = classFormat;
    highlightingRules.append(rule);

    //Comment
    singleLineCommentFormat.setForeground(Qt::green);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::green);

    //Header File
    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("(?<=#include\\s)(<.*>)|(?<=#include)(<.*>)|(?<=#include\\s)(\".*\")|(?<=#include)(\".*\")|\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    //Function
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(QColor(115,182,209));
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");
}

/* Function: highlightBlock()
 * --------------------------------------------
 * Rules for hightlight Block
 */
void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);


    while (startIndex >= 0) {

        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}

