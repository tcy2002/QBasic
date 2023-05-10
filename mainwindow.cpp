#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QString>
#include <iostream>
#include <sstream>
#include <fstream>

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    name(""),
    isRunning(false),
    ui(new Ui::MainWindow),
    program(new Program) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_cmdLineEdit_editingFinished() {
    QString cmd = ui->cmdLineEdit->text();
    ui->cmdLineEdit->clear();
    if (cmd.isEmpty())
        return;

    Tokenizer tokenizer(cmd.toStdString());

    // handle different occasions
    if (!name.empty()) {
        HANDLE(variableInput(tokenizer.tokens);)
    } else if (tokenizer.tokens[0] == "RUN") {
        HANDLE(run();)
    } else if (tokenizer.tokens[0] == "LOAD") {
        HANDLE(load();)
    } else if (tokenizer.tokens[0] == "CLEAR") {
        clear();
    } else if (tokenizer.tokens[0] == "HELP") {
        help();
    } else if (tokenizer.tokens[0] == "QUIT") {
        exit(0);
    } else if (tokenizer.tokens[0] == "LET" ||
             tokenizer.tokens[0] == "PRINT" ||
             tokenizer.tokens[0] == "INPUT") {
        HANDLE(directInput(tokenizer.tokens);)
    } else if (ExpParser::isNumber(tokenizer.tokens[0])) {
        HANDLE(lineInput(tokenizer.tokens);)
    } else {
        UPDATE_OUT("illegal command")
    }

    // to guarantee the " ? " prefix not changed
    if (!name.empty())
        ui->cmdLineEdit->setText(" ? ");
}

void MainWindow::on_cmdLineEdit_textEdited() {
    // to guarantee the " ? " prefix not changed
    if (!name.empty() && ui->cmdLineEdit->text().size() < 3)
        ui->cmdLineEdit->setText(" ? ");
}

void MainWindow::on_btnLoadCode_clicked() {
    HANDLE(load();)
}

void MainWindow::on_btnRunCode_clicked() {
    HANDLE(run();)
}

void MainWindow::on_btnClearCode_clicked() {
    clear();
}

void MainWindow::variableInput(std::vector<token> &tokens) {
    if (tokens.size() < 2)
        throw ParseException("usage: ? <int>");

    std::istringstream ist(tokens[1]);
    int n = 0;
    ist >> n;
    if (ist.fail())
        throw ParseException("usage: ? <int>");

    program->setVariable(name, n);
    if (isRunning) run();
    name.clear();
}

void MainWindow::lineInput(std::vector<token> &tokens) {
    StmtParser parser({tokens.begin() + 1, tokens.end()});
    std::istringstream ist(tokens[0]);
    int n = 0;
    ist >> n;
    if (ist.fail() || n <= 0)
        throw ParseException("illegal line number");
    program->insert(n, parser.statement);

    UPDATE_CODE
}

void MainWindow::directInput(std::vector<token> &tokens) {
    StmtParser parser(tokens);
    std::string ans;
    ProgramState state = program->step(ans, parser.statement);

    // directily input a variable
    if (state == INPUTTING) {
        name = ans;
        ui->cmdLineEdit->setText(" ? ");
        return;
    }

    // show the output on ui
    if (!ans.empty())
        UPDATE_OUT(QString::fromStdString(ans))
}

void MainWindow::load() {
    QString file = QFileDialog::getOpenFileName(this, tr("从文件导入"), QCoreApplication::applicationDirPath(), tr("Basic程序文件(*.basic)"));
    if (file.isEmpty())
        return;

    std::ifstream ifs(file.toStdString());
    if (!ifs.is_open())
        return;

    clear();
    std::string buf;
    while (std::getline(ifs, buf, '\n')) {
        if (buf.empty())
            continue;

        Tokenizer tokenizer(buf);

        // only accepts lines with a line number
        std::istringstream ist(tokenizer.tokens[0]);
        int n = 0;
        ist >> n;
        if (ist.fail() || n <= 0)
            throw ParseException("illegal line number");

        StmtParser parser({tokenizer.tokens.begin() + 1, tokenizer.tokens.end()});
        program->insert(n, parser.statement);
    }

    ifs.close();
    UPDATE_CODE
}

void MainWindow::run() {
    isRunning = true;

    std::string ans;
    ProgramState state = RUNNING;

    if (!name.empty()) {
        state = program->step(ans, true);
        name.clear();
    }

    while (state != BEGIN) {
        state = program->step(ans);

        // input a variable during runtime
        if (state == INPUTTING) {
            name = ans;
            ui->cmdLineEdit->setText(" ? ");
            UPDATE_CODE
            return;
        }

        // show the output on ui
        if (!ans.empty())
            UPDATE_OUT(QString::fromStdString(ans))
    }

    UPDATE_CODE
    if (state == BEGIN) isRunning = false;
}

void MainWindow::clear() {
    ui->CodeDisplay->clear();
    ui->treeDisplay->clear();
    ui->textBrowser->clear();
    delete program;
    program = new Program;
}

void MainWindow::help() {
    UPDATE_OUT("简介：\n"
               "编程语言BASIC（BASIC）是初学者通用符号指令代码的首字母缩写，"
               "由约翰·凯门尼（John Kemeny）和托马斯·库尔茨（Thomas Kurtz）"
               "于20世纪60年代中期在达特茅斯学院（Dartmouth College）开发。"
               "这是最早设计成易于使用和学习的语言之一。尽管BASIC作为一种教学语言"
               "已经消失殆尽，但它的思想仍然存在于微软的Visual BASIC系统中，该"
               "系统仍在广泛使用。")
}
