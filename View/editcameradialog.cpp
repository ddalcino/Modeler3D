#include "editcameradialog.h"
#include "ui_editcameradialog.h"

EditCameraDialog::EditCameraDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditCameraDialog)
{
    ui->setupUi(this);
}

EditCameraDialog::~EditCameraDialog()
{
    delete ui;
}
