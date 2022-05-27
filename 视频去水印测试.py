import cv2 as cv
import numpy as np
import os
'''
大概思路
    1·把视频分解成每一帧这一张图
    2·定位每张图的水印位置
    3·将其剪切下来
    4·将剪切下来进行处理
    5·处理好将其复位
    6·然后将所有图片合成视频
'''
video = cv.VideoCapture('./010.mp4')#读入一个文件

w = int(video.get(cv.CAP_PROP_FRAME_WIDTH))# 获取视频帧的宽

h = int(video.get(cv.CAP_PROP_FRAME_HEIGHT))# 获取视频帧的高
size2= (w,h)#720,1080
# image=cv.imread('./1/160_160.jpg')
# image_info=image.shape
# height=image_info[0]#1280
# width=image_info[1]#720
# size=(width,height)
fps = video.get(cv.CAP_PROP_FPS)# 获取视频帧的帧率
fcount = video.get(cv.CAP_PROP_FRAME_COUNT)# 获取视频流的总帧数
j = 0
i = 0
save_path = './2/'# 保存图片的路径
save_path1 = './4/'# 保存图片的路径
while True:
    success, frame = video.read()
    i += 1
    print('video 开始存视频存',i)
    if i == fcount:
        print('video 视频存完了')
        break
    if i % 1 == 0:
        # 保存图片
        j += 1
        save_name = save_path + str(j) + '_' + str(i) + '.jpg'
        cv.imwrite(save_name, frame)
         # print('image of %s is saved' % save_name)
    else:
        print('video is all read')
        break
'''
imgname_list = os.listdir(path)
img_list = []
k = 0
for imgname in imgname_list:
    # 就算文件夹里有别的文件也没有瓜西！
    # .endwith()可以把你想要的文件按照后缀名筛选出来
    if imgname.endswith('.jpg'):
        img = cv.imread(imgname_list + '/' + imgname)
        # img_n = cv.resize(img, cv.INTER_AREA)
        filename = path + str(i) + '.png'
        cv.imwrite(filename, img)  # need temporary files of the right size
        i = i + 1
        print(i)
    # return img_list
list = read_path(path)  # 图片文件夹的路径
print(list)  # 输出文件名
'''
path = save_path # 目标文件夹的路径

def read_path(pathname):
    # 按照数字进行排序后按顺序读取文件夹下的图片
    imgname_list = os.listdir(save_path)
    imgname_list.sort(key=lambda x: int(x.replace("frame", "").split('.')[0]))#排序整个文件夹的目录列表“frame”拿出这个字符串转成int
    # print(save_path)
    img_list = []
    i = 0
    # 图片列表
    for imgname in imgname_list:
    	#整个列表的名字循环处理
            print(imgname_list)
            img = cv.imread(save_path + '/' + imgname)#文件夹+/+文件名字

            # img.sort(key=lambda x: int(x.replace("frame", "").split('.')[0]))
            # img_1 = cv.strcat(save_path + '/' + imgname)
            # print(img.shape)
            gray = cv.cvtColor(img, cv.COLOR_RGB2GRAY)#二值化处理即是将其变成灰色
            img3 = cv.imread("./007Tk.JPG", 0)#读入一张要处理的目标图片
            h, w = img3.shape[:2]#获取目标图的高度，宽度
            res = cv.matchTemplate(gray, img3, cv.TM_SQDIFF_NORMED)#对比查找目标图&跟原图相识的坐标值
            min_val, max_val, min_loc, max_loc = cv.minMaxLoc(res)
            br = (min_loc[0] + w, min_loc[1] + h)
            roi = img[min_loc[1]:br[1], min_loc[0]:br[0]]  # 定位有水印部分

            roi_hsv = cv.cvtColor(roi, cv.COLOR_RGB2GRAY)  # 灰度化定位部分

            kernel = np.ones((3, 3), np.uint8)  # 处理水印蒙层

            blackhat = cv.morphologyEx(roi_hsv, cv.MORPH_BLACKHAT, kernel)  # 礼帽处理

            dilare_w = cv.dilate(blackhat, kernel, iterations=10)  # 膨胀操作

            res_1 = cv.inpaint(roi, dilare_w, 1, flags=cv.INPAINT_TELEA)  # 修补蓝色
            img[min_loc[1]:br[1], min_loc[0]:br[0]] = 0  # 对图片水印部分进行白色处理255是白色黑色是0
            img[min_loc[1]:br[1], min_loc[0]:br[0]] = res_1  # 将处理好的图片放回去

            print('执行处理，水印图片')
            if i > fcount:
                print("执行完毕")
                break
            filename = save_path1 + 'chuluguo' + str(i) + '.png'
            cv.imwrite(filename, img)  # need temporary files of the right size
            i = i + 1
    return img_list
read_path(path)
i=0
video = cv.VideoWriter('007.mp4', cv.VideoWriter_fourcc(*"mp4v"), fps, size2, True) #创建视频流对象-格式一
imgname_list = os.listdir(save_path1)
imgname_list.sort(key=lambda x: int(x.replace("chuluguo", "").split('.')[0]))
print(imgname_list)
for imgname in imgname_list:
    img = cv.imread(save_path1 + imgname)
    i=i+1
    print('执行图片合成视频中....',i)
    video.write(img)# 向视频文件写入一帧--只有图像，没有声音
print('执行完毕')
cv.waitKey()
video.release()
cv.destroyAllWindows()