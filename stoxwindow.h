#pragma once

#include <QMainWindow>
#include <QtSql/QSqlDatabase>
#include "wosell.h"

QT_BEGIN_NAMESPACE
namespace Ui {
    class StoxWindow;
}
QT_END_NAMESPACE

class StoxWindow : public QMainWindow {
    Q_OBJECT

    public:
        StoxWindow(QWidget *parent = nullptr);
        ~StoxWindow();
        QSqlDatabase* GetDatabase() { return db; };

        void UpdateClientTable();
        void UpdateProductTable();

    private slots:
        void on_product_insert_clicked();
        void on_product_delete_clicked();
        void on_product_update_clicked();

        void on_product_ean_textChanged();
        void on_product_amount_textChanged();
        void on_product_price_textChanged();
        void on_product_cost_textChanged();

        void on_product_table_doubleClicked(const QModelIndex &index);

        void on_client_insert_clicked();
        void on_client_delete_clicked();
        void on_client_update_clicked();

        void on_client_table_doubleClicked(const QModelIndex &index);

        void on_wo_sell_triggered();

        void on_wo_buy_triggered();

        void on_wo_manage_triggered();

    private:
        Ui::StoxWindow *ui;
        QSqlDatabase *db = new QSqlDatabase(QSqlDatabase::addDatabase("QMYSQL"));
};
