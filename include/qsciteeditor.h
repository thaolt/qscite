#ifndef QSCITEEDITOR_H_
#define QSCITEEDITOR_H_
#include <Qsci/qsciscintilla.h>
class MainWindow;
class QsciteEditor : public QsciScintilla {

    Q_OBJECT
  
  public:
    QsciteEditor(MainWindow * _parent);
    void append (const QString & text );
    void autoCompleteFromAll ();
    void autoCompleteFromAPIs ();
    void autoCompleteFromDocument ();
    void callTip ();
    void clear ();
    void copy ();
    void cut ();
    void ensureCursorVisible ();
    void ensureLineVisible (int line );
    void foldAll (bool children);
    void foldLine (int line );
    void indent (int line );
    void insert (const QString & text );
    void insertAt (const QString & text , int line , int index );
    void moveToMatchingBrace ();
    void paste ();
    void redo ();
    void removeSelectedText ();
    void resetSelectionBackgroundColor ();
    void resetSelectionForegroundColor ();
    void selectAll (bool select = true );
    void selectToMatchingBrace ();
    void setAutoCompletionCaseSensitivity (bool cs );
    void setAutoCompletionReplaceWord (bool replace );
    void setAutoCompletionShowSingle (bool single );
    void setAutoCompletionSource (QsciScintilla::AutoCompletionSource source );
    void setAutoCompletionThreshold (int thresh );
    void setAutoIndent (bool autoindent );
    void setBraceMatching (QsciScintilla::BraceMatch bm );
    void setBackspaceUnindents (bool unindent );
    void setCaretForegroundColor (const QColor & col );
    void setCaretLineBackgroundColor (const QColor & col );
    void setCaretLineVisible (bool enable );
    void setCaretWidth (int width );
    void setColor (const QColor & c );
    void setCursorPosition (int line , int index );
    void setEolMode (QsciScintilla::EolMode mode );
    void setEolVisibility (bool visible );
    void setFolding (QsciScintilla::FoldStyle fold , int margin = 2 );
    void setIndentation (int line , int indentation );
    void setIndentationGuides (bool enable );
    void setIndentationGuidesBackgroundColor (const QColor & col );
    void setIndentationGuidesForegroundColor (const QColor & col );
    void setIndentationsUseTabs (bool tabs );
    void setIndentationWidth (int width );
    void setLexer (QsciLexer * lexer = 0 );
    void setMarginsBackgroundColor (const QColor & col );
    void setMarginsFont (const QFont & f );
    void setMarginsForegroundColor (const QColor & col );
    void setMarginLineNumbers (int margin , bool lnrs );
    void setMarginMarkerMask (int margin , int mask );
    void setMarginSensitivity (int margin , bool sens );
    void setMarginWidth (int margin , int width );
    void setMarginWidth (int margin , const QString & s );
    void setModified (bool m );
    void setPaper (const QColor & c );
    void setReadOnly (bool ro );
    void setSelection (int lineFrom , int indexFrom , int lineTo , int indexTo );
    void setSelectionBackgroundColor (const QColor & col );
    void setSelectionForegroundColor (const QColor & col );
    void setTabIndents (bool indent );
    void setTabWidth (int width );
    void setText (const QString & text );
    void setUtf8 (bool cp );
    void setWhitespaceVisibility (QsciScintilla::WhitespaceVisibility mode );
    void setWrapMode (QsciScintilla::WrapMode mode );
    void undo ();
    void unindent (int line );
    void zoomIn (int range );
    void zoomIn ();
    void zoomOut (int range );
    void zoomOut ();
    void zoomTo (int size );
    QStringList apiContext (int pos , int & context_start , int & last_word_start );
    void annotate (int line , const QString & text , int style );
    void annotate (int line , const QString & text , const QsciStyle & style );
    void annotate (int line , const QsciStyledText & text );
    void annotate (int line , const QList<QsciStyledText> & text );
    QString annotation (int line ) const;
    void clearAnnotations (int line = -1 );
    bool autoCompletionCaseSensitivity () const;
    bool autoCompletionFillupsEnabled () const;
    bool autoCompletionReplaceWord () const;
    bool autoCompletionShowSingle () const;
    QsciScintilla::AutoCompletionSource autoCompletionSource () const;
    int autoCompletionThreshold () const;
    bool autoIndent () const;
    bool backspaceUnindents () const;
    void beginUndoAction ();
    QsciScintilla::BraceMatch braceMatching () const;
    QsciScintilla::CallTipsStyle callTipsStyle () const;
    int callTipsVisible () const;
    void cancelList ();
    bool caseSensitive () const;
    void clearRegisteredImages ();
    QColor color () const;
    void convertEols (QsciScintilla::EolMode mode );
    QsciDocument document () const;
    void endUndoAction ();
    QColor edgeColor () const;
    int edgeColumn () const;
    QsciScintilla::EdgeMode edgeMode () const;
    void setFont (const QFont & f );
    QsciScintilla::EolMode eolMode () const;
    bool eolVisibility () const;
    bool findFirst (const QString & expr , bool re , bool cs , bool wo , bool wrap , bool forward = true , int line = -1 , int index = -1 , bool show = true );
    bool findNext ();
    int firstVisibleLine () const;
    QsciScintilla::FoldStyle folding () const;
    void getCursorPosition (int * line , int * index ) const;
    void getSelection (int * lineFrom , int * indexFrom , int * lineTo , int * indexTo ) const;
    bool hasSelectedText () const;
    int indentation (int line ) const;
    bool indentationGuides () const;
    bool indentationsUseTabs () const;
    int indentationWidth () const;
    bool isCallTipActive () const;
    bool isListActive () const;
    bool isModified () const;
    bool isReadOnly () const;
    bool isRedoAvailable () const;
    bool isUndoAvailable () const;
    bool isUtf8 () const;
    bool isWordCharacter (char ch ) const;
    int lineAt (const QPoint & pos ) const;
    void lineIndexFromPosition (int position , int * line , int * index ) const;
    int lineLength (int line ) const;
    int lines () const;
    int length () const;
    QsciLexer * lexer () const;
    bool marginLineNumbers (int margin ) const;
    int marginMarkerMask (int margin ) const;
    bool marginSensitivity (int margin ) const;
    QsciScintilla::MarginType marginType (int margin ) const;
    int marginWidth (int margin ) const;
    int markerDefine (char ch , int mnr = -1 );
    int markerDefine (const QPixmap & pm , int mnr = -1 );
    int markerAdd (int linenr , int mnr );
    unsigned markersAtLine (int linenr ) const;
    void markerDelete (int linenr , int mnr = -1 );
    void markerDeleteAll (int mnr = -1 );
    void markerDeleteHandle (int mhandle );
    int markerLine (int mhandle ) const;
    int markerFindNext (int linenr , unsigned mask ) const;
    int markerFindPrevious (int linenr , unsigned mask ) const;
    QColor paper () const;
    int positionFromLineIndex (int line , int index ) const;
    bool read (QIODevice * io );
    void recolor (int start = 0 , int end = -1 );
    void registerImage (int id , const QPixmap & pm );
    void replace (const QString & replaceStr );
    void resetFoldMarginColors ();
    void setFoldMarginColors (const QColor & fore , const QColor & back );
    void setAutoCompletionFillupsEnabled (bool enabled );
    void setAutoCompletionFillups (const char * fillups );
    void setAutoCompletionWordSeparators (const QStringList & separators );
    void setCallTipsBackgroundColor (const QColor & col );
    void setCallTipsForegroundColor (const QColor & col );
    void setCallTipsHighlightColor (const QColor & col );
    void setCallTipsStyle (QsciScintilla::CallTipsStyle style );
    void setCallTipsVisible (int nr );
    void setDocument (const QsciDocument & document );
    void setEdgeColor (const QColor & col );
    void setEdgeColumn (int colnr );
    void setEdgeMode (QsciScintilla::EdgeMode mode );
    void setMarginText (int line , const QString & text , int style );
    void setMarginText (int line , const QString & text , const QsciStyle & style );
    void setMarginText (int line , const QsciStyledText & text );
    void setMarginType (int margin , QsciScintilla::MarginType type );
    void clearMarginText (int line = -1 );
    void setMarkerBackgroundColor (const QColor & col , int mnr = -1 );
    void setMarkerForegroundColor (const QColor & col , int mnr = -1 );
    void setMatchedBraceBackgroundColor (const QColor & col );
    void setMatchedBraceForegroundColor (const QColor & col );
    void setUnmatchedBraceBackgroundColor (const QColor & col );
    void setUnmatchedBraceForegroundColor (const QColor & col );
    QString selectedText () const;
    bool selectionToEol () const;
    void setSelectionToEol (bool filled );
    void showUserList (int id , const QStringList & list );
    QsciCommandSet * standardCommands () const;
    bool tabIndents () const;
    int tabWidth () const;
    QString text () const;
    QString text (int line ) const;
    int textHeight (int linenr ) const;
    QsciScintilla::WhitespaceVisibility whitespaceVisibility () const;
    QString wordAtPoint (const QPoint & point ) const;
    const char * wordCharacters () const;
    QsciScintilla::WrapMode wrapMode () const;
    bool write (QIODevice * io ) const;


  protected:
    void keyPressEvent(QKeyEvent * event);
    void unindentHandler();
    void dropEvent(QDropEvent *event);
    MainWindow * parent;

};
    
#endif /*QSCITEEDITOR_H_*/
