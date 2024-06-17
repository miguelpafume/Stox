#pragma once

#include <QDialog>
#include <QtSql/QSqlDatabase>

namespace Ui {
    class WOBuy;
}

class WOBuy : public QDialog {
    Q_OBJECT

    public:
        explicit WOBuy(QWidget *parent = nullptr);
        ~WOBuy();
        void SetDatabase(QSqlDatabase *db);

    private slots:
        void on_product_ean_textChanged();

        void on_product_amount_textChanged();

        void on_product_table_doubleClicked(const QModelIndex &index);

        void on_client_table_doubleClicked(const QModelIndex &index);

        void on_buy_cancel_clicked();

        void on_buy_confirm_clicked();

    private:
        QSqlDatabase *db;
        Ui::WOBuy *ui;
        void ClearInputFields();
};
