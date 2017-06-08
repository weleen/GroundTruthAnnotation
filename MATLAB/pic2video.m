%***********************************************************************************
clear;close all;clc

for i=1:400
        if(i<10)
            fname=strcat('./Ball/Ball_000',num2str(i),'.png');
        elseif(i>=10 && i<100)
            fname=strcat('./Ball/Ball_00',num2str(i),'.png');
        elseif ( i>=100)
            fname=strcat('./Ball/Ball_0',num2str(i),'.png');
        end
        im(:,:,:,i)=imread(fname);  %注意im括号里的维数，而且很容易就超出电脑内存
        imshow(im(:,:,:,i)) 
        M(i) = getframe; 
end 

movie2avi(M,'out.avi','FPS',8)%输出视频
%***********************************************************************************