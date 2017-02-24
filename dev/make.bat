@echo off

if [%1]==[justcompile] goto :compilestage

echo MAKING GRAPHICS
cd ..\gfx

echo Making palettes
..\utils\mkts.exe mode=pals pals=palts.png out=..\dev\work\palts0.h label=pal_bg_0 silent
..\utils\mkts.exe mode=pals pals=palss.png out=..\dev\work\palss0.h label=pal_fg_0 silent
..\utils\mkts.exe mode=pals pals=palts2.png out=..\dev\work\palts1.h label=pal_bg_1 silent
..\utils\mkts.exe mode=pals pals=palss2.png out=..\dev\work\palss1.h label=pal_fg_1 silent
..\utils\mkts.exe mode=pals pals=palts3.png out=..\dev\work\palts2.h label=pal_bg_2 silent
..\utils\mkts.exe mode=pals pals=palss3.png out=..\dev\work\palss2.h label=pal_fg_2 silent
copy /b ..\dev\work\palts0.h + ..\dev\work\palss0.h + ..\dev\work\palts1.h + ..\dev\work\palss1.h + ..\dev\work\palts2.h + ..\dev\work\palss2.h ..\dev\assets\palettes.h

echo Making bg charset
..\utils\mkts.exe mode=chars in=font.png pals=palts.png out=chrts1.bin size=32,2 silent
..\utils\mkts.exe mode=coltiles in=ts.png pals=palts.png out=chrts2.bin mode=tiles size=16,3 tsmap=..\dev\assets\tspals.h label=ts silent
copy /b chrts1.bin + chrts2.bin chrts.bin > nul
del chrts1.bin > nul
del chrts2.bin > nul

echo Making obj charset
..\utils\mkts.exe mode=sprites in=sprites.png pals=palss.png out=chrss1.bin tsmap=..\dev\assets\sspl.h offset=0,0 size=7,1 metasize=2,3 sprorg=-4,-8 label=spr_player genflipped silent
..\utils\mkts.exe mode=sprites in=sprites.png pals=palss.png out=chrss2a.bin tsmap=..\dev\assets\ssen1.h offset=0,3 size=4,1 metasize=2,3 sprorg=0,-8 label=spr_enems1 tmapoffset=32 genflipped silent
..\utils\mkts.exe mode=sprites in=sprites.png pals=palss.png out=chrss2b.bin tsmap=..\dev\assets\ssen2.h offset=0,6 size=2,1 metasize=3,2 sprorg=-4,0 label=spr_enems2 tmapoffset=53 genflipped silent
..\utils\mkts.exe mode=sprites in=sprites.png pals=palss.png out=chrss2c.bin tsmap=..\dev\assets\ssen3.h offset=0,8 size=2,1 metasize=2,1 sprorg=0,0 label=spr_enems3 tmapoffset=62 genflipped silent
..\utils\mkts.exe mode=sprites in=sprites.png pals=palss.png out=chrss2d.bin tsmap=..\dev\assets\ssen4.h offset=4,8 size=1,1 metasize=2,2 sprorg=0,0 label=spr_enems4 tmapoffset=63 silent
..\utils\mkts.exe mode=sprites in=sprites.png pals=palss.png out=chrss3.bin tsmap=..\dev\assets\ssending.h offset=0,10 size=2,1 metasize=3,3 sprorg=0,0 label=spr_ending tmapoffset=64 silent
rem 81... C A K x 0 1 2 3 4 5 6 7 8 9 
..\utils\mkts.exe mode=chars in=sprites.png pals=palss.png out=chrss4.bin offset=0,13 size=14,1 silent
rem 95...
..\utils\mkts.exe mode=sprites in=sprites.png pals=palss.png out=chrss5.bin tsmap=..\dev\assets\ssmt.h offset=0,14 size=1,1 metasize=6,2 sprorg=0,0 label=spr_logo tmapoffset=95 silent
rem 107...

rem Extra, hidden, diablura stuff
..\utils\mkts.exe mode=sprites in=sprites.png pals=palss2.png out=chrss6.bin tsmap=..\dev\assets\sspl2.h offset=0,16 size=8,1 metasize=2,3 sprorg=-4,-8 label=spr_player_2 tmapoffset=107 genflipped silent
..\utils\mkts.exe mode=sprites in=sprites.png pals=palss2.png out=chrss7.bin tsmap=..\dev\assets\ssextra.h offset=0,19 size=1,1 metasize=2,2 sprorg=0,0 label=spr_extra tmapoffset=145 genflipped silent
..\utils\mkts.exe mode=sprites in=sprites.png pals=palss3.png out=chrss8.bin tsmap=..\dev\assets\ssextra2.h offset=0,21 size=2,1 metasize=3,2 sprorg=-8,0 label=spr_extra2 tmapoffset=148 genflipped silent
..\utils\mkts.exe mode=sprites in=sprites.png pals=palss3.png out=chrss9.bin tsmap=..\dev\assets\sspl3.h offset=0,23 size=7,1 metasize=2,3 sprorg=-4,-8 label=spr_player_3 tmapoffset=157 genflipped silent

copy /b chrss1.bin + chrss2a.bin + chrss2b.bin + chrss2c.bin + chrss2d.bin + chrss3.bin + chrss4.bin + chrss5.bin + chrss6.bin + chrss7.bin + chrss8.bin + chrss9.bin chrss.bin > nul
del chrss1.bin > nul
del chrss2a.bin > nul
del chrss2b.bin > nul
del chrss2c.bin > nul
del chrss2d.bin > nul
del chrss3.bin > nul
del chrss4.bin > nul
del chrss5.bin > nul
del chrss6.bin > nul
del chrss7.bin > nul
del chrss8.bin > nul
del chrss9.bin > nul
..\utils\fillto.exe chrss.bin 4080

..\utils\mkts.exe mode=chars in=sprite0.png pals=palts.png out=sprite0.bin size=1,1 silent

echo Building chr
copy /b chrts.bin + chrss.bin + sprite0.bin ..\dev\tileset.chr
del chrts.bin > nul
del chrss.bin > nul
del sprite0.bin > nul

echo BUILDING MAP
cd ..\map 
..\utils\mapcnvababol.exe mapa.map 256 3
copy map.h ..\dev\assets
del map.h > nul
del map.bin > nul

echo BUILDING ENEMS
cd ..\enems 
..\utils\eneexp3.exe enems.ene ..\dev\assets\enems.h ROM 2 nohotspots

echo Compiling
cd ..\dev

:compilestage
cc65 -Oi ababol.c --add-source
ca65 crt0.s
ca65 ababol.s

ld65 -C nes.cfg -o ababol_p.nes crt0.o ababol.o runtime.lib  -Ln labels.txt

del ababol.s > nul
del ababol.o > nul
del crt0.o > nul
