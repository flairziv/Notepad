#include "mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QFile>
#include <QFontDialog>
#include <QInputDialog>
#include <QTextCursor>
#include <QTextDocument>
#include <QCloseEvent>
#include <QFileInfo>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QStatusBar>
#include <QToolBar>
#include <QApplication>
#include <QLabel>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , textEdit(nullptr)
    , currentFile("")
    , isModified(false)
{
    createUI();
    createActions();
    createMenus();
    createToolBar();
    createStatusBar();

    // 设置窗口属性
    resize(800, 600);
    setWindowTitle(tr("简易记事本"));

    // 设置窗口图标
    setWindowIcon(QIcon(":/icon"));

    // 设置默认字体
    QFont font("Consolas", 11);
    textEdit->setFont(font);

    // 初始化为新文件
    textEdit->clear();
    updateTitle();
}

MainWindow::~MainWindow()
{
    // Qt 的父子机制会自动删除子对象
}

// ==================== UI 创建 ====================

void MainWindow::createUI()
{
    // 创建中心文本编辑器
    textEdit = new QTextEdit(this);
    textEdit->setLineWrapMode(QTextEdit::WidgetWidth);  // 默认自动换行

    // 连接文本变化信号
    connect(textEdit, &QTextEdit::textChanged, this, &MainWindow::textChanged);

    // 设置为中心部件
    setCentralWidget(textEdit);
}

void MainWindow::createActions()
{
    // 这里可以创建 QAction 对象，但我们直接在菜单中创建
}

void MainWindow::createMenus()
{
    // 创建菜单栏
    QMenuBar *menuBar = new QMenuBar(this);
    setMenuBar(menuBar);

    // ========== 文件菜单 ==========
    QMenu *fileMenu = menuBar->addMenu(tr("文件(&F)"));

    QAction *newAction = fileMenu->addAction(tr("新建(&N)"));
    newAction->setShortcut(QKeySequence::New);  // ← 设置了 Ctrl+N
    newAction->setStatusTip(tr("创建新文件"));
    // &QAction::triggered当用户单击菜单选项、工具栏按钮或按下作的快捷键组合时，或者当调用 trigger（）时发出此信号
    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);

    QAction *openAction = fileMenu->addAction(tr("打开(&O)..."));
    openAction->setShortcut(QKeySequence::Open);  // ← 设置了 Ctrl+O
    openAction->setStatusTip(tr("打开已有文件"));
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

    QAction *saveAction = fileMenu->addAction(tr("保存(&S)"));
    saveAction->setShortcut(QKeySequence::Save);  // ← 设置了 Ctrl+S
    saveAction->setStatusTip(tr("保存文件"));
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

    QAction *saveAsAction = fileMenu->addAction(tr("另存为(&A)..."));
    saveAsAction->setShortcut(QKeySequence::SaveAs);  // ← 设置了 Ctrl+Shift+S
    saveAsAction->setStatusTip(tr("另存为新文件"));
    connect(saveAsAction, &QAction::triggered, this, &MainWindow::saveFileAs);

    fileMenu->addSeparator();  // <- 设置分割线，在文件这个Action中

    QAction *exitAction = fileMenu->addAction(tr("退出(&X)"));
    exitAction->setShortcut(QKeySequence::Quit);  // ← 设置了 Ctrl+Q
    exitAction->setStatusTip(tr("退出程序"));
    connect(exitAction, &QAction::triggered, this, &MainWindow::exitApp);

    // ========== 编辑菜单 ==========
    QMenu *editMenu = menuBar->addMenu(tr("编辑(&E)"));

    QAction *undoAction = editMenu->addAction(tr("撤销(&U)"));
    undoAction->setShortcut(QKeySequence::Undo);
    undoAction->setStatusTip(tr("撤销上一步操作"));
    connect(undoAction, &QAction::triggered, this, &MainWindow::undo);

    QAction *redoAction = editMenu->addAction(tr("重做(&R)"));
    redoAction->setShortcut(QKeySequence::Redo);
    redoAction->setStatusTip(tr("重做"));
    connect(redoAction, &QAction::triggered, this, &MainWindow::redo);

    editMenu->addSeparator();

    QAction *cutAction = editMenu->addAction(tr("剪切(&T)"));
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("剪切选中的文本"));
    connect(cutAction, &QAction::triggered, this, &MainWindow::cut);

    QAction *copyAction = editMenu->addAction(tr("复制(&C)"));
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("复制选中的文本"));
    connect(copyAction, &QAction::triggered, this, &MainWindow::copy);

    QAction *pasteAction = editMenu->addAction(tr("粘贴(&P)"));
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("粘贴文本"));
    connect(pasteAction, &QAction::triggered, this, &MainWindow::paste);

    editMenu->addSeparator();

    QAction *findAction = editMenu->addAction(tr("查找(&F)..."));
    findAction->setShortcut(QKeySequence::Find);
    findAction->setStatusTip(tr("查找文本"));
    connect(findAction, &QAction::triggered, this, &MainWindow::find);

    QAction *replaceAction = editMenu->addAction(tr("替换(&H)..."));
    replaceAction->setShortcut(QKeySequence::Replace);
    replaceAction->setStatusTip(tr("替换文本"));
    connect(replaceAction, &QAction::triggered, this, &MainWindow::replace);

    editMenu->addSeparator();

    QAction *selectAllAction = editMenu->addAction(tr("全选(&A)"));
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    selectAllAction->setStatusTip(tr("选择全部文本"));
    connect(selectAllAction, &QAction::triggered, this, &MainWindow::selectAll);

    // ========== 格式菜单 ==========
    QMenu *formatMenu = menuBar->addMenu(tr("格式(&O)"));

    QAction *wordWrapAction = formatMenu->addAction(tr("自动换行(&W)"));
    // 点击后会在选中 ✓ 和 未选中 之间切换
    wordWrapAction->setCheckable(true);  // 使 Action 可以被选中/取消选中
    wordWrapAction->setChecked(true);  // 设置初始选中状态
    wordWrapAction->setStatusTip(tr("切换自动换行"));
    connect(wordWrapAction, &QAction::triggered, this, &MainWindow::toggleWordWrap);

    QAction *fontAction = formatMenu->addAction(tr("字体(&F)..."));
    fontAction->setStatusTip(tr("设置字体"));
    connect(fontAction, &QAction::triggered, this, &MainWindow::selectFont);

    // ========== 帮助菜单 ==========
    QMenu *helpMenu = menuBar->addMenu(tr("帮助(&H)"));

    QAction *aboutAction = helpMenu->addAction(tr("关于(&A)"));
    aboutAction->setStatusTip(tr("关于本程序"));
    connect(aboutAction, &QAction::triggered, this, &MainWindow::about);
}

void MainWindow::createToolBar()
{
    QToolBar *toolBar = addToolBar(tr("工具栏"));
    toolBar->setMovable(false);  // 禁止移动

    // 添加工具栏按钮
    QAction *newAction = toolBar->addAction(tr("新建"));
    newAction->setStatusTip(tr("新建文件"));
    connect(newAction, &QAction::triggered, this, &MainWindow::newFile);

    QAction *openAction = toolBar->addAction(tr("打开"));
    openAction->setStatusTip(tr("打开文件"));
    connect(openAction, &QAction::triggered, this, &MainWindow::openFile);

    QAction *saveAction = toolBar->addAction(tr("保存"));
    saveAction->setStatusTip(tr("保存文件"));
    connect(saveAction, &QAction::triggered, this, &MainWindow::saveFile);

    toolBar->addSeparator();

    QAction *cutAction = toolBar->addAction(tr("剪切"));
    connect(cutAction, &QAction::triggered, this, &MainWindow::cut);

    QAction *copyAction = toolBar->addAction(tr("复制"));
    connect(copyAction, &QAction::triggered, this, &MainWindow::copy);

    QAction *pasteAction = toolBar->addAction(tr("粘贴"));
    connect(pasteAction, &QAction::triggered, this, &MainWindow::paste);

    toolBar->addSeparator();

    QAction *undoAction = toolBar->addAction(tr("撤销"));
    connect(undoAction, &QAction::triggered, this, &MainWindow::undo);

    QAction *redoAction = toolBar->addAction(tr("重做"));
    connect(redoAction, &QAction::triggered, this, &MainWindow::redo);
}

void MainWindow::createStatusBar()
{
    QStatusBar *statusBar = new QStatusBar(this);
    setStatusBar(statusBar);
    statusBar->showMessage(tr("就绪"));
}

// ==================== 文件操作 ====================

void MainWindow::newFile()
{
    if (maybeSave()) {
        textEdit->clear();
        currentFile.clear();
        isModified = false;
        updateTitle();
        statusBar()->showMessage(tr("新建文件"), 2000);
    }
}

void MainWindow::openFile()
{
    if (!maybeSave()) {
        return;
    }

    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("打开文件"),
        "",
        tr("文本文件 (*.txt);;所有文件 (*.*)")
        );

    if (!fileName.isEmpty()) {
        load(fileName);
    }
}

void MainWindow::saveFile()
{
    if (currentFile.isEmpty()) {
        saveFileAs();
    } else {
        save(currentFile);
    }
}

void MainWindow::saveFileAs()
{
    QString fileName = QFileDialog::getSaveFileName(
        this,
        tr("保存文件"),
        "",
        tr("文本文件 (*.txt);;所有文件 (*.*)")
        );

    if (!fileName.isEmpty()) {
        save(fileName);
    }
}

void MainWindow::exitApp()
{
    if (maybeSave()) {
        QApplication::quit();
    }
}

// ==================== 编辑操作 ====================

void MainWindow::cut()
{
    textEdit->cut();
    statusBar()->showMessage(tr("已剪切"), 2000);
}

void MainWindow::copy()
{
    textEdit->copy();
    statusBar()->showMessage(tr("已复制"), 2000);
}

void MainWindow::paste()
{
    textEdit->paste();
    statusBar()->showMessage(tr("已粘贴"), 2000);
}

void MainWindow::undo()
{
    textEdit->undo();
    statusBar()->showMessage(tr("已撤销"), 2000);
}

void MainWindow::redo()
{
    textEdit->redo();
    statusBar()->showMessage(tr("已重做"), 2000);
}

void MainWindow::selectAll()
{
    textEdit->selectAll();
    statusBar()->showMessage(tr("已全选"), 2000);
}

void MainWindow::find()
{
    bool ok;
    QString searchText = QInputDialog::getText(
        this,
        tr("查找"),
        tr("查找内容:"),
        QLineEdit::Normal,
        "",
        &ok
        );

    if (ok && !searchText.isEmpty()) {
        QTextDocument *document = textEdit->document();
        QTextCursor cursor = document->find(searchText, textEdit->textCursor());

        if (!cursor.isNull()) {
            textEdit->setTextCursor(cursor);
            statusBar()->showMessage(tr("找到: %1").arg(searchText), 2000);
        } else {
            QMessageBox::information(this, tr("查找"), tr("找不到 \"%1\"").arg(searchText));
            statusBar()->showMessage(tr("未找到"), 2000);
        }
    }
}

void MainWindow::replace()
{
    bool ok;
    QString findText = QInputDialog::getText(
        this,
        tr("替换"),
        tr("查找内容:"),
        QLineEdit::Normal,
        "",
        &ok
        );

    if (!ok || findText.isEmpty()) {
        return;
    }

    QString replaceText = QInputDialog::getText(
        this,
        tr("替换"),
        tr("替换为:"),
        QLineEdit::Normal,
        "",
        &ok
        );

    if (ok) {
        QString text = textEdit->toPlainText();
        int count = text.count(findText);
        text.replace(findText, replaceText);
        textEdit->setPlainText(text);

        QMessageBox::information(
            this,
            tr("替换"),
            tr("已替换 %1 处").arg(count)
            );
        statusBar()->showMessage(tr("替换完成"), 2000);
    }
}

// ==================== 格式操作 ====================

void MainWindow::selectFont()
{
    bool ok;
    QFont font = QFontDialog::getFont(&ok, textEdit->font(), this, tr("选择字体"));

    if (ok) {
        textEdit->setFont(font);
        statusBar()->showMessage(tr("字体已更改"), 2000);
    }
}

void MainWindow::toggleWordWrap(bool checked)
{
    if (checked) {
        textEdit->setLineWrapMode(QTextEdit::WidgetWidth);
        statusBar()->showMessage(tr("已启用自动换行"), 2000);
    } else {
        textEdit->setLineWrapMode(QTextEdit::NoWrap);
        statusBar()->showMessage(tr("已禁用自动换行"), 2000);
    }
}

// ==================== 帮助操作 ====================

void MainWindow::about()
{
    QMessageBox::about(
        this,
        tr("关于简易记事本"),
        tr("<h2>简易记事本 v1.0</h2>"
           "<p><b>纯代码实现版本</b></p>"
           "<p>一个使用 Qt 开发的简单文本编辑器</p>"
           "<p>作者: <b>JulyJolly </b></p>"
           "<p>日期: <b>2025-11-15</b></p>"
           "<hr>"
           "<p><b>功能特性:</b></p>"
           "<ul style='margin-left: 20px;'>"
           "<li>✅ 新建、打开、保存文件</li>"
           "<li>✅ 剪切、复制、粘贴</li>"
           "<li>✅ 撤销、重做</li>"
           "<li>✅ 查找和替换</li>"
           "<li>✅ 字体设置</li>"
           "<li>✅ 自动换行</li>"
           "<li>✅ 完整的菜单和工具栏</li>"
           "<li>✅ 状态栏提示</li>"
           "<li>✅ 文件修改提示</li>"
           "</ul>"
           "<hr>"
           "<p style='color: gray;'><i>完全使用 C++ 代码实现，无 UI 文件</i></p>")
        );
}

// ==================== 辅助函数 ====================

void MainWindow::textChanged()
{
    isModified = true;
    updateTitle();
}

bool MainWindow::save(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(
            this,
            tr("保存文件"),
            tr("无法保存文件 %1:\n%2")
                .arg(QFileInfo(fileName).fileName())
                .arg(file.errorString())
            );
        return false;
    }

    QTextStream out(&file);

    // 设置编码为 UTF-8
    out.setCodec("UTF-8");

    out << textEdit->toPlainText();
    file.close();

    setCurrentFile(fileName);
    isModified = false;
    updateTitle();

    statusBar()->showMessage(tr("文件已保存: %1").arg(QFileInfo(fileName).fileName()), 3000);
    return true;
}

bool MainWindow::load(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(
            this,
            tr("打开文件"),
            tr("无法打开文件 %1:\n%2")
                .arg(QFileInfo(fileName).fileName())
                .arg(file.errorString())
            );
        return false;
    }

    QTextStream in(&file);

    // 设置编码为 UTF-8
    in.setCodec("UTF-8");

    textEdit->setPlainText(in.readAll());
    file.close();

    setCurrentFile(fileName);
    isModified = false;
    updateTitle();

    statusBar()->showMessage(tr("文件已加载: %1").arg(QFileInfo(fileName).fileName()), 3000);
    return true;
}

bool MainWindow::maybeSave()
{
    if (!isModified) {
        return true;
    }

    QMessageBox::StandardButton ret = QMessageBox::warning(
        this,
        tr("简易记事本"),
        tr("文档 '%1' 已被修改。\n\n是否保存更改？")
            .arg(currentFile.isEmpty() ? "未命名" : QFileInfo(currentFile).fileName()),
        QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
        QMessageBox::Save
        );

    if (ret == QMessageBox::Save) {
        saveFile();
        return true;
    } else if (ret == QMessageBox::Cancel) {
        return false;
    }

    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    currentFile = fileName;
}

void MainWindow::updateTitle()
{
    QString shownName = currentFile.isEmpty() ? "未命名" : QFileInfo(currentFile).fileName();
    QString modified = isModified ? " *" : "";
    setWindowTitle(tr("%1%2 - 简易记事本").arg(shownName).arg(modified));
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()) {
        event->accept();
    } else {
        event->ignore();
    }
}
