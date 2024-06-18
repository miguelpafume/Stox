#include "stoxwindow.h"
#include "ui_stoxwindow.h"
#include "wosell.h"
#include "wobuy.h"
#include "CustomFunc.h"
#include "womanage.h"

#include <QtSql>
#include <QTableWidget>

StoxWindow::StoxWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::StoxWindow) {
    ui->setupUi(this);

    db->setHostName("127.0.0.1");
    db->setUserName("projectsql");
    db->setPassword("qwe123!@#");
    db->setPort(3306);
    db->setDatabaseName("stock");

    bool connection = db->open();

    if(connection) {
        SimpleMessageBox("Conexão bem sucedida!", 0);
        UpdateClientTable();
        UpdateProductTable();
    } else {
        SimpleMessageBox("Falha na conexão.", 1);
    }

    db->close();
}

StoxWindow::~StoxWindow() {
    db->close();
    delete db;
    delete ui;
}

// PRODUCT

void StoxWindow::on_product_insert_clicked()  {
    db->open();

    QSqlQuery qry;

    qry.prepare("SELECT * FROM debug_product WHERE ean = ?");
    qry.addBindValue(ui->product_ean->toPlainText());
    qry.exec();

    if (ui->product_name->toPlainText() == "") {
        SimpleMessageBox("Nome vazio.\nCampo não pode estar incompleto.", 1);
        db->close();
        return;
    } else if (ui->product_ean->toPlainText() == "") {
        SimpleMessageBox("EAN vazio.\nCampo não pode estar incompleto.", 1);
        db->close();
        return;
    }

    if (ui->product_ean->toPlainText().length() < 13 || qry.size() > 0) {
        SimpleMessageBox("EAN inválido.\nNúmeros insuficientes ou esse produto já está cadastrado.", 1);
        db->close();
        return;
    }

    if (ui->product_amount->toPlainText() == "") { ui->product_amount->setPlainText("0"); }
    if (ui->product_price->toPlainText() == "") { ui->product_price->setPlainText("0.00"); }
    if (ui->product_cost->toPlainText() == "") { ui->product_cost->setPlainText("0.00"); }

    qry.prepare("INSERT INTO debug_product (name, amount, ean, supplier, contact, price, cost, description)"
                "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

    qry.addBindValue(ui->product_name->toPlainText().toUpper());
    qry.addBindValue(ui->product_amount->toPlainText().toInt());
    qry.addBindValue(ui->product_ean->toPlainText());
    qry.addBindValue(ui->product_supplier->toPlainText());
    qry.addBindValue(ui->product_contact->toPlainText());
    qry.addBindValue(ui->product_price->toPlainText().toDouble());
    qry.addBindValue(ui->product_cost->toPlainText().toDouble());
    qry.addBindValue(ui->product_description->toPlainText());

    qry.exec();

    UpdateProductTable();
    db->close();
}

void StoxWindow::on_product_delete_clicked() {
    db->open();

    if (ui->product_ean->toPlainText() == "") {
        SimpleMessageBox("EAN vazio.\nCampo não pode estar incompleto.", 1);
        db->close();
        return;
    }

    QSqlQuery qry;
    qry.prepare("DELETE FROM debug_product WHERE ean = ?");

    qry.addBindValue(ui->product_ean->toPlainText());

    qry.exec();

    UpdateProductTable();
    db->close();
}

void StoxWindow::on_product_update_clicked() {
    db->open();

    if (ui->product_name->toPlainText() == "") {
        SimpleMessageBox("Nome vazio.\nCampo não pode estar incompleto.", 1);
        db->close();
        return;
    } else if (ui->product_ean->toPlainText() == "") {
        SimpleMessageBox("EAN vazio.\nCampo não pode estar incompleto.", 1);
        db->close();
        return;
    }

    if (ui->product_ean->toPlainText() == "") {
        SimpleMessageBox("EAN vazio.\nCampo não pode estar incompleto.", 1);
        db->close();
        return;
    }

    QSqlQuery qry;
    qry.prepare("UPDATE debug_product SET name = ?, amount = ?, supplier = ?, contact = ?, price = ?, cost = ?, description = ? WHERE ean = ?");

    qry.addBindValue(ui->product_name->toPlainText().toUpper());
    qry.addBindValue(ui->product_amount->toPlainText().toInt());
    qry.addBindValue(ui->product_supplier->toPlainText());
    qry.addBindValue(ui->product_contact->toPlainText());
    qry.addBindValue(ui->product_price->toPlainText().toDouble());
    qry.addBindValue(ui->product_cost->toPlainText().toDouble());
    qry.addBindValue(ui->product_description->toPlainText());
    qry.addBindValue(ui->product_ean->toPlainText());

    qry.exec();

    UpdateProductTable();
    db->close();
}

void StoxWindow::on_product_ean_textChanged() {
    if (ui->product_ean->toPlainText().contains(RE_NOT_NUMDOTMINUS)) {
        QString newStr = ui->product_ean->toPlainText();
        newStr.remove(RE_NOT_NUMDOTMINUS);
        ui->product_ean->setPlainText(newStr);

        QTextCursor cursor(ui->product_ean->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->product_ean->setTextCursor(cursor);
    }

    if (ui->product_ean->toPlainText().length() > 13) {
        QString newStr = ui->product_ean->toPlainText();
        newStr.chop(ui->product_ean->toPlainText().length() - 13);
        ui->product_ean->setPlainText(newStr);

        QTextCursor cursor(ui->product_ean->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->product_ean->setTextCursor(cursor);
    }
}

void StoxWindow::on_product_amount_textChanged() {
    if (ui->product_amount->toPlainText().contains(RE_NOT_NUMDOT)) {
        QString newStr = ui->product_amount->toPlainText();
        newStr.remove(RE_NOT_NUMDOT);
        ui->product_amount->setPlainText(newStr);

        QTextCursor cursor(ui->product_amount->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->product_amount->setTextCursor(cursor);
    }
}

void StoxWindow::on_product_price_textChanged() {
    if (ui->product_price->toPlainText().contains(RE_NOT_NUM) || ui->product_price->toPlainText().contains(",")) {
        QString newStr = ui->product_price->toPlainText();
        newStr.remove(RE_NOT_NUM);
        newStr.replace(",", ".");
        ui->product_price->setPlainText(newStr);

        QTextCursor cursor(ui->product_price->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->product_price->setTextCursor(cursor);
    }
}

void StoxWindow::on_product_cost_textChanged() {
    if (ui->product_cost->toPlainText().contains(RE_NOT_NUM) || ui->product_cost->toPlainText().contains(",")) {
        QString newStr = ui->product_cost->toPlainText();
        newStr.remove(RE_NOT_NUM);
        newStr.replace(",", ".");
        ui->product_cost->setPlainText(newStr);

        QTextCursor cursor(ui->product_cost->textCursor());
        cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
        ui->product_cost->setTextCursor(cursor);
    }
}

void StoxWindow::on_product_table_doubleClicked(const QModelIndex &index) {
    QAbstractItemModel *model = ui->product_table->model();
    ui->product_name->setPlainText(model->index(index.row(), 0).data().toString());
    ui->product_amount->setPlainText(model->index(index.row(), 1).data().toString());
    ui->product_ean->setPlainText(model->index(index.row(), 2).data().toString());
    ui->product_supplier->setPlainText(model->index(index.row(), 3).data().toString());
    ui->product_contact->setPlainText(model->index(index.row(), 4).data().toString());
    ui->product_price->setPlainText(model->index(index.row(), 5).data().toString());
    ui->product_cost->setPlainText(model->index(index.row(), 6).data().toString());
    ui->product_description->setPlainText(model->index(index.row(), 7).data().toString());
}

void StoxWindow::UpdateProductTable() {
    db->open();

    // Product table
    QSqlQueryModel *product_model = new QSqlQueryModel;
    product_model->setQuery("select * from debug_product");

    product_model->setHeaderData(0, Qt::Horizontal, tr("NOME"));
    product_model->setHeaderData(1, Qt::Horizontal, tr("QTD."));
    product_model->setHeaderData(2, Qt::Horizontal, tr("EAN"));
    product_model->setHeaderData(3, Qt::Horizontal, tr("EMPRESA"));
    product_model->setHeaderData(4, Qt::Horizontal, tr("CONTATO"));
    product_model->setHeaderData(5, Qt::Horizontal, tr("PREÇO"));
    product_model->setHeaderData(6, Qt::Horizontal, tr("CUSTO"));
    product_model->setHeaderData(7, Qt::Horizontal, tr("DESCRIÇÃO"));
    ui->product_table->setModel(product_model);

    db->close();
}

// CLIENT

void StoxWindow::on_client_insert_clicked() {
    db->open();

    QSqlQuery qry;

    qry.prepare("SELECT * FROM debug_client WHERE name = ? AND company = ?");
    qry.addBindValue(ui->client_name->toPlainText());
    qry.addBindValue(ui->client_company->toPlainText());
    qry.exec();

    if (qry.size() > 0) {
        SimpleMessageBox("Cliente e empresa já estão cadastrados.", 0);
        db->close();
        return;
    }

    if (ui->client_name->toPlainText() == "") {
        SimpleMessageBox("Nome vazio.\nCampo não pode estar incompleto.", 1);
        db->close();
        return;
    }

    qry.prepare("INSERT INTO debug_client (name, company, contact, address)"
                "VALUES (?, ?, ?, ?)");

    qry.addBindValue(ui->client_name->toPlainText().toUpper());
    qry.addBindValue(ui->client_company->toPlainText());
    qry.addBindValue(ui->client_contact->toPlainText());
    qry.addBindValue(ui->client_address->toPlainText());

    qry.exec();

    UpdateClientTable();
    db->close();
}

void StoxWindow::on_client_delete_clicked() {
    db->open();

    if (ui->client_name->toPlainText() == "") {
        SimpleMessageBox("Nome vazio.\nCampo não pode estar incompleto.", 1);
        db->close();
        return;
    }

    QSqlQuery qry;
    qry.prepare("DELETE FROM debug_client WHERE name = ? AND company = ?");

    qry.addBindValue(ui->client_name->toPlainText());
    qry.addBindValue(ui->client_company->toPlainText());

    qry.exec();

    UpdateClientTable();
    db->close();
}

void StoxWindow::on_client_update_clicked() {
    db->open();

    if (ui->client_name->toPlainText() == "") {
        SimpleMessageBox("Nome vazio.\nCampo não pode estar incompleto.", 1);
        db->close();
        return;
    }

    QSqlQuery qry;

    qry.prepare("SELECT * FROM debug_client WHERE name = ? AND company = ?");
    qry.addBindValue(ui->client_name->toPlainText());
    qry.addBindValue(ui->client_company->toPlainText());
    qry.exec();

    qry.next();
    if (qry.size() == 0) {
        SimpleMessageBox("Nome e Empresa não podem ser alterados.", 1);
        db->close();
        return;
    }

    qry.prepare("UPDATE debug_client SET contact = ?, address = ? WHERE name = ? AND company = ?");

    qry.addBindValue(ui->client_contact->toPlainText());
    qry.addBindValue(ui->client_address->toPlainText());
    qry.addBindValue(ui->client_name->toPlainText());
    qry.addBindValue(ui->client_company->toPlainText());

    qry.exec();

    UpdateClientTable();
    db->close();
}

void StoxWindow::on_client_table_doubleClicked(const QModelIndex &index) {
    QAbstractItemModel *model = ui->client_table->model();
    ui->client_name->setPlainText(model->index(index.row(), 0).data().toString());
    ui->client_company->setPlainText(model->index(index.row(), 1).data().toString());
    ui->client_contact->setPlainText(model->index(index.row(), 2).data().toString());
    ui->client_address->setPlainText(model->index(index.row(), 3).data().toString());
}

void StoxWindow::UpdateClientTable() {
    db->open();

    // Client Table
    QSqlQueryModel *client_model = new QSqlQueryModel;
    client_model->setQuery("select * from debug_client");

    client_model->setHeaderData(0, Qt::Horizontal, tr("NOME"));
    client_model->setHeaderData(1, Qt::Horizontal, tr("EMPRESA"));
    client_model->setHeaderData(2, Qt::Horizontal, tr("CONTATO"));
    client_model->setHeaderData(3, Qt::Horizontal, tr("ENDEREÇO"));
    ui->client_table->setModel(client_model);

    db->close();
}

// WORK ORDER

void StoxWindow::on_wo_sell_triggered() {
    WOSell *SellWindow = new WOSell;
    SellWindow->SetDatabase(this->db);
    SellWindow->setModal(true);
    SellWindow->exec();
    UpdateProductTable();
}

void StoxWindow::on_wo_buy_triggered() {
    WOBuy *BuyWindow = new WOBuy;
    BuyWindow->SetDatabase(this->db);
    BuyWindow->setModal(true);
    BuyWindow->exec();
    UpdateProductTable();
}


void StoxWindow::on_wo_manage_triggered() {
    WOManage *ManageWindow = new WOManage;
    ManageWindow->SetDatabase(this->db);
    ManageWindow->setModal(true);
    ManageWindow->exec();
    UpdateProductTable();
}


void StoxWindow::on_product_clean_clicked() {
    ui->product_name->setPlainText("");
    ui->product_amount->setPlainText("");
    ui->product_ean->setPlainText("");
    ui->product_supplier->setPlainText("");
    ui->product_contact->setPlainText("");
    ui->product_price->setPlainText("");
    ui->product_cost->setPlainText("");
    ui->product_description->setPlainText("");
}


void StoxWindow::on_client_clean_clicked() {
    ui->client_name->setPlainText("");
    ui->client_company->setPlainText("");
    ui->client_contact->setPlainText("");
    ui->client_address->setPlainText("");
}

