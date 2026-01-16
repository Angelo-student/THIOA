function [opt_points, max_curv, peak_curvatures,t_peaks] = optimize_current_points(points, bounds)
    % 将点转换为优化变量
    X0 = reshape(points', [], 1);
    n = size(points,1);
    
    % 构建约束边界
    lb = zeros(2*n,1);
    ub = zeros(2*n,1);
    for i = 1:n
        lb(2*i-1) = bounds(i,1);
        ub(2*i-1) = bounds(i,2);
        lb(2*i)   = bounds(i,3);
        ub(2*i)   = bounds(i,4);
    end
    
    % 优化配置
    % 开启并行计算
    options = optimoptions('fmincon',...
        'Display', 'none',...
        'Algorithm', 'interior-point',...
        'MaxIterations', 1000, ...
        'UseParallel', true);
    
    % 执行优化
    objfun = @(X) compute_max_curvature(reshape(X,2,[])');
    [X_opt, ~] = fmincon(objfun, X0, [], [], [], [], lb, ub, [], options);
    
    % 后处理
    opt_points = reshape(X_opt,2,[])';
    [max_curv, peak_curvatures, t_peaks] = compute_max_curvature(opt_points);
end