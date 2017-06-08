function video_05showH(path,savePath)
    gt_init=load([path,'gt_init.txt']);
    im = imread([path,'00000.jpg']);
    model=im(gt_init(2):gt_init(2)+gt_init(4),gt_init(1):gt_init(1)+gt_init(3));
    [rows1,cols1] = size(model);
    [rows2,cols2] = size(im);
    % Create a new image showing the two images side by side.
    model(rows2,1) = 0;
    %% init H from gt_homography.txt
    tempH=load([path,'gt_homography.txt']);
    len=size(tempH,1);
    H=zeros(3,3,len);
    for i=1:len
        H(:,:,i)=reshape(tempH(i,:),3,3)';
    end
    %% show the box
    figure(1);
    axis off                          % set off the axis
    set(gca, 'position', [0 0  1 1]); % the axis must be normalized
    for i=1:len-1
%         if mod(i,100)~=1
%             continue;
%         end
        image = imread([path,num2str(i-1,'%05d'),'.jpg']);
        % Now append both images side-by-side.
        % Display both images overlayed with inlying matched feature points
        imshow(image), hold on;
        % Display the bounding box
        box1Points=[[0;0;1],[cols1;0;1],[0;rows1;1],[cols1;rows1;1]];
        box2Points=H(:,:,i)*box1Points;
        box2=box2Points*diag(1./box2Points(3,:));
        line([box2(1,[1,2]),box2(1,[4,3,1])],[box2(2,[1,2]),box2(2,[4,3,1])],'color',[1,0,0]...
            ,'Color','r','LineWidth',3);
        hold off;
        print(1,'-djpeg',[savePath,num2str(i-1,'%05d'),'.jpg']);
    end
end