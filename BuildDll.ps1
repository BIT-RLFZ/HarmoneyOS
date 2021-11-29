param([string]$buildtfm = 'x86')

$build64	 = $buildtfm -eq 'x64'
$build86	 = $buildtfm -eq 'x86'

$msbuildPath = Split-Path (& "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.Component.MSBuild -find MSBuild\Current\Bin\amd64\MSBuild.exe | Select-Object -First 1) -Parent
$env:PATH = $msbuildPath + ';' + $env:PATH

if($build86){
    Write-Host "Building Dll Project x86...(Release)"
    msbuild HarmoneyOSDll/HarmoneyOSDll.vcxproj /p:Platform=x86 /p:Configuration=Release
    Write-Host "Building Dll Project x86...(Debug)"
    msbuild HarmoneyOSDll/HarmoneyOSDll.vcxproj /p:Platform=x86 /p:Configuration=Debug
}

if($build64){
    Write-Host "Building Dll Project x64...(Release)"
    msbuild HarmoneyOSDll/HarmoneyOSDll.vcxproj /p:Platform=x64 /p:Configuration=Release
    Write-Host "Building Dll Project x64...(Debug)"
    msbuild HarmoneyOSDll/HarmoneyOSDll.vcxproj /p:Platform=x64 /p:Configuration=Debug
}