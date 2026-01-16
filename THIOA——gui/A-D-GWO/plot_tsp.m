function plot_tsp(coordinates, tour)
    figure('Position', [100, 100, 800, 600]);
    scatter(coordinates(:, 1), coordinates(:, 2), 50, 'filled', 'MarkerFaceColor', 'blue');
    hold on;
    for i = 1:length(tour)
        start_point = coordinates(tour(i), :);
        end_point = coordinates(tour(mod(i, length(tour)) + 1), :);
        plot([start_point(1), end_point(1)], [start_point(2), end_point(2)], 'r-', 'LineWidth', 1);
    end
    title('TSP Tour');
    xlabel('X coordinate');
    ylabel('Y coordinate');
    grid on;
    axis equal;
    xlim([min(coordinates(:,1)) - 100, max(coordinates(:,1)) + 100]);
    ylim([min(coordinates(:,2)) - 100, max(coordinates(:,2)) + 100]);
    hold off;
end
