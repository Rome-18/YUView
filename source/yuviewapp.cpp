/*  This file is part of YUView - The YUV player with advanced analytics toolset
*   <https://github.com/IENT/YUView>
*   Copyright (C) 2015  Institut für Nachrichtentechnik, RWTH Aachen University, GERMANY
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 3 of the License, or
*   (at your option) any later version.
*
*   In addition, as a special exception, the copyright holders give
*   permission to link the code of portions of this program with the
*   OpenSSL library under certain conditions as described in each
*   individual source file, and distribute linked combinations including
*   the two.
*   
*   You must obey the GNU General Public License in all respects for all
*   of the code used other than OpenSSL. If you modify file(s) with this
*   exception, you may extend this exception to your version of the
*   file(s), but you are not obligated to do so. If you do not wish to do
*   so, delete this exception statement from your version. If you delete
*   this exception statement from all source files in the program, then
*   also delete it here.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "yuviewapp.h"

#include "mainwindow.h"
#include "typedef.h"

int main(int argc, char *argv[])
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 6, 0)
  QApplication::setAttribute(Qt::AA_EnableHighDpiScaling); // DPI support
  QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps); // DPI support
#endif
  QApplication::setAttribute(Qt::AA_SynthesizeMouseForUnhandledTouchEvents,false);
  QApplication::setAttribute(Qt::AA_SynthesizeTouchForUnhandledMouseEvents,false);

  QApplication app(argc, argv);

  //printf("Build Version: %s \n",YUVIEW_HASH);
  // check the YUVIEW_HASH against the JSON output from here:
  // https://api.github.com/repos/IENT/YUView/commits
  // if comparison mismatch, there is a new version available!

  QString versionString = QString::fromUtf8(YUVIEW_VERSION);

  QApplication::setApplicationName("YUView");
  QApplication::setApplicationVersion(versionString);
  QApplication::setOrganizationName("Institut für Nachrichtentechnik, RWTH Aachen University");
  QApplication::setOrganizationDomain("ient.rwth-aachen.de");

  MainWindow w;
  app.installEventFilter(&w);

  QStringList args = app.arguments();

  if (UPDATE_FEATURE_ENABLE && is_Q_OS_WIN && args.size() == 2 && args.last() == "updateElevated")
  {
    // The process should now be elevated and we will force an update
    w.forceUpdateElevated();
    args.removeLast();
  }
  else
    w.autoUpdateCheck();

  QStringList fileList = args.mid(1);
  if (!fileList.empty())
    w.loadFiles(fileList);

  w.show();
  return app.exec();
}
