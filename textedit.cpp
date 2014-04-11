
#include "textedit.h"
#include <QAction>
#include <QApplication>
#include <QTextLine>
#include <QStackedLayout>
#include <QClipboard>
#include <QColorDialog>
#include <QComboBox>
#include <QFontComboBox>
#include <QFile>
#include <QFileDialog>
#include <QFileInfo>
#include <QFontDatabase>
#include <QMenu>
#include <QMenuBar>
#include <QPrintDialog>
#include <QPrinter>
#include <QTextCodec>
#include <QTextEdit>
#include <QToolBar>
#include <QTextCursor>
#include <QTextDocumentWriter>
#include <QTextList>
#include <QtDebug>
#include <QCloseEvent>
#include <QButtonGroup>
#include <QPushButton>
#include <QMessageBox>
#include <QPrintPreviewDialog>
#include <QScrollArea>
#include <QTextDocumentFragment>
//#include <QtXml>
//#include <QDomNodeList>

QT_FORWARD_DECLARE_CLASS(QTextDocumentFragment)


//>>>>>>>>>>>>>>>>>load canvas which show the marks<<<<<<<<<<<
/**
 * 载入第curPn(当前）页面的画板，显示黄线标注等
 */
void TextEdit::loadCanvas(){
    canvas->reload();
    for(int i=0;i<extInfo.marks[curPn].size();i++){
        QPair<int,int> s=extInfo.marks[curPn][i];
        int l=s.first,r=s.second;

        QTextBlock block=extInfo.docs[curPn]->findBlock(l);
        paintLines(block,l,r,Qt::yellow);
    }
}
//>>>>>>>>>>>>>>>>>>>>>>paint the marks<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<,
/**
*获取在一行内的文本区间[textStart,textEnd]对应的坐标区间
 */
QLine getLine(QTextBlock *block, int textStart,int textEnd){
    int blockStart=block->position();
    QTextLayout* layout=block->layout();
    QPoint blockOffset=layout->position().toPoint();
    QTextLine layoutLine=layout->lineForTextPosition(textStart-blockStart);
    int lineHeight=layoutLine.naturalTextRect().height();
    QPoint lineOffset=layoutLine.position().toPoint();
    int lineTextStart=blockStart+layoutLine.textStart();
    int lineTextEnd=lineTextStart+layoutLine.textLength();
    int lineWidth=layoutLine.naturalTextWidth();
    int x0=blockOffset.x()+lineOffset.x()+((float)(textStart-lineTextStart)/(lineTextEnd-lineTextStart)*lineWidth);
    int y0=blockOffset.y()+lineOffset.y()+lineHeight;
    int x1=blockOffset.x()+lineOffset.x()+((float)(textEnd-lineTextStart)/(lineTextEnd-lineTextStart)*lineWidth);
    QPoint linePoint(x0,y0);
    QPoint lineEndPoint=QPoint(x1,y0);
    return QLine(linePoint,lineEndPoint);
}
/**
  *get the text whose position is most close to p in doc
 */
int findTextFromPosition(QTextDocument* doc,QPointF p,QTextBlock *_block){
    float x=p.rx(),y=p.ry();
    QTextBlock block=doc->begin();
    int targetIndex=-1;//
    //遍历block寻找离p最近的block
    for(;block!=doc->end();block=block.next()){
        //first text index in block
        int blockStart=block.position();
        //int blockEnd=blockStart+block.length();
        QTextLayout* layout=block.layout();
        QPoint  blockOffset=layout->position().toPoint();
        for(int i=0;i<layout->lineCount();i++){
            //对第i行
           QTextLine layoutLine=layout->lineAt(i);
           //QRectF rect=layoutLine.naturalTextRect();
           QPoint lineOffset=layoutLine.position().toPoint();
           int lineHeight=layoutLine.naturalTextRect().height();
           int minY=blockOffset.y()+lineOffset.y();
           int maxY=blockOffset.y()+lineOffset.y()+lineHeight;

           if(y>=minY&&y<=maxY){//find the line
               int lineWidth=layoutLine.naturalTextWidth();
               int lineTextStart=blockStart+layoutLine.textStart();


               targetIndex=((float)x-blockOffset.x()-lineOffset.x())*(layoutLine.textLength())/lineWidth + lineTextStart;
               if(_block!=NULL){
                   *_block=block;
               }
               return targetIndex;
           }
       }
    }
    return -1;//没找到
}

/**
 *对文本区间［textStart,textEnd]画黄线标注
*/
//paint the underline of texts from textstart to textend
void TextEdit::paintLines(QTextBlock &block,int textStart,int textEnd,QColor color){
    int a=textStart-1,b=a;
    int i=0;
    while(b<textEnd){
        //printLines(&block);
        int blockStart=block.position();
        int blockEnd=blockStart+block.length();
        if(textStart>textEnd) return;
        QTextLayout* layout=block.layout();
        while( b < textEnd && b < blockEnd - 1 ){
            a=b+1;
            QTextLine layoutLine=layout->lineForTextPosition(a-blockStart);
            if( !layoutLine.isValid() ) { b++; continue; }
            int lineTextStart=blockStart+layoutLine.textStart();
            int lineTextEnd=lineTextStart+layoutLine.textLength();
            b=MIN(textEnd,lineTextEnd);
            //draw the highlight line on the canvas
            QLine line=getLine(&block,a,b);
            cout<<"paint line from "<<a<<" to "<<b<<endl;
//            irPositionChanged(line.p1(),line.p2(),true);
//            int s=findTextFromPosition(textEdit->document(),line.p1(),NULL);
//            cout<<"find text at position:"<<s<<endl;
            canvas->paintLine(line,color);
        }
        if(textEnd>b){
            block=block.next();
            b=block.position();
        }
    }
}

/**
 * @brief调整文本格式的文本大小
 */
void TextEdit::iniFontSize(){
    cout<<"#textedit::inifontsize";
    qreal pointSize = FONTSIZE;
    QTextCharFormat fmt;
    fmt.setFontPointSize(pointSize);
    QTextCursor cursor = textEdit->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeCharFormat(fmt);
    hideText();
    //  textEdit->setCurrentCharFormat(fmt);
}
//set the color of the text of the textedit
void TextEdit::hideText(){
    QColor col = FONTCOLOR;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    QTextCursor cursor = textEdit->textCursor();
    cursor.select(QTextCursor::Document);
    cursor.mergeCharFormat(fmt);
    textEdit->setCurrentCharFormat(fmt);
}
/**
*调整文本格式的行高
*被load载入文件调用
 */
void iniLineMargin(QTextEdit *textEdit){
    QTextDocument* doc=textEdit->document();
    int t=0;
    for(QTextBlock it = doc->begin(); it != doc->end();t++,it = it.next())
    {
        QTextCursor textCursor=QTextCursor(it);
        QTextBlockFormat textBlockFormat = it.blockFormat();
        textBlockFormat.setBottomMargin(0);//set line margin
        textBlockFormat.setTopMargin(0);
        cout<<"#"<<t;
        cout<<textCursor.block().text();
        textCursor.setBlockFormat(textBlockFormat);
        //    textEdit->setTextCursor(textCursor);
    }
}
/**
*跳到页面s
*/
void TextEdit::pageChanged(int s){
    if(s>=extInfo.size()||s<0) {
        cout<<"textedit.pageChanged:page "<<s<<" not found"<<endl;
        return;
    }
    curPn=s;
   // comboPn->setCurrentIndex(curPn);
    textEdit->setDocument(extInfo.docs[curPn]);
    cout<<"textedit.setDocument "<<curPn<<endl;
    iniFontSize();
    iniLineMargin(textEdit);
    loadCanvas();
}
void TextEdit::pageChanged(QString s){
    return pageChanged(s.toInt()-1);
}
//jump to next page
void TextEdit::nextPage(){
    int t=curPn+1;
    if(t>=extInfo.size()) return;
    pageChanged(t);
}
//jump to previous page
void TextEdit::prevPage(){
    int t=curPn-1;
    if(t<0) return;
    pageChanged(t);
}
/**
 *载入文本文件后要分页，切分文本文件成多个页面
 *被loadFile调用
 */
void TextEdit::dividePages(){
    iniFontSize();
    iniLineMargin(textEdit);
    QTextDocument* doc=textEdit->document();
    cout<<doc->textWidth()<<","<<doc->pageCount()<<","<<doc->pageSize();
    QTextBlock block=doc->begin();
    QString str="";
    int pageHeight=4;
    while(true){
        //cout<<"";
        //cout<<"block "<<block.blockNumber()<<"\tposx:"<<block.layout()->position().x()<<"\tposy:"<<block.layout()->position().y();
        //cout<<block.text();
        QTextLayout* layout=block.layout();
        //    if(layout==NULL) return;
        int i=0;
        //for(;i<=blockLength;i++){
        for(;i<layout->lineCount();i++){
            //QTextLine layoutLine=layout->lineForTextPosition(i);
            QTextLine layoutLine=layout->lineAt(i);
            int lineTextStart=layoutLine.textStart();
            int lineTextLength=layoutLine.textLength();
            int lineHeight=layoutLine.height();
            if(pageHeight+lineHeight>PAGEHEIGHT-4){ //new page
                extInfo.append(new QTextDocument(str));
                str=block.text().mid(lineTextStart,lineTextLength)+'\n';
                pageHeight=lineHeight+4;
            }
            else{
                str.append(block.text().mid(lineTextStart,lineTextLength)+'\n');
                //cout<<layoutLine.lineNumber()<<","<<layoutLine.rect()<<","<<layoutLine.position()
                //<<","<<layoutLine.textStart()<<","<<layoutLine.textLength()<<":"<<block.text().mid(lineTextStart,lineTextLength);
                pageHeight+=lineHeight;
            }
            // if(i==layout->lineCount()-1) str+="\n";
        }
        block=block.next();
        if(!block.isValid()) break;
    }
    if(str!=""||extInfo.size()<=0) extInfo.append(new QTextDocument(str));
    //init the  marks, which is used by our canvas
    //extInfo保存画板信息
    extInfo.clearMarks();
    extInfo.iniMarks();
    //comboPn->clear();
//    for(int size=1;size<=extInfo.size();size++)
//        comboPn->addItem(QString::number(size));
    //show the first page at first
    pageChanged(0);
    cout<<"dividePages end";
}

/**
 *初始化函数
 *设置界面（画板canvas和编辑器textEdit)
 */
TextEdit::TextEdit(QWidget *parent)
    : QWidget(parent)
{
   // setToolButtonStyle(Qt::ToolButtonFollowStyle);
    //setupFileActions();
    //setupTextActions();

    //create the text edit widget
    textEdit = new QTextEdit(this);
//    connect(textEdit, SIGNAL(currentCharFormatChanged(QTextCharFormat)),
//            this, SLOT(currentCharFormatChanged(QTextCharFormat)));
    connect(textEdit, SIGNAL(cursorPositionChanged()),
            this, SLOT(cursorPositionChanged()));
    //connect(textEdit,SIGNAL(textChanged()),this,SLOT(dividePages()));


    //set textedit as fix size
    textEdit->setFixedSize(QSize(A4WIDTH,A4HEIGHT));

    QPalette pl = textEdit->palette();
    pl.setBrush(QPalette::Base,QBrush(QColor(255,0,0,0)));
    textEdit->setPalette(pl);

    //create a transparent canvas and put it on the top of textEdit
    canvas =new MyCanvas(A4WIDTH,A4HEIGHT,this);
    canvas->setFixedSize(QSize(A4WIDTH,A4HEIGHT));
    canvas->setStyleSheet(QString::fromUtf8("border:1px solid #eeeeee;"));

    //create a scrollarea contains the widgets as canvas and textedit
    QScrollArea* scrollArea = new QScrollArea;
    scrollArea->setFixedSize(QSize(A4WIDTH,A4HEIGHT));
    scrollArea->setBackgroundRole(QPalette::Light);

    //set layout of the scrollarea which contains canvas and textedit
    QStackedLayout *stackedLayout = new QStackedLayout;
    stackedLayout->addWidget(canvas);
    stackedLayout->addWidget(textEdit);
    textEdit->setAlignment(Qt::AlignHCenter);
    stackedLayout->setStackingMode(QStackedLayout::StackAll);
    scrollArea->setLayout(stackedLayout);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setStyleSheet(QString::fromUtf8("border:1px dashed #777;"));

    //翻页条
    //pnUp=new QPushButton;
    //pnDown=new QPushButton;
    //connect(pnUp,SIGNAL(clicked()),this,SLOT(prevPage()));
    //connect(pnDown,SIGNAL(clicked()),this,SLOT(nextPage()));

    QVBoxLayout *vLayout=new QVBoxLayout(this);
    //vLayout->addWidget(pnUp);
    vLayout->addWidget(scrollArea);
    //vLayout->addWidget(pnDown);
    vLayout->setAlignment(Qt::AlignCenter);
    /*leftArea = new QScrollArea;
    leftArea->setFixedSize(QSize(A4WIDTH+40,A4HEIGHT+80));
    leftArea->setBackgroundRole(QPalette::Light);
    leftArea->setLayout(vLayout);

    QHBoxLayout *bLayout=new QHBoxLayout;
    bLayout->addWidget(leftArea);*/
//    bLayout->addWidget(rightArea);

    /*QScrollArea* outerScrollArea = new QScrollArea(this);
    outerScrollArea->setMinimumWidth(2*A4WIDTH+200);
    outerScrollArea->setMinimumHeight(A4HEIGHT+100);
    outerScrollArea->setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);
    outerScrollArea->setBackgroundRole(QPalette::Light);
    outerScrollArea->setAlignment(Qt::AlignCenter);
    outerScrollArea->setLayout(bLayout);*/


    //setCentralWidget(outerScrollArea);
    //@invalidate codes
    //after canvas handle the mouse-drag event, emit it to the edittext for farther handling
    //connect(canvas,SIGNAL(mouseMoveSig(QMouseEvent*)),this,SLOT(onMouseMove(QMouseEvent*)));
    //connect(canvas,SIGNAL(mouseMoveSig(QMouseEvent*)),textEdit,SLOT(mouseMoveEvent(QMouseEvent*)));
    //connect(canvas,SIGNAL(mouseMoveSig(QMouseEvent*)),textEdit,SLOT(cursorPositionChanged(QMouseEvent*)));
    //connect(this,SIGNAL(mouseMoveSig(QMouseEvent*)),canvas,SLOT(mouseMoveSlot(QMouseEvent*)));
    //connect(textEdit,SIGNAL(mouseMoveEvent(QMouseEvent*)),canvas,SLOT(mouseMoveSlot(QMouseEvent*)));
    setWindowModified(textEdit->document()->isModified());

}
/**
 *初始化界面中的文件操作工具条
 */
/*void TextEdit::setupFileActions()
{
    QToolBar *tb = new QToolBar(this);
    tb->setWindowTitle(tr("File Actions"));
    //addToolBar(tb);

    QAction *a;
    a = new QAction(QIcon::fromTheme("document-open", QIcon(rsrcPath + "/fileopen.png")),
                    tr("&Open..."), this);
    a->setShortcut(QKeySequence::Open);
    connect(a, SIGNAL(triggered()), this, SLOT(fileOpen()));
    tb->addAction(a);

    a = new QAction(tr("&Quit"), this);
    connect(a, SIGNAL(triggered()), this, SLOT(close()));
}*/
/**
 *初始化界面中的工具栏
 */
/*void TextEdit::setupTextActions()
{
    QToolBar *tb = new QToolBar(this);
    tb->setWindowTitle(tr("Format Actions"));
    //addToolBar(tb);

    QActionGroup *grp = new QActionGroup(this);
//    connect(grp, SIGNAL(triggered(QAction*)), this, SLOT(textAlign(QAction*)));
    tb->addActions(grp->actions());

    QPixmap pix(16, 16);
    pix.fill(Qt::black);
    actionTextColor = new QAction(pix, tr("&Color..."), this);
    connect(actionTextColor, SIGNAL(triggered()), this, SLOT(textColor()));
    tb->addAction(actionTextColor);

    comboPn = new QComboBox(tb);
    comboPn->setObjectName("comboPn");
    tb->addWidget(comboPn);
    comboPn->setEditable(true);

    connect(comboPn, SIGNAL(activated(QString)),
            this, SLOT(pageChanged(QString)));
}*/
/**
 *载入文件，调整文本格式
 */
bool TextEdit::load(const QString &f)
{
    cout<<"textedit.load:"<<f;
    if (!QFile::exists(f)){
        cout<<"textedit.load:not exists"<<f;
        return false;
    }
    QFile file(f);
    if (!file.open(QFile::ReadOnly))
        return false;

    QByteArray data = file.readAll();
    QTextCodec *codec = Qt::codecForHtml(data);
    QString str = codec->toUnicode(data);
    if (Qt::mightBeRichText(str)) {
        cout<<"textedit.load:";
        textEdit->setHtml(str);
      }else {
        str = QString::fromLocal8Bit(data);
        textEdit->setPlainText(str);
    }

    //init extinfo?
    extInfo.clear();
    cout<<"textedit ajust";
    dividePages();

    return true;
}

/**
*打开文件
*/
void TextEdit::fileOpen()
{
    QString fn = QFileDialog::getOpenFileName(this, tr("Open File..."),
                                              //QString(), tr("HTML-Files (*.htm *.html);;Al Files (*)"));
                                              QString(), tr("TEXT-Files (*.txt);;Al Files (*)"));
    cout<<"TextEdit.fileOPen:"<<fn;
    if (!fn.isEmpty())
        load(fn);
}

/**
 *对选择区域文字加粗
 *＠depreciated 废弃的函数．依赖于界面加粗工具条已经被删除
 */
void TextEdit::textBold()
{
//     QTextCharFormat fmt;
//     fmt.setFontWeight(actionTextBold->isChecked() ? QFont::Bold : QFont::Normal);
//     mergeFormatOnWordOrSelection(fmt);
}
/**
 *对选择区域文字加下划线
 @deprecated 已废弃
 */
void TextEdit::textUnderline()
{
//    QTextCharFormat fmt;
//    fmt.setFontUnderline(actionTextUnderline->isChecked());
//    mergeFormatOnWordOrSelection(fmt);
}
/**
 *对选择区域文字倾斜
 ＠depreciated 废弃
 */
void TextEdit::textItalic()
{
//  QTextCharFormat fmt;
//  fmt.setFontItalic(actionTextItalic->isChecked());
//  mergeFormatOnWordOrSelection(fmt);
}
/**
*对选择区域文字改变大小
**/
void TextEdit::textSize(const QString &p)
{
    qreal pointSize = p.toFloat();
    if (p.toFloat() > 0) {
        cout<<"textedit.textsize:"<<p;
        QTextCharFormat fmt;
        //fmt.setFontPointSize(pointSize);
        fmt.setFontPointSize(FONTSIZE);
        mergeFormatOnWordOrSelection(fmt);
    }
}
/**
 *对选择区域文字改变颜色
 * */
void TextEdit::textColor()
{
    QColor col = QColorDialog::getColor(textEdit->textColor(), this);
    if (!col.isValid())
        return;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    mergeFormatOnWordOrSelection(fmt);

}

void printPoint(QPointF p){
    cout<<p.x()<<","<<p.y();
}

//for debug only
//打印一个block中包含的各line
void printLines(QTextBlock *block){
    cout<<"#printlines";
    cout<<block->text();
    QTextLayout* layout=block->layout();
    for(int i=0;i<layout->lineCount();i++){
        QTextLine layoutLine=layout->lineAt(i);
        int s=layoutLine.textStart(),t=s+layoutLine.textLength();
        cout<<layoutLine.lineNumber()<<":"<<s<<","<<t;
        cout<<block->text().mid(s,t-s);
    }
}

/**
 *>>>>>>>>>>>>>>>>>>>>>>.cursor envent<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
 */
//when you select a text block using cursor
/**
 *处理鼠标cursor被点击并且位置发生变化的事件
 *当前cursor
 */
void TextEdit::cursorPositionChanged()
{
    //alignmentChanged(textEdit->alignment());
    cout<<"#TextEdit::cursorPositionChanged:";

    QColor col = Qt::gray;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    QTextCursor cursor = textEdit->textCursor();
    cursor.mergeCharFormat(fmt);
 //   colorChanged(col);

    int selectionStart=cursor.selectionStart(),
            selectionEnd=cursor.selectionEnd();
    cout<<"cursorPositionchanged:selection start and end:"<<selectionStart<<","<<selectionEnd;

    //pages not divided yet
    if(extInfo.size()<1) return;

    //如果只是点击一个点
    if(!cursor.hasSelection()){
        cursor.select(QTextCursor::WordUnderCursor);
    //    cout<<"cursorpositionchanged:translate "+cursor.selectedText();
    //    translate(cursor.selectedText());
        return;
    }

    //如果是持续点击（拖动）选择一条线或一个区域
    int textStart=selectionStart,textEnd=selectionEnd;
    QTextDocument* doc=textEdit->document();
    QTextBlock block=doc->findBlock(textStart);//cursor.block();
    //paint lines
    paintLines(block,textStart,textEnd,Qt::yellow);
    //store the marks
    extInfo.addMark(curPn,textStart,textEnd);
}

/**
 *处理红外位置发生变化的事件
 */
void TextEdit::irPositionChanged(QPointF begin,QPointF end,bool continued)
{
    //alignmentChanged(textEdit->alignment());
    cout<<"#TextEdit::cursorPositionChanged:";

    //check current state
    //if pages not divided yet, exit
    if(extInfo.size()<1) return;

    //ignore pure click event
    if(continued==false) return;
    //如果只是点击一个点,翻译该点对应单词
    /*if(!cursor.hasSelection()){
        cursor.select(QTextCursor::WordUnderCursor);
    //    cout<<"cursorpositionchanged:translate "+cursor.selectedText();
    //    translate(cursor.selectedText());
        return;
    }*/

    //selected text turn gray
    QColor col = Qt::gray;
    QTextCharFormat fmt;
    fmt.setForeground(col);
    QTextCursor cursor = textEdit->textCursor();
    cursor.mergeCharFormat(fmt);
    //colorChanged(col);

    //如果是持续点击（拖动）选择一条线或一个区域
    //get the start text index and the end text index
    QTextBlock block;
    int textStart=findTextFromPosition(textEdit->document(),begin,&block),
        textEnd=findTextFromPosition(textEdit->document(),end,NULL);
    cout<<"irpositionchanged:selection start and end:"<<textStart<<","<<textEnd;

    if(textStart==-1&&textEnd==-1){
        cout<<"both zuobiao invalid"<<endl;
        return;
    }

    //int textStart=cursor.selectionStart(),textEnd=cursor.selectionEnd();
    //QTextBlock block=textEdit->document()->findBlock(textStart);//cursor.block();

    //paint lines
    if(textStart!=-1&&textEnd!=-1){
        paintLines(block,textStart,textEnd,Qt::red);
        //store the marks
        extInfo.addMark(curPn,textStart,textEnd);
    }
    else  if(textStart!=-1) paintLines(block,textStart-1,textStart+1,Qt::red);
    else  paintLines(block,textEnd-1,textEnd+1,Qt::red);

}

/**
 * private function
 * used to change the selected text area's format,
 * called by textColor, textBold and so on
 */
void TextEdit::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textEdit->textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cout<<"TextEdit.mergeFormat..:"<<cursor.selectionStart()<<","<<cursor.selectionEnd();
    cursor.mergeCharFormat(format);
    textEdit->mergeCurrentCharFormat(format);
}
