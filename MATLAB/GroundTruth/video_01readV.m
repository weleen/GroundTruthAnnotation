obj = VideoReader(fileName);
scale=rate/30;
len = floor(obj.NumberOfFrames/scale);% 帧的总数
for i = 1 : len% 读取数据
    frame = read(obj,i*scale);
    res=rgb2gray(frame);
    imwrite(res,[path,num2str(i-1,'%05d'),'.jpg']);% 保存帧
end
disp('video_01readV is ok! ');