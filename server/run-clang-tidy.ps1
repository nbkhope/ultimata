# Run clang-tidy on all C++ source files
# Usage: .\run-clang-tidy.ps1

Push-Location $PSScriptRoot

Write-Host "Running clang-tidy on server source files..." -ForegroundColor Cyan

# Check if clang-tidy is available
$clangTidyPath = Get-Command clang-tidy -ErrorAction SilentlyContinue
if (-not $clangTidyPath) {
    Write-Host "Error: clang-tidy not found in PATH" -ForegroundColor Red
    Write-Host ""
    Write-Host "To install clang-tidy on Windows, you can:" -ForegroundColor Yellow
    Write-Host "  1. Install LLVM from https://releases.llvm.org/" -ForegroundColor Yellow
    Write-Host "  2. Or install via winget: winget install LLVM.LLVM" -ForegroundColor Yellow
    Write-Host "  3. Or install via Chocolatey: choco install llvm" -ForegroundColor Yellow
    Write-Host ""
    Write-Host "After installation, make sure LLVM's bin directory is in your PATH." -ForegroundColor Yellow
    Pop-Location
    exit 1
}

Write-Host "Found clang-tidy: $($clangTidyPath.Source)" -ForegroundColor Green

# Ensure build directory and compile_commands.json exist
if (-not (Test-Path "build\compile_commands.json")) {
    Write-Host "compile_commands.json not found. Generating..." -ForegroundColor Yellow
    
    # Remove existing build directory to avoid generator conflicts
    if (Test-Path "build") {
        Write-Host "Cleaning existing build directory..." -ForegroundColor Yellow
        Remove-Item -Path "build" -Recurse -Force
    }
    
    # Detect vcpkg toolchain
    $vcpkgToolchain = $null
    if ($env:VCPKG_ROOT) {
        $vcpkgToolchain = Join-Path $env:VCPKG_ROOT "scripts\buildsystems\vcpkg.cmake"
    } elseif (Test-Path "vcpkg_installed") {
        # Try common vcpkg locations
        $possiblePaths = @(
            "C:\vcpkg\scripts\buildsystems\vcpkg.cmake",
            "C:\tools\vcpkg\scripts\buildsystems\vcpkg.cmake",
            "$env:USERPROFILE\vcpkg\scripts\buildsystems\vcpkg.cmake"
        )
        foreach ($path in $possiblePaths) {
            if (Test-Path $path) {
                $vcpkgToolchain = $path
                break
            }
        }
    }
    
    # Build CMake command
    $cmakeArgs = @("-B", "build", "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON")
    if ($vcpkgToolchain -and (Test-Path $vcpkgToolchain)) {
        Write-Host "Using vcpkg toolchain: $vcpkgToolchain" -ForegroundColor Green
        $cmakeArgs += "-DCMAKE_TOOLCHAIN_FILE=$vcpkgToolchain"
    } else {
        Write-Host "Warning: vcpkg toolchain not found. Set VCPKG_ROOT environment variable if using vcpkg." -ForegroundColor Yellow
    }
    
    # Use Ninja if available, otherwise use default generator
    $ninjaAvailable = (Get-Command ninja -ErrorAction SilentlyContinue) -ne $null
    if ($ninjaAvailable) {
        $cmakeArgs += @("-G", "Ninja")
    }
    
    & cmake @cmakeArgs
    
    if ($LASTEXITCODE -ne 0) {
        Write-Host "Failed to generate compile_commands.json" -ForegroundColor Red
        Pop-Location
        exit 1
    }
}

# Get all .cpp files in src directory
$sourceFiles = Get-ChildItem -Path "src" -Filter "*.cpp" -Recurse | Select-Object -ExpandProperty FullName

# Run clang-tidy on each file
$checks = "-*,modernize-*,-modernize-use-trailing-return-type,performance-*,readability-*,-readability-magic-numbers,-readability-identifier-length,bugprone-*,clang-analyzer-*"

foreach ($file in $sourceFiles) {
    Write-Host "Checking: $file" -ForegroundColor Gray
    clang-tidy -p build --checks=$checks --format-style=file $file
}

Write-Host "clang-tidy complete!" -ForegroundColor Green
Pop-Location
