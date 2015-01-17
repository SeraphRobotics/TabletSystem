#include "confirm.h"
#include <stdio.h>  // printf
#include <QDebug>
#include <QMessageBox>
#include <QApplication> // for clipboard access
#include <QClipboard>



//----[  _confirm_failed_fn  ]-------------------------------------------------
void _confirm_failed_fn_fah(const char* expression,
                        const char* file,
                        unsigned int line,
                        bool* sentinel) {
  char message[4096];

  // this message prints to a console window
  sprintf(message,
          "confirm(%s)\nFile:\t%s\nLine:\t%ui\n",
          expression,
          file,
          line);
  qWarning() << "Debug Error:  " << message;

  if (*sentinel) {

    sprintf(
      message,
      "Debug Error!\r\n\r\nThis program has encountered an error:"\
      "\r\n\r\nFailed:\t%s\r\nFile:\t%s\r\nLine:\t%ui\r\n\r\n\r\n"\
      "Press Ignore to continue normally and avoid seeing this message "\
      "again.\r\n\r\nTo open a debugger and examine the problem, press Retry."\
      "Select Abort to immediately end the program.\r\n\r\nThis text will be "\
      "copied to your clipboard.",
      expression,
      file,
      line);

//    QApplication::clipboard()->setText(message);
//    QMessageBox message_box(QMessageBox::Critical,
//                            "Debug Error",
//                            message,
//                            QMessageBox::Abort|QMessageBox::Retry|QMessageBox::Ignore);
//    message_box.setWindowModality(Qt::ApplicationModal);

//    switch (message_box.exec()) {
//    default:
//    case QMessageBox::Ignore:
//      // ignore this in the future
//      *sentinel = false;
//      qWarning() << message << "; user chose to ignore";
//      break;
//    case QMessageBox::Abort:
//      // exit the application
//      qCritical() << message;
//      qFatal("confirm() failed; user chose to exit"); // this should exit...
//      exit(0); // ... but we'll just be sure
//      break;
//    case QMessageBox::Retry:
//      // continue on our way; the macro will invoke a breakpoint
//      qWarning() << message << "; user chose to breakpoint";
//      break;
//    }
  }
}

