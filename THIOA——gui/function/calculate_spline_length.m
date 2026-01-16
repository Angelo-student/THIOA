function length = calculate_spline_length(points)
    % 通过采样计算样条曲线近似长度
    pp = cscvn(points');
    t = linspace(pp.breaks(1), pp.breaks(end), 10000); % 高密度采样
    xy = ppval(pp, t);
    
    % 计算差分
    diff_x = diff(xy(1,:));
    diff_y = diff(xy(2,:));
    
    % 累加线段长度
    length = sum(sqrt(diff_x.^2 + diff_y.^2));
end
