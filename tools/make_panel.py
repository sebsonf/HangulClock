#!/usr/bin/python
# -*- coding: utf-8 -sudo python3 -m pip uninstall pip && sudo apt install python3-pip --reinstall*-

# make_panel.py - make silkscreen for panel of hangulclock
#
# Copyright (C) 2011 Homin Lee <ff4500@gmail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.

from PIL import Image, ImageFont, ImageDraw, ImageOps
import os

panelString = '''
열한다세네
두여섯일곱
여덟아홉시
자정이삼십
사오십오분
'''
t = ''.join(panelString.split())
panelChars = list(t.decode('utf-8'))

DPI = 300
#cSizeWithMargin = 15 # 20mm for W & H of one character

panelSizeX = 142.0
panelSizeY = 140.0
panelMarginX = 6.0
panelMarginY = 5.0
cSize = (panelSizeX - 2*panelMarginX) / 5
cPix = int((DPI * cSize )/25.4) # 1 inch == 25.4 mm
panelMarginXPix = int((DPI * panelMarginX)/25.4)
panelMarginYPix = int((DPI * panelMarginY)/25.4)

#cSize = cPix*5
#panelMarginX = cPix
#panelMarginY = cPix
#panelSize = (cSize+panelMarginX*2, cSize+panelMarginY*2) # 236 == 20mm on 300dpi
panelSizeXPix = int((DPI * panelSizeX)/25.4)
panelSizeYPix = int((DPI * panelSizeY)/25.4)


print("panelSize [mm]= [%d, %d]", panelSizeXPix, panelSizeYPix)

image = Image.new('RGB', (panelSizeXPix, panelSizeYPix))
draw = ImageDraw.Draw(image)

# The fonts from http://hangeul.naver.com
# I used it via ttf-nanum package of Ubuntu linux
fontPath = r'/usr/share/fonts/truetype/nanum/NanumGothic.ttf'

# To find proper fontsize fited-in given dimention
fontSize = 0
for i in range(12, 2000):
    font = ImageFont.truetype(fontPath, i, encoding="unic")
    text = "한".decode('utf-8')
    textSize = font.getsize(text)
    if textSize[0] > cPix or textSize[1] > cPix:
        print("Font size", textSize, i)
        fontSize = i - 10
        break

font = ImageFont.truetype(fontPath, fontSize, encoding="unic")

print("cPix =", cPix)
for y in range(5):
    for x in range(5):
        panelChar = panelChars[x+(y*5)]
        charSize = font.getsize(panelChar)
        xMargin = (cPix - charSize[0])/2
        yMargin = (cPix - charSize[1])/2
        #print panelChar.encode('utf-8'), charSize, xMargin, yMargin
        draw.text((x*cPix+xMargin+panelMarginXPix, y*cPix+yMargin+panelMarginYPix),
                  panelChar, font=font)

# mirror it to toner transfer
image = ImageOps.mirror(image)
#image = ImageOps.invert(image)
image.save('panel_%s.png'%os.path.basename(fontPath), dpi=(DPI, DPI))

# vim: et sw=4 fenc=utf-8:
