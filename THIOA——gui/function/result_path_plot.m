function result_path_plot(ax_main, orig_points, opt_points, deleted)
    % 路径对比绘图函数（适配App Designer，指定UIAxes作为主画布）
    % 输入参数：
    %   app         - App对象（用于访问App的figure）
    %   ax_main     - 主图的UIAxes句柄（在App Designer中创建的画布）
    %   orig_points - 原始路径点 (N×2矩阵)
    %   opt_points  - 优化后路径点 (M×2矩阵)
    %   deleted     - 被删除点的索引向量（可为空）
    
    % 清空主画布，避免重复绘图
    cla(ax_main);
    hold(ax_main, 'on');
    
    % 计算优化路径长度（确保calculate_spline_length已定义）
    optimized_length = calculate_spline_length(opt_points);
    
    % ================= 主图绘制（所有对象强制绑定到ax_main）=================
    % 1. 绘制原始路径（红色虚线）
    h_orig_path = plot(ax_main, orig_points(:,1), orig_points(:,2), 'r--', 'LineWidth', 1.5);
    h_orig_path.Parent = ax_main;  % 明确父对象
    
    % 2. 绘制原始点（红色空心圆）
    h_orig_points = scatter(ax_main, orig_points(:,1), orig_points(:,2), 20, 'r', 'o', 'LineWidth', 1.5);
    h_orig_points.Parent = ax_main;  % 明确父对象
    
    % 3. 绘制优化后的样条曲线（绿色实线）
    pp_opt = cscvn(opt_points');
    t_opt = linspace(pp_opt.breaks(1), pp_opt.breaks(end), 10000);
    xy_opt = ppval(pp_opt, t_opt);
    h_opt_path = plot(ax_main, xy_opt(1,:), xy_opt(2,:), 'g-', 'LineWidth', 2);
    h_opt_path.Parent = ax_main;  % 明确父对象
    
    % 4. 绘制优化后的点（红色实心点）
    h_opt_points = scatter(ax_main, opt_points(:,1), opt_points(:,2), 20, 'r', 'filled',...
        'MarkerEdgeColor','k', 'LineWidth', 0.8);
    h_opt_points.Parent = ax_main;  % 明确父对象
    
    % 5. 绘制被删除点（黑色叉号）
    h_deleted = [];  % 用于图例
    if ~isempty(deleted)
        deleted_points = orig_points(deleted, :);
        h_deleted = scatter(ax_main, deleted_points(:,1), deleted_points(:,2), 70, 'k', 'x',...
            'LineWidth', 1.8, 'MarkerEdgeAlpha',0.9);
        h_deleted.Parent = ax_main;  % 明确父对象
    end
    
    % ================= 主图美化（所有属性绑定到ax_main）=================
    title_str = sprintf('Optimization path comparison (Total length: %.2f mm)', optimized_length);
    title(ax_main, title_str, 'FontSize', 14, 'FontWeight','bold');
    
    legend_items = {'Original path','Original points','Optimized path','Optimized points'};
    if ~isempty(deleted)
        legend_items = [legend_items, 'Deleted points'];
        legend(ax_main, [h_orig_path, h_orig_points, h_opt_path, h_opt_points, h_deleted], ...
               legend_items, 'Location','best','FontSize',9);
    else
        legend(ax_main, [h_orig_path, h_orig_points, h_opt_path, h_opt_points], ...
               legend_items, 'Location','best','FontSize',9);
    end
    
    axis(ax_main, 'equal');
    axis(ax_main, 'tight');
    grid(ax_main, 'on');
    set(ax_main, 'FontSize', 11, 'LineWidth', 1.2,...
        'XColor', [0.3 0.3 0.3], 'YColor', [0.3 0.3 0.3]);
    
    drawnow;  % 强制刷新显示
end