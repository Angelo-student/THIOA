function [opt_points,current_bounds, original_indices, deleted_indices] = optim_numofpoint(original_points,rect_bounds,max_allowed_curvature)
    % 初始化变量
    current_points = original_points;
    current_bounds = rect_bounds;
    deleted_indices = [];  % 记录被删除的索引
    original_indices = 1:size(original_points,1); % 新增原始索引跟踪
    % 主优化循环
    min_point_spacing = 1000; % 删除点最小索引间隔
    while true
        % 步骤1: 优化当前点的位置
        [opt_points, max_curv, peak_curvatures, t_peaks] = optimize_current_points(current_points, current_bounds);
        disp(num2str(max_curv))
        % 步骤2: 检查曲率约束
        if max_curv <= max_allowed_curvature
            disp('曲率约束已满足');
            break;
        else
            % 筛选需要处理的峰值
            valid_peaks = t_peaks(peak_curvatures > max_allowed_curvature);
            % 步骤3: 并行选择多个删除点
            delete_indices = select_delete_points(opt_points, valid_peaks, min_point_spacing);
            % 步骤4: 批量删除点
                % 步骤4: 批量删除点
            if ~isempty(delete_indices)
                % 按从后往前删除
                delete_indices = sort(unique(delete_indices), 'descend');
                % 记录原始索引 
                original_deleted = original_indices(delete_indices);
                deleted_indices = [deleted_indices, original_deleted];
                % 更新数据
                current_points(delete_indices,:) = [];
                current_bounds(delete_indices,:) = [];
                original_indices(delete_indices) = []; % 维护索引映射
            end
        end
    end
    % 输出最终结果
    disp('===== 最终结果 =====');
    disp('优化后点坐标:');
    disp(opt_points);
    disp('被删除点索引:');
    disp(unique(deleted_indices));
end

