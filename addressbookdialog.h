#ifndef ADDRESSBOOKDIALOG_H
#define ADDRESSBOOKDIALOG_H

#include <QDialog>

namespace Ui {
    class AddressBookDialog;
}
class AddressTableModel;

class AddressBookDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddressBookDialog(QWidget *parent = 0);
    ~AddressBookDialog();

    enum {
        SendingTab = 0,
        ReceivingTab = 1
    } Tabs;

    void setModel(AddressTableModel *model);
    void setTab(int tab);
private:
    Ui::AddressBookDialog *ui;
    AddressTableModel *model;

private slots:
    void on_newAddressButton_clicked();
    void on_editButton_clicked();
    void on_copyToClipboard_clicked();
    void on_OKButton_clicked();
};

#endif // ADDRESSBOOKDIALOG_H
