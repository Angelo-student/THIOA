function delete_indices = select_delete_points(points, t_peaks, min_spacing)
    % 初始化输出变量
    delete_indices = [];
    
    % 检查输入有效性
    if isempty(t_peaks)
        return;
    end
    
    pp = cscvn(points');
    breaks = pp.breaks;
    num_peaks = length(t_peaks);
    
    % 1. 初始化归约变量为cell数组
    temp_indices = cell(num_peaks, 1);
    
    % 2. 并行处理每个峰值
    parfor i = 1:num_peaks
        selected_idx = process_single_peak(t_peaks(i), breaks, points);
        % 将结果包装成cell元素（空值转换为空cell）
        if isempty(selected_idx)
            temp_indices{i} = {}; 
        else
            temp_indices{i} = {selected_idx}; 
        end
    end
    
    % 3. 正确合并结果
    candidate_indices = [temp_indices{:}];       % 合并为cell数组
    candidate_indices = candidate_indices(~cellfun(@isempty, candidate_indices)); % 过滤空cell
    candidate_indices = [candidate_indices{:}];  % 转换为数值数组
    
    % 4. 后续处理
    if isempty(candidate_indices)
        return;
    end
    
    % 计算局部曲率并排序
    curv_values = arrayfun(@(x) compute_local_curvature(points, x), candidate_indices);
    [~, order] = sort(curv_values, 'descend');
    sorted_candidates = candidate_indices(order);
    
    % 筛选间隔足够的点
    delete_indices = [];
    for idx = sorted_candidates
        if isempty(delete_indices) || all(abs(delete_indices - idx) >= min_spacing)
            delete_indices(end+1) = idx;
        end
    end
    
    % 最终整理输出
    delete_indices = sort(unique(delete_indices), 'descend');
end
