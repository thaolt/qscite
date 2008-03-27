#include <QApplication>
#include <QVector>
#include <QStringList>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QSettings>
#include <QtDebug>

#include "launcher.h"
#include "mainwindow.h"

Launcher::Launcher (QStringList argv, QApplication * _app) {
  app = _app;
  createFirstWindow(argv);
  
  if(QSystemTrayIcon::isSystemTrayAvailable() && QSettings().value("trayIcon", true).toBool()) {
    qDebug() << "Creating the system tray icon";
    createTrayIcon();
    trayIcon->setIcon(QIcon(":images/appIcon.png"));
    trayIcon->show();
  }
}

Launcher::~Launcher() { }

void Launcher::createFirstWindow(QStringList _argv) {
  MainWindow * window = new MainWindow(_argv, this);
  app->installEventFilter(window);
  window->show();
  windows.push_back(window);
}

void Launcher::createNewWindow() {
  QStringList _argv;
  MainWindow * window = new MainWindow(_argv, this);
  app->installEventFilter(window);
  window->show();
  windows.push_back(window);
}

void Launcher::createTrayIcon() {
  createActions();
  trayIconMenu = new QMenu();
  trayIconMenu->addAction(minimizeAction);
  trayIconMenu->addAction(maximizeAction);
  trayIconMenu->addAction(restoreAction);
  trayIconMenu->addSeparator();
  trayIconMenu->addAction(quitAction);

  trayIcon = new QSystemTrayIcon();
  trayIcon->setContextMenu(trayIconMenu);
#ifndef Q_WS_MAC
  connect(trayIcon, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(trayClicked(QSystemTrayIcon::ActivationReason)));
#endif
}

void Launcher::createActions() {
  minimizeAction = new QAction(tr("Mi&nimize"), this);
  //connect(minimizeAction, SIGNAL(triggered()), this, SLOT(hide()));

  maximizeAction = new QAction(tr("Ma&ximize"), this);
  //connect(maximizeAction, SIGNAL(triggered()), this, SLOT(showMaximized()));

  restoreAction = new QAction(tr("&Restore"), this);
  //connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));

  quitAction = new QAction(tr("&Quit"), this);
  connect(quitAction, SIGNAL(triggered()), this, SLOT(quitApplication()));
}

void Launcher::trayClicked(QSystemTrayIcon::ActivationReason reason) {
  if (reason == QSystemTrayIcon::Trigger) { // the icon was clicked
    //this->setVisible(!this->isVisible());
  }
}

void Launcher::quitApplication() {
  qDebug() << "quit clicked";
  
  while (windows.size()) {
    bool closed = windows.front()->closeWindow();
    
    if (!closed) {
      return;
    } else {
      delete windows.front();
      windows.pop_front();
    }
  }
  
  app->quit();
}
