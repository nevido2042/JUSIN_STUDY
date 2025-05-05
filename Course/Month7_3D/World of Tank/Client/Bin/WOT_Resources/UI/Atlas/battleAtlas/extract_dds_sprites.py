from PIL import Image
import xml.etree.ElementTree as ET
import os

# 파일 이름
IMAGE_FILE = "battleAtlas.dds"     # PNG일 경우 확장자만 바꾸세요
XML_FILE = "battleAtlas.xml"
OUTPUT_DIR = "output"

# 이미지 열기
image = Image.open(IMAGE_FILE)

# XML 파싱
tree = ET.parse(XML_FILE)
root = tree.getroot()

# 출력 폴더 생성
os.makedirs(OUTPUT_DIR, exist_ok=True)

# 반복하며 이미지 잘라 저장
for sub in root.findall("SubTexture"):
    name = sub.find("name").text.strip()
    x = int(sub.find("x").text)
    y = int(sub.find("y").text)
    w = int(sub.find("width").text)
    h = int(sub.find("height").text)

    box = (x, y, x + w, y + h)
    cropped = image.crop(box)
    save_path = os.path.join(OUTPUT_DIR, f"{name}.png")
    cropped.save(save_path)
    print(f"Saved: {save_path}")
