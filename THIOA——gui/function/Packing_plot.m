function Packing_plot(ax_main, data, circletab)
    % 清空主坐标轴并设置保持状态
    cla(ax_main);   % 清空主坐标轴
    hold(ax_main, 'on');  % 保持绘图状态，后续图形都绘制在ax_main中
    
    color = [215, 84, 85]/255;  % 散点颜色
    
    % 绘制原始散点（明确指定Parent为ax_main）
    h_scatter = scatter(ax_main, data(:,1), data(:,2), 15, color, 'filled');
    h_scatter.Parent = ax_main;  % 冗余确认（实际scatter第一个参数已指定）
    
    % 绘制虚拟线条（用于图例，明确Parent）
    h_rect_legend = plot(ax_main, NaN, NaN, 'Color', [0.5,0.5,0.5], 'LineStyle', '--', 'LineWidth', 1.2);
    h_rect_legend.Parent = ax_main;  % 冗余确认
    
    % 循环绘制所有矩形及中心
    h_center = [];  % 保存第一个中心圆点的句柄（用于图例）
    for i = 1:length(circletab)
        rect_params = circletab{i};
        cx = rect_params(1);
        cy = rect_params(2);
        rect_len = rect_params(3);
        rect_width = rect_params(4);
        x_left = cx - rect_len/2;
        y_top = cy - rect_width/2;
        
        % 绘制矩形（强制指定Parent为ax_main）
        h_rect = rectangle('Parent', ax_main, ...  % 核心：明确父容器
                  'Position', [x_left, y_top, rect_len, rect_width], ...
                  'EdgeColor', [0.5,0.5,0.5], 'LineStyle', '--', 'FaceColor', 'none', 'LineWidth', 1.2);
        h_rect.Parent = ax_main;  % 双重确认
        
        % 绘制中心圆点（明确Parent）
        if isempty(h_center)
            h_center = scatter(ax_main, cx, cy, 20, [0.5,0.5,0.5], 'filled');
            h_center.Parent = ax_main;  % 确认父对象
        else
            h_scat = scatter(ax_main, cx, cy, 20, [0.5,0.5,0.5], 'filled');
            h_scat.Parent = ax_main;  % 确认父对象
        end
    end
    
    % 主图美化（所有属性设置都关联到ax_main）
    title(ax_main, 'Scatter plot with rectangles', 'FontSize', 14, 'FontWeight', 'bold');
    legend(ax_main, [h_scatter, h_rect_legend, h_center], ...
           {'Data points', 'Rectangles', 'Rectangle centers'}, ...
           'Location', 'best', 'FontSize', 9);
    axis(ax_main, 'equal');
    axis(ax_main, 'tight');
    grid(ax_main, 'on');
    set(ax_main, 'FontSize', 11, 'LineWidth', 1.2, ...
        'XColor', [0.3 0.3 0.3], 'YColor', [0.3 0.3 0.3]);
    hold(ax_main, 'off');
end