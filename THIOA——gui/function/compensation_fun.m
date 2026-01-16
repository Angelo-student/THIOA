function [current_points,current_bounds,deleted_indices] = compensation_fun(opt_points,current_bounds,original_indices, deleted_indices, original_points, rect_bounds,max_allowed_curvature)
    % ===== 补偿阶段 =====
    % 保存当前状态用于补偿
    current_points = opt_points;       % 使用主优化结果
    temp_bounds = current_bounds;      % 保存当前约束
    temp_indices = original_indices;   % 保存当前索引映射
    temp_deleted = deleted_indices;    % 保存当前删除列表
    
    % 按原始索引升序处理
    sorted_deleted = sort(temp_deleted, 'ascend');
    recovered_points = 0;  % 记录恢复点数
    
    for i = 1:length(sorted_deleted)
        candidate = sorted_deleted(i);
        
        % 暂存当前状态用于回滚
        backup_points = current_points;
        backup_bounds = temp_bounds;
        backup_indices = temp_indices;
        
        % 确定插入位置（保持路径顺序）
        insert_pos = find(temp_indices < candidate, 1, 'last') + 1;
        if isempty(insert_pos)
            insert_pos = 1;
        end
        
        % 插入点和约束
        current_points = [current_points(1:insert_pos-1, :); 
                          original_points(candidate, :);
                          current_points(insert_pos:end, :)];
        temp_bounds = [temp_bounds(1:insert_pos-1, :);
                       rect_bounds(candidate, :);
                       temp_bounds(insert_pos:end, :)];
        temp_indices = [temp_indices(1:insert_pos-1), candidate, temp_indices(insert_pos:end)];
        
        % 重新优化
        [opt_pts, max_curv, ~, ~] = optimize_current_points(current_points, temp_bounds);
        
        % 验证曲率约束
        if max_curv <= max_allowed_curvature
            % 更新状态
            current_points = opt_pts;
            recovered_points = recovered_points + 1;
            fprintf('成功恢复点 %d (当前曲率: %.2f)\n', candidate, max_curv);
        else
            % 回滚状态
            current_points = backup_points;
            temp_bounds = backup_bounds;
            temp_indices = backup_indices;
            fprintf('无法恢复点 %d (曲率超标: %.2f)\n', candidate, max_curv);
        end
    end
    %% 
    
    current_bounds = temp_bounds; 
    % 更新删除列表
    deleted_indices = setdiff(temp_deleted, temp_indices);
    fprintf('\n共恢复 %d 个点，最终保留 %d 个点\n',...
           recovered_points, size(current_points,1));
end

