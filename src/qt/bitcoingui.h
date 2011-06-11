#ifndef BITCOINGUI_H
#define BITCOINGUI_H

#include <QMainWindow>
#include <QSystemTrayIcon>

class TransactionTableModel;
class ClientModel;

QT_BEGIN_NAMESPACE
class QLabel;
class QLineEdit;
class QTableView;
class QAbstractItemModel;
class QModelIndex;
QT_END_NAMESPACE

class BitcoinGUI : public QMainWindow
{
    Q_OBJECT
public:
    explicit BitcoinGUI(QWidget *parent = 0);
    void setModel(ClientModel *model);
    
    /* Transaction table tab indices */
    enum {
        AllTransactions = 0,
        SentReceived = 1,
        Sent = 2,
        Received = 3
    } TabIndex;

protected:
    void changeEvent(QEvent *e);
    void closeEvent(QCloseEvent *event);

private:
    ClientModel *model;

    QLineEdit *address;
    QLabel *labelBalance;
    QLabel *labelConnections;
    QLabel *labelBlocks;
    QLabel *labelTransactions;

    QAction *quit;
    QAction *sendcoins;
    QAction *addressbook;
    QAction *about;
    QAction *receivingAddresses;
    QAction *options;
    QAction *openBitcoin;

    QSystemTrayIcon *trayIcon;
    QList<QTableView *> transactionViews;

    void createActions();
    QWidget *createTabs();
    void createTrayIcon();
    void setTabsModel(QAbstractItemModel *transaction_model);

public slots:
    void setBalance(qint64 balance);
    void setAddress(const QString &address);
    void setNumConnections(int count);
    void setNumBlocks(int count);
    void setNumTransactions(int count);
    void error(const QString &title, const QString &message);
    /* It is currently not possible to pass a return value to another thread through
       BlockingQueuedConnection, so use an indirected pointer.
       http://bugreports.qt.nokia.com/browse/QTBUG-10440
    */
    void askFee(qint64 nFeeRequired, bool *payFee);

private slots:
    void sendcoinsClicked();
    void addressbookClicked();
    void optionsClicked();
    void receivingAddressesClicked();
    void aboutClicked();
    void newAddressClicked();
    void copyClipboardClicked();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void transactionDetails(const QModelIndex& idx);
    void incomingTransaction(const QModelIndex & parent, int start, int end);
};

#endif
