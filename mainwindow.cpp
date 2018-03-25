#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QStandardItemModel"
#include "QStandardItem"
#include "filemanager.h"
#include "fontdelegate.h"

#include <QDebug>
#include <QStringRef>
#include <QMessageBox>
#include <QFontDatabase>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QString fontPath = QCoreApplication::applicationDirPath() + "/NelusMold.otf";
    int id = QFontDatabase::addApplicationFont(fontPath); //путь к шрифту
    //QString family = QFontDatabase::applicationFontFamilies(id).at(0); //имя шрифта

    //в конструкторе вычитывает файл с данными
    //преобразует в вектор структур и сразу сортирует по русским словам
    dataRepository = new DataRepository();
    //QVector<Container> containers = this->dataRepository->getContainers();
    // Устанавливаем Контекстное Меню
    ui->tableView->setContextMenuPolicy(Qt::CustomContextMenu);
    //задаем делегат для вида и цвета шрифта
    ui->tableView->setItemDelegate(new FontDelegate());
    //это для вывода в таблицу
    showData2TableView();
    // Подключаем СЛОТ вызова контекстного меню
    connect(ui->tableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));
    //в win 10 groupbox виден,этот стиль его скрывает
    ui->groupBox->setStyleSheet("border:0");
    ui->pushButton->setEnabled(false);
    this->addedMode = true;
    //запрещает редактировать текст в ячейках таблицы
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //ширина столбца
    ui->tableView->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    QString rusWord = ui->lineEdit->text();
    QString nelusWord = ui->lineEdit_2->text();
    QString comment = ui->lineEdit_3->text();
    QString fontCode = ui->lineEdit_4->text();

    if(this->addedMode)
    {
        this->dataRepository->writeContainer(Container(rusWord, nelusWord, fontCode, comment));
    }
    else
    {
        this->addedMode = true;
        ui->pushButton->setText("Добавить");
        this->dataRepository->changeData(*this->container2Change, Container(rusWord, nelusWord, fontCode, comment));
    }
    //строки чистим после любого действия
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->lineEdit_4->setText("");
    //рисуем таблицу
    showData2TableView();
}

void MainWindow::on_lineEdit_textEdited(const QString &arg1)
{
    //QVector<Container> vec2Show = dataRepository->filtration(arg1, true);
    ui->radioButton->setChecked(true);
    //ui->radioButton_2->setChecked(false);
    showData2TableView();
}

void MainWindow::showData2TableView()
{
    //если чекбокс у русского языка,то берем для фильтрации русское слово и посылаем в фильтрацию
    bool check = ui->radioButton->isChecked();
    QString subStr = "";
    if(check)
    {
        subStr = ui->lineEdit->text();
    }
    else
    {
        subStr = ui->lineEdit_2->text();
    }
    //check совпадает с тем,по чему фильтруем
    QVector<Container> containers = this->dataRepository->filtration(subStr, check);
    if(containers.size() == 0 && ui->lineEdit->text() != "" && ui->lineEdit_2->text() != "")
    {
        ui->pushButton->setEnabled(true);
    }
    else
    {
        ui->pushButton->setEnabled(false);
    }
    QStandardItemModel *model = new QStandardItemModel;
    for(int i=0;i < containers.size(); ++i)
    {
        QString rusWord = containers[i].getRusWord();
        QString nelusWord = containers[i].getNelusWord();
        QString fontCode = containers[i].getFontCode();
        QString comment = containers[i].getComment();

        QStandardItem *item_col_01 = new QStandardItem(rusWord);
        QStandardItem *item_col_02 = new QStandardItem(nelusWord);
        QStandardItem *item_col_03 = new QStandardItem(fontCode);
        QStandardItem *item_col_04 = new QStandardItem(comment);
        //item_col_04->setEnabled(true);
        item_col_01->setToolTip(rusWord);
        item_col_02->setToolTip(nelusWord);
        item_col_03->setToolTip(fontCode);
        item_col_04->setToolTip(comment);


        model->appendRow(QList<QStandardItem*>() << item_col_01 << item_col_02 << item_col_03 << item_col_04);
    }
    ui->tableView->setModel(model);
}

void MainWindow::on_lineEdit_2_textEdited(const QString &arg1)
{
    //QVector<Container> vec2Show = dataRepository->filtration(arg1, false);
    ui->radioButton_2->setChecked(true);
    //ui->radioButton_2->setChecked(true);
    showData2TableView();
}

//void MainWindow::on_pushButton_2_clicked()
//{
//    //this->dataRepository->SetNewData("кодыва", "квертишок");
//    this->dataRepository->writeAllData();
//}

void MainWindow::slotCustomMenuRequested(QPoint pos)
{
    /* Создаем объект контекстного меню */
    QMenu * menu = new QMenu(this);
    //устанавливаем цвет бекграунда менюшки по ПКМ и цвет выделенной строки меню
    menu->setStyleSheet("QMenu{ background-color: white;} QMenu::item:selected { background-color: purple; }");
    /* Создаём действия для контекстного меню */
    QAction * editDevice = new QAction(trUtf8("Редактировать"), this);
    QAction * deleteDevice = new QAction(trUtf8("Удалить"), this);
    /* Подключаем СЛОТы обработчики для действий контекстного меню */
    connect(editDevice, SIGNAL(triggered()), this, SLOT(slotEditRecord()));     // Обработчик вызова диалога редактирования
    connect(deleteDevice, SIGNAL(triggered()), this, SLOT(slotRemoveRecord())); // Обработчик удаления записи
    /* Устанавливаем действия в меню */
    menu->addAction(editDevice);
    menu->addAction(deleteDevice);
    /* Вызываем контекстное меню */
    menu->popup(ui->tableView->viewport()->mapToGlobal(pos));
}

/* Метод для активации диалога добавления записей в режиме редактирования
 * с передачей индекса выбранной строки
 * */
void MainWindow::slotEditRecord()
{
    //меняем текст у кнопки и соответственно ее функционал
    ui->pushButton->setText("Изменить");
    //"изменить" не дизейблится
    ui->pushButton->setEnabled(true);
    this->addedMode = false;
    /* Выясняем, какая из строк была выбрана
     * */
    int row = ui->tableView->selectionModel()->currentIndex().row();
    /* Проверяем, что строка была действительно выбрана
     * */
    if(row >= 0)
    {
        //считываем строку из таблицы
        QModelIndex currentDiscount  = ui->tableView->currentIndex();
        QVariant qv = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0);
        QString rusWord = qv.toString();
        qv = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),1),0);
        QString nelusWord = qv.toString();
        qv = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),2),0);
        QString fontCode = qv.toString();
        qv = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),3),0);
        QString comment = qv.toString();
        //записываем в строки данные,которые будут меняться
        ui->lineEdit->setText(rusWord);
        ui->lineEdit_2->setText(nelusWord);
        ui->lineEdit_3->setText(comment);
        ui->lineEdit_4->setText(fontCode);
        this->container2Change = new Container(rusWord, nelusWord, fontCode, comment);
    }
}

/* Слот для удаления записи из таблицы
 * */
void MainWindow::slotRemoveRecord()
{
    /* Выясняем, какая из строк была выбрана
     * */
    int row = ui->tableView->selectionModel()->currentIndex().row();
    /* Проверяем, что строка была действительно выбрана
     * */
    if(row >= 0){
        /* Задаём вопрос, стоит ли действительно удалять запись.
         * При положительном ответе удаляем запись
         * */
        if (QMessageBox::warning(this,
                                 trUtf8("Удаление записи"),
                                 trUtf8("Вы уверены, что хотите удалить эту запись?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        {
            return;
        } else {
            //считываем строку из таблицы
            QModelIndex currentDiscount  = ui->tableView->currentIndex();
            QVariant qv = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),0),0);
            QString rusWord = qv.toString();
            qv = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),1),0);
            QString nelusWord = qv.toString();
            qv = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),2),0);
            QString fontCode = qv.toString();
            qv = ui->tableView->model()->data(ui->tableView->model()->index(currentDiscount.row(),3),0);
            QString comment = qv.toString();
            //удаляем контейнер из вектора и перезаписываем файл
            this->dataRepository->deleteContainer(Container(rusWord, nelusWord, fontCode, comment));
            //переписовываем таблицу
            showData2TableView();
            //QString a =(index.model()->data(index.model()->index(index.row(),index.column())).toString());
            /*
            //Если пользователь согласился удалить запись, удаляем ее из вектора и переписываем файл
            QVector<Container> containers = this->dataRepository->getContainers();
            //это для вывода в таблицу
            showData2TableView(containers);
            */
        }
    }
}

void MainWindow::on_radioButton_clicked(bool checked)
{
    //qDebug() << checked;
}

void MainWindow::on_radioButton_clicked()
{
    showData2TableView();
}

void MainWindow::on_radioButton_2_clicked()
{
    showData2TableView();
}
