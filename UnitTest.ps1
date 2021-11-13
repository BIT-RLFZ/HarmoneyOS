param([string]$buildtfm = 'x86')
$mstestPath = Split-Path (& "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.Component.MSBuild -find Common7\IDE\Extensions\TestPlatform\vstest.console.exe | Select-Object -First 1) -Parent
$env:PATH = $mstestPath + ';' + $env:PATH
$msbuildPath = Split-Path (& "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe" -latest -requires Microsoft.Component.MSBuild -find MSBuild\Current\Bin\amd64\MSBuild.exe | Select-Object -First 1) -Parent
$env:PATH = $msbuildPath + ';' + $env:PATH


$test64	 = $buildtfm -eq 'x64'
$test86	 = $buildtfm -eq 'x86'

if($test86){
    Write-Host "Generating Test Project x86...(Debug)"
    msbuild HarmoneyOSTest/HarmoneyOSTest.vcxproj /p:Platform=x86 /p:Configuration=Debug

    Write-Host "Generating Test Project x86...(Release)"
    msbuild HarmoneyOSTest/HarmoneyOSTest.vcxproj /p:Platform=x86 /p:Configuration=Release

    
    Write-Host "Running Unit Test Project x86...(Debug)"
    VSTest.Console.exe HarmoneyOSTest\Debug\HarmoneyOSTest.dll
    Write-Host "Running Unit Test x86 OK!(Debug)"

    Write-Host "Running Unit Test Project x86...(Release)"
    VSTest.Console.exe HarmoneyOSTest\Release\HarmoneyOSTest.dll
    Write-Host "Running Unit Test x86 OK!(Release)"
}

if($test64){
    Write-Host "Generating Test Project x64...(Debug)"
    msbuild HarmoneyOSTest/HarmoneyOSTest.vcxproj /p:Platform=x64 /p:Configuration=Debug

    Write-Host "Generating Test Project x64...(Release)"
    msbuild HarmoneyOSTest/HarmoneyOSTest.vcxproj /p:Platform=x64 /p:Configuration=Release

    Write-Host "Running Unit Test Project x64...(Debug)"
    VSTest.Console.exe HarmoneyOSTest\x64\Debug\HarmoneyOSTest.dll
    Write-Host "Running Unit Test x64 OK!(Debug)"

    Write-Host "Running Unit Test Project x64...(Release)"
    VSTest.Console.exe HarmoneyOSTest\x64\Release\HarmoneyOSTest.dll
    Write-Host "Running Unit Test x64 OK!(Release)"
}