@echo off
..\utils\nsf2data sounds.nsf -ca65 
..\utils\text2data music.txt -ca65
copy *.s ..\dev > nul
echo DONE!
