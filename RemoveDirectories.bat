@echo off
echo Removing Unreal Directories
echo Removing vs: 
rmdir /q /s ".vs"
echo Removing Binaries: 
rmdir /q /s "Binaries"
echo Removing Intermediate: 
rmdir /q /s "Intermediate"
echo Removing Saved: 
rmdir /q /s "Saved"
echo Removing enc_temp_folder
rmdir /q /s enc_temp_folder
echo Removing DerivedDataCache
rmdir /q /s DerivedDataCache
echo Removing Build
rmdir /q /s Build
echo Removing Sln
del "*.sln" /s /q
Prototype.uproject
