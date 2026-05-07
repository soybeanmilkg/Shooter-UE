@echo off
setlocal enabledelayedexpansion

echo 开始清理中间文件...

REM ===== 1. 删除项目根目录的 Intermediate / Binaries / DerivedDataCache =====
for %%D in (Intermediate Binaries DerivedDataCache) do (
    if exist "%%D" (
        echo 正在删除 %%D...
        rmdir /S /Q "%%D"
    )
)

REM ===== 2. 递归查找所有 Plugins 目录（任意嵌套层级），清理其下的中间目录 =====
REM 使用 dir /s 找出所有包含 \Plugins\ 的目录，然后检查其下是否有 Intermediate 等
for /f "delims=" %%P in ('dir /a:d /s /b "Plugins" 2^>nul') do (
    REM 确保 %%P 是一个 Plugins 目录（避免匹配到文件）
    if exist "%%P\" (
        for %%D in (Intermediate Binaries DerivedDataCache) do (
            set "target=%%P\%%D"
            if exist "!target!" (
                echo 正在删除 !target!...
                rmdir /S /Q "!target!"
            )
        )
    )
)

REM ===== 3. 清理 Visual Studio 文件 =====

REM 删除 .vs 目录（隐藏/系统）
if exist ".vs" (
    echo 正在删除 .vs 目录...
    attrib -h -s -r ".vs" >nul 2>&1
    rmdir /S /Q ".vs"
)

REM 删除 .vsconfig
if exist ".vsconfig" (
    echo 正在删除 .vsconfig...
    del /Q ".vsconfig"
)

REM 删除所有 .sln 文件（通常只在根目录，但保险起见递归）
for /f "delims=" %%F in ('dir /a:-d /s /b "*.sln" 2^>nul') do (
    echo 正在删除 %%F...
    del /Q "%%F"
)

REM 可选：删除根目录残留的 .vcxproj（Intermediate 已删，通常无需此步）
REM for %%X in (*.vcxproj *.vcxproj.filters *.vcxproj.user) do (
REM     if exist "%%X" del /Q "%%X"
REM )

echo.
echo 中间文件及 Visual Studio 工程文件已清理完毕。
pause