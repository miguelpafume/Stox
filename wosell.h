#pragma once

#include <QDialog>
#include <QtSql/QSqlDatabase>

namespace Ui {
    class WOSell;
}

class WOSell : public QDialog {
    Q_OBJECT

    public:
        void SetDatabase(QSqlDatabase *db);
        explicit WOSell(QWidget *parent = nullptr);
        ~WOSell();

    private slots:
        void on_product_table_doubleClicked(const QModelIndex &index);
        void on_client_table_doubleClicked(const QModelIndex &index);

        void on_sell_cancel_clicked();
        void on_sell_confirm_clicked();

        void on_product_ean_textChanged();
        void on_product_amount_textChanged();

    private:
        Ui::WOSell *ui;
        QSqlDatabase *db;
        void ClearInputFields();
};
