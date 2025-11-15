# 简易记事本 (Simple Notepad)

一个使用Qt框架开发的简单文本编辑器，完全使用C++代码实现（无UI设计文件）。

![Notepad Icon](images/icon.png)

## 📋 项目简介

这是一个功能完整的记事本应用程序，提供了基本的文本编辑功能。项目采用纯代码编程方式，没有使用Qt Designer的UI文件，所有界面元素都通过C++代码动态创建。

## ✨ 功能特性

### 📁 文件操作
- ✅ 新建文件 (Ctrl+N)
- ✅ 打开文件 (Ctrl+O)
- ✅ 保存文件 (Ctrl+S)
- ✅ 另存为 (Ctrl+Shift+S)
- ✅ 退出程序 (Ctrl+Q)

### ✏️ 编辑功能
- ✅ 撤销/重做 (Ctrl+Z/Ctrl+Y)
- ✅ 剪切/复制/粘贴 (Ctrl+X/Ctrl+C/Ctrl+V)
- ✅ 全选 (Ctrl+A)
- ✅ 查找文本 (Ctrl+F)
- ✅ 替换文本 (Ctrl+H)

### 🎨 格式设置
- ✅ 自动换行开关
- ✅ 字体选择器
- ✅ 默认Consolas字体

### 🔧 用户体验
- ✅ 完整的菜单栏
- ✅ 工具栏快捷操作
- ✅ 状态栏信息提示
- ✅ 文件修改状态指示
- ✅ 关闭前保存提醒
- ✅ UTF-8编码支持

## 🛠️ 技术栈

- **开发语言**: C++17
- **GUI框架**: Qt 5.15.0
- **编译器**: MinGW 64-bit
- **构建系统**: qmake

## 📦 项目结构

```
Notepad/
├── main.cpp           # 程序入口点
├── mainwindow.h       # 主窗口头文件
├── mainwindow.cpp     # 主窗口实现
├── Notepad.pro        # Qt项目配置文件
├── res.qrc           # 资源文件配置
├── images/           # 图标资源
│   ├── icon.png      # 应用程序图标
│   └── bag.png       # 其他图标
└── build/            # 构建输出目录
```

## 🚀 快速开始

### 环境要求

- Qt 5.15.0 或更高版本
- 支持C++17的编译器
- qmake构建工具

### 编译运行

1. **克隆项目**
   ```bash
   git clone <your-repo-url>
   cd Notepad
   ```

2. **使用Qt Creator**
   - 打开 [`Notepad.pro`](Notepad.pro)
   - 点击"构建"按钮
   - 运行项目

3. **命令行编译**
   ```bash
   qmake Notepad.pro
   make
   ```

### 快捷键说明

| 快捷键 | 功能 |
|--------|------|
| Ctrl+N | 新建文件 |
| Ctrl+O | 打开文件 |
| Ctrl+S | 保存文件 |
| Ctrl+Shift+S | 另存为 |
| Ctrl+Q | 退出程序 |
| Ctrl+Z | 撤销 |
| Ctrl+Y | 重做 |
| Ctrl+X | 剪切 |
| Ctrl+C | 复制 |
| Ctrl+V | 粘贴 |
| Ctrl+A | 全选 |
| Ctrl+F | 查找 |
| Ctrl+H | 替换 |

## 🏗️ 代码架构

### 主要类说明

- [`MainWindow`](mainwindow.h): 主窗口类，继承自QMainWindow
- 核心组件：[`QTextEdit`](mainwindow.cpp) 作为文本编辑器

### 关键方法

#### UI创建
- [`createUI()`](mainwindow.cpp): 创建用户界面
- [`createMenus()`](mainwindow.cpp): 创建菜单栏
- [`createToolBar()`](mainwindow.cpp): 创建工具栏
- [`createStatusBar()`](mainwindow.cpp): 创建状态栏

#### 文件操作
- [`save()`](mainwindow.cpp): 保存文件到指定路径
- [`load()`](mainwindow.cpp): 从指定路径加载文件
- [`maybeSave()`](mainwindow.cpp): 检查是否需要保存修改

## 📝 开发说明

### 资源文件

项目使用 [`res.qrc`](res.qrc) 管理资源文件：
- 应用程序图标：`:/icon` (对应 `images/icon.png`)
- 其他图标：`:/bag` (对应 `images/bag.png`)

### 字符编码

项目支持UTF-8编码，确保中文等多字节字符正常显示和保存。

### 窗口关闭处理

重写了 [`closeEvent()`](mainwindow.cpp) 方法，确保在关闭程序前提醒用户保存未保存的修改。

## 🐛 已知问题

目前项目运行稳定，暂无已知重要问题。

## 🔮 未来计划

- [ ] 添加语法高亮功能
- [ ] 支持多标签页编辑
- [ ] 添加行号显示
- [ ] 支持正则表达式查找
- [ ] 添加主题切换功能
- [ ] 支持插件系统

## 👨‍💻 开发者信息

- **作者**: flairziv (JulyJolly)
- **版本**: 1.0
- **开发日期**: 2025-11-15
- **联系方式**: flairziv.com


⭐ 如果这个项目对你有帮助，请给一个Star！
