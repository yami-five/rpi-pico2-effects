from PIL import Image
import io
image = Image.open("title.bmp")

print(image.size)
(img_x,img_y)=image.size
converted_img=""
for x in range (0,img_x):
    for y in range (0,img_y):
        (r,g,b)=image.getpixel((x, y))
        r = max(0, min(255, r))
        g = max(0, min(255, g))
        b = max(0, min(255, b))
        r5 = r >> 3
        g6 = g >> 2
        b5 = b >> 3
        # r5 = (r*31)//255
        # g6 = (g*63)//255
        # b5 = (b*31)//255
        rgb565 = (r5 << 11) | (g6 << 5) | b5
        high_byte = f"0x{((rgb565 >> 8) & 0xFF):02X}"
        low_byte = f"0x{(rgb565 & 0xFF):02X}"
        # converted_img+=f"{str(rgb565)},"
        converted_img+=low_byte+","+high_byte+","

f = open("img_converted.txt", "w", encoding="utf-8")
f.write(converted_img[:-1]) 
f.close()