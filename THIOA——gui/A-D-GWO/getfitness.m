function total_distance=getfitness(tour, distance_matrix)
    total_distance = 0;
    for i = 1:length(tour)-1
        total_distance = total_distance + distance_matrix(tour(i), tour(i+1));
    end
    total_distance = total_distance + distance_matrix(tour(end), tour(1)); % 返回起点的距离
end


