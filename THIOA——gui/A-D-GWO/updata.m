function [min_value, best_route]=updata(wolf,wolf_leader,a,distance_matrix)
    %wolf_leader存储了头狼等三只狼的信息，每一行代表一个狼，wolf是狼个体

    fitnesses=[];
    route=[];

    for i=1:3
        data=wolf_leader(i,:);
        [~, jk, sk] = Hamming(data, wolf);
        
        if rand<=a
            result_t = shift(wolf);
        else
            ga_o=[sk',jk'];
            result_t=exchange(ga_o, wolf);
        end

        [best_tour, best_distance] = two_opt(result_t, distance_matrix);
        fitnesses=[fitnesses,best_distance];
        route=[route;best_tour];

    end
    [min_value, min_index] = min(fitnesses);
    best_route=route(min_index,:);
    
end

