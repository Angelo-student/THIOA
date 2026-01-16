function result = find_nearby_points_kd(points, threshold)
    % points: 一个 N x 2 的矩阵，每一行表示一个点的坐标 (x, y)
    % threshold: 距离阈值
    
    % 创建一个 k-d 树
    kdtree = KDTreeSearcher(points);
    
    N = size(points, 1); % 点的数量
    result = cell(N, 1); % 用于存储每个点的近邻点序号
    for i = 1:N
        % 查询点 i 的邻近点，返回距离小于 threshold 的点的索引和距离
        [idx, dist] = rangesearch(kdtree, points(i, :), threshold);
        result{i} = idx{1}(dist{1} >= 0);
    end
end


