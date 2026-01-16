function [length_opt_points,optimized_length] = optim_distance(current_points,current_bounds,max_allowed_curvature)
% ===== 路径长度优化 =====（原位置保持不变）
% 使用补偿后的点作为初始点
length_opt_points = current_points;

% 计算初始长度
original_length = calculate_spline_length(length_opt_points);
fprintf('\n优化前曲线长度: %.2f\n', original_length);

% 设置长度优化参数

max_retry = 3; % 最大重试次数

for retry = 1:max_retry
    % 优化配置
    options = optimoptions('fmincon',...
        'Display', 'iter',...
        'Algorithm', 'interior-point',...
        'MaxIterations', 500,...
        'UseParallel', true);
    
    % 构建优化问题
    n = size(length_opt_points,1);
    X0 = reshape(length_opt_points', [], 1);
    lb = zeros(2*n,1);
    ub = zeros(2*n,1);
    for i = 1:n
        lb(2*i-1) = current_bounds(i,1);
        ub(2*i-1) = current_bounds(i,2);
        lb(2*i)   = current_bounds(i,3);
        ub(2*i)   = current_bounds(i,4);
    end
    
    % 运行优化
    [X_opt, ~] = fmincon(@(X) combined_objective(X, max_allowed_curvature),...
        X0, [], [], [], [], lb, ub, [], options);
    
    % 后处理
    length_opt_points = reshape(X_opt, 2, [])';
    
    % 验证曲率约束
    [final_curv, ~] = compute_max_curvature(length_opt_points);
    if final_curv <= max_allowed_curvature
        break;
    else
        fprintf('第%d次优化后曲率仍然超标，增加惩罚权重重试\n', retry);
    end
end

% 计算最终长度
optimized_length = calculate_spline_length(length_opt_points);
fprintf('优化后曲线长度: %.2f\n', optimized_length);
% 
% disp(["删除点的列表："])
% deleted_indices
end

