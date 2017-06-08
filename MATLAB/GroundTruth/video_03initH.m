model = imread([path,'00000.jpg']);
imshow(model);
xy=ginput(2);
xy=round(xy);
fid_init=fopen([path,'gt_init.txt'],'w');
fprintf(fid_init,'%3.0d %3.0d %3.0d %3.0d',xy(1,1),xy(1,2),xy(2,1)-xy(1,1),xy(2,2)-xy(1,2));
fclose(fid_init);
disp('video_03initH is ok! ');