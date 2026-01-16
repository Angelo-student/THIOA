function [max_curv, peak_curvatures, t_peaks] = compute_max_curvature(points)
    % 计算样条曲率
    pp = cscvn(points');
    dpp = fnder(pp,1);
    ddpp = fnder(pp,2);
    
    % 密集采样
    t = linspace(pp.breaks(1), pp.breaks(end), 10000);
    dx = ppval(dpp, t);
    ddx = ppval(ddpp, t);
    
    % 曲率计算
    numerator = abs(dx(1,:).*ddx(2,:) - dx(2,:).*ddx(1,:));
    denominator = (dx(1,:).^2 + dx(2,:).^2).^(3/2);
    curvatures = numerator ./ max(denominator, eps);
    
    % 检测所有局部曲率峰值
    local_max = islocalmax(curvatures);
    peak_indices = find(local_max);
    peak_curvatures = curvatures(peak_indices);
    t_peaks = t(peak_indices);
    max_curv = max(curvatures); % 全局最大曲率
end