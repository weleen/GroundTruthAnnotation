obj = VideoReader(fileName);
scale=rate/30;
len = floor(obj.NumberOfFrames/scale);% ֡������
for i = 1 : len% ��ȡ����
    frame = read(obj,i*scale);
    res=rgb2gray(frame);
    imwrite(res,[path,num2str(i-1,'%05d'),'.jpg']);% ����֡
end
disp('video_01readV is ok! ');