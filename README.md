# dayforge

`dayforge` 是一个用 C++17 写的本地开发日志工具。

你可以把它理解成一个放在终端里的“每日进度本”：每天写了什么、用了哪些标签、这一周有没有持续推进，都可以用它记录和查看。数据默认保存在当前目录的 `dayforge.tsv`，是普通文本文件，不需要数据库。

## 它现在能做什么

- 记录一条开发日志
- 查看今天做了什么
- 查看最近 7 天的进度
- 打开一个简陋的终端仪表盘
- 按标签、日期筛选记录
- 统计标签出现次数
- 导出 Markdown 报告

## 编译

如果你已经装好了 CMake、g++ 和 Ninja，在项目目录执行：

```powershell
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
```

编译完成后，程序在：

```powershell
.\build\dayforge.exe
```

## 快速使用

添加一条记录：

```powershell
.\build\dayforge.exe add --title "完善终端界面" --tag cpp --tag ui --note "新增了 dashboard 模块"
```

看今天的记录：

```powershell
.\build\dayforge.exe today
```

看最近 7 天：

```powershell
.\build\dayforge.exe week
```

打开终端仪表盘：

```powershell
.\build\dayforge.exe ui
```

导出 Markdown 报告：

```powershell
.\build\dayforge.exe export-md --output reports\week.md
```

## 常用命令

```powershell
.\build\dayforge.exe add --title "标题" --tag cpp --note "备注"
.\build\dayforge.exe today
.\build\dayforge.exe week
.\build\dayforge.exe ui
.\build\dayforge.exe list --tag cpp
.\build\dayforge.exe list --from 2026-05-01 --to 2026-05-14
.\build\dayforge.exe stats
.\build\dayforge.exe export-md --output reports\week.md
```

如果你想把日志文件放到别的位置，可以加 `--file`：

```powershell
.\build\dayforge.exe ui --file data\my-ledger.tsv
```

## 命令说明

- `add`：新增一条记录
- `today`：查看今天的记录和标签统计
- `week`：查看最近 7 天每天有多少条记录
- `ui`：显示一个终端仪表盘
- `list`：列出记录，可以按标签或日期过滤
- `stats`：统计标签
- `export-md`：导出 Markdown 报告

## Windows 上安装编译工具

推荐用 MSYS2 安装 g++、CMake 和 Ninja。

先安装 MSYS2：

```powershell
winget install MSYS2.MSYS2
```

然后打开开始菜单里的 **MSYS2 UCRT64**，执行：

```bash
pacman -Syu
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
```

把下面这个目录加入 Windows 的 `Path` 环境变量：

```text
C:\msys64\ucrt64\bin
```

重新打开 PowerShell，检查是否安装成功：

```powershell
g++ --version
cmake --version
ninja --version
```

## 项目方向

`dayforge` 会继续按“小步但真实”的方式扩展。后面可以继续加：

- 交互式终端菜单
- 编辑和删除记录
- 连续提交天数统计
- JSON 导出
- 更漂亮的终端界面
