%http://www.mathworks.cn/cn/help/vision/feature-detection-extraction-and-matching.html
close all;
gt_init=load([path,'gt_init.txt']);
% len=187;
load([path,'H.mat']);
for i=1:len
    for j=1:3
        H(j,3,i)=H(j,1,i)*gt_init(1)+H(j,2,i)*gt_init(2)+H(j,3,i);
    end
    H(:,:,i)=H(:,:,i)./H(3,3,i);
end
fid=fopen([path,'gt_homography.txt'],'w');
for i=1:len
    for j=1:3
        for k=1:3
            fprintf(fid,'%.6f ',H(j,k,i));
        end
    end
    fprintf(fid,'\r\n');
end
fclose(fid);
disp('video_04writeH is ok!');