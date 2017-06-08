function H=detect(image,model,features1,valid_points1,debug)
    [rows1,cols1] = size(model);
    points2 = detectSURFFeatures(image,'MetricThreshold',1000);
    [features2, valid_points2] = extractFeatures(image, points2);
    %match features
    indexPairs = matchFeatures(features1, features2);
    matchedPoints1 = valid_points1(indexPairs(:, 1), :);
    matchedPoints2 = valid_points2(indexPairs(:, 2), :);

    % Create a new image showing the two images side by side.
    rows2 = size(image,1);
    % if (rows1 < rows2)
    model(rows2,1) = 0;
    % Now append both images side-by-side.
    im3 = [model,image];
    loc1=valid_points1.Location;
    loc2=valid_points2.Location;
    m1=matchedPoints1.Location;
    m2=matchedPoints2.Location;
    % Display both images overlayed with inlying matched feature points
    if(debug)
        imshow(im3), hold on;
        plot(loc1(:,1),loc1(:,2),'r+');
        plot(loc2(:,1)+cols1,loc2(:,2),'g+');
        for n = 1:size(m1,1)
            line([m1(n,1),m2(n,1)+cols1], [m1(n,2) m2(n,2)],'color',[0 0 1]);
        end
        if size(matchedPoints1)<4
            drawnow;
            return;
        end
    end
    % Assemble homogeneous feature coordinates for fitting of the
    % homography, note that [x,y] corresponds to [col, row]
    [H, inliers] = findHomography( m1', m2');  
    % Display inliers
    if(debug)
        for n = 1:size(inliers,2)
            line([m1(inliers(n),1),m2(inliers(n),1)+cols1], [m1(inliers(n),2) m2(inliers(n),2)],'color',[1 1 0]);
        end
        % Display the bounding box
        box1Points=[[0;0;1],[cols1;0;1],[0;rows1;1],[cols1;rows1;1]];
        box2Points=H*box1Points;
        box2=box2Points*diag(1./box2Points(3,:));
        line([box2(1,[1,2])+cols1,box2(1,[4,3,1])+cols1],[box2(2,[1,2]),box2(2,[4,3,1])],'color',[1,0,0]...
            ,'Color','r','LineWidth',3);
        drawnow;
        hold off;
    end
end     %end of function