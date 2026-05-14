$ErrorActionPreference = "Continue"

$project = "C:\Users\35753\Documents\Codex\2026-05-14\c-github"
$toolchain = "C:\msys64\ucrt64\bin"

Set-Location $project
$env:Path = "$toolchain;$env:Path"

Write-Host "== dayforge local run =="
Write-Host "Project: $project"
Write-Host "Toolchain: $toolchain"
Write-Host ""

Write-Host "== versions =="
g++ --version
cmake --version
ninja --version
Write-Host ""

Write-Host "== configure =="
cmake -S . -B build -G Ninja
Write-Host ""

Write-Host "== build =="
cmake --build build
Write-Host ""

Write-Host "== test =="
ctest --test-dir build --output-on-failure
Write-Host ""

Write-Host "== ui =="
.\build\dayforge.exe ui
Write-Host ""

Write-Host "Finished. You can type commands here, for example:"
Write-Host ".\build\dayforge.exe add --title `"test`" --tag cpp"
Write-Host ".\build\dayforge.exe today"
