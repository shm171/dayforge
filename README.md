# dayforge

`dayforge` 是一个 C++17 小工具，用来记每天写了什么。

它默认把记录存在当前目录的 `dayforge.tsv` 里。这个文件就是普通文本，不需要数据库，想备份也很方便。

## 能做什么

- 记一条开发记录
- 看今天写了什么
- 看最近 7 天的情况
- 打开一个简单的终端界面
- 按标签或日期查记录
- 统计标签
- 导出 Markdown 报告

## 编译

需要先装好 CMake、g++ 和 Ninja。

```powershell
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
```

程序位置：

```powershell
.\build\dayforge.exe
```

## 先试一下

添加一条记录：

```powershell
.\build\dayforge.exe add --title "写了终端界面" --tag cpp --tag ui --note "新增 dashboard 模块"
```

看今天：

```powershell
.\build\dayforge.exe today
```

看最近 7 天：

```powershell
.\build\dayforge.exe week
```

打开终端界面：

```powershell
.\build\dayforge.exe ui
```

导出 Markdown：

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

如果不想用默认的 `dayforge.tsv`，可以指定文件：

```powershell
.\build\dayforge.exe ui --file data\my-ledger.tsv
```

## 命令说明

- `add`：新增记录
- `today`：看今天的记录
- `week`：看最近 7 天
- `ui`：显示终端界面
- `list`：列出记录
- `stats`：统计标签
- `export-md`：导出 Markdown

## Windows 编译工具

推荐用 MSYS2。

```powershell
winget install MSYS2.MSYS2
```

打开 **MSYS2 UCRT64**，执行：

```bash
pacman -Syu
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-ninja
```

把这个目录加到 Windows 的 `Path`：

```text
C:\msys64\ucrt64\bin
```

重新打开 PowerShell，检查一下：

```powershell
g++ --version
cmake --version
ninja --version
```

## 后面可以写

- 交互式菜单
- 编辑和删除记录
- 连续记录天数统计
- JSON 导出
- 更好看的终端界面
