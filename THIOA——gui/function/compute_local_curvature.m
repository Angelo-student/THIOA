% 局部曲率计算函数
function max_curv = compute_local_curvature(points, idx)
    window = 2;
    start_idx = max(1, idx - window);
    end_idx = min(size(points,1), idx + window);
    segment = points(start_idx:end_idx, :);
    
    pp = cscvn(segment');
    t = linspace(pp.breaks(1), pp.breaks(end), 100);
    
    dpp = fnder(pp,1);
    ddpp = fnder(pp,2);
    dx = ppval(dpp, t);
    ddx = ppval(ddpp, t);
    
    curv = abs(dx(1,:).*ddx(2,:)-dx(2,:).*ddx(1,:)) ./ (dx(1,:).^2 + dx(2,:).^2).^(3/2);
    max_curv = max(curv);
end