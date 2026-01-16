function total_cost = combined_objective(X, curv_threshold)
    % 组合目标函数：曲线长度 + 曲率惩罚
    points = reshape(X, 2, [])';
    
    % 计算曲线长度
    spline_length = calculate_spline_length(points);
    
    % 计算曲率惩罚
    [max_curv, ~] = compute_max_curvature(points);
    penalty = 1e6 * max(0, max_curv - curv_threshold);
    
    % 组合成本
    total_cost = spline_length + penalty;
end