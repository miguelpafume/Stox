#include "womanage.h"
#include "ui_womanage.h"

#include <QtSql>
#include <QMessageBox>

WOManage::WOManage(QWidget *parent) : QDialog(parent), ui(new Ui::WOManage) {
    ui->setupUi(this);
}

WOManage::~WOManage() {
    delete db;
    delete ui;
}

void WOManage::SetDatabase(QSqlDatabase *db) {
    this->db = db;
    UpdateTables();
}


void WOManage::UpdateTables() {
    // In progress table
    db->open();
    QSqlQueryModel *inprogress_model = new QSqlQueryModel;
    inprogress_model->setQuery("SELECT product, amount, client, company, reason, wo, ean, revenue FROM debug_workorder WHERE wo = 0");

    inprogress_model->setHeaderData(0, Qt::Horizontal, tr("PRODUTO"));
    inprogress_model->setHeaderData(1, Qt::Horizontal, tr("QTD."));
    inprogress_model->setHeaderData(2, Qt::Horizontal, tr("CLIENTE"));
    inprogress_model->setHeaderData(3, Qt::Horizontal, tr("EMPRESA"));
    inprogress_model->setHeaderData(4, Qt::Horizontal, tr("RAZÃO"));
    inprogress_model->setHeaderData(7, Qt::Horizontal, tr("FATURAMENTO"));

    ui->inprogress_table->setModel(inprogress_model);
    ui->inprogress_table->setColumnHidden(5, true);
    ui->inprogress_table->setColumnHidden(6, true);

    // concluded table
    QSqlQueryModel *concluded_model = new QSqlQueryModel;
    concluded_model->setQuery("SELECT product, amount, client, company, reason, wo, ean, revenue FROM debug_workorder WHERE wo = 1");

    concluded_model->setHeaderData(0, Qt::Horizontal, tr("PRODUTO"));
    concluded_model->setHeaderData(1, Qt::Horizontal, tr("QTD."));
    concluded_model->setHeaderData(2, Qt::Horizontal, tr("CLIENTE"));
    concluded_model->setHeaderData(3, Qt::Horizontal, tr("EMPRESA"));
    concluded_model->setHeaderData(4, Qt::Horizontal, tr("RAZÃO"));
    concluded_model->setHeaderData(7, Qt::Horizontal, tr("FATURAMENTO"));

    ui->concluded_table->setModel(concluded_model);
    ui->concluded_table->setColumnHidden(5, true);
    ui->concluded_table->setColumnHidden(6, true);


    // Canceled table
    QSqlQueryModel *canceled_model = new QSqlQueryModel;
    canceled_model->setQuery("SELECT product, amount, client, company, reason, wo, ean, revenue FROM debug_workorder WHERE wo = 2");

    canceled_model->setHeaderData(0, Qt::Horizontal, tr("PRODUTO"));
    canceled_model->setHeaderData(1, Qt::Horizontal, tr("QTD."));
    canceled_model->setHeaderData(2, Qt::Horizontal, tr("CLIENTE"));
    canceled_model->setHeaderData(3, Qt::Horizontal, tr("EMPRESA"));
    canceled_model->setHeaderData(4, Qt::Horizontal, tr("RAZÃO"));
    canceled_model->setHeaderData(7, Qt::Horizontal, tr("FATURAMENTO"));

    ui->canceled_table->setModel(canceled_model);
    ui->canceled_table->setColumnHidden(5, true);
    ui->canceled_table->setColumnHidden(6, true);

    db->close();
}

void WOManage::on_inprogress_table_doubleClicked(const QModelIndex &index) {
    QAbstractItemModel *model = ui->inprogress_table->model();
    ui->selected_product->setPlainText(model->index(index.row(), 0).data().toString());
    ui->selected_amount->setPlainText(model->index(index.row(), 1).data().toString());
    ui->selected_client->setPlainText(model->index(index.row(), 2).data().toString());
    ui->selected_company->setPlainText(model->index(index.row(), 3).data().toString());
    ui->selected_status->setPlainText(model->index(index.row(), 4).data().toString());
    ui->selected_ean->setPlainText(model->index(index.row(), 6).data().toString());
}


void WOManage::on_concluded_table_doubleClicked(const QModelIndex &index) {
    QAbstractItemModel *model = ui->concluded_table->model();
    ui->selected_product->setPlainText(model->index(index.row(), 0).data().toString());
    ui->selected_amount->setPlainText(model->index(index.row(), 1).data().toString());
    ui->selected_client->setPlainText(model->index(index.row(), 2).data().toString());
    ui->selected_company->setPlainText(model->index(index.row(), 3).data().toString());
    ui->selected_status->setPlainText(model->index(index.row(), 4).data().toString());
    ui->selected_ean->setPlainText(model->index(index.row(), 6).data().toString());
}


void WOManage::on_canceled_table_doubleClicked(const QModelIndex &index) {
    QAbstractItemModel *model = ui->canceled_table->model();
    ui->selected_product->setPlainText(model->index(index.row(), 0).data().toString());
    ui->selected_amount->setPlainText(model->index(index.row(), 1).data().toString());
    ui->selected_client->setPlainText(model->index(index.row(), 2).data().toString());
    ui->selected_company->setPlainText(model->index(index.row(), 3).data().toString());
    ui->selected_status->setPlainText(model->index(index.row(), 4).data().toString());
    ui->selected_ean->setPlainText(model->index(index.row(), 6).data().toString());
}



void WOManage::on_inprogress_button_clicked() {
    db->open();

    QSqlQuery qry;

    qry.prepare("SELECT wo FROM debug_workorder WHERE ean = ? AND amount = ? AND reason = ?");
    qry.addBindValue(ui->selected_ean->toPlainText());
    qry.addBindValue(ui->selected_amount->toPlainText().toInt());
    qry.addBindValue(ui->selected_status->toPlainText());
    qry.exec();

    qry.next();
    if (qry.value(0).toInt() == 1) {
        qry.prepare("SELECT amount FROM debug_product WHERE ean = ?");
        qry.addBindValue(ui->selected_ean->toPlainText());
        qry.exec();

        qry.next();
        int new_amount = qry.value(0).toInt();

        qry.prepare("SELECT reason FROM debug_workorder WHERE ean = ? AND amount = ? AND reason = ?");
        qry.addBindValue(ui->selected_ean->toPlainText());
        qry.addBindValue(ui->selected_amount->toPlainText().toInt());
        qry.addBindValue(ui->selected_status->toPlainText());
        qry.exec();
        qry.next();

        if (qry.value(0).toString() == "Compra") {
            new_amount -= ui->selected_amount->toPlainText().toInt();
        } else {
            new_amount += ui->selected_amount->toPlainText().toInt();
        }

        qry.prepare("UPDATE debug_product SET amount = ? WHERE ean = ?");
        qry.addBindValue(new_amount);
        qry.addBindValue(ui->selected_ean->toPlainText());
        qry.exec();
    }

    qry.prepare("UPDATE debug_workorder SET wo = 0 WHERE ean = ? AND amount = ? AND reason = ?");
    qry.addBindValue(ui->selected_ean->toPlainText());
    qry.addBindValue(ui->selected_amount->toPlainText().toInt());
    qry.addBindValue(ui->selected_status->toPlainText());
    qry.exec();

    UpdateTables();
    db->close();
}


void WOManage::on_conclude_button_clicked() {
    db->open();

    QSqlQuery qry;

    qry.prepare("SELECT wo FROM debug_workorder WHERE ean = ? AND amount = ? AND reason = ?");
    qry.addBindValue(ui->selected_ean->toPlainText());
    qry.addBindValue(ui->selected_amount->toPlainText().toInt());
    qry.addBindValue(ui->selected_status->toPlainText());
    qry.exec();

    qry.next();
    if (qry.value(0).toInt() == 1) {
        db->close();
        return;
    }

    qry.prepare("UPDATE debug_workorder SET wo = 1 WHERE ean = ? AND amount = ? AND reason = ?");
    qry.addBindValue(ui->selected_ean->toPlainText());
    qry.addBindValue(ui->selected_amount->toPlainText().toInt());
    qry.addBindValue(ui->selected_status->toPlainText());
    qry.exec();

    qry.prepare("SELECT amount FROM debug_product WHERE ean = ?");
    qry.addBindValue(ui->selected_ean->toPlainText());
    qry.exec();

    qry.next();
    int new_amount = qry.value(0).toInt();

    qry.prepare("SELECT reason FROM debug_workorder WHERE ean = ? AND amount = ? AND reason = ?");
    qry.addBindValue(ui->selected_ean->toPlainText());
    qry.addBindValue(ui->selected_amount->toPlainText().toInt());
    qry.addBindValue(ui->selected_status->toPlainText());
    qry.exec();
    qry.next();

    if (qry.value(0).toString() == "Compra") {
        new_amount += ui->selected_amount->toPlainText().toInt();
    } else {
        new_amount -= ui->selected_amount->toPlainText().toInt();
    }

    qry.prepare("UPDATE debug_product SET amount = ? WHERE ean = ?");
    qry.addBindValue(new_amount);
    qry.addBindValue(ui->selected_ean->toPlainText());
    qry.exec();

    UpdateTables();
    db->close();
}


void WOManage::on_cancel_button_clicked() {
    db->open();

    QSqlQuery qry;

    qry.prepare("SELECT wo FROM debug_workorder WHERE ean = ? AND amount = ? AND reason = ?");
    qry.addBindValue(ui->selected_ean->toPlainText());
    qry.addBindValue(ui->selected_amount->toPlainText().toInt());
    qry.addBindValue(ui->selected_status->toPlainText());
    qry.exec();

    qry.next();
    if (qry.value(0).toInt() == 1) {
        qry.prepare("SELECT amount FROM debug_product WHERE ean = ?");
        qry.addBindValue(ui->selected_ean->toPlainText());
        qry.exec();

        qry.next();
        int new_amount = qry.value(0).toInt();

        qry.prepare("SELECT reason FROM debug_workorder WHERE ean = ? AND amount = ? AND reason = ?");
        qry.addBindValue(ui->selected_ean->toPlainText());
        qry.addBindValue(ui->selected_amount->toPlainText().toInt());
        qry.addBindValue(ui->selected_status->toPlainText());
        qry.exec();
        qry.next();

        if (qry.value(0).toString() == "Compra") {
            new_amount -= ui->selected_amount->toPlainText().toInt();
        } else {
            new_amount += ui->selected_amount->toPlainText().toInt();
        }

        qry.prepare("UPDATE debug_product SET amount = ? WHERE ean = ?");
        qry.addBindValue(new_amount);
        qry.addBindValue(ui->selected_ean->toPlainText());
        qry.exec();
    }

    qry.prepare("UPDATE debug_workorder SET wo = 2 WHERE ean = ? AND amount = ? AND reason = ?");
    qry.addBindValue(ui->selected_ean->toPlainText());
    qry.addBindValue(ui->selected_amount->toPlainText().toInt());
    qry.addBindValue(ui->selected_status->toPlainText());
    qry.exec();

    UpdateTables();
    db->close();
}


void WOManage::on_delete_button_clicked() {
    db->open();

    QSqlQuery qry;
    qry.prepare("DELETE FROM debug_workorder WHERE ean = ? AND amount = ? AND reason = ?");
    qry.addBindValue(ui->selected_ean->toPlainText());
    qry.addBindValue(ui->selected_amount->toPlainText().toInt());
    qry.addBindValue(ui->selected_status->toPlainText());
    qry.exec();

    UpdateTables();
    db->close();
}


void WOManage::on_clear_button_clicked() {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirmação", "Você tem certeza?", QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        db->open();
        QSqlQuery qry;
        qry.prepare("DELETE FROM debug_workorder");
        qry.exec();
        UpdateTables();
        db->close();
    }
}

