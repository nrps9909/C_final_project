import cv2
import numpy as np

# 讀取圖片
image = cv2.imread("D:/1.png")

# 將圖片轉換為 HSV 色彩空間
hsv = cv2.cvtColor(image, cv2.COLOR_BGR2HSV)

# 定義綠色的範圍
lower_green = np.array([45, 100, 100])
upper_green = np.array([65, 255, 255])

# 創建遮罩
mask = cv2.inRange(hsv, lower_green, upper_green)

# 反轉遮罩
mask_inv = cv2.bitwise_not(mask)

# 保留原圖中的非綠色部分
result = cv2.bitwise_and(image, image, mask=mask_inv)

# 創建透明背景
bgra = cv2.cvtColor(result, cv2.COLOR_BGR2BGRA)
bgra[:, :, 3] = mask_inv

# 保存結果
cv2.imwrite("D:/2.png", bgra)
