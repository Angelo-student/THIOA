% 辅助函数：计算曲率样本
function [t, curv_samples] = compute_curvature_samples(points, n_samples)
    pp = cscvn(points');
    t = linspace(pp.breaks(1), pp.breaks(end), n_samples);
    
    % 计算导数
    dpp = fnder(pp,1);
    ddpp = fnder(pp,2);
    dx = ppval(dpp, t);
    ddx = ppval(ddpp, t);
    
    % 曲率计算（添加数值稳定性处理）
    numerator = abs(dx(1,:).*ddx(2,:) - dx(2,:).*ddx(1,:));
    denominator = (dx(1,:).^2 + dx(2,:).^2).^(3/2);
    denominator(denominator < eps) = eps; % 避免除零
    
    curv_samples = numerator ./ denominator;
    curv_samples(isnan(curv_samples)) = 0; % 处理无效值
end
