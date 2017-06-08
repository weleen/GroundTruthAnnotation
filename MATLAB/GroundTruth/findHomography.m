% RANSACFITHOMOGRAPHY - fits 2D homography using RANSAC
%
% Usage:   [H, inliers] = ransacfithomography(x1, x2, t)
%
% Arguments:
%          x1  - 2xN or 3xN set of homogeneous points.  If the data is
%                2xN it is assumed the homogeneous scale factor is 1.
%          x2  - 2xN or 3xN set of homogeneous points such that x1<->x2.
%          t   - The distance threshold between data point and the model
%                used to decide whether a point is an inlier or not. 
%                Note that point coordinates are normalised to that their
%                mean distance from the origin is sqrt(2).  The value of
%                t should be set relative to this, say in the range 
%                0.001 - 0.01  
%
% Note that it is assumed that the matching of x1 and x2 are putative and it
% is expected that a percentage of matches will be wrong.
%
% Returns:
%          H       - The 3x3 homography such that x2 = H*x1.
%          inliers - An array of indices of the elements of x1, x2 that were
%                    the inliers for the best model.
%
% See Also: ransac, homography2d, homography1d

% Copyright (c) 2004-2005 Peter Kovesi
% School of Computer Science & Software Engineering
% The University of Western Australia
% http://www.csse.uwa.edu.au/
% 
% Permission is hereby granted, free of charge, to any person obtaining a copy
% of this software and associated documentation files (the "Software"), to deal
% in the Software without restriction, subject to the following conditions:
% 
% The above copyright notice and this permission notice shall be included in 
% all copies or substantial portions of the Software.
%
% The Software is provided "as is", without warranty of any kind.

% February 2004 - original version
% July     2004 - error in denormalising corrected (thanks to Andrew Stein)
% August   2005 - homogdist2d modified to fit new ransac specification.

function [H, inliers] = findHomography(x1, x2, t)
    if nargin<3
        t=0.01;
    end
    if ~all(size(x1)==size(x2))
        error('Data sets x1 and x2 must have the same dimension');
    end
    
    [rows,npts] = size(x1);
    if rows~=2 && rows~=3
        error('x1 and x2 must have 2 or 3 rows');
    end
    
    if npts < 4
        error('Must have at least 4 points to fit homography');
    end
    
    if rows == 2    % Pad data with homogeneous scale factor of 1
        x1 = [x1; ones(1,npts)];
        x2 = [x2; ones(1,npts)];        
    end
        
    % Normalise each set of points so that the origin is at centroid and
    % mean distance from origin is sqrt(2).  normalise2dpts also ensures the
    % scale parameter is 1.  Note that 'homography2d' will also call
    % 'normalise2dpts' but the code in 'ransac' that calls the distance
    % function will not - so it is best that we normalise beforehand.
    [x1, T1] = normalise2dpts(x1);
    [x2, T2] = normalise2dpts(x2);
    
    s = 4;  % Minimum No of points needed to fit a homography.
    
    fittingfn = @homography2d;
    distfn    = @homogdist2d;
    degenfn   = @isdegenerate;
    % x1 and x2 are 'stacked' to create a 6xN array for ransac
    [~, inliers] = ransac([x1; x2], fittingfn, distfn, degenfn, s, t);
    
    % Now do a final least squares fit on the data points considered to
    % be inliers.
    H = homography2d(x1(:,inliers), x2(:,inliers));
    
    % Denormalise
    H = T2\H*T1;
%% homogdist2d Function to evaluate the symmetric transfer error of a homography with
    % respect to a set of matched points as needed by RANSAC.
    function [inliers, H] = homogdist2d(H, x, t)

        x1 = x(1:3,:);   % Extract x1 and x2 from x
        x2 = x(4:6,:);    

        % Calculate, in both directions, the transfered points    
        Hx1    = H*x1;
        invHx2 = H\x2;

        % Normalise so that the homogeneous scale parameter for all coordinates
        % is 1.

        x1     = hnormalise(x1);
        x2     = hnormalise(x2);     
        Hx1    = hnormalise(Hx1);
        invHx2 = hnormalise(invHx2); 

        d2 = sum((x1-invHx2).^2)  + sum((x2-Hx1).^2);
        inliers = find(abs(d2) < t);    
    end

%% isdegenerate Function to determine if a set of 4 pairs of matched  points give rise
    % to a degeneracy in the calculation of a homography as needed by RANSAC.
    % This involves testing whether any 3 of the 4 points in each set is
    % colinear. 
    function r = isdegenerate(x)

        x1 = x(1:3,:);    % Extract x1 and x2 from x
        x2 = x(4:6,:);    

        r = ...
        iscolinear(x1(:,1),x1(:,2),x1(:,3)) | ...
        iscolinear(x1(:,1),x1(:,2),x1(:,4)) | ...
        iscolinear(x1(:,1),x1(:,3),x1(:,4)) | ...
        iscolinear(x1(:,2),x1(:,3),x1(:,4)) | ...
        iscolinear(x2(:,1),x2(:,2),x2(:,3)) | ...
        iscolinear(x2(:,1),x2(:,2),x2(:,4)) | ...
        iscolinear(x2(:,1),x2(:,3),x2(:,4)) | ...
        iscolinear(x2(:,2),x2(:,3),x2(:,4));
    end
%% ISCOLINEAR - are 3 points colinear
    %
    % Usage:  r = iscolinear(p1, p2, p3, flag)
    %
    % Arguments:
    %        p1, p2, p3 - Points in 2D or 3D.
    %        flag       - An optional parameter set to 'h' or 'homog'
    %                     indicating that p1, p2, p3 are homogneeous
    %                     coordinates with arbitrary scale.  If this is
    %                     omitted it is assumed that the points are
    %                     inhomogeneous, or that they are homogeneous with
    %                     equal scale.
    %
    % Returns:
    %        r = 1 if points are co-linear, 0 otherwise
    function r = iscolinear(p1, p2, p3, flag)

        if nargin == 3   % Assume inhomogeneous coords
        flag = 'inhomog';
        end

        if ~all(size(p1)==size(p2)) || ~all(size(p1)==size(p3)) || ...
            ~(length(p1)==2 || length(p1)==3)                              
            error('points must have the same dimension of 2 or 3');
        end

        % If data is 2D, assume they are 2D inhomogeneous coords. Make them
        % homogeneous with scale 1.
        if length(p1) == 2    
            p1(3) = 1; p2(3) = 1; p3(3) = 1;
        end

        if flag(1) == 'h'
        % Apply test that allows for homogeneous coords with arbitrary
            % scale.  p1 X p2 generates a normal vector to plane defined by
            % origin, p1 and p2.  If the dot product of this normal with p3
            % is zero then p3 also lies in the plane, hence co-linear.
        r =  abs(dot(cross(p1, p2),p3)) < eps;
        else
        % Assume inhomogeneous coords, or homogeneous coords with equal
            % scale.
        r =  norm(cross(p2-p1, p3-p1)) < eps;
        end
    end
%% HNORMALISE - Normalises array of homogeneous coordinates to a scale of 1
    %
    % Usage:  nx = hnormalise(x)
    %
    % Argument:
    %         x  - an Nxnpts array of homogeneous coordinates.
    %
    % Returns:
    %         nx - an Nxnpts array of homogeneous coordinates rescaled so
    %              that the scale values nx(N,:) are all 1.
    %
    % Note that any homogeneous coordinates at infinity (having a scale value of
    % 0) are left unchanged.
    function nx = hnormalise(x)
        [rows,~] = size(x);
        nx = x;

        % Find the indices of the points that are not at infinity
        finiteind = find(abs(x(rows,:)) > eps);
        % Normalise points not at infinity
        for r = 1:rows-1
            nx(r,finiteind) = x(r,finiteind)./x(rows,finiteind);
        end
        nx(rows,finiteind) = 1;
    end
end