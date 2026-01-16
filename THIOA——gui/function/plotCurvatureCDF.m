function plotCurvatureCDF(ax, orig_points, opt_points, max_allowed_curvature)
    % 对数曲率累积分布对比绘图函数（适配App Designer）
    % 输入参数：
    %   ax                    - 绘图的UIAxes句柄（App中的画布）
    %   orig_points           - 原始路径点 (N×2矩阵)
    %   opt_points            - 优化后路径点 (M×2矩阵)
    %   max_allowed_curvature - 曲率阈值（用于绘制阈值线）
    
    % 清空画布，避免重复绘图
    cla(ax);
    hold(ax, 'on');
    axis(ax, 'normal'); 
    
    % 计算曲率数据（需确保compute_curvature_samples函数已定义）
    [~, curv_orig] = compute_curvature_samples(orig_points, 10000);
    [~, curv_opt] = compute_curvature_samples(opt_points, 10000);
    
    % 对曲率取对数（过滤非正值，避免log计算错误）
    log_curv_orig = log10(curv_orig(curv_orig > 0));
    log_curv_opt = log10(curv_opt(curv_opt > 0));
    
    % 计算累积分布函数(CDF)
    [F_orig, x_orig] = ecdf(log_curv_orig);
    [F_opt, x_opt] = ecdf(log_curv_opt);
    
    % 绘制CDF曲线（明确父对象并绑定）
    h_orig = plot(ax, x_orig, F_orig, 'r-', 'LineWidth', 2);
    h_orig.Parent = ax;  % 强制绑定到ax
    
    h_opt = plot(ax, x_opt, F_opt, 'g-', 'LineWidth', 2);
    h_opt.Parent = ax;  % 强制绑定到ax
    
    % 绘制曲率阈值线（明确父对象并绑定）
    log_threshold = log10(max_allowed_curvature);
    h_threshold = xline(ax, log_threshold, '--', 'Color', [0 0 0], 'LineWidth', 1.8);
    h_threshold.Parent = ax;  % 强制绑定到ax
    
    % 图形美化（所有属性绑定到ax）
    title(ax, 'Logarithmic curvature cumulative distribution comparison', ...
        'FontSize', 14, 'FontWeight', 'bold');
    xlabel(ax, 'Logarithmic curvature \it{log10(\kappa)}', 'FontSize', 11);
    ylabel(ax, 'Cumulative probability', 'FontSize', 11);
    
    % 添加图例（使用对象句柄，绑定到ax）
    legend(ax, [h_orig, h_opt, h_threshold], ...
        {'Original distribution', 'Optimized distribution', 'Curvature threshold'}, ...
        'FontSize', 10, 'Location', 'southeast');
    
    % 设置坐标轴范围
    x_min = min([x_orig; x_opt]) - 0.5;
    x_max = max([x_orig; x_opt]) + 0.5;
    x_max = max(x_max, log_threshold + 0.5);  % 确保阈值线完全显示
    xlim(ax, [x_min, x_max]);
    ylim(ax, [0, 1.05]);
    
    % 网格和坐标轴样式
    grid(ax, 'on');
    set(ax, 'FontSize', 11, 'LineWidth', 1.2, ...
        'XColor', [0.3 0.3 0.3], 'YColor', [0.3 0.3 0.3], ...
        'GridAlpha', 0.4, 'MinorGridAlpha', 0.2);
    
    drawnow;  % 强制刷新显示
end