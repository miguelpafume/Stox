#pragma once

#include <QDialog>
#include <QtSql/QSqlDatabase>

namespace Ui {
    class WOManage;
}

class WOManage : public QDialog {
    Q_OBJECT

    public:
        void SetDatabase(QSqlDatabase *db);
        explicit WOManage(QWidget *parent = nullptr);
        ~WOManage();

    private slots:
        void on_inprogress_table_doubleClicked(const QModelIndex &index);

        void on_concluded_table_doubleClicked(const QModelIndex &index);

        void on_canceled_table_doubleClicked(const QModelIndex &index);

        void on_inprogress_button_clicked();

        void on_conclude_button_clicked();

        void on_cancel_button_clicked();

        void on_delete_button_clicked();

        void on_clear_button_clicked();

    private:
        Ui::WOManage *ui;
        QSqlDatabase *db;
        void UpdateTables();

};
