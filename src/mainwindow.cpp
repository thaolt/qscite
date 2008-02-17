/***************************************************************************
 *   Copyright (C) 2007-2008 by Jared Sutton <jpsutton@gmail.com>          *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 
#include <QtGui>
#include <Qsci/qsciscintilla.h>
#include <Qsci/qscilexer.h>

#ifdef QSCITE_DEBUG
#include <iostream>
#endif

#include "mainwindow.h"
#include "utils.h"
#include "lexer_utils.h"
#include "prefs.h"
#include "qterminal.h"

MainWindow::MainWindow() {
  openFiles = new std::vector<QsciScintilla *>;
  fileNames = new std::vector<QString>;
  fileNames->push_back(QString(""));

  tabWidget = new QTabWidget(this);
  setCentralWidget(tabWidget);
  createDocument();
  createActions();
  createMenus();
  createToolBars();
  createStatusBar();

  QToolButton * closeTabButton = new QToolButton(tabWidget);
  closeTabButton->setDefaultAction(closeAct);
  tabWidget->setCornerWidget(closeTabButton);

  readSettings();
  connect(curDoc, SIGNAL(modificationChanged(bool)), this, SLOT(setDocumentModified(bool)));
  connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(curDocChanged(int)));
  setWindowTitleForFile("");
}

void MainWindow::createDocument() {
  curDoc = new QsciScintilla();
  
  QSettings settings;
  if (settings.value("version", 0).toInt() < 1) {
#ifdef QSCITE_DEBUG
    std::cout << "Using default preferences" << std::endl;
#endif
    writeDefaultSettings(settings);
  }

  // Default EOL mode to LF
  curDoc->setEolMode(
  	static_cast<QsciScintilla::EolMode>(
  		settings.value("EOLMode", QsciScintilla::EolUnix).toInt()
  	)
  );
  
  // Default wrap mode to WrapWord
  curDoc->setWrapMode(
  	static_cast<QsciScintilla::WrapMode>(
  		settings.value("wrapMode", QsciScintilla::WrapWord).toInt()
  	)
  );
  // Turn on line numbers by default
  curDoc->setMarginLineNumbers(1, true);
  // set default margin width to 4 characters (will adjust with different files)
  curDoc->setMarginWidth(1, "9999");
  // Don't use tab characters for indents
  curDoc->setIndentationsUseTabs(settings.value("indentUseTabs", false).toBool());
  // Default to using two spaces for each indent
  curDoc->setIndentationWidth(settings.value("indentWidth", 2).toInt());
  curDoc->setTabWidth(settings.value("indentWidth", 2).toInt());
  // Make backspaces unindent
  curDoc->setBackspaceUnindents(settings.value("backspaceUnindents", true).toBool());
  // Turn on strict brace matching by default
  curDoc->setBraceMatching(
  	static_cast<QsciScintilla::BraceMatch>(
  		settings.value("braceMatchMode", QsciScintilla::StrictBraceMatch).toInt()
  	)
  );
  // use Monospaced font at size 10 by default
  QFont baseFont(settings.value("font/family", QSCITE_MONO_FAMILY).toString(), settings.value("font/size", 10).toInt());

  curDoc->setFont(baseFont);
  
#ifdef QSCITE_DEBUG
  QFontInfo test(baseFont);
  std::cout << "Current font family: " << test.family().toStdString() << endl;
  std::cout << "Exact font match: " << (test.exactMatch() ? "true" : "false") << endl;
#endif
  
  openFiles->push_back(curDoc);
  fileNames->push_back("");
  tabWidget->addTab(curDoc, "Untitled");
  modified.push_back(false);
  changeTabs(tabWidget->count() - 1);
}

void MainWindow::createTerminal() {
  QTerminal * term = new QTerminal(this);
  tabWidget->addTab(term, "BASH Terminal");
}

void MainWindow::changeTabs(int index) {
#ifdef QSCITE_DEBUG
  std::cout << "attempting to change tabs to index " << index << endl;
#endif
  tabWidget->setCurrentIndex(index);
  //curDocChanged(index);
}

void MainWindow::closeEvent(QCloseEvent *event) {
  while(tabWidget->count()) {
    changeTabs(0);
    if (!closeFile()) {
      event->ignore();
      return;
    }
  }
  
  writeSettings();
  
  event->accept();
}

void MainWindow::newFile() {
  createDocument();
}

bool MainWindow::closeFile() {
  if (tabWidget->count()) {
    if (maybeSave()) {
      QsciScintilla * theEditor = curDoc;
      int curTabIndex = tabWidget->currentIndex();
      tabWidget->removeTab(curTabIndex);
      fileNames->erase(fileNames->begin() + curTabIndex);
      modified.erase(modified.begin() + curTabIndex);
      delete theEditor;
      if (tabWidget->count()) {	// there are tabs left
        changeTabs(curTabIndex > 1 ? curTabIndex - 1 : 0);
      } else {
        // just to make sure...
        curFile = "";
        setWindowTitle(tr("QSciTE"));
        setWindowModified(false);
      }
      return true;
    }
  }
  
  return false;
}

void MainWindow::open() {
  QString fileName = QFileDialog::getOpenFileName(this);
  
  if (!fileName.isEmpty()) {
    if ((!tabWidget->count()) || (!curFile.isEmpty()) || modified[tabWidget->currentIndex()]) {
      createDocument();
    }
    
    loadFile(fileName);
  }
}

bool MainWindow::save() {
  if (tabWidget->count()) {
    if (curFile.isEmpty()) {
      return saveAs();
    } else {
      return saveFile(curFile);
    }
  }
  
  // make the compiler happy :)
  return false;
}

bool MainWindow::saveAs() {
  if (tabWidget->count()) {
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty()) {
      return false;
    }
  
    return saveFile(fileName);
  }
  
  return false;
}

void MainWindow::fontDialog() {
  if (tabWidget->count()) {
  	QsciLexer * lexer = curDoc->lexer();
  	bool ok;
  	if (lexer) {
  	  QFont baseFont = QFontDialog::getFont(&ok, lexer->font(lexer->defaultStyle()));
  	  if (ok) {
  	    setLexerFont(lexer, baseFont.family(), baseFont.pointSize());
  	  }
  	} else {
      QFont font = QFontDialog::getFont(&ok, curDoc->font());
      if (ok) {
        curDoc->setFont(font);
      }
  	}
  }
}

void MainWindow::about() {
   QMessageBox::about(this, tr("About QSciTE"),
            tr("<b>QSciTE</b> is a clone of SciTE, based on the Scintilla library"
               " and Qt4. It is heavily based on the example code included with"
               " Qscintilla2, and is licensed under the GNU GPL version 2."));
}

void MainWindow::setDocumentModified(bool wasModified) {
#ifdef QSCITE_DEBUG
  std::cout << "setDocumentModified(" << wasModified << ')' << std::endl;
 #endif
  int who = tabWidget->currentIndex();
  if (modified[who] != wasModified) {
    modified[who] = wasModified;
	setCurrentTabTitle();
    setWindowModified(wasModified);
  }

}

void MainWindow::curDocChanged(int idx) {
  QString currentWidgetClass = tabWidget->currentWidget()->metaObject()->className(); 
  if (currentWidgetClass == "QTerminal") {
    //do something
  } else {
    curDoc->disconnect(SIGNAL(modificationChanged(bool)));
    curDoc = (QsciScintilla *) tabWidget->currentWidget();
    connect(curDoc, SIGNAL(modificationChanged(bool)), this, SLOT(setDocumentModified(bool)));
    curFile = (*fileNames)[idx];
    setWindowTitleForFile(curFile);
    setWindowModified(modified[idx]);
  }
}


void MainWindow::readSettings() {
    QSettings settings;
    QPoint pos = settings.value("pos", QPoint(200, 200)).toPoint();
    QSize size = settings.value("size", QSize(400, 400)).toSize();
    resize(size);
    move(pos);
}

void MainWindow::writeSettings() {
  QSettings settings;
  settings.setValue("pos", pos());
  settings.setValue("size", size());
}

void MainWindow::globalPrefs() {
	MainPrefsDialog * dlgPrefs = new MainPrefsDialog(this, Qt::Sheet);
	dlgPrefs->show();
}

bool MainWindow::maybeSave() {
  if (modified[tabWidget->currentIndex()]) {
    int ret = QMessageBox::warning(this, tr("QSciTE"),
                 tr((strippedName(curFile).toStdString() + " has been modified.\n"
                    "Do you want to save your changes?").c_str()),
                 QMessageBox::Yes | QMessageBox::Default,
                 QMessageBox::No,
                 QMessageBox::Cancel | QMessageBox::Escape);
    if (ret == QMessageBox::Yes) {
      return save();
    } else if (ret == QMessageBox::Cancel) {
      return false;
    }
  }
  return true;
}

void MainWindow::loadFile(const QString &fileName) {
  QFile file(fileName);
  if (!file.open(QFile::ReadOnly)) {
    QMessageBox::warning(this, tr("QSciTE"),
                         tr("Cannot read file %1:\n%2.")
                         .arg(fileName)
                         .arg(file.errorString()));
    return;
  }

  QTextStream in(&file);
  QApplication::setOverrideCursor(Qt::WaitCursor);
  curDoc->setText(in.readAll());
  QApplication::restoreOverrideCursor();
  
  curFile = fileName;
  (*fileNames)[tabWidget->currentIndex()] = fileName;

  setWindowTitleForFile(fileName);
  curDoc->setModified(false);
  
  redoSetMargin();
  
  QFont currentFont = curDoc->font();
  QsciLexer * newLexer = getLexerForDocument(fileName, curDoc->text());
  
  if (newLexer != NULL) {
#ifdef QSCITE_DEBUG
    std::cout << "Using lexer " << newLexer->lexer() << std::endl;
#endif
  	newLexer->setParent(curDoc);
    curDoc->setLexer(newLexer);
    setLexerFont(newLexer, currentFont.family(), currentFont.pointSize());
  }

//   setDocumentModified(false);
//   setCurrentTabTitle();
  statusBar()->showMessage(tr("File loaded"), 2000);
}


void MainWindow::redoSetMargin() {
  double numLines = (double)curDoc->lines();
  QString exLength = "99999";
  numLines /= 10000;
  
  while (numLines > 1) {
    exLength += "9";
    numLines /= 10.0;
  }

  curDoc->setMarginWidth(1, exLength);
}

bool MainWindow::saveFile(const QString &fileName) {
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly)) {
        QMessageBox::warning(this, tr("QSciTE"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << curDoc->text();
    QApplication::restoreOverrideCursor();
    statusBar()->showMessage(tr("File saved"), 2000);
    setDocumentModified(false);
    return true;
}

void MainWindow::setWindowTitleForFile(const QString & fileName) {
    QString shownName;
    if (fileName.isEmpty()) {
        shownName = "Untitled";
    } else {
        shownName = strippedName(fileName);
    }
    setWindowTitle(tr("%1[*] - %2").arg(shownName).arg(tr("QSciTE")));
}

void MainWindow::setCurrentTabTitle() {
	int idx = tabWidget->currentIndex();
	QString displayName = curFile.isEmpty() ? "Untitled" : strippedName(curFile);
	if (modified[idx]) {
		displayName += "*";
	}
	tabWidget->setTabText(idx, displayName);
}