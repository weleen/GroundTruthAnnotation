%http://www.mathworks.cn/cn/help/vision/feature-detection-extraction-and-matching.html
close all;
model = imread([path,'00000.jpg']);
points = detectSURFFeatures(model,'MetricThreshold',1000);
[features, valid_points] = extractFeatures(model, points);
% len = 187;% Ö¡µÄ×ÜÊý
H=zeros(3,3,len);
baseI=1;
for i=1:len
    image = imread([path,num2str(i-1,'%05d'),'.jpg']);
    H(:,:,i)=detect(image,model,features,valid_points,1);
    H(:,:,i)=H(:,:,i)*H(:,:,baseI);
    if(mod(i,2)==0)
        baseI=i;
        model = image;
        points = detectSURFFeatures(model,'MetricThreshold',1000);
        [features, valid_points] = extractFeatures(model, points);
    end
end
save([path,'H.mat'],'H');
disp('video_02calcH is ok! ');