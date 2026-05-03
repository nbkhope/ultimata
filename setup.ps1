# =============================================================================
# setup.ps1 — One-command setup & run for the Ultimata project on Windows.
#
# Installs all prerequisites, builds, and starts a component automatically.
# Run this script immediately after cloning the repository.
#
# Usage (from PowerShell):
#   .\setup.ps1                  # interactive: asks which component to run
#   .\setup.ps1 server           # C++ multiplayer server
#   .\setup.ps1 client           # C++ SDL2 game client
#   .\setup.ps1 python-server    # Python stub/test server
#   .\setup.ps1 nodejs-client    # TypeScript/Node.js client
#
# The script will request UAC elevation if administrator privileges are
# required to install packages.
# =============================================================================

param(
    [Parameter(Position = 0)]
    [ValidateSet("server", "client", "python-server", "nodejs-client", "")]
    [string]$Component = ""
)

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$RepoRoot = Split-Path -Parent $MyInvocation.MyCommand.Path

# ---------------------------------------------------------------------------
# Colour helpers
# ---------------------------------------------------------------------------
function Write-Info    { param([string]$msg) Write-Host "▶ $msg" -ForegroundColor Cyan }
function Write-Success { param([string]$msg) Write-Host "✓ $msg" -ForegroundColor Green }
function Write-Warn    { param([string]$msg) Write-Host "⚠  $msg" -ForegroundColor Yellow }
function Write-Error2  { param([string]$msg) Write-Host "✗ $msg" -ForegroundColor Red; exit 1 }
function Write-Header  { param([string]$msg) Write-Host "`n═══ $msg ═══`n" -ForegroundColor White }

# Run a block in a target directory and always return to the previous one.
function Invoke-InDirectory {
    param(
        [Parameter(Mandatory = $true)]
        [string]$Path,
        [Parameter(Mandatory = $true)]
        [ScriptBlock]$Script
    )

    Push-Location $Path
    try {
        & $Script
    }
    finally {
        Pop-Location
    }
}

# ---------------------------------------------------------------------------
# Elevation helper
# ---------------------------------------------------------------------------
function Test-IsAdmin {
    $id = [Security.Principal.WindowsIdentity]::GetCurrent()
    $principal = New-Object Security.Principal.WindowsPrincipal($id)
    return $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
}

function Invoke-Elevated {
    param([string]$Command)
    if (Test-IsAdmin) {
        Invoke-Expression $Command
    } else {
        Write-Warn "Elevated privileges required. Requesting UAC confirmation..."
        Start-Process powershell -ArgumentList "-NoProfile -Command `"$Command`"" -Verb RunAs -Wait
    }
}

# ---------------------------------------------------------------------------
# Winget / Chocolatey helpers
# ---------------------------------------------------------------------------
function Get-PackageManager {
    if (Get-Command winget -ErrorAction SilentlyContinue) { return "winget" }
    if (Get-Command choco  -ErrorAction SilentlyContinue) { return "choco" }
    return "none"
}

function Install-Chocolatey {
    if (Get-Command choco -ErrorAction SilentlyContinue) { return }
    Write-Info "Installing Chocolatey (Windows package manager)..."
    $question = "Chocolatey will be installed. This requires administrator privileges. Continue? [Y/n]"
    $answer = Read-Host $question
    if ($answer -match "^[Nn]") { Write-Error2 "Installation cancelled." }

    Invoke-Elevated "Set-ExecutionPolicy Bypass -Scope Process -Force; [Net.ServicePointManager]::SecurityProtocol = [Net.SecurityProtocolType]::Tls12; iex ((New-Object Net.WebClient).DownloadString('https://community.chocolatey.org/install.ps1'))"
    # Refresh PATH
    $env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")
    Write-Success "Chocolatey installed."
}

function Ensure-Command {
    param([string]$Cmd, [string]$WingetId = "", [string]$ChocoId = "")
    if (Get-Command $Cmd -ErrorAction SilentlyContinue) { return }

    Write-Info "Installing $Cmd..."
    $pm = Get-PackageManager
    if ($pm -eq "winget" -and $WingetId) {
        winget install --id $WingetId --silent --accept-package-agreements --accept-source-agreements
    } elseif ($pm -eq "choco" -or ($pm -eq "none" -and $ChocoId)) {
        Install-Chocolatey
        Invoke-Elevated "choco install $ChocoId -y"
    } else {
        Write-Error2 "Cannot auto-install $Cmd. Please install it manually and re-run."
    }

    # Refresh PATH
    $env:Path = [System.Environment]::GetEnvironmentVariable("Path","Machine") + ";" + [System.Environment]::GetEnvironmentVariable("Path","User")

    if (-not (Get-Command $Cmd -ErrorAction SilentlyContinue)) {
        # Some tools require a shell restart to be found — inform the user
        Write-Warn "$Cmd was installed but is not yet in PATH."
        Write-Warn "Please restart this terminal and re-run the script."
        exit 1
    }
    Write-Success "$Cmd is available."
}

# ---------------------------------------------------------------------------
# vcpkg
# ---------------------------------------------------------------------------
$VcpkgDefaultDir = "$env:USERPROFILE\.vcpkg"

function Find-Or-Install-Vcpkg {
    # 1. VCPKG_ROOT already set and valid?
    if ($env:VCPKG_ROOT -and (Test-Path "$env:VCPKG_ROOT\vcpkg.exe")) {
        $env:Path += ";$env:VCPKG_ROOT"
        return
    }

    # 2. In PATH?
    if (Get-Command vcpkg -ErrorAction SilentlyContinue) {
        $env:VCPKG_ROOT = Split-Path -Parent (Get-Command vcpkg).Source
        return
    }

    # 3. Common locations
    $candidates = @(
        "$env:USERPROFILE\.vcpkg",
        "C:\vcpkg",
        "C:\dev\vcpkg",
        "$env:LOCALAPPDATA\vcpkg"
    )
    foreach ($c in $candidates) {
        if (Test-Path "$c\vcpkg.exe") {
            $env:VCPKG_ROOT = $c
            $env:Path += ";$c"
            return
        }
    }

    # 4. Not found — bootstrap it
    Write-Warn "vcpkg not found. Cloning and bootstrapping into $VcpkgDefaultDir ..."
    Write-Host "This is a one-time operation that may take several minutes." -ForegroundColor Yellow
    $answer = Read-Host "Press Enter to continue, or type N to abort"
    if ($answer -match "^[Nn]") { Write-Error2 "Setup cancelled." }

    Ensure-Command "git" "Git.Git" "git"

    if (-not (Test-Path $VcpkgDefaultDir)) {
        git clone --depth=1 https://github.com/Microsoft/vcpkg.git $VcpkgDefaultDir
    }
    & "$VcpkgDefaultDir\bootstrap-vcpkg.bat" -disableMetrics

    $env:VCPKG_ROOT = $VcpkgDefaultDir
    $env:Path += ";$VcpkgDefaultDir"

    Write-Success "vcpkg installed at $VcpkgDefaultDir"
    Write-Warn "Tip: add VCPKG_ROOT=$VcpkgDefaultDir to your system environment variables to persist this."
}

# ---------------------------------------------------------------------------
# CMake build helper
# ---------------------------------------------------------------------------
function Invoke-CmakeBuild {
    param([string]$SrcDir, [string]$BuildDir, [string]$ExtraCmakeArgs = "")

    Write-Info "Configuring with CMake..."
    $toolchain = "$env:VCPKG_ROOT\scripts\buildsystems\vcpkg.cmake"
    $args = @("-S", $SrcDir, "-B", $BuildDir, "-DCMAKE_TOOLCHAIN_FILE=$toolchain")
    if ($ExtraCmakeArgs) { $args += $ExtraCmakeArgs.Split(" ") }

    $configured = $false
    & cmake @args
    if ($LASTEXITCODE -eq 0) {
        $configured = $true
    } else {
        $cacheFile = Join-Path $BuildDir "CMakeCache.txt"
        if (Test-Path $cacheFile) {
            Write-Warn "CMake cache may have been generated from a different path."
            Write-Warn "Cleaning stale CMake cache in $BuildDir and retrying configuration..."

            Remove-Item $cacheFile -Force -ErrorAction SilentlyContinue
            Remove-Item (Join-Path $BuildDir "CMakeFiles") -Recurse -Force -ErrorAction SilentlyContinue

            & cmake @args
            if ($LASTEXITCODE -eq 0) {
                $configured = $true
            }
        }
    }
    if (-not $configured) { Write-Error2 "CMake configuration failed." }

    Write-Info "Building..."
    & cmake --build $BuildDir --config Release
    if ($LASTEXITCODE -ne 0) { Write-Error2 "Build failed." }
}

# ---------------------------------------------------------------------------
# Component: C++ Server
# ---------------------------------------------------------------------------
function Setup-Server {
    Write-Header "Setting up C++ Server"

    Ensure-Command "cmake" "Kitware.CMake" "cmake"
    Ensure-Command "git"   "Git.Git"       "git"

    # Ensure MSVC or available C++ compiler (Visual Studio or Build Tools)
    if (-not (Get-Command cl -ErrorAction SilentlyContinue) -and
        -not (Get-Command g++ -ErrorAction SilentlyContinue)) {
        Write-Warn "No C++ compiler found."
        Write-Info "Installing Visual Studio Build Tools (MSVC)..."
        Ensure-Command "cl" "Microsoft.VisualStudio.2022.BuildTools" "visualstudio2022buildtools"
    }

    Find-Or-Install-Vcpkg

    $src   = "$RepoRoot\server"
    $build = "$src\build"

    Invoke-CmakeBuild $src $build

    Write-Success "Server built successfully."
    Write-Host ""
    Write-Info "Starting the server (listening on port 8099)..."
    Write-Host "Press Ctrl-C to stop." -ForegroundColor Yellow
    Write-Host ""

    $candidates = @(
        "$build\ultimata-server.exe",
        "$build\Release\ultimata-server.exe",
        "$build\Debug\ultimata-server.exe"
    )
    $exe = $candidates | Where-Object { Test-Path $_ } | Select-Object -First 1
    if (-not $exe) { Write-Error2 "Build succeeded but ultimata-server.exe not found in $build" }

    & $exe
}

# ---------------------------------------------------------------------------
# Component: C++ SDL2 Client
# ---------------------------------------------------------------------------
function Setup-Client {
    Write-Header "Setting up C++ Game Client"

    Ensure-Command "cmake" "Kitware.CMake" "cmake"
    Ensure-Command "git"   "Git.Git"       "git"

    Find-Or-Install-Vcpkg

    $src   = "$RepoRoot\client"
    $build = "$src\build"

    Invoke-CmakeBuild $src $build

    Write-Success "Client built successfully."
    Write-Host ""
    Write-Warn "Make sure the server is running on localhost:8099 first."
    Write-Host ""
    Write-Info "Starting the game client..."
    Write-Host ""

    $candidates = @(
        "$build\bin\Ultimata.exe",
        "$build\bin\Release\Ultimata.exe",
        "$build\bin\Debug\Ultimata.exe"
    )
    $exe = $candidates | Where-Object { Test-Path $_ } | Select-Object -First 1
    if (-not $exe) { Write-Error2 "Build succeeded but Ultimata.exe not found in $build\bin" }

    Invoke-InDirectory "$RepoRoot\client" {
        & $exe
    }
}

# ---------------------------------------------------------------------------
# Component: Python Server
# ---------------------------------------------------------------------------
function Setup-PythonServer {
    Write-Header "Setting up Python Server"

    Ensure-Command "python" "Python.Python.3" "python3"

    # Ensure pip
    $pipOk = python -m pip --version 2>$null
    if (-not $pipOk) {
        Write-Info "Installing pip..."
        python -m ensurepip --upgrade
    }

    $req = "$RepoRoot\python_server\requirements-dev.txt"
    if (Test-Path $req) {
        Write-Info "Installing Python dependencies..."
        python -m pip install -r $req --quiet
    }

    Write-Success "Python server ready."
    Write-Host ""
    Write-Info "Starting the Python server (listening on port 8099)..."
    Write-Host "Press Ctrl-C to stop." -ForegroundColor Yellow
    Write-Host ""

    Invoke-InDirectory "$RepoRoot\python_server" {
        python simple_server.py
    }
}

# ---------------------------------------------------------------------------
# Component: Node.js Client
# ---------------------------------------------------------------------------
function Setup-NodejsClient {
    Write-Header "Setting up Node.js Client"

    Ensure-Command "node" "OpenJS.NodeJS.LTS" "nodejs-lts"
    Ensure-Command "npm"  "OpenJS.NodeJS.LTS" "nodejs-lts"

    $dir = "$RepoRoot\nodejs-client"
    Write-Info "Installing npm dependencies..."
    npm --prefix $dir install --silent
    if ($LASTEXITCODE -ne 0) { Write-Error2 "npm install failed." }

    Write-Success "Node.js client ready."
    Write-Host ""
    Write-Warn "The Node.js client requires the server to be running on localhost:8099."
    Write-Warn "Start the server first (.\setup.ps1 server) in another terminal."
    Write-Host ""
    Write-Info "Connecting to server..."
    Write-Host "Press Ctrl-C to disconnect." -ForegroundColor Yellow
    Write-Host ""

    Invoke-InDirectory $dir {
        node --import tsx index.ts
    }
}

# ---------------------------------------------------------------------------
# Interactive menu
# ---------------------------------------------------------------------------
function Pick-Component {
    Write-Host ""
    Write-Host "Which component would you like to set up and run?" -ForegroundColor White
    Write-Host ""
    Write-Host "  1) server         — C++ multiplayer game server (port 8099)"
    Write-Host "  2) client         — C++ SDL2 game client"
    Write-Host "  3) python-server  — Python stub server (port 8099)"
    Write-Host "  4) nodejs-client  — TypeScript/Node.js client (needs server)"
    Write-Host ""
    $choice = Read-Host "Enter choice [1-4]"
    switch ($choice) {
        "1" { return "server" }
        "2" { return "client" }
        "3" { return "python-server" }
        "4" { return "nodejs-client" }
        default { Write-Error2 "Invalid choice: $choice" }
    }
}

# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------
Write-Header "Ultimata — Setup & Run (Windows)"
Write-Host "Repository: $RepoRoot" -ForegroundColor Cyan
Write-Host "Platform  : Windows" -ForegroundColor Cyan

$OriginalLocation = Get-Location
try {
    if (-not $Component) { $Component = Pick-Component }

    switch ($Component) {
        "server"        { Setup-Server }
        "client"        { Setup-Client }
        "python-server" { Setup-PythonServer }
        "nodejs-client" { Setup-NodejsClient }
        default         { Write-Error2 "Unknown component: $Component" }
    }
}
finally {
    Set-Location $OriginalLocation
}
