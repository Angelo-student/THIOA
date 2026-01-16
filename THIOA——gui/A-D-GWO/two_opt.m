function [best_tour, best_distance] = two_opt(tour, distance_matrix)
    num_cities = length(tour);
    best_tour = tour;
    improved = true;
    
    % 预计算总距离
    best_distance = 0;
    for i = 1:num_cities-1
        best_distance = best_distance + distance_matrix(best_tour(i), best_tour(i+1));
    end
    best_distance = best_distance + distance_matrix(best_tour(end), best_tour(1));
    
    % 预计算下一个索引的数组（用于环形路径）
    next_index = [2:num_cities, 1];
    
    while improved
        improved = false;
        for i = 2:num_cities-1
            % 预计算常用索引
            i_prev = i-1;
            city_i_prev = best_tour(i_prev);
            city_i = best_tour(i);
            
            for j = i+1:num_cities
                % 获取j的下一个索引和对应城市
                j_next = next_index(j);
                city_j = best_tour(j);
                city_j_next = best_tour(j_next);
                
                % 计算距离变化
                original_dist = distance_matrix(city_i_prev, city_i) + ...
                               distance_matrix(city_j, city_j_next);
                new_dist = distance_matrix(city_i_prev, city_j) + ...
                          distance_matrix(city_i, city_j_next);
                delta = original_dist - new_dist;
                
                % 只有当改进足够大时才执行翻转
                if delta > 1e-6
                    % 执行翻转操作
                    best_tour(i:j) = best_tour(j:-1:i);
                    
                    % 更新总距离
                    best_distance = best_distance - delta;
                    
                    % 更新局部变量（因为路径已改变）
                    city_i = best_tour(i);
                    city_j = best_tour(j);
                    
                    improved = true;
                end
            end
        end
    end
end