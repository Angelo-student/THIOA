function selected_idx = process_single_peak(t_peak, breaks, points)
    seg_idx = find(breaks <= t_peak, 1, 'last');
    seg_idx = min(seg_idx, length(breaks)-2);  % 防止越界
    
    candidate_indices = [seg_idx, seg_idx+1];
    candidate_indices = candidate_indices(candidate_indices <= size(points,1));
    
    if isempty(candidate_indices)
        selected_idx = [];
        return;
    end
    
    % 向量化距离计算
    prev_points = points(max(1, candidate_indices-1), :);
    distances = vecnorm(points(candidate_indices,:) - prev_points, 2, 2);
    
    [~, min_idx] = min(distances);
    selected_idx = candidate_indices(min_idx);
end