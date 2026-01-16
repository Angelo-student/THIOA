function plotCurvaturePDF(ax, orig_points, opt_points, max_allowed_curvature)
    % 对数曲率概率密度分布对比绘图函数（适配App Designer）
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
    
    % 对曲率取自然对数（过滤非正值，避免log计算错误）
    log_curv_orig = log(curv_orig(curv_orig > 0));
    log_curv_opt = log(curv_opt(curv_opt > 0));
    log_threshold = log(max_allowed_curvature);  % 阈值对数转换
    
    % 绘制直方图（概率密度归一化，明确父对象）
    bin_edges = linspace(min(log_curv_orig), max(log_curv_opt), 500);  % 统一 bins 范围
    h1 = histogram(ax, log_curv_orig, bin_edges,...
        'FaceColor', [1 0.2 0.2], 'EdgeColor', 'none',...
        'FaceAlpha', 0.4, 'Normalization', 'pdf');
    h1.Parent = ax;  % 强制绑定到ax
    
    h2 = histogram(ax, log_curv_opt, bin_edges,...
        'FaceColor', [0.2 0.8 0.2], 'EdgeColor', 'none',...
        'FaceAlpha', 0.4, 'Normalization', 'pdf');
    h2.Parent = ax;  % 强制绑定到ax
    
    % 计算核密度估计并绘制曲线（明确父对象）
    [f_orig, x_orig] = ksdensity(log_curv_orig, 'Bandwidth', 0.3);
    [f_opt, x_opt] = ksdensity(log_curv_opt, 'Bandwidth', 0.3);
    p1 = plot(ax, x_orig, f_orig, 'r-', 'LineWidth', 2.5);
    p1.Parent = ax;  % 强制绑定到ax
    
    p2 = plot(ax, x_opt, f_opt, 'g-', 'LineWidth', 2.5);
    p2.Parent = ax;  % 强制绑定到ax
    
    % 绘制阈值线（明确父对象）
    yl = ylim(ax);  % 获取当前y轴范围
    threshold_line = line(ax, [log_threshold log_threshold], [0 yl(2)*1.1],...
        'Color', 'k', 'LineStyle', '--', 'LineWidth', 1.8);
    threshold_line.Parent = ax;  % 强制绑定到ax
    
    % 文本标注（阈值说明，明确父对象）
    txt = text(ax, log_threshold+0.1, yl(2)*0.9,...
        sprintf(' Threshold\n ln(%.1f) = %.2f', max_allowed_curvature, log_threshold),...
        'FontSize', 10, 'VerticalAlignment', 'top');
    txt.Parent = ax;  % 强制绑定到ax
    
    % 图形美化（所有属性绑定到ax）
    title(ax, 'Comparison of probability density distribution of logarithmic curvature', ...
        'FontSize', 14, 'FontWeight', 'bold');
    xlabel(ax, 'Logarithmic curvature \it{ln(\kappa)}', 'FontSize', 11);
    ylabel(ax, 'Probability density', 'FontSize', 11);
    
    % 添加图例（绑定到ax，使用对象句柄）
    legend(ax, [h1, h2, threshold_line],...
        {'Original distribution', 'Optimized distribution', 'Curvature threshold'},...
        'FontSize', 10, 'Location', 'northeast');
    
    % 坐标轴设置
    xlim(ax, [min(x_orig)-0.5, max([x_opt, log_threshold])+0.5]);
    ylim(ax, [0, yl(2)*1.1]);
    set(ax, 'FontSize', 11, 'LineWidth', 1.2,...
        'XColor', [0.3 0.3 0.3], 'YColor', [0.3 0.3 0.3],...
        'Layer', 'top', 'GridAlpha', 0.3);
    
    grid(ax, 'on');
    drawnow;  % 强制刷新显示
end