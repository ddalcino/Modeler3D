#ifndef EDITCAMERADIALOG_H
#define EDITCAMERADIALOG_H

#include <QDialog>



namespace Ui {
class EditCameraDialog;
}

class EditCameraDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditCameraDialog(QWidget *parent = 0);
    ~EditCameraDialog();

private:
    Ui::EditCameraDialog *ui;
};

#endif // EDITCAMERADIALOG_H
