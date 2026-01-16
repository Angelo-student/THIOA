function population = initialization(pop_size, n)
    % pop_size: 种群数量
    % n: 城市的数量
    
    % 初始化种群矩阵,随机初始化
    population = zeros(pop_size, n);
    
    for i = 1:pop_size
        % 对每个个体，生成一个城市的随机排列
        population(i, :) = randperm(n);
    end
end
