#include "optionsdialog.h"
#include "mainoptionspage.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QListWidget>
#include <QStackedWidget>

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent), contents_widget(0), pages_widget(0)
{
    contents_widget = new QListWidget();
    contents_widget->setMaximumWidth(128);

    pages_widget = new QStackedWidget();
    pages_widget->setMinimumWidth(300);

    QListWidgetItem *item_main = new QListWidgetItem(tr("Main"));
    contents_widget->addItem(item_main);
    pages_widget->addWidget(new MainOptionsPage(this));

    contents_widget->setCurrentRow(0);

    QHBoxLayout *main_layout = new QHBoxLayout();
    main_layout->addWidget(contents_widget);
    main_layout->addWidget(pages_widget, 1);

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addLayout(main_layout);

    QHBoxLayout *buttons = new QHBoxLayout();
    buttons->addStretch(1);
    QPushButton *ok_button = new QPushButton(tr("OK"));
    buttons->addWidget(ok_button);
    QPushButton *cancel_button = new QPushButton(tr("Cancel"));
    buttons->addWidget(cancel_button);
    QPushButton *apply_button = new QPushButton(tr("Apply"));
    buttons->addWidget(apply_button);

    layout->addLayout(buttons);


    setLayout(layout);
    setWindowTitle(tr("Options"));


}

void OptionsDialog::changePage(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);
    if(current)
    {
        pages_widget->setCurrentIndex(contents_widget->row(current));
    }
}
